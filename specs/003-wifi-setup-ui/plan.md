# Implementation Plan: WiFi Setup UI

**Branch**: `feature/003-wifi-setup-ui` | **Date**: 2026-06-02 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/003-wifi-setup-ui/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command.

## Summary

Implement a touch-driven WiFi configuration flow for the ESP32. This feature includes non-blocking network scanning, a scrollable SSID list, and a custom on-screen QWERTY keyboard for password entry. Discovered credentials will be persisted in `config.json` via `LittleFS` using the existing `ConfigStore` architecture.

## Technical Context

**Language/Version**: C++/Arduino (PlatformIO)

**Primary Dependencies**: `WiFi.h`, `LittleFS`, `TFT_eSPI`, `XPT2046_Touchscreen`, `ArduinoJson`

**Storage**: `config.json` via `LittleFS`

**Testing**: Hardware-in-the-loop verification of scan results and connection success.

**Target Platform**: Elegoo ESP32 DevKit V1

**Project Type**: Embedded Appliance

**Performance Goals**: Scan results < 5s, Connect feedback < 2s.

**Constraints**: Keyboard keys MUST be >= 40x40px for touch accuracy. UI MUST remain non-blocking during radio activity.

## Constitution Check

- **I. Visibility-First UI**: Keyboard and network list will use high-contrast white-on-black theme (#101010).
- **II. Passive Appliance Stability**: WiFi scanning is implemented as an asynchronous task to prevent UI freezing.
- **III. Strict Layered Architecture**: All WiFi radio logic remains in `WiFiManager`; UI logic remains in `NetworkListScreen` and `KeyboardScreen`.
- **IV. Deterministic Dependency Management**: Standard ESP32 libraries used; no manual modifications required.
- **V. Intuitive On-Device Setup**: Entire flow triggered by tapping the "UNCONFIGURED" area on the Home Screen.

## Project Structure

### Documentation (this feature)

```text
specs/003-wifi-setup-ui/
├── plan.md              # This file
├── research.md          # Keyboard and Scan decisions
├── data-model.md        # WiFi and Credential entities
├── quickstart.md        # How to test the flow
└── contracts/           # config.json schema
```

### Source Code (repository root)

```text
src/
├── network/
│   └── WiFiManager.*    # Updated for scan support
├── storage/
│   └── ConfigStore.*    # Updated for useDbScale
└── ui/
    └── Screens/
        ├── NetworkListScreen.* # NEW: SSID Selection
        ├── KeyboardScreen.*    # NEW: Password Entry
        └── SetupStatusScreen.* # NEW: Connection Feedback
```

**Structure Decision**: Integrated into existing `src/` layers.

## Complexity Tracking

| Violation                  | Why Needed         | Simpler Alternative Rejected Because |
| -------------------------- | ------------------ | ------------------------------------ |
| N/A                        | N/A                | Follows existing architecture.       |
