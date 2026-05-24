# Feature: Networking

## WiFi Logic

- **First Boot**: If no stored SSID/password exists, show the WiFi setup screen.
- **Persistence**: Store credentials in LittleFS.
- **Auto-Reconnect**: Automatically reconnect to the stored network on boot or after signal loss.

## Receiver Discovery (SSDP)

- Perform an SSDP/UPnP search for Marantz/Denon compatible devices.
- Store the discovered IP in configuration.
- Fallback to manual IP entry if discovery fails.

## Marantz API Client

- **Polling**: Fetch status every 1–2 seconds.
- **Status Data**:
  - Volume level (dB)
  - Input source (e.g., Blu-ray, Game)
  - Audio channel mode (e.g., Dolby TrueHD, DTS:X)
- **Performance**: Use non-blocking HTTP requests to keep the UI responsive.
