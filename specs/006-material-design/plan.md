# Implementation Plan: Material Design UI

**Branch**: `feature/006-material-design` | **Date**: 2026-06-04 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/006-material-design/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See
`.specify/templates/plan-template.md` for the execution workflow.

## Summary

Refresh all current on-device screens with a shared Material-compatible design system while preserving
the device's dark, high-contrast, 15-foot appliance experience. The implementation will introduce
UI-layer style primitives for buttons, lists, searching/progress screens, typography, spacing, and
state treatments; apply them across every current screen; and document the durable component/style
catalog in `docs/ui-reference.md`.

## Technical Context

**Language/Version**: C++/Arduino using PlatformIO for ESP32.

**Primary Dependencies**: `TFT_eSPI` for display rendering, existing `DisplayManager` color constants,
existing `IconRenderer` and `IconBitmaps` assets, `LittleFS` through existing configuration flows, and
current network/storage managers for setup data.

**Storage**: No new persistent storage. Existing Wi-Fi and receiver configuration storage remains
unchanged.

**Testing**: `pre-commit run --all-files`, `platformio run` when PlatformIO is available, `git diff
--check`, and hardware visual verification on the 480x320 touchscreen.

**Target Platform**: Elegoo ESP32 DevKit V1 with 4" ST7796 TFT and XPT2046 touch controller.

**Project Type**: Embedded touchscreen appliance.

**Performance Goals**: UI remains responsive during setup and network timeouts; progress movement and
screen refreshes do not visibly lag touch input or disrupt the existing 1-2 second receiver polling
cadence.

**Constraints**: Dark mode only; high contrast; readable from 15 feet; no idle animation; no network
I/O from screens; no new runtime asset decoding; all current on-device screens are in scope; standard
actions use icon-with-text buttons except documented compact contexts such as keyboard controls.

**Scale/Scope**: All current `src/ui/Screens` flows: Home, Settings, Network list, Keyboard, Wi-Fi
status, Receiver list, Receiver IP entry, Receiver verification/status, Setup status/result, and Touch
Calibration screens.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Visibility-First UI**: PASS. The design explicitly preserves dark mode, high contrast,
  15-foot readability, home-screen visual hierarchy, and restrained animation.
- **II. Passive Appliance Stability**: PASS. Shared UI helpers and progress animation stay
  non-blocking and do not add network work, storage work, or runtime image decoding to screen drawing.
- **III. Strict Layered Architecture**: PASS. Changes are contained to the UI layer plus durable
  documentation; network and storage ownership remains unchanged.
- **IV. Deterministic Dependency Management**: PASS. No new firmware dependency is required. Existing
  PlatformIO dependency management remains the build authority.
- **V. Intuitive On-Device Setup**: PASS. The plan standardizes touch setup controls, searching
  screens, selection lists, keyboard actions, and recovery states.

**Gate Status**: PASS. No constitution violations identified.

## Project Structure

### Documentation (this feature)

```text
specs/006-material-design/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   ├── component_style_catalog.md
│   ├── screen_application.md
│   └── validation_contract.md
└── tasks.md
```

### Source Code (repository root)

```text
src/
└── ui/
    ├── DisplayManager.h/.cpp          # Existing color/display access
    ├── IconRenderer.h/.cpp            # Existing bitmap icon rendering
    ├── MaterialStyle.h/.cpp           # Shared component, spacing, and state helpers
    └── Screens/
        ├── HomeScreen.h/.cpp
        ├── SettingsScreen.h/.cpp
        ├── NetworkListScreen.h/.cpp
        ├── KeyboardScreen.h/.cpp
        ├── SetupStatusScreen.h/.cpp
        ├── ReceiverListScreen.h/.cpp
        ├── ReceiverIpScreen.h/.cpp
        ├── ReceiverStatusScreen.h/.cpp
        └── CalibrationScreen.h/.cpp

docs/
└── ui-reference.md                    # Durable shared component/style catalog
```

**Structure Decision**: Add a small UI-layer `MaterialStyle` helper for common rendering primitives
and constants, then migrate each screen to those primitives. Keep style documentation in
`docs/ui-reference.md`; keep planning contracts in `specs/006-material-design/contracts/`.

## Documentation Alignment

- Update `docs/ui-reference.md` with a shared component/style catalog covering buttons, lists,
  searching screens, typography, spacing, icons, state treatments, and exceptions.
- Keep feature-specific research, contracts, and verification guidance in `specs/006-material-design/`.
- Update `CONTRIBUTING.md` only if implementation introduces new validation commands beyond the
  current `pre-commit run --all-files`, `platformio run`, and `git diff --check` flow.
- No `README.md` change is expected unless the user-facing device behavior changes beyond visual
  consistency.

## Complexity Tracking

No constitution violations or exceptional complexity are required.

## Phase 0 Research Summary

See [research.md](research.md). All planning unknowns are resolved:

- Use a shared UI-layer style helper rather than per-screen duplicated drawing code.
- Standard actions use icon-with-text buttons; keyboard compact controls may be icon-only.
- Wi-Fi and receiver selection lists use one shared list-row pattern.
- Wi-Fi and receiver searching screens use one shared searching/progress pattern.
- Progress movement is immediate on explicit searching screens and delayed for other loading states
  until the operation remains active for at least 500 ms.
- Durable style guidance lives in `docs/ui-reference.md`.

## Phase 1 Design Summary

See [data-model.md](data-model.md), [contracts/component_style_catalog.md](contracts/component_style_catalog.md),
[contracts/screen_application.md](contracts/screen_application.md),
[contracts/validation_contract.md](contracts/validation_contract.md), and [quickstart.md](quickstart.md).

## Post-Design Constitution Check

- **I. Visibility-First UI**: PASS. Contracts require 15-foot review, home hierarchy preservation,
  consistent typography, no overlap, and restrained animation.
- **II. Passive Appliance Stability**: PASS. Contracts require non-blocking progress animation and
  forbid introducing screen-owned network/storage work.
- **III. Strict Layered Architecture**: PASS. Contracts keep style rendering in `src/ui` and leave
  network/storage behavior delegated to existing managers.
- **IV. Deterministic Dependency Management**: PASS. Contracts require no new firmware dependencies
  unless explicitly justified during implementation planning.
- **V. Intuitive On-Device Setup**: PASS. Contracts standardize setup buttons, lists, searching,
  keyboard actions, and recoverable error actions.

**Gate Status**: PASS. No constitution violations identified after Phase 1 design.
