#include "WiFiManager.h"

#include <algorithm>
#include <iterator>
#include <map>

#include "../diagnostics/DiagnosticLog.h"

void WiFiManager::begin() {
    WiFi.onEvent([](arduino_event_id_t event, arduino_event_info_t info) {
        WiFiManager::getInstance().handleEvent(event, info);
    });
    WiFi.mode(WIFI_STA);
    // Modem sleep saves little on a mains-powered display and is a common cause of dropped connections.
    WiFi.setSleep(false);
}

void WiFiManager::update() {
    std::lock_guard<std::mutex> lock(_mutex);
    uint32_t now = millis();
    if (_targetSsid.length() == 0 || isConnected()) {
        _lastProgressMs = now;
        _retryAttempt = 0;
        return;
    }

    // Restarting the radio would abort a scan the user is waiting on.
    if (now - _lastProgressMs < retryDelayMs() || WiFi.scanComplete() == WIFI_SCAN_RUNNING) {
        return;
    }

    _retryAttempt++;
    _retryCount++;
    uint8_t reason = _lastDisconnectReason;
    // Log the first retry and reason changes only, so a long outage doesn't push out the events that explain it.
    if (_retryAttempt == 1 || reason != _lastLoggedRetryReason) {
        _lastLoggedRetryReason = reason;
        DiagnosticLog::getInstance().add("Wi-Fi retry %lu for %s, last reason %u %s",
                                         static_cast<unsigned long>(_retryAttempt),
                                         _targetSsid.c_str(),
                                         reason,
                                         disconnectReasonName(reason).c_str());
    }
    beginConnectAttempt();
}

void WiFiManager::startConnect(const String& ssid, const String& password) {
    std::lock_guard<std::mutex> lock(_mutex);
    _targetSsid = ssid;
    _targetPassword = password;
    _retryAttempt = 0;
    _lastDisconnectReason = 0;
    DiagnosticLog::getInstance().add("Wi-Fi connecting to %s", ssid.c_str());
    beginConnectAttempt();
}

void WiFiManager::stopConnecting() {
    std::lock_guard<std::mutex> lock(_mutex);
    _targetSsid = "";
    _targetPassword = "";
    WiFi.disconnect();
}

void WiFiManager::beginConnectAttempt() {
    _lastProgressMs = millis();
    // Restart the radio so a stuck connection attempt cannot reject the new one.
    if (!WiFi.disconnect(true)) {
        DiagnosticLog::getInstance().add("Wi-Fi radio restart failed");
    }
    if (WiFi.begin(_targetSsid.c_str(), _targetPassword.c_str()) == WL_CONNECT_FAILED) {
        DiagnosticLog::getInstance().add("Wi-Fi driver rejected connection request");
    }
}

uint32_t WiFiManager::retryDelayMs() const {
    uint32_t delayMs = RetryBaseDelayMs * (_retryAttempt + 1);
    return delayMs < RetryMaxDelayMs ? delayMs : RetryMaxDelayMs;
}

void WiFiManager::handleEvent(arduino_event_id_t event, arduino_event_info_t info) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            _wasConnected = true;
            DiagnosticLog::getInstance().add("Wi-Fi connected: IP %s, RSSI %d dBm, channel %u, BSSID %s",
                                             WiFi.localIP().toString().c_str(),
                                             static_cast<int>(WiFi.RSSI()),
                                             static_cast<unsigned>(WiFi.channel()),
                                             WiFi.BSSIDstr().c_str());
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED: {
            uint8_t reason = info.wifi_sta_disconnected.reason;
            bool wasConnected = _wasConnected.exchange(false);
            if (reason == WIFI_REASON_ASSOC_LEAVE) {
                // The firmware asked to disconnect; keep the last real failure reason.
                break;
            }
            uint8_t previousReason = _lastDisconnectReason.exchange(reason);
            // Count lost connections, not each of the driver's own failed reconnect attempts.
            if (wasConnected) {
                _disconnectCount++;
            }
            // The driver retries rapidly on its own; only log when something changes.
            if (wasConnected || reason != previousReason) {
                DiagnosticLog::getInstance().add("Wi-Fi disconnected: reason %u %s, RSSI %d dBm",
                                                 reason,
                                                 disconnectReasonName(reason).c_str(),
                                                 static_cast<int>(info.wifi_sta_disconnected.rssi));
            }
            break;
        }
        default:
            break;
    }
}

