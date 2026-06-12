# Quickstart: Touch Calibration Persistence

## Scope

Implement real on-device touch calibration, runtime application of a saved calibration profile, and a selective `Reset to defaults` settings flow. Keep the current measured affine coefficients as the shipped default profile, avoid new dependencies, and preserve central touch ownership in `TouchManager`.

## References

- Feature spec: `specs/012-touch-calibration-persistence/spec.md`
- Plan: `specs/012-touch-calibration-persistence/plan.md`
- Contracts:
  - `specs/012-touch-calibration-persistence/contracts/calibration_session_contract.md`
  - `specs/012-touch-calibration-persistence/contracts/config_persistence_contract.md`
  - `specs/012-touch-calibration-persistence/contracts/settings_reset_defaults_contract.md`
  - `specs/012-touch-calibration-persistence/contracts/validation_contract.md`
- Durable references:
  - `docs/hardware.md`
  - `docs/ui-reference.md`
  - `docs/architecture.md`

## Implementation Outline

1. Extend `DeviceConfig` and `ConfigStore` to read, write, and selectively clear an optional persisted touch-calibration profile.
2. Refactor `TouchManager` so it exposes the shipped default profile separately from the active runtime profile and can apply either a saved or default profile on demand.
3. Load the saved calibration profile during startup after `ConfigStore` initializes, falling back to the shipped default profile if no valid saved profile exists.
4. Rework `CalibrationScreen` into an on-device calibration state machine that guides the existing 9-point flow, rejects out-of-bounds or suspicious taps, and computes a candidate affine profile locally.
5. Apply and persist a candidate profile only after it passes validation; otherwise keep the previously active profile unchanged.
6. Add a new `ResetDefaultsScreen` under `src/ui/Screens/` for `Wi-Fi`, `Receiver`, and `Calibration` resets with explicit `Reset` and `Cancel` confirmation.
7. Update `SettingsScreen` so `Reset to defaults` is the last settings item and still preserves readable pagination.
8. Keep the setup-path `Calibrate` entry available when the device is unconfigured.
9. Update `README.md`, `docs/hardware.md`, `docs/ui-reference.md`, and `docs/architecture.md` to describe the new calibration and reset behavior.

## Validation

Run static validation:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

Run hardware verification:

1. Boot without a saved calibration and confirm touch works from the shipped default profile.
2. Start calibration and confirm on-device guidance does not mention serial capture.
3. Attempt out-of-bounds and double-tap input during calibration and confirm the current target does not advance.
4. Complete a valid calibration and confirm touch alignment updates immediately.
5. Reboot and confirm the saved calibration persists.
6. Cancel or fail a calibration attempt and confirm the prior profile remains active.
7. Open `Reset to defaults`, confirm it is the last settings item, and verify `Cancel` leaves the selected settings unchanged.
8. Reset `Calibration` and confirm the shipped default profile is restored without clearing Wi-Fi or receiver settings.
9. Reset `Wi-Fi` and `Receiver` independently and confirm each reset affects only the selected settings type.
