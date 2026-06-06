# Data Model: Receiver Off Screen Sleep

## ReceiverPowerState

Represents the current user-visible receiver condition used by the Home Screen.

### Receiver Power Values

- `live`: Receiver status is fresh, powered on, and has a usable current volume.
- `receiverOff`: Receiver status is fresh and confirms the saved receiver is powered off.
- `receiverUnavailable`: Receiver is configured but does not provide usable current status.
- `wifiSetupRequired`: No saved Wi-Fi configuration exists.
- `wifiConnecting`: Saved Wi-Fi exists but the device is not connected.
- `receiverSetupRequired`: Wi-Fi is connected but no saved receiver target exists.

### Receiver Power Validation Rules

- `receiverOff` requires fresh current receiver status with known powered-off state.
- Missing volume, failed refresh, unknown power state, missing Wi-Fi, or missing receiver
  configuration must not produce `receiverOff`.
- `live` requires fresh current receiver status with powered-on state and usable volume.

## HomeDisplayMode

Represents what the Home Screen is currently presenting for the receiver condition.

### Home Display Values

- `liveVisible`: Normal live volume display.
- `receiverOffVisible`: Visible `Receiver off` state with Settings entry.
- `receiverOffBlank`: Software-blanked black display entered after receiver-off timeout.
- `receiverUnavailableVisible`: Visible unavailable state with Settings entry.
- `setupVisible`: Wi-Fi, receiver setup, or connecting state remains visible.

### State Transitions

- `liveVisible -> receiverOffVisible`: Fresh status confirms receiver has powered off.
- `receiverOffVisible -> receiverOffBlank`: 3 seconds pass with receiver still off and no visible
  receiver-off interaction requiring the timer to remain visible.
- `receiverOffBlank -> receiverOffVisible`: User touches the blank display while receiver remains
  off.
- `receiverOffBlank -> receiverUnavailableVisible`: User touches the blank display after
  receiver-off status can no longer be confirmed and receiver status is unavailable.
- `receiverOffVisible -> receiverOffVisible`: User touches the visible receiver-off screen outside
  Settings before the timeout expires, restarting the 3-second timer.
- `receiverOffBlank -> liveVisible`: Fresh active receiver status resumes while blanked.
- `receiverOffVisible -> liveVisible`: Fresh active receiver status resumes before the timeout.
- `receiverOffVisible -> settingsSession`: User taps Settings while the visible receiver-off screen is
  active.
- `settingsSession -> receiverOffVisible`: User exits Settings and receiver is still confirmed off.
- `settingsSession -> liveVisible`: User exits Settings and fresh active receiver status is available.
- `receiverOffVisible -> receiverUnavailableVisible`: Receiver-off status can no longer be freshly
  confirmed before the display has blanked and receiver status is unavailable.

### Home Display Validation Rules

- `receiverOffBlank` is reachable only from `receiverOffVisible`.
- `receiverOffBlank` must not show stale live volume, source, mode, or audio-family details.
- `receiverOffBlank` must continue to allow touch wake and status refresh.
- `receiverOffBlank` remains blank if receiver-off confirmation is lost after blanking, unless active
  receiver status resumes or the user touches the screen.
- `receiverUnavailableVisible` and setup states must not enter `receiverOffBlank`.

## ReceiverOffTimer

Tracks the visible receiver-off period before software blanking.

### Timer Fields

- `startedAtMs`: Timestamp when the visible receiver-off period began.
- `durationMs`: Timeout duration. Initial value is 3000 ms.
- `active`: Whether the timer is currently controlling receiver-off blanking.

### Timer Validation Rules

- Timer starts when Home enters `receiverOffVisible` from any non-blank state.
- Timer restarts when the monitor wakes from `receiverOffBlank` to `receiverOffVisible`.
- Timer restarts when the user touches visible `receiverOffVisible` outside the Settings entry.
- Timer restarts when returning Home from Settings while receiver-off remains confirmed.
- Timer is inactive while Home is live, unavailable, setup-required, or while another screen is
  active.

## WakeInteraction

Represents a touch event while the monitor is software-blanked due to receiver-off status.

### Wake Fields

- `touchPoint`: Calibrated touch point captured by the existing touch dispatch path.
- `receivedAtMs`: Timestamp of wake touch.

### Wake Validation Rules

- Any valid touch point wakes from `receiverOffBlank`.
- Wake interaction must not activate Settings or any setup flow in the same touch event.
- If receiver-off remains confirmed after wake, the visible receiver-off timer starts again.
- If receiver-off can no longer be confirmed after wake, the latest current visible state is shown.

## SettingsSession

Represents Settings or settings-launched setup screens while the receiver may be off.

### Settings Validation Rules

- Receiver-off timeout does not apply during Settings or settings-launched setup screens.
- Settings remains visible and usable while receiver stays off.
- Returning Home re-evaluates receiver status and chooses live, receiver-off visible, unavailable, or
  setup-visible state.
