# Implementation Plan: Display Current Settings

**Branch**: `feature/010-display-current-settings` | **Date**: 2026-06-11 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/010-display-current-settings/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/plan-template.md` for the execution workflow.

## Summary

Add a new first-entry `Current Settings` destination to `Settings` that opens a read-only screen showing current Wi-Fi and receiver details. The design keeps the existing Material-style labeled settings rows, uses pagination on the settings menu rather than compressing rows or switching to icon-only navigation, refreshes live status while the overview screen remains visible, and reuses existing network/storage managers for saved values, Wi-Fi status, receiver status, and receiver identity lookup.

## Technical Context

**Language/Version**: C++/Arduino using PlatformIO for ESP32.

**Primary Dependencies**: Existing `TFT_eSPI` drawing, existing `XPT2046_Touchscreen` input via `TouchManager`, existing `ScreenManager`, existing `MaterialStyle`, existing `SettingsScreen`, `WiFiManager`, `MarantzClient`, `ReceiverDiscovery`, and `ConfigStore`.

**Storage**: Existing LittleFS `config.json` through `ConfigStore` for saved Wi-Fi SSID and receiver IP. No new persisted settings are planned.

**Testing**: `pre-commit run --all-files`, `git diff --check`, `/Users/travis.illig/.platformio/penv/bin/platformio run`, plus hardware verification on the 480x320 touchscreen.

**Target Platform**: Elegoo ESP32 DevKit V1 with 4" ST7796 TFT and XPT2046 touch controller.

**Project Type**: Embedded touchscreen appliance.

**Performance Goals**: Open `Current Settings` from `Settings` in 2 taps or fewer; refresh live Wi-Fi and receiver status on open and at a 1-second cadence while visible; reflect live status changes within 3 seconds; preserve touch responsiveness during refresh.

**Constraints**: The screen is read-only. No new setup actions, storage schema changes, or icon-only settings menu. Preserve labeled Material-compatible navigation, existing screen return paths, and layered ownership where UI delegates to network/storage managers for state.

**Scale/Scope**: Primary code changes are in `src/ui/Screens/SettingsScreen.*` and a new `src/ui/Screens/CurrentSettingsScreen.*`, with supporting changes in `src/ui/MaterialStyle.*`, `src/network/WiFiManager.*`, and `src/network/ReceiverDiscovery.*` or equivalent network-layer metadata helpers. Durable documentation updates are expected in `docs/ui-reference.md` and `README.md`.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Visibility-First UI**: PASS. The feature adds a diagnostics-style screen without disturbing the live volume display and keeps the overview readable on the dark high-contrast appliance display.
- **II. Passive Appliance Stability**: PASS. The planned live refresh aligns with the existing 1-second polling pattern and avoids turning the screen into a high-frequency dashboard.
- **III. Strict Layered Architecture**: PASS. Saved values remain in `ConfigStore`, live Wi-Fi state remains in `WiFiManager`, receiver state remains in `MarantzClient`, and receiver identity lookup remains in the network layer rather than the UI.
- **IV. Deterministic Dependency Management**: PASS. No new libraries or manual driver changes are required.
- **V. Intuitive On-Device Setup**: PASS. Existing setup destinations stay reachable, and the new overview screen improves troubleshooting without replacing on-device configuration flows.

**Gate Status**: PASS. No constitution violations identified.

## Project Structure

### Documentation (this feature)

```text
specs/010-display-current-settings/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   ├── current_settings_screen_contract.md
│   ├── settings_menu_navigation_contract.md
│   └── validation_contract.md
└── tasks.md
```

### Source Code (repository root)

```text
src/
├── network/
│   ├── MarantzClient.h/.cpp              # Existing live receiver status polling
│   ├── ReceiverDiscovery.h/.cpp          # Reused or extended receiver identity lookup
│   └── WiFiManager.h/.cpp                # Current Wi-Fi connection state, IP, and RSSI access
├── storage/
│   └── ConfigStore.h/.cpp                # Existing saved Wi-Fi SSID and receiver IP source
└── ui/
    ├── MaterialStyle.h/.cpp              # Shared row, pagination, and read-only status presentation helpers
    ├── ScreenManager.h/.cpp              # Existing screen navigation ownership
    └── Screens/
        ├── CurrentSettingsScreen.h/.cpp  # New read-only current settings overview and refresh loop
        └── SettingsScreen.h/.cpp         # New first entry and paginated settings navigation

docs/
└── ui-reference.md                       # Durable settings-menu and current-settings UI guidance

README.md                                # User-facing note about the new Current Settings screen
AGENTS.md                                # Updated plan reference between Spec Kit markers
```

**Structure Decision**: Keep the feature inside the existing embedded firmware structure. Add one new screen for the overview, adapt `SettingsScreen` for paginated menu navigation, and extend existing managers only where they already own the required state.

## Documentation Alignment

- Update `docs/ui-reference.md` with the `Current Settings` screen pattern, the paginated `Settings` menu behavior, and the Wi-Fi signal graphic plus numeric-value rule.
- Update `README.md` with a concise user-facing note that `Settings` now includes a `Current Settings` overview for troubleshooting current Wi-Fi and receiver state.
- `CONTRIBUTING.md` does not need changes because the validation and workflow commands remain unchanged.

## Complexity Tracking

No constitution violations or exceptional complexity are required.

## Phase 0 Research Summary

See [research.md](research.md). All planning unknowns are resolved:

- Use paginated settings navigation instead of shrinking rows or switching to icon-only controls.
- Refresh current settings on open and continue refreshing at the existing 1-second polling cadence while the screen remains visible.
- Reuse the Wi-Fi selection screen's signal-strength graphic and pair it with numeric RSSI when available.
- Resolve receiver name or type through a network-layer metadata lookup and cache it for the screen session instead of changing persisted config.
- Keep the overview screen read-only with one `OK` return action to `Settings`.

## Phase 1 Design Summary

See [data-model.md](data-model.md), [contracts/current_settings_screen_contract.md](contracts/current_settings_screen_contract.md), [contracts/settings_menu_navigation_contract.md](contracts/settings_menu_navigation_contract.md), [contracts/validation_contract.md](contracts/validation_contract.md), and [quickstart.md](quickstart.md).

## Post-Design Constitution Check

- **I. Visibility-First UI**: PASS. The design keeps live monitoring untouched and uses a structured section layout for readable current-state inspection.
- **II. Passive Appliance Stability**: PASS. Contracts keep refreshes bounded to the existing polling rhythm and avoid high-frequency redraw or blocking wait loops.
- **III. Strict Layered Architecture**: PASS. UI owns presentation and navigation, while saved and live status data remain sourced from existing managers.
- **IV. Deterministic Dependency Management**: PASS. No new dependency, build target, or hardware configuration is introduced.
- **V. Intuitive On-Device Setup**: PASS. Existing setup destinations remain reachable from `Settings`, and the new overview improves recovery without adding hidden gestures.

**Gate Status**: PASS. No constitution violations identified after Phase 1 design.
