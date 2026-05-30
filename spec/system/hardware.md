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
- Backlight powered from 3.3V (controlled via GPIO 32 for PWM)

## Touch Controller

- XPT2046
- SPI shared with display (VSPI)

## Hardware Wiring (Elegoo ESP32)

### SPI TFT Display Wiring

The display uses the **VSPI** controller pins by default.

| TFT Pin      | ESP32 Pin                  | Purpose                            |
| ------------ | -------------------------- | ---------------------------------- |
| **VCC**      | **3V3**                    | Main power for display logic       |
| **GND**      | **GND**                    | Ground                             |
| **CS**       | **GPIO 5**                 | Chip select                        |
| **RESET**    | **GPIO 22**                | Reset line                         |
| **DC**       | **GPIO 21**                | Data/command                       |
| **MOSI**     | **GPIO 23**                | SPI MOSI (VSPI)                    |
| **MISO**     | **GPIO 19**                | SPI MISO (VSPI)                    |
| **SCK**      | **GPIO 18**                | SPI clock (VSPI)                   |
| **LED / BL** | **GPIO 32**                | Backlight (PWM capable)            |

### Touch Controller (XPT2046)

The touch controller shares the SPI bus with the display.

| Touch Pin | ESP32 Pin                  | Purpose                                    |
| --------- | -------------------------- | ------------------------------------------ |
| **T_CS**  | **GPIO 14**                | Touch chip select                          |
| **T_IRQ** | **GPIO 27**                | Touch interrupt (optional)                 |
| **T_DO**  | **GPIO 19**                | MISO (shared SPI)                          |
| **T_DIN** | **GPIO 23**                | MOSI (shared SPI)                          |
| **T_CLK** | **GPIO 18**                | SCK (shared SPI)                           |
