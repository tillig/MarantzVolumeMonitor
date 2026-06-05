# Quickstart: Display Volume Level

## Prerequisites

- PlatformIO environment configured for this repository.
- Pre-commit hooks available.
- Hardware device available for final visual verification.
- Existing home-screen Material-compatible styling already present from `specs/006-material-design`.

## Planning Inputs

- Feature spec: `specs/007-display-volume-level/spec.md`
- UI reference: `docs/ui-reference.md`
- Architecture reference: `docs/architecture.md`
- Home display contracts:
  - `specs/007-display-volume-level/contracts/home_display_states.md`
  - `specs/007-display-volume-level/contracts/volume_motion_contract.md`
  - `specs/007-display-volume-level/contracts/validation_contract.md`

## Implementation Outline

1. Audit current `HomeScreen` rendering paths for full-screen redraw triggers versus live-update-only
   changes.
2. Add explicit home-state classification for live, powered-off, unavailable, and setup-required
   conditions.
3. Preserve Settings access from both powered-off and unavailable receiver states.
4. Add one-decimal home-screen formatting for live volume presentation.
5. Introduce home-screen animation state that tracks displayed versus target volume.
6. Animate the gauge and numeric value with one shared calm motion treatment and smooth retargeting.
7. Split home-screen redraw behavior so animation updates repaint only dirty regions while major state
   changes still redraw the full layout.
8. Update durable docs for home-screen motion and non-live receiver-state behavior.
9. Complete static validation and hardware visual verification.

## Validation

Run from the repository root:

```bash
pre-commit run --all-files
/Users/travis.illig/.platformio/penv/bin/platformio run
git diff --check
```

If the PlatformIO environment is unavailable locally, record that limitation and run the remaining
checks.

Hardware visual verification:

1. Confirm the live home screen shows volume with one decimal place and preserves 15-foot readability.
2. Trigger small and large volume changes and confirm the gauge and number animate together.
3. Confirm animation slows into the final value rather than stopping abruptly.
4. Trigger multiple quick volume changes and confirm motion retargets smoothly to the newest value.
5. Watch the display during animation and confirm unaffected areas do not flash or visibly repaint.
6. Verify a powered-off receiver shows `Receiver off`, does not show stale live volume, and still
   keeps Settings accessible.
7. Verify an unavailable receiver shows a distinct unavailable state and still keeps Settings
   accessible.
8. Verify the home screen returns to live volume presentation automatically when active receiver
   status resumes.

## Expected Artifacts

- Updated `src/ui/Screens/HomeScreen.h`
- Updated `src/ui/Screens/HomeScreen.cpp`
- Updated `src/network/MarantzClient.h`
- Updated `src/network/MarantzClient.cpp`
- Updated `docs/ui-reference.md`
- Updated `README.md` if user-facing behavior text changes
