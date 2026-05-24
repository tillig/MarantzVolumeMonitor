# Development Roadmap

## Phase 1 — Hardware Bring-Up

- Wire TFT and Touch controller.
- Run TFT test sketch to verify SPI and power.
- Run touch calibration.
- Validate SPI performance and visual clarity.

## Phase 2 — Core Software

- Implement `DisplayManager` and `TouchManager`.
- Build `HomeScreen` UI layout.
- Implement basic `MarantzClient` with hardcoded IP.

## Phase 3 — Configuration System

- Implement `ConfigStore` using LittleFS.
- Build WiFi setup screen and keyboard.
- Build Receiver setup screen with SSDP discovery logic.

## Phase 4 — Integration

- Connect UI navigation to `ConfigStore`.
- Connect `MarantzClient` to dynamic IP from configuration.
- Add error handling and auto-retry logic for network instability.

## Phase 5 — Polish

- Add smooth animations for volume changes and screen transitions.
- Implement brightness control.
- Optional: Add a web-based configuration portal for recovery.
