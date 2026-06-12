# Implementation Plan: Volume Display Scale

**Branch**: `feature/011-volume-scale-setting` | **Date**: 2026-06-12 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/011-volume-scale-setting/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/plan-template.md` for the execution workflow.

## Summary

Add a new `Volume Display Scale` destination to `Settings` that opens a dedicated single-choice screen with `0-100` and `dB` options, explicit `OK` and `Cancel` actions, and a visible selected-state indicator. The plan reuses the existing persisted `useDbScale` flag in `ConfigStore`, keeps the home-screen gauge on the current normalized scale, and limits behavior changes to the home-screen numeric volume readout plus the new settings flow.

## Technical Context

**Language/Version**: C++/Arduino using PlatformIO for ESP32.

**Primary Dependencies**: Existing `TFT_eSPI` drawing, existing `XPT2046_Touchscreen` input via `TouchManager`, existing `ScreenManager`, existing `MaterialStyle`, existing `SettingsScreen`, `HomeScreen`, `MarantzClient`, and `ConfigStore`.

**Storage**: Existing LittleFS `config.json` through `ConfigStore`, reusing the already-present `useDbScale` preference field. No storage schema change is planned.

**Testing**: `pre-commit run --all-files`, `git diff --check`, `/Users/travis.illig/.platformio/penv/bin/platformio run`, plus hardware verification on the 480x320 touchscreen.

**Target Platform**: Elegoo ESP32 DevKit V1 with 4" ST7796 TFT and XPT2046 touch controller.

**Project Type**: Embedded touchscreen appliance.

**Performance Goals**: Reach `Volume Display Scale` from `Settings` in 1 selection; apply the confirmed scale by the next normal home-screen refresh; preserve the existing 1-second receiver polling cadence and live volume responsiveness; avoid introducing extra full-screen animation or flashing.

**Constraints**: Keep the gauge behavior and mapping unchanged; preserve one-decimal formatting in both scales; return to `Settings` after both `OK` and `Cancel`; use a single-choice selected-state indicator that does not rely on color alone; preserve readable labeled settings navigation; avoid new dependencies and avoid a `config.json` migration.

**Scale/Scope**: Primary code changes are expected in `src/ui/Screens/SettingsScreen.*`, a new `src/ui/Screens/VolumeScaleScreen.*`, and `src/ui/Screens/HomeScreen.*`, with shared presentation support in `src/ui/MaterialStyle.*`. `src/storage/ConfigStore.*` already contains the persisted flag and may need at most small reuse-focused adjustments. Durable documentation updates are expected in `docs/ui-reference.md` and `README.md`.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Visibility-First UI**: PASS. The feature changes only the home-screen number format, keeps one decimal place, and leaves the existing gauge hierarchy and readability intact.
- **II. Passive Appliance Stability**: PASS. The planned work reuses the current polling cadence and adds only a lightweight settings flow plus numeric-format selection.
- **III. Strict Layered Architecture**: PASS. UI screens remain responsible for presentation and navigation, `MarantzClient` remains the source of receiver dB volume, and `ConfigStore` remains the owner of persisted user preference.
- **IV. Deterministic Dependency Management**: PASS. No new libraries, build tools, or hardware configuration changes are required.
- **V. Intuitive On-Device Setup**: PASS. The feature fits the existing touch-first `Settings` model, preserves return paths to `Settings`, and does not hide critical configuration behind gestures.

**Gate Status**: PASS. No constitution violations identified.

## Project Structure

### Documentation (this feature)

```text
specs/011-volume-scale-setting/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   ├── home_volume_format_contract.md
│   ├── settings_menu_navigation_contract.md
│   ├── validation_contract.md
│   └── volume_display_scale_screen_contract.md
└── tasks.md
```

### Source Code (repository root)

```text
src/
├── network/
│   └── MarantzClient.h/.cpp             # Existing source of raw receiver dB volume
├── storage/
│   └── ConfigStore.h/.cpp               # Existing persisted useDbScale preference in config.json
└── ui/
    ├── MaterialStyle.h/.cpp             # Shared single-choice indicator and button treatment helpers
    ├── ScreenManager.h/.cpp             # Existing screen navigation ownership
    └── Screens/
        ├── HomeScreen.h/.cpp            # Home numeric volume formatting switch, gauge unchanged
        ├── SettingsScreen.h/.cpp        # New Volume Display Scale entry and navigation wiring
        └── VolumeScaleScreen.h/.cpp     # New dedicated selection screen with OK and Cancel

docs/
└── ui-reference.md                      # Durable volume-format and selection-screen guidance

README.md                                # User-facing note about the new Volume Display Scale setting
AGENTS.md                                # Updated plan reference between Spec Kit markers
```

**Structure Decision**: Keep the feature inside the existing embedded firmware structure. Add one new settings child screen, wire it from `SettingsScreen`, update `HomeScreen` formatting behavior, and reuse the existing config field instead of changing storage shape.

## Documentation Alignment

- Update `docs/ui-reference.md` with the `Volume Display Scale` menu entry, the dedicated single-choice screen pattern, the selected-state indicator rule, and the new home-screen numeric-format behavior.
- Update `README.md` to replace the note that `useDbScale` is not yet exposed in the UI and describe the new `Settings` path for switching between `0-100` and `dB`.
- `CONTRIBUTING.md` does not need changes because the build and validation workflow stay the same.

## Complexity Tracking

No constitution violations or exceptional complexity are required.

## Phase 0 Research Summary

See [research.md](research.md). All planning unknowns are resolved:

- Reuse the existing persisted `useDbScale` field instead of changing the config schema.
- Keep the home-screen gauge on the normalized `0-100` internal scale even when the number is shown in `dB`.
- Use a dedicated selection screen with a radio-style single-choice indicator plus explicit `OK` and `Cancel` actions.
- Return to `Settings` after both `OK` and `Cancel`, with `Cancel` discarding pending choice changes.

## Phase 1 Design Summary

See [data-model.md](data-model.md), [contracts/settings_menu_navigation_contract.md](contracts/settings_menu_navigation_contract.md), [contracts/volume_display_scale_screen_contract.md](contracts/volume_display_scale_screen_contract.md), [contracts/home_volume_format_contract.md](contracts/home_volume_format_contract.md), [contracts/validation_contract.md](contracts/validation_contract.md), and [quickstart.md](quickstart.md).

## Post-Design Constitution Check

- **I. Visibility-First UI**: PASS. The design keeps the prominent volume presentation stable and changes only the numeric scale label and value formatting.
- **II. Passive Appliance Stability**: PASS. The design does not add new polling loops or background tasks beyond the existing home refresh behavior.
- **III. Strict Layered Architecture**: PASS. Receiver data stays in the network layer, persisted preference stays in storage, and the new screen manages only UI state and confirmation flow.
- **IV. Deterministic Dependency Management**: PASS. No new dependency, build change, or driver configuration is introduced.
- **V. Intuitive On-Device Setup**: PASS. The settings flow remains explicit, reversible with `Cancel`, and consistent with the repo’s touch-first screen transitions.

**Gate Status**: PASS. No constitution violations identified after Phase 1 design.
