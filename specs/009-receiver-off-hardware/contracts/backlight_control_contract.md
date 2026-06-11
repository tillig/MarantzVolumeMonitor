# Contract: Backlight Control Behavior

## Purpose

Define the firmware-visible behavior for the optional receiver-off hardware backlight-control signal.

## Hardware Signal

- The documented control signal is ESP32 GPIO13.
- The signal is active-high: high means backlight on, low means backlight off.
- The signal controls only the optional backlight switch circuit.
- The signal must be safe when unconnected on software-only hardware.
- The signal must not directly power the TFT backlight load.

## Initialization

- Display startup configures the backlight-control signal as an output.
- Display startup drives the signal on before or while the visible UI is initialized.
- No Settings option, saved preference, or alternate firmware build is required.

## State Mapping

| Home or UI state | Backlight signal |
| --- | --- |
| Live volume display | On |
| Receiver-off visible state | On |
| Receiver-off blank state | Off |
| Receiver unavailable | On |
| Wi-Fi setup required | On |
| Wi-Fi connecting | On |
| Receiver setup required | On |
| Settings or settings-launched setup | On |

## Entry Rules

- Entering `receiverOffBlank` turns the backlight signal off after the existing software blank draw.
- Entering any visible state turns the backlight signal on before the user must read or touch the
  screen.
- Returning from Settings to Home follows the current receiver status classification before deciding
  whether the backlight remains on or later turns off after the receiver-off timeout.

## Exit Rules

- A wake touch while `receiverOffBlank` turns the backlight signal on and consumes the touch.
- Fresh active receiver status while blanked turns the backlight signal on and returns to live volume.
- If receiver-off confirmation is lost while blanked, the backlight signal remains off until touch or
  fresh active receiver status.

## Acceptance Rules

- Software-only hardware with GPIO13 unconnected still blanks visually and wakes by touch.
- Hardware-capable wiring turns the backlight off only in receiver-off blank state.
- Hardware-capable wiring never turns off TFT logic power, touch power, ESP32 power, or receiver
  power.
- Backlight transitions must not block the existing 1-second receiver polling cadence.
