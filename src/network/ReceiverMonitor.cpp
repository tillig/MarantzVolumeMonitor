#include "ReceiverMonitor.h"

#include "WiFiManager.h"

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
    }

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
        hasStatus = _hasStatus;
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

void ReceiverMonitor::taskEntry(void* parameter) {
    static_cast<ReceiverMonitor*>(parameter)->run();
}

void ReceiverMonitor::run() {
    for (;;) {
        String receiverIp;
        bool polling = beginCycle(receiverIp);
        if (polling && receiverIp.length() > 0 && WiFiManager::getInstance().isConnected()) {
            storeStatus(receiverIp, MarantzClient::getInstance().fetchStatus(receiverIp));
        } else {
            clearStatus();
        }

        // While idle, sleep until a reader or a receiver change wakes the task.
        ulTaskNotifyTake(pdTRUE, polling ? pdMS_TO_TICKS(PollIntervalMs) : portMAX_DELAY);
    }
}

bool ReceiverMonitor::beginCycle(String& receiverIp) {
    std::lock_guard<std::mutex> lock(_mutex);
    receiverIp = _receiverIp;
    _polling = millis() - _lastReadAtMs < IdleAfterMs;
    return _polling;
}

void ReceiverMonitor::storeStatus(const String& receiverIp, const MarantzStatus& status) {
    std::lock_guard<std::mutex> lock(_mutex);
    // Drop results for a receiver that was replaced while the request was in flight.
    if (receiverIp != _receiverIp) {
        return;
    }

    _status = status;
    _hasStatus = true;
    _statusAtMs = millis();
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
