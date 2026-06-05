# Contract: Validation

## Purpose

Define the checks required to prove the Material-compatible UI refresh is complete.

## Static Validation

Run from the repository root:

```bash
pre-commit run --all-files
platformio run
git diff --check
```

If `platformio` is not available in the current environment, record the limitation and run the other
checks.

## Documentation Validation

Verify `docs/ui-reference.md` includes:

- Shared component/style catalog
- Button variants and exceptions
- Selection-list pattern
- Searching-screen pattern
- Typography roles
- Spacing and geometry rules
- State treatments
- Screen application notes
- Hardware visual review checklist

Verify `CONTRIBUTING.md` is updated if new validation commands are introduced.

## Hardware Visual Verification

Use the 480x320 touchscreen device and inspect every current screen:

1. Home screen
2. Settings screen
3. Wi-Fi network searching and list screens
4. Keyboard screen
5. Wi-Fi connecting, success, and failure states
6. Receiver searching and list screens
7. Manual receiver IP entry
8. Receiver verification, success, and failure states
9. Touch calibration screens

## Required Visual Checks

- Home volume, source, listening mode, and audio-family state are identifiable from 15 feet within 5
  seconds.
- Standard actions use icon-with-text buttons across equivalent contexts.
- Keyboard icon-only buttons are aligned and visually centered.
- Buttons use consistent shape, typography, icon placement, and state treatment.
- Wi-Fi and receiver lists use consistent row size, typography, spacing, and state treatment.
- Wi-Fi and receiver searching screens share title, text, icon, progress, and action placement.
- Searching screens show progress movement immediately after search starts.
- Other loading states show progress only when not effectively immediate.
- Recoverable errors include a concise message and clear recovery action.
- No visible text overlaps, clips primary labels, or collides with buttons.

## Acceptance Criteria

- All static validation steps pass or have an environment limitation recorded.
- Hardware visual verification covers every screen listed above.
- Any documented exception includes a reason and still passes touch, readability, and consistency
  checks.
- The final implementation satisfies every measurable outcome in the feature spec.
