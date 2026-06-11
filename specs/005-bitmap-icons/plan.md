# Implementation Plan: Bitmap Icon Assets

**Branch**: `feature/005-bitmap-icons` | **Date**: 2026-06-04 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/005-bitmap-icons/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/plan-template.md` for the execution workflow.

## Summary

Replace current text and manually drawn UI glyphs with a Material-compatible bitmap icon system for the on-device display. The implementation will keep source artwork in the repository, generate compact monochrome bitmap masks for firmware rendering, use owner-approved Dolby and DTS logo sources for the home audio-family row, create custom PCM and Other icons, and document every icon in a durable inventory.

## Technical Context

**Language/Version**: C++/Arduino using PlatformIO for ESP32; asset preparation may use repository scripts run from the development workstation.

**Primary Dependencies**: `TFT_eSPI` for display rendering, existing `DisplayManager` color constants, official Material Symbols source artwork for non-brand icons, owner-approved Dolby and DTS SVG source candidates, and custom source artwork for PCM and Other.

**Storage**: Source artwork and generated icon metadata live in the repository. Runtime icon masks are compiled into firmware as read-only bitmap data; LittleFS is not used for UI icon assets.

**Testing**: `pre-commit run --all-files`, `platformio run`, generated asset consistency checks, and hardware visual verification on the 480x320 TFT.

**Target Platform**: Elegoo ESP32 DevKit V1 with 4" ST7796 TFT and XPT2046 touch controller.

**Project Type**: Embedded touchscreen appliance.

**Performance Goals**: Icon drawing must not introduce visible screen lag, must preserve the current home-screen refresh cadence, and must keep 24x24 and 102x32 icons recognizable at normal room distance.

**Constraints**: UI remains dark-mode, high-contrast, and readable from 15 feet; no runtime SVG or PNG decoding on the ESP32; no unmanaged display dependency changes; brand-logo source and approval notes must be retained in the icon inventory.

**Scale/Scope**: Initial icon set covers the home audio-family row, home settings control, settings navigation arrows, keyboard caps lock/password visibility/backspace/OK/Cancel, and setup/status controls listed in the spec. The design leaves room for future Material Design screen styling without redesigning the icon set.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Visibility-First UI**: PASS. Icons are specified for high contrast, dark mode, 15-foot readability, and no distracting animation.
- **II. Passive Appliance Stability**: PASS. Icons are compiled as small read-only masks and do not add network calls, runtime filesystem access, or blocking decode work to screen rendering.
- **III. Strict Layered Architecture**: PASS. Rendering helpers and bitmap data stay in the UI layer; network and storage layers are unchanged.
- **IV. Deterministic Dependency Management**: PASS. Firmware uses existing PlatformIO dependencies. Any workstation asset-generation tooling must be documented and deterministic.
- **V. Intuitive On-Device Setup**: PASS. Setup and keyboard icons improve touch-first setup clarity without removing necessary text.

**Gate Status**: PASS. No constitution violations identified.

## Project Structure

### Documentation (this feature)

```text
specs/005-bitmap-icons/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   ├── asset_pipeline.md
│   ├── icon_inventory.md
│   └── ui_rendering.md
└── tasks.md
```

### Source Code (repository root)

```text
assets/
└── icons/
    ├── inventory.md              # Durable source, license, size, and usage table
    ├── source/                   # SVG source assets and custom source artwork
    └── generated/                # Generated previews and intermediate generated assets

scripts/
└── icons/
    └── generate-icons.*          # Deterministic asset conversion entry point

src/
└── ui/
    ├── IconRenderer.h/.cpp       # UI-layer helper for drawing colored bitmap masks
    ├── assets/
    │   └── IconBitmaps.h/.cpp    # Generated bitmap mask data compiled into firmware
    └── Screens/
        ├── HomeScreen.h/.cpp     # Draw audio-family and settings bitmap icons
        ├── SettingsScreen.cpp    # Draw bitmap row arrows
        └── KeyboardScreen.h/.cpp # Draw bitmap keyboard function controls

docs/
└── ui-reference.md               # Durable icon usage guidance and inventory pointer
```

**Structure Decision**: Add a small UI-layer icon rendering helper plus generated bitmap data under `src/ui`. Keep source artwork and the icon inventory outside firmware code under `assets/icons` so contributors can audit sources and regenerate firmware assets. Keep generated firmware masks deterministic and checked in so a normal PlatformIO build does not require image tooling.

## Documentation Alignment

- Update `docs/ui-reference.md` to replace the current text audio-family tile guidance with icon state guidance for Dolby, DTS, PCM, and Other.
- Add or update `assets/icons/inventory.md` as the durable icon inventory required by the spec.
- Update `CONTRIBUTING.md` if asset generation introduces a contributor command beyond `platformio run` and `pre-commit run --all-files`.
- Keep implementation-specific contracts and validation flow under `specs/005-bitmap-icons/`.

## Complexity Tracking

No constitution violations or exceptional complexity are required.

## Phase 0 Research Summary

See [research.md](research.md). All planning unknowns are resolved:

- Use source SVG artwork plus generated monochrome firmware bitmap masks.
- Use Material Symbols for generic controls and states when a suitable symbol exists.
- Use owner-approved Dolby and DTS SVG sources for brand family icons.
- Create custom Material-compatible PCM and Other icons.
- Compile generated icon masks into firmware instead of loading icon files from LittleFS.

## Phase 1 Design Summary

See [data-model.md](data-model.md), [contracts/asset_pipeline.md](contracts/asset_pipeline.md), [contracts/icon_inventory.md](contracts/icon_inventory.md), [contracts/ui_rendering.md](contracts/ui_rendering.md), and [quickstart.md](quickstart.md).

## Post-Design Constitution Check

- **I. Visibility-First UI**: PASS. UI contracts define green active and dark-gray inactive states, minimum sizes, and visual review expectations.
- **II. Passive Appliance Stability**: PASS. Contracts avoid runtime asset decoding and filesystem reads during screen rendering.
- **III. Strict Layered Architecture**: PASS. Icon rendering remains under `src/ui`; no network or storage ownership changes are introduced.
- **IV. Deterministic Dependency Management**: PASS. Generated firmware assets are checked in and validation can compare generated output to committed output.
- **V. Intuitive On-Device Setup**: PASS. Settings and keyboard contracts retain readable text where needed and replace only arrows/glyphs that should be iconographic.

**Gate Status**: PASS. No constitution violations identified after Phase 1 design.
