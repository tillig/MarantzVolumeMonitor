# MarantzVolumeMonitor v2 — Modernization Plan

Target Platform: NodeMCU Mini Lua WiFi + 4" SPI TFT Touchscreen
Goal: Replace Arduino + LCD + Ethernet shield with a WiFi‑enabled touchscreen system that displays Marantz receiver volume/source/channel info and adds configuration menus.

---

## 1. Project Overview

The original project uses:

- Arduino Uno R3
- DFRobot 1602 LCD Shield
- W5100 Ethernet Shield
- Marantz HTTP API polling for:
  - Volume
  - Input source
  - Audio channel mode

The new version will:

- Use a **NodeMCU Mini Lua WiFi** board
- Use a **4" SPI TFT touchscreen** (ILI9341 / ILI9488 / ST7796)
- Connect via **WiFi** instead of Ethernet
- Add **touch‑driven configuration menus**
- Add **WiFi setup** (SSID/password)
- Add **receiver auto‑discovery** (SSDP/UPnP) or manual IP entry
- Maintain the same core functionality: display real‑time receiver status
- Migrate tooling from **Visual Micro** to **PlatformIO** in VS Code

---

## 2. Hardware Plan

### 2.1 Selected Microcontroller

Use the **NodeMCU Mini Lua WiFi** because:

- It exposes a **5V pin**, ideal for powering a large TFT display
- It has a compact footprint
- It provides the full D0–D8 pin set needed for SPI + touch
- It simplifies wiring and reduces power‑related issues

### 2.2 Required Components

- NodeMCU Mini Lua WiFi
- 4" SPI TFT display (ILI9341 / ILI9488 / ST7796)
- Touch controller (XPT2046 or integrated)
- **5V power source** (USB charger or dedicated 5V 2A supply)
- Optional: transistor + resistor for PWM backlight control
- Shared ground between all components
- Optional: 3D‑printed or laser‑cut enclosure

---

## 3. Power Wiring (Critical)

A 4" TFT display draws **300–800 mA**, which is too much for weak USB supplies or the NodeMCU’s onboard regulator.
You have **two valid powering options**:

---

### 3.1 Option A — Power via USB Charger (Simple, Good for Development)

- Plug a **USB wall charger (2A recommended)** into the NodeMCU’s USB port
- The NodeMCU’s USB 5V rail powers:
  - The board itself
  - The **5V pin**, which can power the TFT

**Wiring:**

```text
USB Charger → NodeMCU USB Port
NodeMCU 5V → TFT VCC
NodeMCU GND → TFT GND
```

This works well **as long as the USB charger can supply at least 1A**.

---

### 3.2 Option B — Dedicated 5V Power Supply (Recommended for Final Build)

Use a **5V 2A regulated supply** and wire it directly:

```text
[5V Supply +] ────────┬────────> NodeMCU 5V
                      │
                      └────────> TFT VCC

[5V Supply GND] ──────┬────────> NodeMCU GND
                      │
                      └────────> TFT GND
```

This provides maximum stability and avoids brownouts.

---

### 3.3 Notes

- The NodeMCU generates **3.3V logic** internally; SPI pins are already 3.3V.
- Most ESP‑compatible TFT modules accept **5V power** but use **3.3V logic**.
- If the TFT’s **LED/BL** pin is raw (no resistor), use a transistor + resistor for PWM brightness control.
- If the TFT has onboard regulation + level shifting, powering VCC from 5V is correct.

---

## 4. Hardware Wiring (NodeMCU Mini Lua WiFi)

### 4.1 SPI TFT Display Wiring

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

### 4.2 Touch Controller (XPT2046)

| Touch Pin | NodeMCU Mini Pin                     | Purpose                                    |
| --------- | ------------------------------------ | ------------------------------------------ |
| **T_CS**  | **D2**                               | Touch chip select                          |
| **T_IRQ** | **D1**                               | Touch interrupt (optional but recommended) |
| **T_DO**  | **D6**                               | MISO (shared SPI)                          |
| **T_DIN** | **D7**                               | MOSI (shared SPI)                          |
| **T_CLK** | **D5**                               | SCK (shared SPI)                           |
| **VCC**   | **3V3** or **5V** (module‑dependent) | Touch controller power                     |
| **GND**   | **G**                                | Ground                                     |

---

## 5. Software Architecture

