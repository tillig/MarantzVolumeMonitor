# Implementation Plan: Touch Calibration Persistence

**Branch**: `feature/012-touch-calibration-persistence` | **Date**: 2026-06-12 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/012-touch-calibration-persistence/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/plan-template.md` for the execution workflow.

## Summary

Replace the current serial-capture-only touch calibration flow with a fully on-device calibration flow that computes and applies a guarded affine touch profile, persists it in device configuration, and keeps the existing measured coefficients as the shipped default when no user calibration exists. The plan also adds a `Reset to defaults` settings destination as the last settings item so `Wi-Fi`, `Receiver`, and `Calibration` can be reset individually with explicit confirmation, while preserving non-blocking appliance behavior and layered ownership between UI, storage, and touch input.

## Technical Context

**Language/Version**: C++/Arduino using PlatformIO for ESP32.

**Primary Dependencies**: Existing `TFT_eSPI` rendering, existing `XPT2046_Touchscreen` input via `TouchManager`, existing `ScreenManager`, existing `MaterialStyle`, existing `SettingsScreen`, existing `CalibrationScreen`, existing `HomeScreen`, existing `ArduinoJson`, and existing `ConfigStore`.

**Storage**: Existing LittleFS `/config.json` through `ConfigStore`, extended to store an optional persisted touch-calibration profile and to support selective reset of `Wi-Fi`, `Receiver`, and `Calibration` settings.

**Testing**: `pre-commit run --all-files`, `git diff --check`, `/Users/travis.illig/.platformio/penv/bin/platformio run`, plus hardware verification on the 480x320 touchscreen.

**Target Platform**: Elegoo ESP32 DevKit V1 with 4" ST7796 TFT and XPT2046 touch controller.

**Project Type**: Embedded touchscreen appliance.

**Performance Goals**: Preserve boot under 5 seconds, keep the existing roughly 1-second network refresh cadence, apply a saved or reset calibration by the current session without reboot, and avoid new full-screen flashing in live UI paths.

**Constraints**: No serial monitor or manual code edits required for normal calibration; the current measured affine coefficients remain the shipped default; calibration must reject out-of-bounds, duplicate, and suspicious samples; reset operations must affect only the selected settings type; no new third-party libraries; UI changes must remain touch-first and readable on the 480x320 screen.

**Scale/Scope**: Primary code changes are expected in `src/storage/ConfigStore.*`, `src/ui/TouchManager.*`, `src/ui/Screens/CalibrationScreen.*`, `src/ui/Screens/SettingsScreen.*`, `src/main.cpp`, and one new settings child screen for reset operations. Durable documentation updates are expected in `README.md`, `docs/hardware.md`, `docs/ui-reference.md`, and `docs/architecture.md`.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Visibility-First UI**: PASS. The feature keeps calibration and reset flows inside explicit settings/setup screens without disturbing the stable home-screen hierarchy.
- **II. Passive Appliance Stability**: PASS. The plan keeps calibration local, avoids new background work, and applies persisted touch settings at startup without adding network or blocking dependencies.
- **III. Strict Layered Architecture**: PASS. `TouchManager` owns runtime calibration math, `ConfigStore` owns persisted profiles and selective reset data, and screens own UI guidance, capture sequencing, and confirmation flow.
- **IV. Deterministic Dependency Management**: PASS. The design uses the existing Arduino/PlatformIO stack and does not add external math or storage libraries.
- **V. Intuitive On-Device Setup**: PASS. Calibration becomes fully touch-driven on the device, and reset recovery remains visible and explicit inside `Settings`.

**Gate Status**: PASS. No constitution violations identified.

## Project Structure

### Documentation (this feature)

```text
specs/012-touch-calibration-persistence/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   ├── calibration_session_contract.md
│   ├── config_persistence_contract.md
│   ├── settings_reset_defaults_contract.md
│   └── validation_contract.md
└── tasks.md
```

### Source Code (repository root)

```text
src/
├── main.cpp                              # Load saved calibration at boot after config load
├── storage/
│   └── ConfigStore.h/.cpp               # Persist calibration profile and selective reset behavior
└── ui/
    ├── MaterialStyle.h/.cpp             # Reused list-row, status, and button helpers
    ├── ScreenManager.h/.cpp             # Existing screen transitions
    ├── TouchManager.h/.cpp              # Default coefficients, saved-profile application, runtime reset
    └── Screens/
        ├── CalibrationScreen.h/.cpp     # Guided on-device calibration, guarded sample capture, save/apply result
        ├── HomeScreen.h/.cpp            # Existing setup-path calibration entry remains available
        ├── SettingsScreen.h/.cpp        # Add Reset to defaults as last settings entry
        └── ResetDefaultsScreen.h/.cpp   # New settings child flow for selective reset and confirmation

