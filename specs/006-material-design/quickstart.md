# Quickstart: Material Design UI

## Prerequisites

- PlatformIO environment configured for this repository.
- Pre-commit hooks available.
- Hardware device available for final visual verification.
- Current bitmap icon assets from `specs/005-bitmap-icons` available in the repository.

## Planning Inputs

- Feature spec: `specs/006-material-design/spec.md`
- UI reference: `docs/ui-reference.md`
- Architecture reference: `docs/architecture.md`
- Material Design contracts:
  - `specs/006-material-design/contracts/component_style_catalog.md`
  - `specs/006-material-design/contracts/screen_application.md`
  - `specs/006-material-design/contracts/validation_contract.md`

## Implementation Outline

1. Add shared UI style primitives under `src/ui/MaterialStyle.h` and `src/ui/MaterialStyle.cpp`.
2. Define button variants, typography roles, spacing rules, state treatments, list rows, and searching screen helpers.
3. Update `docs/ui-reference.md` with the shared component/style catalog before migrating screens.
4. Migrate standard action buttons to icon-with-text rendering.
5. Migrate keyboard compact controls to the documented icon-only variant and align OK/Cancel.
6. Migrate Wi-Fi and receiver selection lists to the shared list-row pattern.
7. Migrate Wi-Fi and receiver searching screens to the shared searching/progress pattern.
8. Apply shared typography, spacing, button, and state patterns to every current on-device screen.
9. Document any remaining screen-specific design exceptions in `docs/ui-reference.md`.
10. Complete static validation and hardware visual verification.

## Validation

Run from the repository root:

```bash
pre-commit run --all-files
platformio run
git diff --check
```

If `platformio` is not available in the local environment, record that limitation and run the remaining checks.

Hardware visual verification:

1. Confirm the home screen preserves the volume-first hierarchy from a 15-foot viewing distance.
2. Compare standard buttons across setup, status, receiver, and settings screens.
3. Confirm keyboard OK and Cancel are aligned and use the compact icon-only variant.
4. Compare Wi-Fi and receiver searching screens for matching title, message, icon, progress, and action placement.
5. Confirm searching progress moves immediately after search starts and does not block touch or screen updates.
6. Compare Wi-Fi and receiver list rows for matching height, font roles, spacing, metadata placement, and state treatment.
7. Walk through success, warning, failure, empty, unavailable, and retry states.
8. Confirm every current screen is covered and any exception is documented.

## Expected Artifacts

- `src/ui/MaterialStyle.h`
- `src/ui/MaterialStyle.cpp`
- Updated `src/ui/Screens/*.cpp`
- Updated `docs/ui-reference.md`
- Updated `CONTRIBUTING.md` only if validation commands change
