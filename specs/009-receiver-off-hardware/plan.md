# Implementation Plan: Receiver Off Hardware Handling

**Branch**: `feature/009-receiver-off-hardware` | **Date**: 2026-06-06 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/009-receiver-off-hardware/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/plan-template.md` for the execution workflow.

## Summary

Add hardware backlight handling to the existing receiver-off screen sleep flow. When Home reaches the receiver-off blank state, firmware will drive a documented GPIO13 backlight-control signal so a canonical high-side switch turns off only the TFT `LED/BL` backlight feed. TFT logic, touch, ESP32 power, receiver polling, wake touch, Settings access, and software-only fallback behavior remain unchanged. USB power continues to enter through the ESP32, with TFT/touch and backlight-switch load power supplied from ESP32 power pins rather than a separate direct supply. The feature also requires durable hardware documentation with a bill of materials, connection map, safety warnings, validation steps, and last-resort modification limits.

## Technical Context

**Language/Version**: C++/Arduino using PlatformIO for ESP32.

**Primary Dependencies**: Existing `TFT_eSPI` display driver, existing `XPT2046_Touchscreen` input via `TouchManager`, existing `DisplayManager`, existing `HomeScreen` receiver-off state, existing `ScreenManager`, existing `MarantzClient` receiver status polling, and existing `ConfigStore`.

**Storage**: No new persistent storage. Hardware capability is determined by wiring and validation, not by Settings, saved preferences, or separate firmware builds.

**Testing**: `pre-commit run --all-files`, `git diff --check`, `/Users/travis.illig/.platformio/penv/bin/platformio run`, plus hardware verification on both software-only wiring and the hardware-capable backlight circuit.

**Target Platform**: Elegoo ESP32 DevKit V1 with 4" ST7796 SPI TFT and XPT2046 touch controller.

**Project Type**: Embedded touchscreen appliance with one local firmware target.

**Performance Goals**: Receiver-off visible timeout remains 3 seconds with +/-0.5 second tolerance; backlight off/on transitions occur within 0.5 seconds of display state changes; receiver polling continues on the existing 1-second Home refresh cadence; touch wake remains first-touch responsive without triggering Settings in the same gesture.

**Constraints**: Control only the TFT backlight/illumination path; do not switch TFT `VCC`, touch power, ESP32 power, or receiver power. Use one canonical non-destructive inline circuit. Firmware always drives the documented control signal; unmodified hardware must remain safe when the signal is unconnected. Preserve the single USB 5V 2A supply plugged into the ESP32, and route display/touch and backlight-switch load power from ESP32 power pins rather than a separate direct supply. No new runtime dependency, storage schema, receiver protocol, or Settings toggle.

**Scale/Scope**: One ESP32 device, one optional hardware wiring upgrade, one Home receiver-off flow. Primary code changes are in `src/ui/DisplayManager.*`, `src/ui/Screens/HomeScreen.*`, and `platformio.ini` if a tracked backlight-control pin define is used. Durable documentation changes are required in `docs/hardware.md` and `README.md`; `docs/ui-reference.md` should mention that hardware backlight control replaces software-only blanking when wired.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Visibility-First UI**: PASS. Live volume UI remains unchanged; receiver-off idle becomes less visually distracting by removing backlight glow after the existing visible confirmation period.
- **II. Passive Appliance Stability**: PASS. The design preserves non-blocking polling, touch wake, and responsive Settings access. It does not power-cycle TFT logic or touch.
- **III. Strict Layered Architecture**: PASS. `HomeScreen` owns receiver-off presentation state; `DisplayManager` owns hardware display/backlight control; networking and storage remain unchanged.
- **IV. Deterministic Dependency Management**: PASS. No new libraries are required. Hardware-specific pin configuration remains tracked in repository source/build configuration.
- **V. Intuitive On-Device Setup**: PASS. Settings remains reachable after wake and no new hidden setup requirement is introduced.

**Gate Status**: PASS. No constitution violations identified.

## Project Structure

### Documentation (this feature)

```text
specs/009-receiver-off-hardware/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   ├── backlight_control_contract.md
│   ├── hardware_upgrade_guide_contract.md
│   └── validation_contract.md
└── tasks.md
```

### Source Code (repository root)

```text
src/
├── main.cpp                              # Existing startup order remains; DisplayManager begins before Home
├── ui/
│   ├── DisplayManager.h/.cpp             # Backlight-control pin initialization and on/off API
│   ├── ScreenManager.h/.cpp              # Existing navigation ownership; no new global sleep owner
│   ├── TouchManager.h/.cpp               # Existing touch wake path remains active while backlight is off
│   └── Screens/
│       ├── HomeScreen.h/.cpp             # Calls DisplayManager as receiver-off visible/blank/live states change
│       └── SettingsScreen.h/.cpp         # Existing Settings path remains visible while active
├── network/
│   └── MarantzClient.h/.cpp              # Receiver power status source; no protocol change
└── storage/
    └── ConfigStore.h/.cpp                # No hardware-control setting or storage change

