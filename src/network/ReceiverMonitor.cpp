#include "ReceiverMonitor.h"

#include <HTTPClient.h>

#include "WiFiManager.h"
#include "../diagnostics/DiagnosticLog.h"

void ReceiverMonitor::begin() {
    if (_task != nullptr) {
        return;
    }

    xTaskCreate(taskEntry, "receiver-monitor", TaskStackBytes, this, 1, &_task);
}

void ReceiverMonitor::setReceiverIp(const String& ip) {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (ip == _receiverIp) {
            return;
        }
        _receiverIp = ip;
        _hasStatus = false;
        _consecutiveFailures = 0;
    }
    DiagnosticLog::getInstance().add("Receiver set to %s", ip.length() > 0 ? ip.c_str() : "(none)");

    // Poll the new receiver right away instead of waiting out the current interval.
    if (_task != nullptr) {
        xTaskNotifyGive(_task);
    }
}

bool ReceiverMonitor::latestStatus(const String& receiverIp, MarantzStatus& status) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_hasStatus || receiverIp != _receiverIp) {
        return false;
    }

    status = _status;
    return true;
}

ReceiverMonitor::Diagnostics ReceiverMonitor::diagnostics() {
    std::lock_guard<std::mutex> lock(_mutex);
    Diagnostics result;
    result.receiverIp = _receiverIp;
    result.hasStatus = _hasStatus;
    result.status = _status;
    result.lastPollAtMs = _lastPollAtMs;
    result.consecutiveFailures = _consecutiveFailures;
    return result;
}

void ReceiverMonitor::taskEntry(void* parameter) {
    static_cast<ReceiverMonitor*>(parameter)->run();
}

void ReceiverMonitor::run() {
    for (;;) {
        String receiverIp = currentReceiverIp();
        if (receiverIp.length() > 0 && WiFiManager::getInstance().isConnected()) {
            storeStatus(receiverIp, MarantzClient::getInstance().fetchStatus(receiverIp));
        } else {
            clearStatus();
        }

        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(PollIntervalMs));
    }
}

String ReceiverMonitor::currentReceiverIp() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _receiverIp;
}

void ReceiverMonitor::storeStatus(const String& receiverIp, const MarantzStatus& status) {
    std::lock_guard<std::mutex> lock(_mutex);
    // Drop results for a receiver that was replaced while the request was in flight.
    if (receiverIp != _receiverIp) {
        return;
    }

    // Log only transitions so a receiver that stays down does not flush older events.
    if (!status.isValid && _consecutiveFailures == 0) {
        DiagnosticLog::getInstance().add("Receiver %s poll failed: %d %s",
                                         receiverIp.c_str(),
                                         status.httpCode,
                                         status.httpCode < 0 ? HTTPClient::errorToString(status.httpCode).c_str()
                                                             : "unexpected HTTP status");
    } else if (status.isValid && _consecutiveFailures > 0) {
        DiagnosticLog::getInstance().add("Receiver %s reachable again after %lu failed polls",
                                         receiverIp.c_str(),
                                         static_cast<unsigned long>(_consecutiveFailures));
    }

    _consecutiveFailures = status.isValid ? 0 : _consecutiveFailures + 1;
    _status = status;
    _hasStatus = true;
    _lastPollAtMs = millis();
}

void ReceiverMonitor::clearStatus() {
    std::lock_guard<std::mutex> lock(_mutex);
    _hasStatus = false;
}
