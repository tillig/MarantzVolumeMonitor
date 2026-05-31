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

## Example `platformio.ini` (Elegoo ESP32 DevKit V1)

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

monitor_speed = 115200

lib_deps =
    bodmer/TFT_eSPI
    paulstoffregen/XPT2046_Touchscreen
    bblanchon/ArduinoJson
    LittleFS
```

## Migration from Visual Micro

1. Install VS Code + PlatformIO extension.
2. Create project targeting `esp32dev`.
3. Copy source files into `src/` and headers into `include/` or `src/`.
4. Add libraries to `platformio.ini` under `lib_deps`.
5. Reference `TFT_eSPI_Setup.h` via `build_flags`.
6. Use PlatformIO tools for firmware and filesystem (LittleFS) uploads.
