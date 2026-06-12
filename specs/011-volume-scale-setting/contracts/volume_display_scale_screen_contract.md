# Contract: Volume Display Scale Screen

## Purpose

Define the user-visible behavior of the dedicated `Volume Display Scale` selection screen.

## Screen Content

- The screen title is `Volume Display Scale`.
- The screen presents exactly two options: `0-100` and `dB`.
- The currently selected saved option is visible on entry.
- The selected option uses a dedicated single-choice indicator and does not rely on color alone.

## Interaction Rules

- Tapping an option updates the pending selection on the screen.
- Tapping `OK` applies the pending selection, saves it, and returns to `Settings`.
- Tapping `Cancel` discards any pending selection change and returns to `Settings`.
- If the user selects a different option and then taps `Cancel`, the saved setting remains unchanged.

## Persistence Rules

- The default saved option is `0-100`.
- Reopening the screen after saving shows the persisted option as selected.
- Restarting the device preserves the last saved option.

## Acceptance Rules

- Exactly one option is visibly selected at a time.
- `OK` and `Cancel` both return to `Settings` on the first attempt.
- `Cancel` never mutates the saved scale.
- The screen remains readable and touch-friendly on the 480x320 display.
