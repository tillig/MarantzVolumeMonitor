# Contract: Validation

## Purpose

Define the checks required to prove the Display Volume Level feature is complete.

## Static Validation

Run from the repository root:

```bash
pre-commit run --all-files
/Users/travis.illig/.platformio/penv/bin/platformio run
git diff --check
```

If the PlatformIO environment is unavailable in the current environment, record that limitation and
run the other checks.

## Documentation Validation

Verify `docs/ui-reference.md` includes durable home-screen guidance for:

- one-decimal live volume formatting
- powered-off versus unavailable receiver presentation
- calm volume motion expectations
- partial-redraw guidance for motion updates

Verify `README.md` is updated if implemented user-facing receiver-state behavior changes.

## Hardware Visual Verification

Use the 480x320 touchscreen device and inspect:

1. Normal live home screen
2. Small live volume changes
3. Large live volume changes
4. Rapid consecutive live volume changes
5. Powered-off receiver state
6. Unavailable receiver state
7. Recovery from powered-off back to live
8. Recovery from unavailable back to live

## Required Visual Checks

- Live volume is readable from 15 feet and always shows one decimal place.
- Gauge and numeric value animate together with calm, readable motion.
- Motion decelerates into the final value without decorative bounce.
- Rapid retargeting settles on the newest value cleanly.
- Animation does not cause full-screen flashing or obvious repaint of unaffected regions.
- Powered-off state is distinct from unavailable state.
- Powered-off and unavailable states both keep Settings accessible.
- No non-live state presents stale volume as if it were current.

## Acceptance Criteria

- All static validation steps pass or have an environment limitation recorded.
- Hardware verification covers every state and motion scenario listed above.
- Final implementation satisfies every measurable outcome in the feature spec.