docs/
├── architecture.md                      # Persisted config schema and settings navigation ownership
├── hardware.md                          # Calibration behavior and shipped-default guidance
└── ui-reference.md                      # Settings ordering, reset flow, and calibration interaction rules

README.md                                # User-facing setup, calibration, and reset behavior
AGENTS.md                                # Updated plan reference between Spec Kit markers
```

**Structure Decision**: Keep the feature inside the existing embedded firmware layout. Extend storage and touch runtime ownership where they already belong, update existing calibration and settings screens, and add one new settings child screen for selective reset instead of scattering reset logic across unrelated screens.

## Documentation Alignment

- Update `README.md` to replace the serial-console calibration description with on-device calibration, persistence, and selective reset guidance.
- Update `docs/hardware.md` to describe the shipped default calibration baseline, on-device recalibration behavior, and the rule that invalid saved calibration falls back to the baseline instead of requiring source edits.
- Update `docs/ui-reference.md` to document the new settings order, `Reset to defaults` as the last settings item, guarded calibration capture behavior, and reset confirmation flow.
- Update `docs/architecture.md` to extend the persisted configuration schema and note the added reset/calibration screen ownership.
- `CONTRIBUTING.md` does not need changes because the build and validation workflow remain the same.

## Complexity Tracking

No constitution violations or exceptional complexity are required.

## Phase 0 Research Summary

See [research.md](research.md). All planning unknowns are resolved:

- Persist the user calibration as an optional affine profile in `config.json`, while keeping the current coefficients as the shipped default.
- Load the saved profile at boot and allow calibration/reset flows to apply runtime changes immediately through `TouchManager`.
- Guard calibration with per-target capture boundaries, release-before-next-step behavior, and duplicate/suspicious sample rejection.
- Add a dedicated `Reset to defaults` settings child flow with explicit per-type confirmation instead of a calibration-only or full-wipe reset path.
- Compute the affine fit on-device from the existing 9-point flow so calibration no longer depends on serial output or manual code edits.

## Phase 1 Design Summary

See [data-model.md](data-model.md), [contracts/calibration_session_contract.md](contracts/calibration_session_contract.md), [contracts/config_persistence_contract.md](contracts/config_persistence_contract.md), [contracts/settings_reset_defaults_contract.md](contracts/settings_reset_defaults_contract.md), [contracts/validation_contract.md](contracts/validation_contract.md), and [quickstart.md](quickstart.md).

## Post-Design Constitution Check

- **I. Visibility-First UI**: PASS. The design contains new behavior inside setup/settings flows and keeps home-screen readability unchanged.
- **II. Passive Appliance Stability**: PASS. The design applies saved calibration locally at boot and uses guarded screen state instead of introducing new polling or external-tool dependencies.
- **III. Strict Layered Architecture**: PASS. Calibration math stays in `TouchManager`, persistence and selective reset stay in `ConfigStore`, and UI screens orchestrate only presentation and user intent.
- **IV. Deterministic Dependency Management**: PASS. The design relies only on the existing repository toolchain and libraries.
- **V. Intuitive On-Device Setup**: PASS. Critical calibration and recovery paths become fully on-device and reversible through visible UI.

**Gate Status**: PASS. No constitution violations identified after Phase 1 design.
