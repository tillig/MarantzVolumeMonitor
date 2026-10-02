# Implementation Plan: Wi-Fi Resilience and Diagnostics

**Branch**: `feature/015-wifi-resilience-diagnostics` | **Date**: 2026-10-02 | **Spec**: [spec.md](spec.md)

## Summary

Arduino-ESP32 3.x does not auto-reconnect after `AUTH_FAIL` or `ASSOC_FAIL`, and `esp_wifi_set_config` rejects new credentials while a connection attempt is running. Both match the symptoms in issue #8. `WiFiManager` gains a retry supervisor and records disconnect reasons from Wi-Fi events; a new `DiagnosticLog` collects events from Wi-Fi, receiver polling, and the Home Screen; `DiagnosticsServer` and `DiagnosticsScreen` expose them.

## Technical Context

**Language/Version**: C++/Arduino on pioarduino `platform-espressif32` (Arduino-ESP32 3.x)

**Primary Dependencies**: Built-in `WiFi` and `WebServer`; `ArduinoJson` for the status endpoint.

**Testing**: Hardware verification per [tasks.md](tasks.md).

## Constitution Check

- **II. Passive Appliance Stability**: Directly addresses auto-reconnect. The HTTP server runs on its own task so browser clients cannot stall the UI loop.
- **III. Strict Layered Architecture**: Wi-Fi retry and HTTP serving live in the network layer; `DiagnosticsScreen` only reads.
- **V. Intuitive On-Device Setup**: The failure reason and diagnostics are visible on the touchscreen; HTTP is optional.

## Design Notes

- Every connection attempt calls `WiFi.disconnect(true)` before `WiFi.begin()`, so a stuck attempt cannot block the next one.
- The event handler ignores `ASSOC_LEAVE`, which the firmware itself causes, so the last reported reason is a real failure.
- `DiagnosticLog` and `ReceiverMonitor` are mutex-protected because the Wi-Fi event task, receiver poller, HTTP task, and UI loop all touch them.
