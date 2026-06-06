# Implementation Plan: Receiver Off Screen Sleep

**Branch**: `feature/008-receiver-off-status` | **Date**: 2026-06-05 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/008-receiver-off-status/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See
`.specify/templates/plan-template.md` for the execution workflow.

## Summary

Refine the home receiver-off experience so a confirmed powered-off receiver shows `Receiver off` for
3 seconds, then software-blanks the monitor display while continuing receiver polling and touch
handling. A touch in the blank state wakes back to the current visible state, usually `Receiver off`;
Settings and settings-launched setup flows remain visible and usable until the user returns Home.

## Technical Context

**Language/Version**: C++/Arduino using PlatformIO for ESP32.

**Primary Dependencies**: Existing `TFT_eSPI` drawing, existing `XPT2046_Touchscreen` input via
`TouchManager`, existing `ScreenManager`, existing `HomeScreen`, existing `SettingsScreen`, existing
`MarantzClient` receiver status polling, `WiFiManager`, and `ConfigStore`.

**Storage**: No new persistent storage. Existing Wi-Fi and receiver configuration storage remains
unchanged.

**Testing**: `pre-commit run --all-files`, `git diff --check`,
`/Users/travis.illig/.platformio/penv/bin/platformio run`, and hardware visual/touch verification on
the 480x320 touchscreen.

**Target Platform**: Elegoo ESP32 DevKit V1 with 4" ST7796 TFT and XPT2046 touch controller.

**Project Type**: Embedded touchscreen appliance.

**Performance Goals**: Receiver-off timeout starts at 3 seconds with +/-0.5 second tolerance; blank
state must not block the existing 1-second home-screen polling cycle; first touch in the blank state
wakes the monitor without triggering a second action; visible receiver-off non-Settings touches reset
the 3-second timeout; Settings remains visible for at least 60 seconds while the receiver stays off.

**Constraints**: Hardware backlight switching is out of scope; screen-off means a software blank to
black. No receiver power-control behavior. Screens must not own network I/O beyond existing manager
delegation. Preserve normal live-volume display, receiver-unavailable display, setup-required flows,
and Settings return behavior.

**Scale/Scope**: Primarily `src/ui/Screens/HomeScreen.h/.cpp`, with durable behavior documentation in
`docs/ui-reference.md` and `README.md`. No network protocol, storage schema, hardware wiring, or
dependency changes.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Visibility-First UI**: PASS. Live volume remains unchanged; receiver-off blanking reduces
  idle room distraction and avoids stale live values.
- **II. Passive Appliance Stability**: PASS. The design preserves non-blocking polling, keeps touch
  responsive, and avoids new delays in the main loop.
- **III. Strict Layered Architecture**: PASS. Receiver status remains in `MarantzClient`; display
  state and wake behavior remain in the UI layer.
- **IV. Deterministic Dependency Management**: PASS. No new dependency or hardware configuration is
  required.
- **V. Intuitive On-Device Setup**: PASS. Settings remains reachable after wake, and settings flows
  remain visible while the user works.

**Gate Status**: PASS. No constitution violations identified.

## Project Structure

### Documentation (this feature)

```text
specs/008-receiver-off-status/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   ├── receiver_off_display_cycle.md
│   ├── settings_wake_contract.md
│   └── validation_contract.md
└── tasks.md
```

### Source Code (repository root)

```text
src/
├── ui/
│   ├── ScreenManager.h/.cpp          # Existing navigation ownership
│   ├── TouchManager.h/.cpp           # Existing touch input and calibration
│   └── Screens/
│       ├── HomeScreen.h/.cpp         # Receiver-off visible/blank state and wake handling
│       └── SettingsScreen.h/.cpp     # Existing Settings return path remains visible while active
└── network/
    └── MarantzClient.h/.cpp          # Existing receiver status source; no protocol changes

docs/
└── ui-reference.md                   # Durable receiver-off blank/wake guidance

README.md                             # User-facing receiver-off behavior note
```

**Structure Decision**: Keep the feature centered in `HomeScreen` because the behavior is a
home-display presentation state. Do not add a hardware/backlight abstraction or network/storage
changes in this feature.

## Documentation Alignment

- Update `docs/ui-reference.md` with durable guidance for receiver-off visible timeout, software
  blank state, touch-to-wake behavior, non-Settings touches resetting the visible timeout, and
  Settings staying visible while active.
- Update `README.md` with a short user-facing note explaining that confirmed receiver-off status
  briefly appears, then the monitor blanks until tapped.
- Keep hardware backlight switching explicitly out of this feature and defer any circuit/wiring
  guidance to a separate hardware feature.
- `CONTRIBUTING.md` does not need changes because validation commands and contribution workflow do
  not change.

## Complexity Tracking

No constitution violations or exceptional complexity are required.

## Phase 0 Research Summary

See [research.md](research.md). All planning unknowns are resolved:

- Use a software blank state for this feature and defer physical backlight switching.
- Keep receiver-off visible/blank state in `HomeScreen`.
- Treat wake touch as wake-only when the display is blank.
- Continue normal receiver status polling while blanked.
- Stay blank if receiver-off confirmation is lost after the monitor already blanked, until touch or
  active receiver status.
- Reset the 3-second timer on non-Settings touches while the visible receiver-off screen is active.
- Keep Settings and settings-launched flows outside the receiver-off timeout.

## Phase 1 Design Summary

See [data-model.md](data-model.md), [contracts/receiver_off_display_cycle.md](contracts/receiver_off_display_cycle.md),
[contracts/settings_wake_contract.md](contracts/settings_wake_contract.md),
[contracts/validation_contract.md](contracts/validation_contract.md), and [quickstart.md](quickstart.md).

## Post-Design Constitution Check

- **I. Visibility-First UI**: PASS. Live volume remains unchanged, and receiver-off blanking reduces
  nonessential room-visible content.
- **II. Passive Appliance Stability**: PASS. Contracts require non-blocking timing, continued polling,
  and touch wake without extra blocking work.
- **III. Strict Layered Architecture**: PASS. UI presentation owns blank/wake behavior; network status
  still comes from existing managers.
- **IV. Deterministic Dependency Management**: PASS. No new dependency or platform configuration is
  introduced.
- **V. Intuitive On-Device Setup**: PASS. Wake and Settings contracts preserve a reachable
  configuration path from receiver-off conditions.

**Gate Status**: PASS. No constitution violations identified after Phase 1 design.
