#ifndef DIAGNOSTICS_SERVER_H
#define DIAGNOSTICS_SERVER_H

#include <Arduino.h>
#include <WebServer.h>

// Read-only HTTP endpoints for troubleshooting without a serial cable.
class DiagnosticsServer {
public:
    static DiagnosticsServer& getInstance() {
        static DiagnosticsServer instance;
        return instance;
    }

    void begin();

private:
    DiagnosticsServer() : _server(80) {}

    static constexpr uint32_t TaskStackBytes = 8192;

    WebServer _server;
    TaskHandle_t _task = nullptr;

    static void taskEntry(void* parameter);
    void handleIndex();
    void handleStatus();
    void handleLog();
    String statusJson();
    static String logText();
};

#endif
