# Implementation Plan: Hardware Integration Spike (Hello World)

**Branch**: `001-hardware-spike` | **Date**: 2026-05-24 | **Spec**: [spec.md](spec.md)

## Summary

This spike verifies the core hardware components (ESP32, ST7796 TFT, XPT2046 Touch) and the build environment (PlatformIO, TFT_eSPI). We will implement a simple standalone `main.cpp` that renders "Hello World" and changes text color upon touch.

## Technical Context

**Language/Version**: C++/Arduino (PlatformIO)

**Primary Dependencies**:

- `TFT_eSPI` (Display Driver)
- `XPT2046_Touchscreen` (Touch Driver)

**Storage**: N/A (RAM only for this spike)

**Testing**: Visual verification on physical hardware.

**Target Platform**: Elegoo ESP32 DevKit V1 (ESP32-WROOM-32)

**Project Type**: Hardware Integration Spike

**Performance Goals**: < 2s boot to "Hello World".

**Constraints**: Must use existing `platformio.ini` build flags for pin mapping.

**Structure Decision**: Single project, raw `main.cpp` for isolation.

## Constitution Check

- **I. Visibility-First UI**: "Hello World" will use a large font (Font 4 or 6) on a black background.
- **II. Passive Appliance Stability**: Minimal logic to ensure fast boot and stability.
- **III. Strict Layered Architecture**: **WAIVED** for this spike to keep it as a "Hello World" in `main.cpp`.
- **IV. Deterministic Dependency Management**: Uses `platformio.ini` for all settings; no library hacks.

## Project Structure

### Documentation (this feature)

```text
specs/001-hardware-spike/
├── spec.md              # Feature specification
├── plan.md              # This file
└── tasks.md             # Implementation tasks
```

### Source Code (repository root)

```text
src/
└── main.cpp             # Hardware spike implementation (temporary)
```

## Complexity Tracking

| Violation      | Why Needed                                                    | Simpler Alternative Rejected Because                                  |
| -------------- | ------------------------------------------------------------- | --------------------------------------------------------------------- |
| Raw `main.cpp` | Ensure no architectural overhead during hardware verification | Using `ScreenManager` adds complexity that might mask hardware issues |
