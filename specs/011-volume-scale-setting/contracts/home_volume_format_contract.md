# Contract: Home Volume Format

## Purpose

Define how the home-screen numeric volume readout responds to the selected display scale while preserving the existing gauge behavior.

## Live Volume Presentation

- In live state, the home-screen number shows one decimal place in both `0-100` and `dB` modes.
- In `0-100` mode, the number shows the normalized home value derived from the receiver dB value.
- In `dB` mode, the number shows the raw receiver dB value.
- The gauge continues to use the normalized scale and does not change range, mapping, or animation behavior when the number format changes.

## Non-Live State Rules

- `not configured`, `unavailable`, and `powered off` states continue to suppress the live numeric value.
- Changing the saved scale while the device is in a non-live state must not replace status messaging with a numeric value.

## Motion And Redraw Rules

- Gauge motion continues to use the existing normalized target values.
- Numeric text updates may change because of scale selection, but the feature must not introduce full-screen redraw behavior into the live animation path.
- Applying the scale change must not interfere with the existing 1-second status refresh loop.

## Acceptance Rules

- The same live receiver volume produces the same gauge position in both numeric modes.
- The same live receiver volume produces different numeric text only when the two scales differ.
- Live numeric formatting remains one decimal place in both modes.
- Non-live states do not display stale or inappropriate live numbers after a scale change.
