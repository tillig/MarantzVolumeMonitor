# Architecture Reference

## Layers

The firmware uses a small layered architecture:

- **UI layer**: Screens, display rendering, touch handling, and screen navigation.
- **Network layer**: Wi-Fi connection management, SSDP/UPnP receiver discovery, and Marantz HTTP status access.
- **Storage layer**: Persistent device configuration using LittleFS.
- **Hardware layer**: TFT_eSPI display access and XPT2046 touch input.

Screens should not perform network I/O directly. They should delegate network work to the network layer and persistence to `ConfigStore`.

## Source Layout

```text
src/
├── main.cpp
├── network/
│   ├── MarantzClient.h/.cpp
│   ├── ReceiverDiscovery.h/.cpp
│   └── WiFiManager.h/.cpp
├── storage/
│   └── ConfigStore.h/.cpp
└── ui/
    ├── DisplayManager.h/.cpp
    ├── IconRenderer.h/.cpp
    ├── MaterialStyle.h/.cpp
    ├── ScreenManager.h/.cpp
    ├── TouchManager.h/.cpp
    ├── assets/
    │   └── IconBitmaps.h/.cpp
    └── Screens/
        ├── HomeScreen.h/.cpp
        ├── SettingsScreen.h/.cpp
        ├── CurrentSettingsScreen.h/.cpp
        ├── NetworkListScreen.h/.cpp
        ├── KeyboardScreen.h/.cpp
        ├── SetupStatusScreen.h/.cpp
        ├── ReceiverListScreen.h/.cpp
        ├── ReceiverIpScreen.h/.cpp
        ├── ReceiverStatusScreen.h/.cpp
        └── CalibrationScreen.h/.cpp
```

Feature work may add classes inside these existing layer folders. New cross-layer behavior should keep the ownership boundary clear: UI presents state and handles touch, network classes perform network operations, and storage classes read or write persistent configuration.

## Runtime Flow

1. Boot initializes display, touch, and LittleFS-backed configuration storage, then attempts Wi-Fi auto-connect and applies any saved receiver IP.
2. `HomeScreen` is the first screen and classifies the current state as Wi-Fi setup required, Wi-Fi connecting, receiver setup required, receiver unavailable, receiver off, or live.
3. If Wi-Fi is not configured, the touchscreen flow branches through `NetworkListScreen`, `KeyboardScreen`, and `SetupStatusScreen` to collect and save credentials.
4. If a receiver is not configured, the setup flow branches through `ReceiverListScreen`, `ReceiverIpScreen`, and `ReceiverStatusScreen` to discover or verify a receiver address before saving it.
5. `SettingsScreen` provides access to `Current Settings`, Wi-Fi setup, receiver setup, and touch calibration, with pagination when all entries do not fit cleanly on one page.
6. `HomeScreen` and `CurrentSettingsScreen` refresh live Wi-Fi and receiver state on an approximately 1-second cadence while visible. The current implementation performs synchronous status fetches through the network layer from those screen update paths.
7. The live Home Screen renders normalized volume (`receiver dB + 80`), source, listening mode, audio-family icons, and receiver-off blank/backlight behavior.

## Persisted Configuration

`ConfigStore` stores the device configuration in `/config.json` on LittleFS. The current schema includes:

- `wifiSsid`
- `wifiPassword`
- `receiverIp`
- `brightness`
- `useDbScale`

The active on-device UI currently uses the Wi-Fi and receiver fields. `brightness` and `useDbScale` are persisted but not yet surfaced as user-configurable settings.

## Documentation Roles

- `README.md` explains how to use and consume the project.
- `CONTRIBUTING.md` explains how to build, validate, and contribute.
- `docs/` contains durable hardware, UI, and architecture references.
- `specs/NNN-*` contains Spec Kit feature definitions, plans, contracts, quickstarts, and tasks.
