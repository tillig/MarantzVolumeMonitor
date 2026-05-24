# Tooling Specification — PlatformIO

## Overview

The project uses **PlatformIO** inside **Visual Studio Code** to ensure a consistent, reproducible build environment and automated library management.

## PlatformIO Project Structure

```text
MarantzVolumeMonitorV2/
  platformio.ini
  src/
    main.cpp
    ui/
    network/
    storage/
  include/
  lib/
  data/        (for LittleFS/SPIFFS)
```

## Example `platformio.ini` (NodeMCU Mini Lua WiFi)

```ini
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
framework = arduino

monitor_speed = 115200

lib_deps =
    bodmer/TFT_eSPI
    paulstoffregen/XPT2046_Touchscreen
    bblanchon/ArduinoJson
    ESP8266WiFi
    ESP8266mDNS

build_flags =
    -DUSER_SETUP_LOADED
    -include src/TFT_eSPI_Setup.h
```

## Migration from Visual Micro

1. Install VS Code + PlatformIO extension.
2. Create project targeting `nodemcuv2`.
3. Copy source files into `src/` and headers into `include/` or `src/`.
4. Add libraries to `platformio.ini` under `lib_deps`.
5. Reference `TFT_eSPI_Setup.h` via `build_flags`.
6. Use PlatformIO tools for firmware and filesystem (LittleFS) uploads.

