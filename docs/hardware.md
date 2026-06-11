# Hardware Reference

## Target Hardware

- **Microcontroller**: Elegoo ESP32 DevKit V1 (ESP32-WROOM-32)
- **Display**: 4" SPI TFT, 480x320, ST7796 driver
- **Touch controller**: XPT2046
- **Power**: 5V regulated supply, 2A recommended
- **Logic level**: 3.3V GPIO. The ESP32 is not 5V tolerant.

## Power

The monitor uses one 5V 2A USB supply plugged into the ESP32. Display and touch power must remain
fed from ESP32 power pins; do not add a separate direct power feed to the TFT/touch assembly.

Current power path:

```text
USB 5V 2A supply ----> ESP32 USB

ESP32 5V/VIN pin ----> TFT VCC
ESP32 GND -----------> TFT GND
```

All components must share ground. The 4" TFT and backlight can draw significant current, so validate
the ESP32-fed 5V path before accepting the build. With the display connected and active, measure:

- USB supply voltage at the ESP32 USB input: pass if it remains near 5V without ESP32 brownouts.
- Voltage between TFT `VCC` and `GND`: pass if it remains at least 4.75V while the display is active.
- Temperature of the ESP32 USB connector, 5V/VIN pin, jumpers, and switch module after 10 minutes:
  pass only if no part is uncomfortably warm.
- Total USB supply capacity: pass only if the 5V 2A supply has margin for ESP32, TFT logic, touch,
  and backlight current together.

If the current-budget check fails, do not work around it by adding a second direct supply to the
display. Keep the software-only receiver-off blanking behavior until the ESP32-fed 5V path can be
made safe with suitable wiring/connectors and a stable 5V 2A USB supply.

## SPI TFT Display Wiring

The display uses the ESP32 VSPI pins.

| TFT Pin      | ESP32 Pin Label | GPIO | Purpose                      |
| ------------ | --------------- | ---- | ---------------------------- |
| **VCC**      | **VIN**         | 5V   | Main power for display logic |
| **GND**      | **GND**         | GND  | Ground                       |
| **CS**       | **D5**          | 5    | Chip select                  |
| **RESET**    | **D22**         | 22   | Reset line                   |
| **DC**       | **D21**         | 21   | Data/command                 |
| **MOSI**     | **D23**         | 23   | SPI MOSI                     |
| **MISO**     | **D19**         | 19   | SPI MISO                     |
| **SCK**      | **D18**         | 18   | SPI clock                    |
| **LED / BL** | **Pololu VOUT** | 5V   | Switched backlight feed      |

## Receiver-Off Backlight Hardware Control

The firmware drives ESP32 GPIO13 as an active-high backlight-control signal. When the receiver is
confirmed off, Home shows `Receiver off` for about 3 seconds, clears the display, then drives GPIO13
low so optional hardware can turn off only the TFT backlight. TFT `VCC`, touch power, ESP32 power,
receiver polling, and touch wake remain active.

### Bill Of Materials

| Qty      | Part                                            | Use                                                | Required notes                                                                                                                                   |
| -------- | ----------------------------------------------- | -------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| 1        | Pololu Mini MOSFET Slide Switch LV, item `2810` | Canonical high-side backlight switch               | Source: <https://www.pololu.com/product/2810>. Use the board's labeled `VIN`, `VOUT`, `ON`, and `GND` pads; verify the silkscreen before wiring. |
| 1        | 5V 2A USB power supply                          | Existing monitor power input                       | Plugs into ESP32 USB only. Do not split it into a second direct display feed.                                                                    |
| 3-5      | 22-26 AWG hookup wires or Dupont jumpers        | ESP32 5V/VIN, GND, GPIO13, and `LED/BL` wiring     | Use short, secure connections suitable for the enclosure.                                                                                        |
| Optional | Inline connector or small terminal block        | Reversible service point                           | Useful for bypassing the switch or removing the display without solder rework.                                                                   |
| 1        | Multimeter                                      | Continuity, voltage, and current-budget validation | Required before first power-on after rewiring.                                                                                                   |

Equivalent high-side switch modules are acceptable only if they support a 5V load path, the measured
backlight current with margin, a 3.3V logic-compatible active-high enable input, and shared-ground
operation. Document the substitute part and repeat the validation steps below.

### Canonical Wiring

Use reversible inline wiring on the TFT `LED/BL` positive feed:

```text
USB 5V 2A supply ----> ESP32 USB

ESP32 5V/VIN pin ----+----> TFT VCC
                     |
                     +----> Pololu 2810 VIN

Pololu 2810 VOUT ----> TFT LED/BL

ESP32 GPIO13 ---------------> Pololu 2810 ON

ESP32 GND -------------------+----> TFT GND
                             |
                             +----> Pololu 2810 GND
```

The two `VIN` pads are the same node, the two `VOUT` pads are the same node, and the four `GND`
pads are the same node. Use whichever matching pad locations make wiring cleanest. The separate
switch-contact pads are optional and are not used by this feature's canonical wiring because the ESP32
drives the `ON` pin directly.

Connection table:

| From                       | To                         | Purpose                          | Validation                                                                  |
| -------------------------- | -------------------------- | -------------------------------- | --------------------------------------------------------------------------- |
| USB 5V 2A supply           | ESP32 USB                  | Single power entry               | Unplugging ESP32 USB removes power from ESP32, TFT, touch, and switch load. |
| ESP32 `5V`/`VIN`           | TFT `VCC`                  | Display logic/touch module power | Directly connected, not switched.                                           |
| ESP32 `5V`/`VIN`           | Pololu 2810 `VIN`          | Backlight source input           | Comes from ESP32 power pin, not a separate external supply.                 |
| Pololu 2810 `VOUT`         | TFT `LED/BL`               | Switched backlight positive feed | Only `LED/BL` is routed through the switch.                                 |
| ESP32 GPIO13               | Pololu 2810 `ON`           | Firmware backlight command       | GPIO13 drives only the `ON` pin, never backlight current.                   |
| ESP32 `GND`                | TFT `GND` and Pololu `GND` | Shared reference                 | Continuity check passes before power-on.                                    |
| Pololu switch-contact pads | Leave open                 | Optional manual switch input     | Not used in canonical ESP32-controlled wiring.                              |

