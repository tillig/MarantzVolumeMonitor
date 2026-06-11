# Contract: Validation

## Static Validation

Run from the repository root:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

## Hardware Verification Matrix

Verify on the 480x320 touchscreen with and without saved Wi-Fi and receiver configuration:

1. Open `Settings` and confirm `Current Settings` is the first destination.
2. Confirm settings pagination behavior if multiple pages are needed.
3. Open `Current Settings` and confirm it is read-only.
4. Confirm the `OK` action returns to `Settings`.
5. Verify Wi-Fi SSID display when Wi-Fi is configured.
6. Verify `setup required` handling when Wi-Fi is not configured.
7. Verify monitor IP and Wi-Fi signal display while connected.
8. Verify Wi-Fi unavailable handling while disconnected.
9. Verify receiver IP display when configured.
10. Verify receiver name or type display when available.
11. Verify powered-on receiver state.
12. Verify powered-off receiver state.
13. Verify receiver unavailable state.
14. Verify not-configured receiver state.
15. Leave `Current Settings` visible and change Wi-Fi or receiver state, then confirm live updates appear within 3 seconds.

## Pass Criteria

- `Current Settings` appears first in `Settings`.
- Existing setup destinations remain reachable and readable.
- The overview screen contains Wi-Fi and receiver sections together.
- The overview screen contains no editing or save actions.
- The `OK` action returns to `Settings` on the first attempt.
- Wi-Fi signal uses the established graphic and shows numeric RSSI whenever available.
- Powered off and unavailable receiver states are visually and textually distinct.
- Missing saved values show `setup required` rather than blank content.
- Live state changes while the screen is open become visible within 3 seconds.
