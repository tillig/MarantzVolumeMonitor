# Contract: Home Display States

## Purpose

Define the user-visible home-screen states for live, powered-off, unavailable, and setup-related
conditions.

## Live State

- Show the current receiver volume as the dominant home value.
- Format the live volume with one decimal place.
- Show the normal home hierarchy: volume, caption, source, listening mode, and audio-family status.
- Keep the Settings icon in its normal top-right position.

## Powered-Off State

- Show a powered-off receiver message such as `Receiver off`.
- Do not label this state as unavailable.
- Do not present the last known volume as current.
- Keep Settings accessible so the user can reconfigure or replace the saved receiver target.

## Unavailable State

- Show a receiver-unavailable message distinct from the powered-off message.
- Do not present the last known volume as current.
- Keep Settings accessible so the user can reconfigure or replace the saved receiver target.

## Setup-Required States

- Preserve the existing Wi-Fi setup, receiver setup, and calibration entry paths.
- Do not regress the current onboarding behavior while adding powered-off handling.

## State Transition Rules

- `live -> poweredOff`: occurs when a valid receiver response reports `power == false`.
- `live -> receiverUnavailable`: occurs when the configured receiver no longer returns usable live
  status.
- `poweredOff -> live`: occurs when valid receiver status resumes with `power == true`.
- `receiverUnavailable -> live`: occurs when valid receiver status resumes.
- Setup-required states continue to follow existing Wi-Fi and receiver-configuration transitions.

## Acceptance Rules

- Powered-off and unavailable states are visually distinct.
- Both powered-off and unavailable states preserve a Settings path.
- No non-live state continues to imply that stale volume is current.
