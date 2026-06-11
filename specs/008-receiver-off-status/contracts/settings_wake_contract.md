# Contract: Wake And Settings Access

## Purpose

Define how users recover from the receiver-off blank state and access Settings.

## Wake Behavior

- Any screen tap while the monitor is software-blanked due to receiver-off status wakes the monitor.
- The wake result is the visible `Receiver off` screen when receiver-off remains confirmed.
- If receiver-off can no longer be confirmed when the user wakes the monitor, the wake result is the latest current visible state.
- The wake tap is consumed by wake behavior and must not trigger Settings or setup navigation.
- After wake to visible `Receiver off`, the receiver-off timer restarts.

## Settings Access

- The visible `Receiver off` screen includes the top-right Settings entry.
- A separate tap on the visible Settings entry opens Settings.
- A non-Settings tap on the visible `Receiver off` screen restarts the 3-second timeout and keeps `Receiver off` visible.
- Settings remains visible and usable while the receiver remains off.
- Settings-launched Wi-Fi setup, receiver setup, and touch calibration flows remain visible while active.

## Returning From Settings

- Returning Home while receiver-off remains confirmed shows the visible `Receiver off` screen and restarts the 3-second timer.
- Returning Home after fresh active receiver status resumes shows the normal live volume display.
- Returning Home when receiver status is unavailable shows the receiver-unavailable state rather than the receiver-off blank cycle.

## Acceptance Rules

- A user can reach Settings from the blank state with two intentional touches: wake, then Settings.
- Settings must not blank after 3 seconds while the user is in Settings or a settings-launched setup flow.
- Receiver-off blanking resumes only after returning to the Home receiver-off state.
