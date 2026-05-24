# Marantz Volume Monitor v2

## Overview

Marantz Volume Monitor v2 is a modernization of the original Arduino-based volume monitor for Marantz receivers. The new version uses an ESP32 microcontroller, a 4" SPI TFT touchscreen, WiFi networking, and a modern UI to display receiver volume, source, and audio mode information.

## Goals

- Replace Arduino + LCD + Ethernet shield with a WiFi-enabled touchscreen system.
- Provide a readable, attractive UI visible from across a room.
- Add touch-driven configuration menus.
- Add WiFi setup and receiver auto-discovery.
- Maintain stable, appliance-like behavior with minimal distraction.
- Migrate tooling to PlatformIO in VS Code.

## Target Hardware

- Elegoo ESP32 DevKit V1 (ESP32-WROOM-32)
- 4" SPI TFT touchscreen (480×320)
- XPT2046 touch controller
- 5V power supply (USB or dedicated 2A supply)
