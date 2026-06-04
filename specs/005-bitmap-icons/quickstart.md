# Quickstart: Bitmap Icon Assets

## Prerequisites

- PlatformIO environment configured for this repository.
- Pre-commit hooks available.
- Asset generation tooling documented during implementation if generated bitmap files need to be
  refreshed.

## Planning Inputs

- Feature spec: `specs/005-bitmap-icons/spec.md`
- UI reference: `docs/ui-reference.md`
- Architecture reference: `docs/architecture.md`
- Icon contracts:
  - `specs/005-bitmap-icons/contracts/asset_pipeline.md`
  - `specs/005-bitmap-icons/contracts/icon_inventory.md`
  - `specs/005-bitmap-icons/contracts/ui_rendering.md`

## Implementation Outline

1. Create `assets/icons/inventory.md` with the required inventory columns.
2. Add source artwork under `assets/icons/source/`.
3. Add deterministic icon generation under `scripts/icons/`.
4. Generate firmware bitmap masks under `src/ui/assets/`.
5. Add a UI-layer icon renderer under `src/ui/`.
6. Replace the home screen audio-family text row and settings gear with bitmap icons.
7. Replace settings row text arrows with bitmap chevrons.
8. Replace keyboard caps lock, password visibility, backspace, OK, and Cancel legacy glyphs or text
   arrows with bitmap icons.
9. Add setup/status icons where existing screens expose matching states.
10. Update `docs/ui-reference.md` with durable icon guidance and the inventory pointer.

## Validation

Run from the repository root:

```bash
pre-commit run --all-files
platformio run
git diff --check
```

Hardware visual verification:

1. Boot to the home screen with a valid receiver status.
2. Confirm Dolby, DTS, PCM, and Other render as icons, not text.
3. Exercise receiver mode samples or test states for Dolby, DTS, PCM, Other, and unavailable status.
4. Confirm active family icon is green and inactive family icons are dark gray.
5. Open Settings and confirm row chevrons are bitmap icons matching row text color.
6. Open the keyboard and confirm caps lock, password visibility, backspace, OK, and Cancel use bitmap
   icons.
7. Walk through Wi-Fi and receiver setup success/failure paths and confirm setup/status icons are
   inventory-backed and readable.

## Expected Artifacts

- `assets/icons/inventory.md`
- `assets/icons/source/*`
- `assets/icons/generated/*` if previews or intermediate outputs are retained
- `src/ui/IconRenderer.h`
- `src/ui/IconRenderer.cpp`
- `src/ui/assets/IconBitmaps.h`
- `src/ui/assets/IconBitmaps.cpp`
- Updated `src/ui/Screens/*.cpp` files
- Updated `docs/ui-reference.md`
