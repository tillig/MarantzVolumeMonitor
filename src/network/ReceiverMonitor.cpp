#include "ReceiverMonitor.h"

#include <HTTPClient.h>

#include "WiFiManager.h"
#include "../diagnostics/DiagnosticLog.h"

void ReceiverMonitor::begin() {
    if (_task != nullptr) {
        return;
    }

    if (xTaskCreate(taskEntry, "receiver-monitor", TaskStackBytes, this, 1, &_task) != pdPASS) {
        _task = nullptr;
        Serial.println("RECEIVER_MONITOR task creation failed; receiver status will stay unavailable");
    }
}

void ReceiverMonitor::setReceiverIp(const String& ip) {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (ip == _receiverIp) {
            return;
        }
        _receiverIp = ip;
        _hasStatus = false;
        _statusSequence++;
        _consecutiveFailures = 0;
    }
    DiagnosticLog::getInstance().add("Receiver set to %s", ip.length() > 0 ? ip.c_str() : "(none)");

    // Poll the new receiver right away instead of waiting out the current interval.
    wake();
}

bool ReceiverMonitor::latestStatus(MarantzStatus& status) {
    bool wasIdle;
    bool hasStatus;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        uint32_t now = millis();
        _lastReadAtMs = now;
        wasIdle = !_polling;
        // An idle monitor's last result is too old to show, so readers wait for a fresh poll.
        hasStatus = _hasStatus && !wasIdle;
        if (hasStatus) {
            // A fetch stuck on an unresponsive receiver must not keep its last answer on screen.
            status = now - _statusAtMs > StaleAfterMs ? MarantzStatus() : _status;
        }
    }

    if (wasIdle) {
        wake();
    }
    return hasStatus;
}

uint32_t ReceiverMonitor::statusSequence() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _statusSequence;
}

ReceiverMonitor::Diagnostics ReceiverMonitor::diagnostics() {
    std::lock_guard<std::mutex> lock(_mutex);
    Diagnostics result;
    result.receiverIp = _receiverIp;
    result.hasStatus = _hasStatus;
    result.status = _status;
    result.lastPollAtMs = _statusAtMs;
    result.consecutiveFailures = _consecutiveFailures;
    return result;
}

void ReceiverMonitor::taskEntry(void* parameter) {
    static_cast<ReceiverMonitor*>(parameter)->run();
}

void ReceiverMonitor::run() {
    for (;;) {
        String receiverIp;
        bool polling = beginCycle(receiverIp);
        // While idle, the last result stays in place for diagnostics.
        if (polling) {
            if (receiverIp.length() > 0 && WiFiManager::getInstance().isConnected()) {
                uint32_t startedAtMs = millis();
                MarantzStatus status = MarantzClient::getInstance().fetchStatus(receiverIp);
                if (status.httpCode == HTTPC_ERROR_CONNECTION_REFUSED) {
                    // Refusals are often momentary; retry once before counting a failure.
                    vTaskDelay(pdMS_TO_TICKS(RetryDelayMs));
                    status = MarantzClient::getInstance().fetchStatus(receiverIp);
                }
                storeStatus(receiverIp, status, millis() - startedAtMs);
            } else {
                clearStatus();
            }
        }

        // While idle, sleep until a reader or a receiver change wakes the task.
        ulTaskNotifyTake(pdTRUE, polling ? pdMS_TO_TICKS(PollIntervalMs) : portMAX_DELAY);
    }
}

bool ReceiverMonitor::beginCycle(String& receiverIp) {
    std::lock_guard<std::mutex> lock(_mutex);
    receiverIp = _receiverIp;
    bool resuming = !_polling;
    _polling = millis() - _lastReadAtMs < IdleAfterMs;
    if (_polling && resuming && _hasStatus) {
        // The result from before the pause is too old to show; readers wait for this poll instead.
        _hasStatus = false;
        _statusSequence++;
    }
    return _polling;
}

void ReceiverMonitor::storeStatus(const String& receiverIp, const MarantzStatus& status, uint32_t pollMs) {
    std::lock_guard<std::mutex> lock(_mutex);
    // Drop results for a receiver that was replaced while the request was in flight.
    if (receiverIp != _receiverIp) {
        return;
    }

    // Log only transitions so a receiver that stays down does not flush older events.
    if (!status.isValid && _consecutiveFailures == 0) {
        DiagnosticLog::getInstance().add("Receiver %s poll failed: %d %s (%lu ms, RSSI %d dBm, heap %lu)",
                                         receiverIp.c_str(),
                                         status.httpCode,
                                         status.httpCode < 0    ? HTTPClient::errorToString(status.httpCode).c_str()
                                         : status.httpCode == 0 ? "request not sent"
                                                                : "unexpected HTTP status",
                                         static_cast<unsigned long>(pollMs),
                                         static_cast<int>(WiFiManager::getInstance().getSignalStrength()),
                                         static_cast<unsigned long>(ESP.getFreeHeap()));
    } else if (status.isValid && _consecutiveFailures > 0) {
        DiagnosticLog::getInstance().add("Receiver %s reachable again after %lu failed polls",
                                         receiverIp.c_str(),
                                         static_cast<unsigned long>(_consecutiveFailures));
    }

    _consecutiveFailures = status.isValid ? 0 : _consecutiveFailures + 1;

    uint32_t now = millis();
    if (!status.isValid && _hasStatus && _status.isValid && now - _statusAtMs <= StaleAfterMs) {
        // Hold the last good result through brief failures.
        return;
    }
    _status = status;
    _hasStatus = true;
    _statusAtMs = now;
    _statusSequence++;
}

void ReceiverMonitor::clearStatus() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_hasStatus) {
        _hasStatus = false;
        _statusSequence++;
    }
}

void ReceiverMonitor::wake() {
    if (_task != nullptr) {
        xTaskNotifyGive(_task);
    }
}
