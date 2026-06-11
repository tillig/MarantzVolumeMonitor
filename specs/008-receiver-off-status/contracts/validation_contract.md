# Contract: Validation

## Static Validation

Run from the repository root:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

## Hardware Verification Matrix

Verify on the 480x320 touchscreen with a saved receiver target:

1. Live receiver state.
2. Confirmed receiver-off state.
3. Receiver-off visible timeout.
4. Receiver-off software blank state.
5. Touch wake from blank state.
6. Touch wake after receiver-off confirmation is lost while blanked.
7. Non-Settings tap on visible `Receiver off`.
8. Settings entry after wake.
9. Settings open longer than 60 seconds while receiver remains off.
10. Return from Settings while receiver remains off.
11. Return from Settings after receiver becomes active.
12. Receiver unavailable or unreachable.
13. Wi-Fi disconnected or setup-required state.

## Pass Criteria

- Confirmed receiver-off status shows `Receiver off` before blanking.
- Visible receiver-off state blanks after 3 seconds with +/-0.5 second tolerance.
- Blank state contains no visible live volume, source, mode, audio-family, or status message content.
- Touch in blank state wakes to visible `Receiver off`.
- If receiver-off confirmation is lost after blanking, the monitor remains blank until user touch or active receiver status.
- Touch after receiver-off confirmation is lost wakes to the latest current visible state.
- Wake touch does not open Settings or setup in the same event.
- Non-Settings touches on visible `Receiver off` reset the 3-second timeout.
- Settings opens from the visible `Receiver off` state.
- Settings remains visible and usable for at least 60 seconds while receiver remains off.
- Returning Home while receiver remains off shows `Receiver off` again, then blanks after the timeout.
- Fresh active receiver status exits receiver-off blank or visible states and returns to live volume.
- Unavailable, unknown, Wi-Fi disconnected, and setup-required states do not enter receiver-off blank.
