# System Architecture

## Components

- **UI Layer**
  - Home screen
  - Settings panel
  - On-screen keyboard
- **Network Layer**
  - WiFi manager
  - Receiver discovery (SSDP)
  - Marantz HTTP API client
- **Storage Layer**
  - Configuration persistence (LittleFS/SPIFFS)
- **Hardware Layer**
  - TFT display (TFT_eSPI)
  - Touch controller (XPT2046)
  - Power subsystem

## Source Code Structure

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

## Data Flow

1. WiFi connects using stored credentials.
2. Receiver IP is discovered or manually configured.
3. Marantz API is polled every 1–2 seconds.
4. UI updates volume, source, and mode.
5. Touch input triggers navigation and settings.
