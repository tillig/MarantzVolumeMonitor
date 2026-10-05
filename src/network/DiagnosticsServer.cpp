#include "DiagnosticsServer.h"

#include <ArduinoJson.h>
#include <numeric>

#include "ReceiverMonitor.h"
#include "WiFiManager.h"
#include "../diagnostics/DeviceInfo.h"
#include "../diagnostics/DiagnosticLog.h"

namespace {
const char IndexHtml[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Marantz Volume Monitor Diagnostics</title>
<style>body{background:#101010;color:#e0e0e0;font-family:sans-serif;margin:1rem}pre{background:#1c1c1c;padding:1rem;overflow-x:auto}</style>
</head>
<body>
<h1>Marantz Volume Monitor Diagnostics</h1>
<p>Raw data: <a href="/api/status">status JSON</a>, <a href="/api/log">event log</a>. Refreshes every 5 seconds.</p>
<h2>Status</h2>
<pre id="status">Loading...</pre>
<h2>Recent Events</h2>
<pre id="log">Loading...</pre>
<script>
async function refresh() {
  try {
    const status = await fetch('/api/status').then(r => r.json());
    document.getElementById('status').textContent = JSON.stringify(status, null, 2);
    document.getElementById('log').textContent = await fetch('/api/log').then(r => r.text());
  } catch (e) {
    document.getElementById('status').textContent = 'Device unreachable: ' + e;
  }
}
refresh();
setInterval(refresh, 5000);
</script>
</body>
</html>
)HTML";
} // namespace

void DiagnosticsServer::begin() {
    if (_task != nullptr) {
        return;
    }

    _server.on("/", HTTP_GET, [this]() { handleIndex(); });
    _server.on("/api/status", HTTP_GET, [this]() { handleStatus(); });
    _server.on("/api/log", HTTP_GET, [this]() { handleLog(); });
    _server.begin();

    // Serve from a separate task so a slow browser cannot stall the UI loop.
    if (xTaskCreate(taskEntry, "diagnostics-http", TaskStackBytes, this, 1, &_task) != pdPASS) {
        _task = nullptr;
        _server.stop();
        DiagnosticLog::getInstance().add("Diagnostics server task creation failed");
    }
}

void DiagnosticsServer::taskEntry(void* parameter) {
    DiagnosticsServer* server = static_cast<DiagnosticsServer*>(parameter);
    for (;;) {
        server->_server.handleClient();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void DiagnosticsServer::handleIndex() {
    _server.send_P(200, "text/html", IndexHtml);
}

void DiagnosticsServer::handleStatus() {
    _server.send(200, "application/json", statusJson());
}

void DiagnosticsServer::handleLog() {
    _server.send(200, "text/plain", logText());
}

String DiagnosticsServer::statusJson() {
    JsonDocument doc;
    uint32_t now = millis();

    JsonObject device = doc["device"].to<JsonObject>();
    device["firmwareVersion"] = DeviceInfo::firmwareVersion();
    device["uptimeMs"] = now;
    device["resetReason"] = DeviceInfo::resetReasonName();
    device["freeHeapBytes"] = ESP.getFreeHeap();
    device["minFreeHeapBytes"] = ESP.getMinFreeHeap();

    WiFiManager& wifiManager = WiFiManager::getInstance();
    uint8_t reason = wifiManager.lastDisconnectReason();
    JsonObject wifi = doc["wifi"].to<JsonObject>();
    wifi["ssid"] = WiFi.SSID();
    wifi["connected"] = wifiManager.isConnected();
    wifi["status"] = static_cast<int>(WiFi.status());
    wifi["ipAddress"] = wifiManager.getIPAddress();
    wifi["rssiDbm"] = wifiManager.getSignalStrength();
    wifi["channel"] = WiFi.channel();
    wifi["bssid"] = WiFi.BSSIDstr();
    wifi["lastDisconnectReason"] = reason;
    wifi["lastDisconnectReasonName"] = WiFiManager::disconnectReasonName(reason);
    wifi["disconnectCount"] = wifiManager.disconnectCount();
    wifi["retryCount"] = wifiManager.retryCount();

    ReceiverMonitor::Diagnostics monitor = ReceiverMonitor::getInstance().diagnostics();
    JsonObject receiver = doc["receiver"].to<JsonObject>();
    receiver["ipAddress"] = monitor.receiverIp;
    receiver["hasStatus"] = monitor.hasStatus;
    receiver["consecutiveFailures"] = monitor.consecutiveFailures;
    if (monitor.hasStatus) {
        // Read the clock after the snapshot, or a poll finishing in between makes the age wrap.
        receiver["lastPollAgeMs"] = millis() - monitor.lastPollAtMs;
        receiver["httpCode"] = monitor.status.httpCode;
        receiver["valid"] = monitor.status.isValid;
        receiver["power"] = monitor.status.power;
        if (monitor.status.hasVolume) {
            receiver["volumeDb"] = monitor.status.volume;
        }
        receiver["input"] = monitor.status.input;
        receiver["mode"] = monitor.status.mode;
    }

    String json;
    serializeJson(doc, json);
    return json;
}

String DiagnosticsServer::logText() {
    std::vector<DiagnosticLog::Entry> entries = DiagnosticLog::getInstance().entries();
    return std::accumulate(
        entries.begin(), entries.end(), String(), [](String text, const DiagnosticLog::Entry& entry) {
            text += String(entry.uptimeMs) + " " + entry.message + "\n";
            return text;
        });
}