### Unsafe Or Out-Of-Scope Wiring

- Do not drive TFT `LED/BL` directly from GPIO13.
- Do not connect ESP32 GPIO13 to any 5V output. ESP32 GPIO is 3.3V logic and is not 5V tolerant.
- Do not substitute GPIO12 for this feature's `ON` connection; GPIO12 is an ESP32 strapping pin and
  is a less robust choice for this startup-sensitive control signal.
- Do not switch TFT `VCC`; touch wake and display logic must remain powered.
- Do not add a separate direct external supply to TFT `VCC`, touch power, or the Pololu `VIN` pads.
- Disconnect USB power before rewiring or moving jumpers.
- Do not treat small-signal MOSFETs such as `2N7000FS-ND` / onsemi `2N7000` as drop-in substitutes.

The `2N7000FS-ND` parts are useful bench parts, but they are not the planned switch for this upgrade.
They are N-channel low-side small-signal MOSFETs, not high-side load-switch modules for the positive
`LED/BL` feed. They also have limited continuous-current and 3.3V-gate-drive margin for an unknown
TFT backlight load. Use them only for separate experiments where the display exposes a measured
low-current logic-enable input or an isolated low-current backlight return.

Adafruit STEMMA MOSFET Driver, item `5648`, is a conditional low-side substitute only when the
specific display exposes an isolated backlight return that can be switched without breaking TFT
logic, touch power, or shared ground. Source: <https://www.adafruit.com/product/5648>. It is not the
canonical path because the current display wiring exposes the positive `LED/BL` path.

### First-Power-On Validation

With USB disconnected:

1. Confirm TFT `VCC` remains connected to ESP32 `5V`/`VIN`.
2. Confirm one of the Pololu `VIN` pads is connected to ESP32 `5V`/`VIN`.
3. Confirm one of the Pololu `VOUT` pads is connected only to TFT `LED/BL`.
4. Confirm GPIO13 is connected to the Pololu `ON` pin.
5. Confirm ESP32 `GND`, TFT `GND`, and any Pololu `GND` pad have continuity.
6. Confirm the optional switch-contact pads are left open unless you intentionally add a manual switch.
7. Confirm no separate external supply is connected directly to TFT/touch or the Pololu `VIN` pads.

Then power the ESP32 by USB:

1. Confirm setup, Settings, receiver unavailable, and live states remain readable.
2. Confirm the ESP32-fed current-budget checks in the Power section pass.
3. Turn the receiver off while it remains reachable and confirm `Receiver off` appears.
4. Confirm the backlight turns off after about 3 seconds while the ESP32 remains running.
5. Tap the dark display and confirm the backlight turns on and the wake tap does not open Settings.
6. While blanked, turn the receiver active and confirm live volume returns without reboot.
7. While blanked, make receiver-off confirmation unavailable, then tap and confirm the latest visible
   state appears rather than assuming the receiver is still off.
8. Restart the monitor while the receiver is already off and confirm touch wake and Settings remain
   recoverable.

### Software-Only Fallback And Rollback

If GPIO13 is unconnected, the firmware remains safe. Receiver-off handling still clears the screen in
software, touch wake still works, and the backlight-control pin has no effect.

To roll back the hardware upgrade, disconnect USB power and bypass the Pololu module by reconnecting
ESP32 `5V`/`VIN` directly to TFT `LED/BL`. Disconnect GPIO13 from `ON`, leave the switch-contact pads
open, and insulate any loose leads. After rollback, repeat software-only validation with GPIO13
unconnected.

### Last-Resort Display Module Modification

Prefer external inline wiring. Only modify display-module traces, jumpers, solder bridges, or onboard
resistors if all of these are true:

- The module cannot expose `LED/BL` through normal pins or inline wiring.
- The backlight path has been identified with a schematic, continuity measurements, or clear module
  markings.
- The change can be documented with photos and measured before/after continuity checks.
- You accept that rollback may require solder repair or replacing the display module.

Do not modify the display module just to use a low-side switch. If the backlight path cannot be
isolated confidently, keep the software-only fallback.

## Touch Controller Wiring

The XPT2046 touch controller shares the SPI bus with the display.

| Touch Pin | ESP32 Pin Label | GPIO | Purpose                   |
| --------- | --------------- | ---- | ------------------------- |
| **T_CS**  | **D14**         | 14   | Touch chip select         |
| **T_IRQ** | **D27**         | 27   | Touch interrupt, optional |
| **T_DO**  | **D19**         | 19   | MISO, shared SPI          |
| **T_DIN** | **D23**         | 23   | MOSI, shared SPI          |
| **T_CLK** | **D18**         | 18   | SCK, shared SPI           |

## Touch Calibration

Touch alignment on this hardware uses a measured 9-point affine transform in `src/ui/TouchManager.*`.
Simple axis min/max calibration plus manual per-screen offsets was not accurate enough for the
keyboard and list screens.

`XPT2046_Touchscreen::setRotation(1)` already rotates controller readings to match the display
orientation. If alignment drifts, use the `CAL` button on the unconfigured/setup Home Screen to
capture a new 9-point dataset from the serial console, then update the affine coefficients in
`TouchManager`. Do not add screen-specific hitbox offsets unless new hardware data proves the affine
model is wrong.
