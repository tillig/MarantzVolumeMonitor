#ifndef RECEIVER_MONITOR_H
#define RECEIVER_MONITOR_H

#include <Arduino.h>
#include <mutex>

#include "MarantzClient.h"

// Polls receiver status on a background task so slow or unreachable receivers never block touch handling.
class ReceiverMonitor {
public:
    static ReceiverMonitor& getInstance() {
        static ReceiverMonitor instance;
        return instance;
    }

    void begin();
    void setReceiverIp(const String& ip);
    bool latestStatus(const String& receiverIp, MarantzStatus& status);

private:
    ReceiverMonitor() {}

    static constexpr uint32_t PollIntervalMs = 1000;
    static constexpr uint32_t TaskStackBytes = 8192;

    std::mutex _mutex;
    TaskHandle_t _task = nullptr;
    String _receiverIp;
    MarantzStatus _status;
    bool _hasStatus = false;

    static void taskEntry(void* parameter);
    void run();
    String currentReceiverIp();
    void storeStatus(const String& receiverIp, const MarantzStatus& status);
    void clearStatus();
};

#endif
