# Contract: Validation

## Static Validation

Run from the repository root:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

## Hardware Verification Matrix

Verify on the 480x320 touchscreen with a configured receiver and with degraded receiver states:

1. Open `Settings` and confirm `Volume Display Scale` appears as a labeled destination.
2. Confirm settings pagination behavior if multiple pages are needed.
3. Open `Volume Display Scale` and confirm both `0-100` and `dB` options are visible.
4. Confirm the saved option is visibly selected on entry.
5. Select the other option and press `Cancel`, then confirm the home-screen number format does not change.
6. Reopen `Volume Display Scale`, select the other option, and press `OK`.
7. Return to the home screen and confirm the numeric volume uses the newly selected format with one decimal place.
8. Confirm the gauge position and motion remain unchanged for the same receiver volume after switching modes.
9. Reopen `Volume Display Scale` and confirm the saved choice is now visibly selected.
10. Restart the device and confirm the saved scale remains active.
11. While the receiver is unavailable or powered off, change the saved scale and confirm the home screen still shows the correct non-live state rather than a numeric value.

## Pass Criteria

- `Volume Display Scale` is reachable from `Settings` without hidden gestures.
- The selection screen always shows exactly one selected option.
- `OK` applies and persists the pending selection.
- `Cancel` discards pending changes and returns to `Settings`.
- Both numeric modes show one decimal place when live volume is available.
- The gauge remains unchanged when only the numeric format changes.
- Restart preserves the saved choice.
- Non-live states remain non-numeric after the setting changes.
