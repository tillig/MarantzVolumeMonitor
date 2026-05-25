# Hardware Specification

## Microcontroller

- NodeMCU V3 (ESP8266)
- 32Mb (4MB) Flash Memory
- USB-Serial: CH340G
- 5V input via USB or external supply
- 3.3V logic for SPI

## Display

- 4" SPI TFT (480×320)
- ILI9341 / ILI9488 / ST7796 compatible
- Backlight powered from 5V

## Touch Controller

- XPT2046
- SPI shared with display

## Hardware Wiring (NodeMCU V3)

### SPI TFT Display Wiring

| TFT Pin      | NodeMCU V3 Pin             | Purpose                            |
| ------------ | -------------------------- | ---------------------------------- |
| **VCC**      | **WN (VIN)**               | Main power for display + backlight |
| **GND**      | **GND**                    | Ground                             |
| **CS**       | **D8**                     | Chip select                        |
| **RESET**    | **D4**                     | Reset line                         |
| **DC**       | **D3**                     | Data/command                       |
| **MOSI**     | **D7**                     | SPI MOSI                           |
| **MISO**     | **D6**                     | SPI MISO                           |
| **SCK**      | **D5**                     | SPI clock                          |
| **LED / BL** | **WN (VIN)** (or PWM)      | Backlight                          |

### Touch Controller (XPT2046)

The touch controller power comes from the display - there is no separate pin for VCC or GND.

| Touch Pin | NodeMCU V3 Pin   | Purpose                                    |
| --------- | ---------------- | ------------------------------------------ |
| **T_CS**  | **D2**           | Touch chip select                          |
| **T_IRQ** | **D1**           | Touch interrupt (optional but recommended) |
| **T_DO**  | **D6**           | MISO (shared SPI)                          |
| **T_DIN** | **D7**           | MOSI (shared SPI)                          |
| **T_CLK** | **D5**           | SCK (shared SPI)                           |
