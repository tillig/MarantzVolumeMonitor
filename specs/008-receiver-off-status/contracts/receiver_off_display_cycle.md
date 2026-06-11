# Contract: Receiver-Off Display Cycle

## Purpose

Define the Home Screen behavior when the saved receiver is confirmed powered off.

## Entry Conditions

- Wi-Fi configuration exists.
- Wi-Fi is connected.
- Receiver configuration exists.
- Current receiver status is fresh, valid, and confirms powered-off state.

## Visible Receiver-Off State

- Show `Receiver off`.
- Do not show the last live volume as current.
- Do not show stale source, listening mode, or audio-family details as current.
- Show the normal Settings entry.
- Start a 3-second receiver-off timer.
- A touch outside the Settings entry restarts the 3-second timer and keeps `Receiver off` visible.

## Software Blank State

- When the timer expires and receiver-off remains confirmed, render a black/blank display.
- Continue normal receiver status polling.
- Continue normal touch detection.
- Do not show any persistent status message, live-volume content, source, mode, or audio-family content.
- Do not require physical backlight control.
- If receiver-off confirmation is later lost while blanked, remain blank until touch or fresh active receiver status.

## Exit Conditions

- If fresh active receiver status becomes available, return to normal live volume display.
- If the user touches the blank display and receiver-off remains current, wake to visible `Receiver off` and restart the timer.
- If the user touches the blank display after receiver-off can no longer be confirmed, wake to the latest current visible state.

## Acceptance Rules

- The blank state is reachable only from confirmed receiver-off status.
- Unavailable receiver, Wi-Fi disconnected, missing receiver config, and unknown power state do not enter this display cycle.
- The first touch while blanked wakes only; it does not activate Settings in the same event.
- Non-Settings touches while `Receiver off` is visible reset the timeout.
