# Quickstart: Receiver Off Screen Sleep

## Scope

Implement software-only receiver-off screen sleep. Do not add physical backlight switching, receiver
power control, new storage, or new dependencies.

## References

- Feature spec: `specs/008-receiver-off-status/spec.md`
- Plan: `specs/008-receiver-off-status/plan.md`
- Contracts:
  - `specs/008-receiver-off-status/contracts/receiver_off_display_cycle.md`
  - `specs/008-receiver-off-status/contracts/settings_wake_contract.md`
  - `specs/008-receiver-off-status/contracts/validation_contract.md`
- Durable UI reference: `docs/ui-reference.md`

## Implementation Outline

1. Review current `HomeScreen` display-state classification and Settings touch handling.
2. Add a Home Screen receiver-off blank presentation state and receiver-off timeout tracking.
3. Start the 3-second timer when Home enters visible receiver-off state.
4. Render a black/blank display when the timer expires and receiver-off remains confirmed.
5. Keep existing receiver polling active while blanked so active receiver status restores live volume.
6. Keep the monitor blank if receiver-off confirmation is lost after blanking, until touch or active
   receiver status.
7. Treat the first touch while blanked as wake-only, returning to the latest current visible state.
8. Restart the 3-second timeout when the user touches visible `Receiver off` outside Settings.
9. Keep Settings and settings-launched setup flows outside the receiver-off timeout.
10. Update `docs/ui-reference.md` and `README.md` for durable/user-facing behavior.

## Validation

Run static validation:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

Run hardware verification:

1. With receiver active, confirm live volume display remains unchanged.
2. Turn receiver off while it remains reachable and confirm `Receiver off` appears.
3. Confirm the display blanks to black after 3 seconds, +/-0.5 seconds.
4. Tap the blank display and confirm it wakes to visible `Receiver off`.
5. Confirm the wake tap does not also open Settings.
6. Wake to visible `Receiver off`, tap outside the Settings icon, and confirm the timeout restarts.
7. From blank receiver-off state, make receiver-off status unavailable and confirm the monitor stays
   blank until touched or active status resumes.
8. Touch after receiver-off status becomes unavailable and confirm the latest current state is shown.
9. Tap Settings and remain there for at least 60 seconds while the receiver stays off.
10. Exit Settings while the receiver is still off and confirm `Receiver off` appears, then blanks
   again after the timeout.
11. From blank receiver-off state, turn the receiver back on and confirm live volume returns without
   rebooting or reopening setup.
12. Make the saved receiver unreachable before any confirmed receiver-off blanking and confirm the
   display shows unavailable/setup state rather than entering the receiver-off blank cycle.
