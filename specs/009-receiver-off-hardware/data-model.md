# Data Model: Receiver Off Hardware Handling

## ReceiverPowerState

Represents the current user-visible receiver condition used by the Home Screen.

### Receiver Power Values

- `live`: Receiver status is fresh, powered on, and has usable current volume.
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

- `liveVisible`: Normal live volume display with backlight on.
- `receiverOffVisible`: Visible `Receiver off` state with Settings entry and backlight on.
- `receiverOffBlank`: Quiet receiver-off state with screen content blanked and hardware backlight
  signal off.
- `receiverUnavailableVisible`: Visible unavailable state with Settings entry and backlight on.
- `setupVisible`: Wi-Fi, receiver setup, or connecting state remains visible with backlight on.
- `settingsSession`: Settings or settings-launched setup flow remains visible with backlight on.

### State Transitions

- `liveVisible -> receiverOffVisible`: Fresh status confirms receiver has powered off.
- `receiverOffVisible -> receiverOffBlank`: 3 seconds pass with receiver still off and no visible
  receiver-off interaction requiring the timer to remain visible.
- `receiverOffBlank -> receiverOffVisible`: User touches the blank display while receiver remains
  off.
- `receiverOffBlank -> receiverUnavailableVisible`: User touches the blank display after
  receiver-off status can no longer be confirmed and receiver status is unavailable.
- `receiverOffBlank -> liveVisible`: Fresh active receiver status resumes while blanked.
- `receiverOffVisible -> receiverOffVisible`: User touches the visible receiver-off screen outside
  Settings before the timeout expires, restarting the 3-second timer.
- `receiverOffVisible -> settingsSession`: User taps Settings while the visible receiver-off screen is
  active.
- `settingsSession -> receiverOffVisible`: User exits Settings and receiver is still confirmed off.
- `settingsSession -> liveVisible`: User exits Settings and fresh active receiver status is available.

### Home Display Validation Rules

- `receiverOffBlank` is reachable only from confirmed receiver-off status.
- `receiverOffBlank` must not show stale live volume, source, mode, or audio-family details.
- `receiverOffBlank` must keep receiver polling and touch wake active.
- `receiverOffBlank` keeps the backlight-control signal off until user wake or fresh active receiver
  status exits the state.
- Setup, unavailable, and Settings states must force the backlight-control signal on.

## BacklightControlSignal

Represents the firmware-owned GPIO output that controls the optional hardware backlight circuit.

### Signal Fields

- `pin`: GPIO number documented for the hardware guide. Planned value: GPIO13.
- `activeHigh`: Whether driving the signal high turns the backlight on. Planned value: `true`.
- `enabledState`: Current intended output state, `on` or `off`.
- `initialized`: Whether the pin has been configured as an output during display startup.

### Signal Validation Rules

- Signal initialization occurs when the display subsystem starts.
- The signal must default to backlight on for visible states.
- The signal must be driven off only for receiver-off blank state.
- The signal must be safe when unconnected on software-only hardware.
- The signal must not require saved configuration, Settings toggles, or separate firmware builds.

## HardwareDisplayControlAvailability

Represents whether the physical device has been wired and verified for hardware backlight control.

### Availability Values

- `softwareOnly`: No verified backlight-control circuit is connected; software blanking is the only
  visible effect.
- `hardwareCapable`: The canonical or equivalent backlight-control circuit is wired and verified.
- `unknown`: Maintainer has not inspected or validated wiring.

### Availability Validation Rules

- Availability is determined by hardware inspection and validation steps in `docs/hardware.md`.
- Firmware does not store or infer availability.
- `softwareOnly` and `unknown` installations must remain safe because the control signal may be
  unconnected.

## HardwareUpgradeGuide

Represents the durable documentation deliverable for the hardware change.

### Required Sections

- Bill of materials with quantities, part identifiers, source links, and minimum ratings.
- Canonical high-side backlight-control circuit.
- Equivalent substitution rules.
- Connection table for USB input, ESP32 power pins, TFT `VCC`, TFT `LED/BL`, ESP32 GPIO13, shared
  ground, and switch module pins.
- Current-budget worksheet for the ESP32-fed display/touch and backlight-switch load path.
- Safety warnings, including no direct backlight drive from ESP32 GPIO.
- Inspection and first-power-on validation.
- Receiver-off hardware behavior validation.
- Software-only fallback validation.
- Last-resort destructive modification prerequisites, risks, and rollback limits.

### Guide Validation Rules

- Every connection has a matching inspection or functional validation step.
- The canonical path is non-destructive inline wiring.
- TFT/touch and switch-load power must originate from ESP32 power pins after USB power enters the
  ESP32, not from a separate direct external supply.
- The guide must include a pass/fail current-budget check before accepting the ESP32-fed power path.
- Destructive module modification is documented only as last resort.
- The guide must be usable before implementation is accepted.

## CanonicalBacklightCircuit

Represents the planned physical connection used by the hardware guide.

### Circuit Fields

- `switchModule`: Pololu Mini MOSFET Slide Switch LV, item `2810`.
- `vinPad`: Pololu `VIN` pad fed from ESP32 `5V`/`VIN` after USB power enters the ESP32.
- `voutPad`: Pololu `VOUT` pad feeding TFT `LED/BL`.
- `onPin`: Pololu `ON` pin driven by ESP32 GPIO13.
- `groundReference`: Shared ground from ESP32 `GND` to TFT `GND` and Pololu `GND`.
- `switchPads`: Optional Pololu switch-contact pads left open in the canonical wiring.
- `unchangedPower`: USB remains plugged into the ESP32; TFT `VCC`, TFT logic ground, touch wiring,
  and ESP32 power remain on the ESP32-fed power path.

### Circuit Validation Rules

- The circuit switches only `LED/BL`, not TFT `VCC`.
- GPIO13 must drive only the Pololu `ON` pin, not the backlight load current.
- The switch module and substitutes must be rated for the measured backlight current and ESP32-fed 5V
  supply path.
- The TFT/touch assembly and Pololu `VIN` pads must not be wired directly to a separate external
  supply.
- The installed wiring must pass the documented current-budget check for the ESP32 power pin and USB
  input path.
- The installed circuit must be inspectable and reversible without display-module modification where
  possible.
