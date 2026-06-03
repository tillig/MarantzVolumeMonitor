# Marantz Volume Monitor v2

A modern, touchscreen-based volume monitor for Marantz home theater receivers, powered by an ESP32 and a 4" SPI TFT display.

## Features

- **Wi-Fi Connectivity:** No more Ethernet cables; connects directly to your home network.
- **4" Touchscreen:** Large, high-contrast display with a modern Dark Mode UI.
- **Real-time Status:** Polling-based updates for Volume, Input Source, and Audio Mode.
- **Interactive UI:** Touch-driven configuration menus for Wi-Fi and Receiver setup.
- **Auto-Discovery:** SSDP support to find your Marantz receiver on the network automatically.

## Hardware Requirements

- **Microcontroller:** Elegoo ESP32 DevKit V1
- **Display:** 4" SPI TFT Touchscreen (ST7796 Driver)
- **Power:** 5V 2A power supply (via USB-C or VIN)

Refer to `spec/system/hardware.md` and `spec/system/power.md` for detailed wiring instructions and power requirements.

## Software Setup

This project uses [PlatformIO](https://platformio.org/) for development and [Spec Kit](https://github.com/tillig/speckit) for project management.

1. Install [VS Code](https://code.visualstudio.com/) and the [PlatformIO extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide).
2. Clone this repository.
3. Open the project folder in VS Code.
4. PlatformIO will automatically download the required libraries.
5. Build and upload to your ESP32.

## Configuration

Upon first boot, use the on-screen menus to:

1. Connect to your Wi-Fi network.
2. Enter the IP address of your Marantz receiver (or use auto-discovery).

Successful Wi-Fi setup stores credentials in `config.json` on LittleFS. On later boots, the device
attempts to reconnect automatically using the saved SSID and password.

If touch alignment is off, use the small `CAL` button on the unconfigured/setup Home Screen to
open the calibration capture flow. That screen records a 9-point touch dataset to the serial
console so the touch transform can be recalculated from measured hardware data instead of
per-screen offsets.

## Project Structure

- `src/ui`: UI management and screen implementations.
- `src/network`: Wi-Fi management and Marantz API client.
- `src/storage`: Persistent configuration storage using LittleFS.
- `spec/`: Technical specifications, architectural plans, and hardware requirements.
- `.gemini/`: Custom commands and configurations for Spec Kit.

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.
