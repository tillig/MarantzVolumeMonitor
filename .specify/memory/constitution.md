<!--
Sync Impact Report:
- Version change: 1.0.0 -> 1.0.1
- List of modified principles:
  - Development Workflow: documentation guidance now separates Spec Kit feature artifacts from durable reference docs
- Added sections: None
- Removed sections: None
- Templates requiring updates:
  - .specify/templates/plan-template.md updated
  - .specify/templates/tasks-template.md updated
- Follow-up TODOs: None
-->

# Marantz Volume Monitor Constitution

## Core Principles

### I. Visibility-First UI

The primary UI must be readable from 15 feet. High contrast, dark mode only (#101010 background). Large volume display (72-96px). No layout shifting or constant animations that distract from the room's ambiance.

### II. Passive Appliance Stability

The device must function as a passive appliance. It must boot in <5s, auto-reconnect to WiFi, and maintain status via non-blocking polling (1-2s interval). UI must remain responsive even during network timeouts.

### III. Strict Layered Architecture

Maintain a clean separation between UI (Screens), Networking (MarantzClient), and Storage (ConfigStore). Screens must not perform network I/O; they delegate to managers. UI logic must be decoupled from driver-specific code where possible.

### IV. Deterministic Dependency Management

Use PlatformIO for all build and dependency management. No manual library modifications. All hardware-specific configurations (like TFT_eSPI_Setup.h) must be tracked in the repository and referenced in platformio.ini.

### V. Intuitive On-Device Setup

All critical configuration (WiFi, Receiver IP) must be achievable via the touchscreen. Auto-discovery (SSDP) is the primary method for receiver setup. Manual entry must use the on-screen keyboard/numpad.

## Hardware and Power Standards

- **Microcontroller**: Elegoo ESP32 DevKit V1 (ESP32-WROOM-32).
- **Display**: 4" SPI TFT (ST7796) + XPT2046 touch controller.
- **Power**: 5V/2A regulated supply is mandatory for stability.
- **Filesystem**: LittleFS for persistent configuration.

## Development Workflow

- **Coding Style**: CamelCase for classes, camelCase for methods/members.
- **Documentation**: All new features must have a Spec Kit entry under `specs/` and an implementation plan. Durable hardware, UI, architecture, usage, and contribution guidance must be kept in `docs/`, `README.md`, or `CONTRIBUTING.md` as appropriate.
- **Testing**: Hardware verification is required for all UI and networking changes.

## Governance

The Constitution is the project's supreme guidance. All architectural decisions must be justified against these principles. Amendments require a version bump and an update to the Sync Impact Report. Compliance is reviewed during the implementation planning phase.

**Version**: 1.0.1 | **Ratified**: 2026-05-23 | **Last Amended**: 2026-06-03