```text
/src
  /ui
    DisplayManager.cpp
    TouchManager.cpp
    Screens/
      HomeScreen.cpp
      SettingsScreen.cpp
      WiFiSetupScreen.cpp
      ReceiverSetupScreen.cpp
  /network
    WiFiManager.cpp
    ReceiverDiscovery.cpp
    MarantzClient.cpp
  /storage
    ConfigStore.cpp (SPIFFS/LittleFS)
  main.cpp
```

### 5.1 Libraries

- TFT_eSPI or Adafruit_ILI9341
- XPT2046_Touchscreen
- ArduinoJson
- ESP8266WiFi / WiFi.h
- ESPmDNS
- LittleFS or SPIFFS
- Optional: ESPAsyncWebServer

---

## 6. UI / UX Flow

There is a full detailed UI mockup in the [UIPlan.md](UIPlan.md) file. Below is a quick summary, but check the full plan for details.

### 6.1 Home Screen

- Large volume indicator
- Current input source
- Audio channel mode
- Auto‑refresh every 1–2 seconds
- Tap top‑right corner → Settings

### 6.2 Settings Menu

- **WiFi Setup**
  - Scan networks
  - Enter password via on‑screen keyboard
- **Receiver Setup**
  - Auto‑discover via SSDP/UPnP
  - Manual IP entry
- **Display Settings**
  - Brightness
  - Theme (optional)

### 6.3 On‑Screen Keyboard

- QWERTY layout
- Numeric mode for IP entry

---

## 7. Networking Logic

### 7.1 WiFi Setup

- On first boot:
  - If no stored SSID/password → show WiFi setup screen
- Store credentials in LittleFS/SPIFFS
- Auto‑reconnect on boot

### 7.2 Receiver Discovery

- SSDP/UPnP search for Marantz/Denon devices
- If found: store IP
- If not: prompt for manual IP

### 7.3 Marantz API Polling

- Poll every 1–2 seconds
- Fetch:
  - Volume
  - Input source
  - Channel mode
- Use non‑blocking HTTP client

---

## 8. Development Phases

### Phase 1 — Hardware Bring‑Up

- Wire TFT + touch
- Run TFT test sketch
- Run touch calibration
- Validate SPI performance

### Phase 2 — Core Software

- Implement DisplayManager
- Implement TouchManager
- Build HomeScreen UI
- Implement MarantzClient

### Phase 3 — Configuration System

- Implement ConfigStore
- Build WiFi setup screen
- Build receiver setup screen
- Add SSDP discovery

### Phase 4 — Integration

- Connect UI → ConfigStore → Network → MarantzClient
- Add error handling
- Add auto‑retry logic

### Phase 5 — Polish

- Add animations
- Add brightness control
- Optional: web‑based configuration portal

---

## 9. Deliverables for Coding Tool

- Hardware wiring diagram
- Full folder structure
- Class stubs for all modules
- TFT + touch initialization code
- WiFi setup workflow
- Receiver discovery + Marantz API client
- UI screens + navigation logic
- Persistent config storage

---

## 10. Tooling Migration to PlatformIO (VS Code)

The original project used **Visual Studio + Visual Micro**, which emulates the Arduino IDE build system.
For the modernization effort, migrate to **PlatformIO** inside **Visual Studio Code** to gain:

- A consistent, reproducible build environment
- Per‑board configuration (ESP8266/ESP32)
- Automatic library dependency resolution
- Built‑in serial monitor, upload, and filesystem tools
- Cleaner project structure aligned with modern embedded development

### 10.1 PlatformIO Project Structure

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

### 10.2 Example `platformio.ini` (ESP8266 NodeMCU Mini Lua WiFi)

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

### 10.3 Migration Steps

1. Install VS Code + PlatformIO extension
2. Create a new PlatformIO project targeting `nodemcuv2`
3. Copy your source files into the `src/` directory
4. Move configuration headers into `include/` or `src/`
5. Add libraries to `platformio.ini` under `lib_deps`
6. Add TFT_eSPI custom setup file and reference it via `build_flags`
7. Move static assets (HTML, JSON, etc.) into `data/` if needed
8. Use PlatformIO’s upload tools for firmware and filesystem
9. Remove Visual Micro–specific files (`*.vcxproj`, `*.ino`, etc.)
10. Commit the new structure to version control

### 10.4 Benefits After Migration

- No more Arduino IDE quirks
- Deterministic builds across machines
- Easy switching between ESP8266 and ESP32
- Cleaner dependency management
- Built‑in filesystem uploader for LittleFS/SPIFFS
- Better IntelliSense and code navigation
