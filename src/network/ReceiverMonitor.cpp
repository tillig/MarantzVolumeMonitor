#include "ReceiverMonitor.h"

#include "WiFiManager.h"

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
    }

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

    _status = status;
    _hasStatus = true;
}

void ReceiverMonitor::clearStatus() {
    std::lock_guard<std::mutex> lock(_mutex);
    _hasStatus = false;
}
