# Research: WiFi Setup UI

## Keyboard Implementation

- **Decision**: Implement a custom `KeyboardScreen` within the existing `Screen` architecture.
- **Rationale**: While libraries like `TFT_eSPI_Widgets` exist, they add heavy dependencies and might not align with our Dark Mode #101010 theme. A custom implementation gives us full control over the 40x40px touch targets required by SC-002.
- **Alternatives considered**: `lvgl` (Too complex for this stage), standard Arduino `Serial` input (Violates Constitution V - Intuitive On-Device Setup).

## WiFi Scanning and Signal Strength

- **Decision**: Use `WiFi.scanNetworks()` in non-blocking mode.
- **Rationale**: `WiFi.scanNetworks(true)` allows the UI to remain responsive (animating a "Scanning..." spinner) while the radio works. RSSI will be mapped:
  - > -50 dBm: 4 bars
  - -50 to -60 dBm: 3 bars
  - -60 to -70 dBm: 2 bars
  - < -70 dBm: 1 bar
- **Alternatives considered**: Blocking scan (Freezes UI, violates Constitution II).

## Credential Persistence

- **Decision**: Store SSID and Password in `config.json` via `LittleFS` using `ConfigStore`.
- **Rationale**: Standardizes storage with the existing `ConfigStore` class.
- **Alternatives considered**: `EEPROM` (Deprecated on ESP32), `NVS` (Overkill for simple JSON config).

## Password Masking UX

- **Decision**: Provide a "Show/Hide" toggle button on the keyboard screen.
- **Rationale**: Essential for user verification on a touch interface where typos are common.
- **Alternatives considered**: Always masked (Frustrating for users), Always visible (Security risk).

## Touch Calibration

- **Decision**: Use a 9-point affine calibration model in `TouchManager` instead of per-screen touch offsets or simple axis-only min/max mapping.
- **Rationale**: Hardware validation on 2026-06-03 showed that the XPT2046 panel on this 4" display has slight cross-axis skew. `XPT2046_Touchscreen::setRotation(1)` already rotates the controller data, so additional manual screen-specific offsets were masking the real problem. An affine transform derived from measured calibration points fixed keyboard and list alignment without per-screen adjustments.
- **Alternatives considered**: Manual button hitbox tuning (too fragile), global Y offset (inaccurate), 3-point edge calibration (insufficient for the observed skew).
