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

## Wiring

- TFT: MOSI=D7, MISO=D6, SCK=D5, CS=D8, DC=D3, RESET=D4
- Touch: T_CS=D2, T_IRQ=D1
