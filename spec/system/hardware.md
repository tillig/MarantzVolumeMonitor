# Hardware Specification

## Microcontroller

- Elegoo ESP32 DevKit V1 (ESP32-WROOM-32)
- 32Mb (4MB) Flash Memory
- Dual-core Processor @ 240MHz
- USB-Serial: Silicon Labs CP2102
- 5V input via USB-C or VIN pin
- 3.3V logic level (NOT 5V tolerant)

## Display

- 4" SPI TFT (480×320)
- ST7796 Driver
- **Main Power**: Powered from **VIN (5V)** for stability.
- **Backlight**: Powered from **VIN (5V)**. If PWM dimming is desired, use a transistor controlled by **D32**.

## Touch Controller

- XPT2046
- SPI shared with display (VSPI)
- Touch alignment on this hardware requires a measured calibration transform. A 9-point affine fit
  is used in `src/ui/TouchManager.*` because simple axis-only min/max calibration plus manual
  offsets was not accurate enough for the keyboard and list screens.
- `XPT2046_Touchscreen::setRotation(1)` already rotates touch controller coordinates to match the
  display orientation. If touch drifts again, re-run the 9-point calibration capture flow and
  update the affine coefficients rather than adding screen-specific offsets.

## Hardware Wiring (Elegoo ESP32)

### SPI TFT Display Wiring

The display uses the **VSPI** controller pins. Note that labels on your board use the **D** prefix.

| TFT Pin      | ESP32 Pin Label | GPIO  | Purpose                      |
| ------------ | --------------- | ----- | ---------------------------- |
| **VCC**      | **VIN**         | 5V    | Main power for display logic |
| **GND**      | **GND**         | GND   | Ground                       |
| **CS**       | **D5**          | 5     | Chip select                  |
| **RESET**    | **D22**         | 22    | Reset line                   |
| **DC**       | **D21**         | 21    | Data/command                 |
| **MOSI**     | **D23**         | 23    | SPI MOSI (VSPI)              |
| **MISO**     | **D19**         | 19    | SPI MISO (VSPI)              |
| **SCK**      | **D18**         | 18    | SPI clock (VSPI)             |
| **LED / BL** | **VIN (or D32)**| 5V/32 | Backlight power/control      |

### Touch Controller (XPT2046)

The touch controller shares the SPI bus with the display.

| Touch Pin | ESP32 Pin Label | GPIO  | Purpose                    |
| --------- | --------------- | ----- | -------------------------- |
| **T_CS**  | **D14**         | 14    | Touch chip select          |
| **T_IRQ** | **D27**         | 27    | Touch interrupt (optional) |
| **T_DO**  | **D19**         | 19    | MISO (shared SPI)          |
| **T_DIN** | **D23**         | 23    | MOSI (shared SPI)          |
| **T_CLK** | **D18**         | 18    | SCK (shared SPI)           |
