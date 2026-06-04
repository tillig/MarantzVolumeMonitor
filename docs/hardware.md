# Hardware Reference

## Target Hardware

- **Microcontroller**: Elegoo ESP32 DevKit V1 (ESP32-WROOM-32)
- **Display**: 4" SPI TFT, 480x320, ST7796 driver
- **Touch controller**: XPT2046
- **Power**: 5V regulated supply, 2A recommended
- **Logic level**: 3.3V GPIO. The ESP32 is not 5V tolerant.

## Power

The 4" TFT can draw roughly 300-800 mA, so weak USB supplies and the ESP32's onboard regulator are
not reliable power sources for the complete device.

For development, a USB-C wall charger can power the ESP32 through USB while the ESP32 VIN pin powers
the TFT VCC pin. Use a charger that can supply at least 1A.

For the final build, use a regulated 5V 2A supply wired to both the ESP32 VIN pin and the TFT VCC pin:

```text
[5V Supply +] ----+----> ESP32 VIN
                  |
                  +----> TFT VCC

[5V Supply GND] --+----> ESP32 GND
                  |
                  +----> TFT GND
```

All components must share ground. If the TFT LED/BL pin is raw and brightness control is needed, use
a transistor and resistor for PWM dimming rather than driving the backlight directly from a GPIO.

## SPI TFT Display Wiring

The display uses the ESP32 VSPI pins.

| TFT Pin      | ESP32 Pin Label | GPIO  | Purpose                      |
| ------------ | --------------- | ----- | ---------------------------- |
| **VCC**      | **VIN**         | 5V    | Main power for display logic |
| **GND**      | **GND**         | GND   | Ground                       |
| **CS**       | **D5**          | 5     | Chip select                  |
| **RESET**    | **D22**         | 22    | Reset line                   |
| **DC**       | **D21**         | 21    | Data/command                 |
| **MOSI**     | **D23**         | 23    | SPI MOSI                     |
| **MISO**     | **D19**         | 19    | SPI MISO                     |
| **SCK**      | **D18**         | 18    | SPI clock                    |
| **LED / BL** | **VIN or D32**  | 5V/32 | Backlight power/control      |

## Touch Controller Wiring

The XPT2046 touch controller shares the SPI bus with the display.

| Touch Pin | ESP32 Pin Label | GPIO | Purpose                    |
| --------- | --------------- | ---- | -------------------------- |
| **T_CS**  | **D14**         | 14   | Touch chip select          |
| **T_IRQ** | **D27**         | 27   | Touch interrupt, optional  |
| **T_DO**  | **D19**         | 19   | MISO, shared SPI           |
| **T_DIN** | **D23**         | 23   | MOSI, shared SPI           |
| **T_CLK** | **D18**         | 18   | SCK, shared SPI            |

## Touch Calibration

Touch alignment on this hardware uses a measured 9-point affine transform in `src/ui/TouchManager.*`.
Simple axis min/max calibration plus manual per-screen offsets was not accurate enough for the
keyboard and list screens.

`XPT2046_Touchscreen::setRotation(1)` already rotates controller readings to match the display
orientation. If alignment drifts, use the `CAL` button on the unconfigured/setup Home Screen to
capture a new 9-point dataset from the serial console, then update the affine coefficients in
`TouchManager`. Do not add screen-specific hitbox offsets unless new hardware data proves the affine
model is wrong.
