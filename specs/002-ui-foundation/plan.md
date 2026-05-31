# Implementation Plan: UI Foundation Integration

**Branch**: `002-ui-foundation` | **Date**: 2026-05-31 | **Spec**: [spec.md](spec.md)

## Summary

This feature transitions the project from a raw hardware spike to its formal layered architecture. We will encapsulate the verified ESP32 hardware configurations within `DisplayManager` and `TouchManager`, implement the `HomeScreen` UI class, and refactor `main.cpp` to use the `ScreenManager` for UI orchestration.

**Visual Iteration**: To support hardware-in-the-loop design, the `HomeScreen` implementation will include a "Prototyping Mode" allowing the user to cycle through different layout presets to verify visual appeal on the physical 4" screen.

## Technical Context

**Language/Version**: C++/Arduino (PlatformIO)

**Primary Dependencies**:

- `TFT_eSPI` (Display Driver)
- `XPT2046_Touchscreen` (Touch Driver)
- `SPI` (Communication)

**Storage**: LittleFS (ConfigStore initialization)

**Testing**: Visual hardware verification of the HomeScreen and touch responsiveness.

**Target Platform**: Elegoo ESP32 DevKit V1 (ESP32-WROOM-32)

**Project Type**: IoT Appliance

**Performance Goals**: < 2s boot time.

**Constraints**: Must adhere to VSPI pin mapping as verified in the spike.

## Constitution Check

- **I. Visibility-First UI**: `DisplayManager` defines the #101010 background and standard color palette. `HomeScreen` will use these constants.
- **II. Passive Appliance Stability**: `TouchManager` uses non-blocking `touched()` calls. `main.cpp` loop remains responsive.
- **III. Strict Layered Architecture**: **CRITICAL FOCUS**. Moving all `TFT_eSPI` and `XPT2046` calls out of `main.cpp` into Managers. `HomeScreen` only knows about the Managers.
- **IV. Deterministic Dependency Management**: Pin mappings are defined as build flags in `platformio.ini`.

## Project Structure

### Documentation (this feature)

```text
specs/002-ui-foundation/
├── spec.md              # Feature specification
├── plan.md              # This file
└── tasks.md             # Implementation tasks
```

### Source Code (repository root)

```text
src/
├── main.cpp             # Entry point (refactored to use managers)
└── ui/
    ├── DisplayManager.* # ESP32 TFT Init
    ├── TouchManager.*   # ESP32 Touch Init + Mapping
    ├── ScreenManager.*  # UI Orchestration
    └── Screens/
        ├── Screen.h     # Base class
        └── HomeScreen.* # UI Implementation
```

**Structure Decision**: Single project structure using the established `src/ui` layout.

## Complexity Tracking

| Violation | Why Needed | Simpler Alternative Rejected Because                              |
| --------- | ---------- | ----------------------------------------------------------------- |
| N/A       | N/A        | This feature reduces complexity by formalizing the architecture.  |
