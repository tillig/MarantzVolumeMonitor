#include "DiagnosticLog.h"

#include <stdarg.h>

void DiagnosticLog::add(const char* format, ...) {
    char message[MaxMessageLength + 1];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    uint32_t now = millis();
    Serial.printf("[%lu] %s\n", static_cast<unsigned long>(now), message);

    std::lock_guard<std::mutex> lock(_mutex);
    _entries[_next].uptimeMs = now;
    _entries[_next].message = message;
    _next = (_next + 1) % Capacity;
    _totalCount++;
}

std::vector<DiagnosticLog::Entry> DiagnosticLog::entries() {
    std::lock_guard<std::mutex> lock(_mutex);
    size_t count = _totalCount < Capacity ? _totalCount : Capacity;
    size_t start = (_next + Capacity - count) % Capacity;

    std::vector<Entry> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        result.push_back(_entries[(start + i) % Capacity]);
    }
    return result;
}

uint32_t DiagnosticLog::totalCount() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _totalCount;
}