docs/
├── hardware.md                           # Canonical circuit, BOM, wiring, safety, validation
└── ui-reference.md                       # Receiver-off hardware backlight behavior note

README.md                                # User-facing hardware-capable receiver-off behavior note
platformio.ini                           # Optional tracked BACKLIGHT_CONTROL_PIN define
```

**Structure Decision**: Keep receiver-off flow ownership in `HomeScreen` and hardware signal ownership in `DisplayManager`. Do not add a global power manager, Settings toggle, storage flag, or network-layer involvement for this feature.

## Documentation Alignment

- Update `docs/hardware.md` with the canonical non-destructive backlight-control circuit: Pololu Mini MOSFET Slide Switch LV item `2810` as the reference high-side switch, ESP32 GPIO13 as the documented control signal, TFT `LED/BL` as the switched load, USB power entering through the ESP32, TFT/switch power sourced from ESP32 power pins, unchanged TFT `VCC`, and explicit "do not drive the backlight directly from GPIO" warnings.
- Include a hardware guide bill of materials, part identifiers/source links, minimum ratings for equivalents, connection table, inspection steps, first-power-on steps, receiver-off validation, and rollback/fallback guidance, including a pass/fail current-budget check for the ESP32-fed power path.
- Document Adafruit STEMMA MOSFET Driver item `5648` only as a non-canonical low-side substitute for modules where an isolated backlight return path is externally accessible.
- Document `2N7000FS-ND` / onsemi `2N7000` as evaluated but rejected for this feature's trusted backlight-control path. It is not a high-side switch and lacks enough 3.3V-drive/current margin for an unknown TFT backlight load.
- Update `docs/ui-reference.md` so receiver-off blanking notes distinguish software-only blanking from hardware-capable backlight-off behavior.
- Update `README.md` with a concise user-facing note that hardware-capable builds can turn off the display backlight after confirmed receiver-off status while software-only wiring still works.
- `CONTRIBUTING.md` does not need changes because build and validation workflow commands do not change.

## Complexity Tracking

No constitution violations or exceptional complexity are required.

## Phase 0 Research Summary

See [research.md](research.md). All planning unknowns are resolved:

- Use a high-side switch on the TFT `LED/BL` feed as the canonical circuit.
- Reject 2N7000FS-ND as the planned switch; it is only potentially useful for separate bench tests when the display exposes a very low-current logic-enable input or a measured low-current isolated backlight return.
- Keep TFT logic power, touch-controller power, and ESP32 power connected.
- Preserve the current power topology: one USB 5V 2A supply plugged into the ESP32, with TFT/touch and backlight-switch load power supplied from ESP32 power pins.
- Use GPIO13 as the documented backlight-control signal because it is unused by the current SPI, touch, and setup wiring and avoids the ESP32 GPIO12 strapping-pin risk while staying physically close to the existing wiring.
- Firmware always drives the signal; software-only installs safely ignore the unconnected output.
- Prefer reversible inline wiring; destructive display-module modification is last-resort only.
- Keep hardware state ownership in `DisplayManager` and receiver-off timing ownership in `HomeScreen`.

## Phase 1 Design Summary

See [data-model.md](data-model.md), [contracts/backlight_control_contract.md](contracts/backlight_control_contract.md), [contracts/hardware_upgrade_guide_contract.md](contracts/hardware_upgrade_guide_contract.md), [contracts/validation_contract.md](contracts/validation_contract.md), and [quickstart.md](quickstart.md).

## Post-Design Constitution Check

- **I. Visibility-First UI**: PASS. The feature preserves live volume readability and reduces idle receiver-off glow after a visible confirmation period.
- **II. Passive Appliance Stability**: PASS. Contracts require backlight-only control, continued polling, touch wake, and software-only fallback.
- **III. Strict Layered Architecture**: PASS. UI state calls a display-layer backlight API; networking and storage remain separate.
- **IV. Deterministic Dependency Management**: PASS. No new dependencies are introduced; hardware configuration remains tracked in code/docs.
- **V. Intuitive On-Device Setup**: PASS. Wake and Settings remain available without adding a configuration toggle.

**Gate Status**: PASS. No constitution violations identified after Phase 1 design.
