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

    struct Diagnostics {
        String receiverIp;
        bool hasStatus = false;
        MarantzStatus status;
        uint32_t lastPollAtMs = 0;
        uint32_t consecutiveFailures = 0;
    };

    void begin();
    void setReceiverIp(const String& ip);
    bool latestStatus(const String& receiverIp, MarantzStatus& status);
    Diagnostics diagnostics();

private:
    ReceiverMonitor() {}

    static constexpr uint32_t PollIntervalMs = 1000;
    static constexpr uint32_t TaskStackBytes = 8192;

    std::mutex _mutex;
    TaskHandle_t _task = nullptr;
    String _receiverIp;
    MarantzStatus _status;
    bool _hasStatus = false;
    uint32_t _lastPollAtMs = 0;
    uint32_t _consecutiveFailures = 0;

    static void taskEntry(void* parameter);
    void run();
    String currentReceiverIp();
    void storeStatus(const String& receiverIp, const MarantzStatus& status);
    void clearStatus();
};

#endif
