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
    ├── ScreenManager.h/.cpp
    ├── TouchManager.h/.cpp
    └── Screens/
        ├── ReceiverListScreen.h/.cpp
        ├── ReceiverIpScreen.h/.cpp
        ├── ReceiverStatusScreen.h/.cpp
```

Feature work may add classes inside these existing layer folders. New cross-layer behavior should keep the ownership boundary clear: UI presents state and handles touch, network classes perform network operations, and storage classes read or write persistent configuration.

## Runtime Flow

1. Boot initializes display, touch, storage, and network managers.
2. Stored Wi-Fi credentials are loaded from LittleFS when available.
3. If Wi-Fi is not configured, the touchscreen setup flow collects and saves credentials.
4. If a receiver is not configured, the receiver setup flow discovers or accepts a receiver address.
5. Receiver setup saves the receiver IP only after a live receiver-status request succeeds.
6. The Marantz client polls receiver status every 1-2 seconds without blocking UI updates.
7. The Home Screen renders volume, source, mode, connectivity, and setup state.

## Documentation Roles

- `README.md` explains how to use and consume the project.
- `CONTRIBUTING.md` explains how to build, validate, and contribute.
- `docs/` contains durable hardware, UI, and architecture references.
- `specs/NNN-*` contains Spec Kit feature definitions, plans, contracts, quickstarts, and tasks.
