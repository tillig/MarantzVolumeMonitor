# Implementation Plan: Display Volume Level

**Branch**: `feature/007-display-volume-level` | **Date**: 2026-06-05 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/007-display-volume-level/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/plan-template.md` for the execution workflow.

## Summary

Refine the home screen so it presents live receiver volume as the dominant value with one decimal place, animates gauge and numeric changes using calm Material-compatible standard motion, distinguishes powered-off and unavailable receiver states, and redraws only affected regions during animation to avoid screen flashing.

## Technical Context

**Language/Version**: C++/Arduino using PlatformIO for ESP32.

**Primary Dependencies**: `TFT_eSPI` for direct drawing, existing `DisplayManager` color constants, existing `MaterialStyle` status/button helpers, `IconRenderer` bitmap assets, `MarantzClient` live status polling, `WiFiManager`, and `ConfigStore`.

**Storage**: No new persistent storage. Existing Wi-Fi and receiver configuration storage remains unchanged.

**Testing**: `pre-commit run --all-files`, `git diff --check`, `/Users/travis.illig/.platformio/penv/bin/platformio run`, and hardware visual verification on the 480x320 touchscreen.

**Target Platform**: Elegoo ESP32 DevKit V1 with 4" ST7796 TFT and XPT2046 touch controller.

**Project Type**: Embedded touchscreen appliance.

**Performance Goals**: Volume updates feel immediate during normal receiver adjustments, animation does not block the existing 1-second home-screen polling cycle, and redraw work stays limited to dirty regions so the display does not flash during motion.

**Constraints**: Dark mode only; readable from 15 feet; no distracting idle animation; no screen-owned network I/O; preserve passive appliance stability; distinguish powered-off vs unavailable receiver states; show one decimal place whenever live status is available; redraw only affected regions during volume animation.

**Scale/Scope**: Primarily `src/ui/Screens/HomeScreen.h/.cpp`, with supporting status classification in `src/network/MarantzClient.h/.cpp` and durable UI/usage documentation in `docs/ui-reference.md` and `README.md`.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Visibility-First UI**: PASS. The plan preserves the home screen as a volume-first display, keeps one-decimal formatting explicit, and constrains motion to brief, readable value changes.
- **II. Passive Appliance Stability**: PASS. Motion is non-blocking, redraws are limited to affected regions, and no new network/storage work is introduced into the screen.
- **III. Strict Layered Architecture**: PASS. Receiver fetching stays in `MarantzClient`; state presentation and animation stay in the UI layer.
- **IV. Deterministic Dependency Management**: PASS. No new firmware dependency is required.
- **V. Intuitive On-Device Setup**: PASS. Both unavailable and powered-off states preserve a direct Settings path for receiver reconfiguration.

**Gate Status**: PASS. No constitution violations identified.

## Project Structure

### Documentation (this feature)

```text
specs/007-display-volume-level/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   ├── home_display_states.md
│   ├── volume_motion_contract.md
│   └── validation_contract.md
└── tasks.md
```

### Source Code (repository root)

```text
src/
├── network/
│   └── MarantzClient.h/.cpp          # Live receiver status including power + validity
└── ui/
    ├── DisplayManager.h/.cpp         # Shared colors and TFT access
    ├── MaterialStyle.h/.cpp          # Existing status/button primitives
    ├── IconRenderer.h/.cpp           # Bitmap icon rendering
    └── Screens/
        └── HomeScreen.h/.cpp         # Home layout, status states, and volume animation

docs/
└── ui-reference.md                   # Durable home-screen motion/state guidance

README.md                             # User-facing explanation of powered-off vs unavailable states
```

**Structure Decision**: Keep the feature centered on `HomeScreen` with minimal supporting changes in `MarantzClient` for status classification. Document durable home-screen behavior in `docs/ui-reference.md` and brief user-facing expectations in `README.md`.

## Documentation Alignment

- Update `docs/ui-reference.md` with durable home-screen guidance for one-decimal volume formatting, powered-off vs unavailable presentation, and partial-redraw animation behavior.
- Update `README.md` if implemented behavior changes how users interpret powered-off vs unavailable receiver states or how Settings remains reachable during those states.
- Keep feature-specific design decisions, state contracts, and validation guidance in `specs/007-display-volume-level/`.
- `CONTRIBUTING.md` does not need changes unless implementation introduces new validation commands.

## Complexity Tracking

No constitution violations or exceptional complexity are required.

## Phase 0 Research Summary

See [research.md](research.md). All planning unknowns are resolved:

- Keep animation state local to `HomeScreen` rather than introducing a new cross-screen animation framework.
- Treat receiver status as three home-display outcomes: live, powered off, and unavailable.
- Use partial redraw regions for gauge/value/status updates instead of full-screen redraws during motion.
- Document one-decimal formatting and non-live state behavior in durable UI/usage docs.

## Phase 1 Design Summary

See [data-model.md](data-model.md), [contracts/home_display_states.md](contracts/home_display_states.md), [contracts/volume_motion_contract.md](contracts/volume_motion_contract.md), [contracts/validation_contract.md](contracts/validation_contract.md), and [quickstart.md](quickstart.md).

## Post-Design Constitution Check

- **I. Visibility-First UI**: PASS. The design keeps the volume value dominant, defines one-decimal formatting, and constrains motion to brief, readable change feedback.
- **II. Passive Appliance Stability**: PASS. Contracts require non-blocking animation, dirty-region redraws, and no additional screen-owned polling.
- **III. Strict Layered Architecture**: PASS. Contracts keep live status retrieval in `MarantzClient` and home-state rendering/animation in `HomeScreen`.
- **IV. Deterministic Dependency Management**: PASS. Design adds no new libraries or runtime assets.
- **V. Intuitive On-Device Setup**: PASS. Contracts preserve Settings access from both unavailable and powered-off states.

**Gate Status**: PASS. No constitution violations identified after Phase 1 design.
