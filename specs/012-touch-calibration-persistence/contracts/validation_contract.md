# Contract: Validation

## Static Validation

Run from the repository root:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

## Hardware Verification Matrix

Verify on the 480x320 touchscreen using the canonical hardware build:

1. Boot a device with no saved calibration and confirm touch uses the shipped default baseline without forcing calibration first.
2. Open `Settings` and confirm `Reset to defaults` appears as the last settings item.
3. Open `Touch Calibration` from `Settings` and confirm the screen states that calibration happens on-device rather than through serial capture.
4. During calibration, tap outside the active target boundary and confirm the same target remains active.
5. During calibration, attempt a rapid double-tap and confirm only one step can be captured from the intended press.
6. Complete a full valid calibration and confirm touch alignment improves immediately in the same session.
7. Reboot the device and confirm the saved calibration remains active.
8. Reopen calibration and confirm an incomplete or canceled attempt leaves the previously active profile unchanged.
9. Open `Reset to defaults`, choose `Calibration`, press `Cancel`, and confirm the saved calibration remains active.
10. Reopen the reset flow, confirm `Calibration`, and verify the device returns to the shipped default touch profile without clearing saved `Wi-Fi` or `Receiver` settings.
11. Repeat the reset flow for `Wi-Fi` and `Receiver`, verifying each reset clears only the selected setting type.
12. Confirm setup-path calibration remains reachable when the device is unconfigured.

## Pass Criteria

- Calibration is optional because the shipped default profile remains usable.
- Successful calibration applies immediately and persists across reboot.
- Invalid, canceled, or suspicious calibration input never overwrites the active profile.
- `Reset to defaults` remains reachable and behaves selectively with explicit confirmation.
- Wi-Fi, receiver, and calibration resets stay isolated from one another.
