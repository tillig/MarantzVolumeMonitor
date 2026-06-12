# Quickstart: Volume Display Scale

## Scope

Implement a new `Volume Display Scale` destination in `Settings`, a dedicated single-choice selection screen with `OK` and `Cancel`, and the related home-screen numeric formatting update. Do not change gauge behavior, add new dependencies, or introduce a storage migration.

## References

- Feature spec: `specs/011-volume-scale-setting/spec.md`
- Plan: `specs/011-volume-scale-setting/plan.md`
- Contracts:
  - `specs/011-volume-scale-setting/contracts/settings_menu_navigation_contract.md`
  - `specs/011-volume-scale-setting/contracts/volume_display_scale_screen_contract.md`
  - `specs/011-volume-scale-setting/contracts/home_volume_format_contract.md`
  - `specs/011-volume-scale-setting/contracts/validation_contract.md`
- Durable UI reference: `docs/ui-reference.md`

## Implementation Outline

1. Add a new `VolumeScaleScreen` under `src/ui/Screens/` with two single-choice options, a visible selected-state indicator, and bottom `OK` and `Cancel` actions.
2. Update `SettingsScreen` so `Volume Display Scale` opens that new screen while preserving readable labeled navigation and any required pagination.
3. Load the saved `useDbScale` value from `ConfigStore` when the selection screen opens and treat `false` as the default `0-100` mode.
4. Keep a pending selection on the selection screen so tapping an option does not save immediately.
5. On `OK`, persist the selected `useDbScale` value through `ConfigStore` and return to `Settings`.
6. On `Cancel`, discard any pending change and return to `Settings`.
7. Update `HomeScreen` so live numeric volume formatting uses one decimal place in either mode, switching between normalized `0-100` and raw receiver `dB` text while keeping the gauge on the normalized scale.
8. Preserve all existing non-live home-screen states so they do not show a numeric value after the setting changes.
9. Update `docs/ui-reference.md` and `README.md` to document the new settings destination and remove the note that `useDbScale` is not exposed in the UI.

## Validation

Run static validation:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

Run hardware verification:

1. Open `Settings` and confirm `Volume Display Scale` appears as a labeled destination.
2. If pagination is present, confirm `PREV` and `NEXT` keep all settings destinations reachable.
3. Open `Volume Display Scale` and confirm one option is visibly selected.
4. Select the other option and press `Cancel`, then confirm the home-screen number format does not change.
5. Reopen the screen, select the other option, and press `OK`.
6. Confirm the home-screen number switches format and still shows one decimal place.
7. Confirm the gauge position and animation remain unchanged for the same live receiver volume.
8. Restart the device and confirm the saved format remains active.
9. With the receiver unavailable or powered off, confirm the home screen still shows the correct non-live state instead of a number after changing the saved scale.