wl_status_t WiFiManager::getConnectStatus() {
    return WiFi.status();
}

uint8_t WiFiManager::lastDisconnectReason() const {
    return _lastDisconnectReason;
}

uint32_t WiFiManager::disconnectCount() const {
    return _disconnectCount;
}

uint32_t WiFiManager::retryCount() const {
    return _retryCount;
}

String WiFiManager::disconnectReasonName(uint8_t reason) {
    if (reason == 0) {
        return "NONE";
    }
    return WiFi.disconnectReasonName(static_cast<wifi_err_reason_t>(reason));
}

String WiFiManager::describeDisconnectReason(uint8_t reason) {
    switch (reason) {
        case 0:
            return "No response from the network.";
        case WIFI_REASON_NO_AP_FOUND:
        case WIFI_REASON_NO_AP_FOUND_W_COMPATIBLE_SECURITY:
        case WIFI_REASON_NO_AP_FOUND_IN_AUTHMODE_THRESHOLD:
        case WIFI_REASON_NO_AP_FOUND_IN_RSSI_THRESHOLD:
            return "Network not found.";
        case WIFI_REASON_AUTH_EXPIRE:
        case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
        case WIFI_REASON_HANDSHAKE_TIMEOUT:
        case WIFI_REASON_AUTH_FAIL:
            // The driver reports the same codes for a wrong password and for a signal too weak to finish the handshake.
            return "Wrong password or weak signal.";
        case WIFI_REASON_BEACON_TIMEOUT:
            return "Signal lost.";
        default:
            return "Network refused the connection.";
    }
}

bool WiFiManager::connect(const String& ssid, const String& password) {
    WiFi.begin(ssid.c_str(), password.c_str());

    int counter = 0;
    while (WiFi.status() != WL_CONNECTED && counter < 20) {
        delay(500);
        Serial.print(".");
        counter++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\nWiFi connection failed");
        return false;
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIPAddress() {
    return WiFi.localIP().toString();
}

int32_t WiFiManager::getSignalStrength() {
    return WiFi.RSSI();
}

uint8_t WiFiManager::signalLevelForRssi(int32_t rssi) {
    if (rssi < -80) {
        return 1;
    }
    if (rssi < -70) {
        return 2;
    }
    return 3;
}

std::vector<WiFiManager::NetworkInfo> WiFiManager::getScanResults() {
    std::vector<NetworkInfo> networks;
    int n = WiFi.scanComplete();
    if (n > 0) {
        // Use a map to deduplicate by SSID, keeping the strongest RSSI
        std::map<String, NetworkInfo> deduped;

        for (int i = 0; i < n; ++i) {
            String ssid = WiFi.SSID(i);
            int32_t rssi = WiFi.RSSI(i);
            uint8_t enc = WiFi.encryptionType(i);

            if (deduped.find(ssid) == deduped.end() || rssi > deduped[ssid].rssi) {
                deduped[ssid] = {ssid, rssi, enc};
            }
        }

        networks.reserve(deduped.size());
        std::transform(deduped.begin(),
                       deduped.end(),
                       std::back_inserter(networks),
                       [](const std::pair<const String, NetworkInfo>& entry) { return entry.second; });

        // Sort by RSSI (strongest first)
        std::sort(networks.begin(), networks.end(), [](const NetworkInfo& a, const NetworkInfo& b) {
            return a.rssi > b.rssi;
        });

        WiFi.scanDelete();
    }
    return networks;
}

void WiFiManager::startScan() {
    WiFi.scanNetworks(true);
}

int16_t WiFiManager::getScanStatus() {
    return WiFi.scanComplete();
}

std::vector<WiFiManager::NetworkInfo> WiFiManager::scanNetworks() {
    std::vector<NetworkInfo> networks;
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
        networks.push_back({WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i)});
    }
    return networks;
}
