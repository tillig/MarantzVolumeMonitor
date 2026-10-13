#ifndef RECEIVER_MONITOR_H
#define RECEIVER_MONITOR_H

#include <Arduino.h>
#include <mutex>

#include "MarantzClient.h"

// Polls receiver status on a background task so status fetches never block the UI loop. Polling pauses while no
// screen reads status.
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
    // False until a poll of the current receiver completes. Failed polls keep the last good result until it is older
    // than StaleAfterMs, so brief refusals do not flap the display; after that the result comes back invalid.
    bool latestStatus(MarantzStatus& status);
    // Changes whenever a poll completes, so readers can react to new results without polling the monitor's cadence.
    uint32_t statusSequence();
    Diagnostics diagnostics();

private:
    ReceiverMonitor() {}

    static constexpr uint32_t PollIntervalMs = 1000;
    static constexpr uint32_t IdleAfterMs = 3000;
    static constexpr uint32_t StaleAfterMs = 10000;
    static constexpr uint32_t RetryDelayMs = 200;
    static constexpr uint32_t TaskStackBytes = 8192;

    std::mutex _mutex;
    TaskHandle_t _task = nullptr;
    String _receiverIp;
    MarantzStatus _status;
    bool _hasStatus = false;
    uint32_t _statusAtMs = 0; // Last result stored; failures inside the stale window leave it at the last good poll.
    uint32_t _statusSequence = 0;
    uint32_t _lastReadAtMs = 0;
    bool _polling = false;
    uint32_t _consecutiveFailures = 0;

    static void taskEntry(void* parameter);
    void run();
    bool beginCycle(String& receiverIp);
    void storeStatus(const String& receiverIp, const MarantzStatus& status, uint32_t pollMs);
    void clearStatus();
    void wake();
};

#endif
