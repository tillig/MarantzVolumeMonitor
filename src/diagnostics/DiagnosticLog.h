#ifndef DIAGNOSTIC_LOG_H
#define DIAGNOSTIC_LOG_H

#include <Arduino.h>
#include <mutex>
#include <vector>

// Keeps the most recent connectivity events in RAM so they can be reviewed on-device or over HTTP.
class DiagnosticLog {
public:
    struct Entry {
        uint32_t uptimeMs = 0;
        String message;
    };

    static DiagnosticLog& getInstance() {
        static DiagnosticLog instance;
        return instance;
    }

    void add(const char* format, ...) __attribute__((format(printf, 2, 3)));
    std::vector<Entry> entries();
    uint32_t totalCount();

private:
    DiagnosticLog() {}

    static constexpr size_t Capacity = 50;
    static constexpr size_t MaxMessageLength = 120;

    std::mutex _mutex;
    Entry _entries[Capacity];
    size_t _next = 0;
    uint32_t _totalCount = 0;
};

#endif
