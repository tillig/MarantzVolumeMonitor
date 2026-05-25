# Hardware Specification

## Microcontroller

- NodeMCU Mini Lua WiFi (ESP8266)
- 5V input via USB or external supply
- 3.3V logic for SPI

## Display

- 4" SPI TFT (480×320)
- ILI9341 / ILI9488 / ST7796 compatible
- Backlight powered from 5V

## Touch Controller

- XPT2046
- SPI shared with display

## Hardware Wiring (NodeMCU Mini Lua WiFi)

### SPI TFT Display Wiring

| TFT Pin      | NodeMCU Mini Pin           | Purpose                            |
| ------------ | -------------------------- | ---------------------------------- |
| **VCC**      | **5V**                     | Main power for display + backlight |
| **GND**      | **G**                      | Ground                             |
| **CS**       | **D8**                     | Chip select                        |
| **RESET**    | **D4**                     | Reset line                         |
| **DC**       | **D3**                     | Data/command                       |
| **MOSI**     | **D7**                     | SPI MOSI                           |
| **MISO**     | **D6**                     | SPI MISO                           |
| **SCK**      | **D5**                     | SPI clock                          |
| **LED / BL** | **5V** (or transistor/PWM) | Backlight                          |

### Touch Controller (XPT2046)

The touch controller power comes from the display - there is no separate pin for VCC or GND.

| Touch Pin | NodeMCU Mini Pin | Purpose                                    |
| --------- | ---------------- | ------------------------------------------ |
| **T_CS**  | **D2**           | Touch chip select                          |
| **T_IRQ** | **D1**           | Touch interrupt (optional but recommended) |
| **T_DO**  | **D6**           | MISO (shared SPI)                          |
| **T_DIN** | **D7**           | MOSI (shared SPI)                          |
| **T_CLK** | **D5**           | SCK (shared SPI)                           |
