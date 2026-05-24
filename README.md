# Marantz Volume Monitor v2

A modern, touchscreen-based volume monitor for Marantz home theater receivers, powered by the NodeMCU Mini Lua WiFi and a 4" SPI TFT display.

## Features

- **WiFi Connectivity:** No more Ethernet cables; connects directly to your home network.
- **4" Touchscreen:** Large, high-contrast display with a modern Dark Mode UI.
- **Real-time Status:** Polling-based updates for Volume, Input Source, and Audio Mode.
- **Interactive UI:** Touch-driven configuration menus for WiFi and Receiver setup.
- **Auto-Discovery:** SSDP support to find your Marantz receiver on the network automatically.

## Hardware Requirements

- **Microcontroller:** NodeMCU Mini Lua WiFi (ESP8266)
- **Display:** 4" SPI TFT Touchscreen (ST7796 Driver)
- **Power:** 5V 2A power supply (via USB or direct wiring)

Refer to `UpdatePlan.md` for detailed wiring instructions and power requirements.

## Software Setup

This project uses [PlatformIO](https://platformio.org/) for development.

1. Install [VS Code](https://code.visualstudio.com/) and the [PlatformIO extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide).
2. Clone this repository.
3. Open the project folder in VS Code.
4. PlatformIO will automatically download the required libraries.
5. Build and upload to your NodeMCU.

## Configuration

Upon first boot, use the on-screen menus to:
1. Connect to your WiFi network.
2. Enter the IP address of your Marantz receiver (or use auto-discovery).

## Project Structure

- `src/ui`: UI management and screen implementations.
- `src/network`: WiFi management and Marantz API client.
- `src/storage`: Persistent configuration storage using LittleFS.
- `docs/`: Original project plans and design specifications.

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.
