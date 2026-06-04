# Implementation Plan: Receiver Detection and Configuration

**Branch**: `feature/004-receiver-detection-config` | **Date**: 2026-06-03 | **Spec**: [spec.md](spec.md)

**Input**: Feature specification from `/specs/004-receiver-detection-config/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/plan-template.md` for the execution workflow.

## Summary

Implement on-device receiver discovery and setup after Wi-Fi is connected. The feature adds SSDP/UPnP discovery for compatible Marantz/Denon receivers, an IPv4-only manual entry fallback, live receiver-status verification through the existing Marantz status request, and persistence of the verified receiver IP in `config.json` through `ConfigStore`.

## Technical Context

**Language/Version**: C++/Arduino using PlatformIO for ESP32

**Primary Dependencies**: `WiFi.h`, `WiFiUdp.h`, `HTTPClient`, `TFT_eSPI`, `XPT2046_Touchscreen`, `ArduinoJson`, `LittleFS`

**Storage**: `config.json` on LittleFS through `ConfigStore`; persist `receiverIp` only after live status verification succeeds

**Testing**: `pre-commit run --all-files`, `platformio run`, and hardware-in-the-loop validation with reachable and unreachable Marantz/Denon receivers

**Target Platform**: Elegoo ESP32 DevKit V1 with 4" ST7796 TFT and XPT2046 touch controller

**Project Type**: Embedded touchscreen appliance

**Performance Goals**: Discovery shows progress immediately, completes or times out within 15 seconds, verifies a reachable receiver within 15 seconds, and preserves 1-2 second status polling after setup

**Constraints**: UI must remain responsive during discovery and verification; screens must not perform network I/O directly; manual entry accepts IPv4 only; saved receiver configuration must survive temporary offline status

**Scale/Scope**: One active receiver configuration, local-network discovery only, no hostnames, no custom ports, no cloud pairing, no multi-receiver switching

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Visibility-First UI**: Receiver setup screens use the existing dark high-contrast display language and show receiver name plus IPv4 address for scanability.
- **II. Passive Appliance Stability**: Discovery and verification are modeled as stepwise state machines to avoid blocking the UI loop during network timeouts.
- **III. Strict Layered Architecture**: UI screens delegate discovery and verification to networking classes; persistence remains in `ConfigStore`.
- **IV. Deterministic Dependency Management**: Uses existing PlatformIO-managed dependencies and ESP32 core networking libraries; no new external library is required.
- **V. Intuitive On-Device Setup**: Receiver discovery, manual IPv4 entry, verification, retry, and replacement are all performed through the touchscreen.

**Gate Status**: PASS. No constitution violations identified.

## Project Structure

### Documentation (this feature)

```text
specs/004-receiver-detection-config/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   ├── config_schema.md
│   └── receiver_setup_flow.md
└── tasks.md
```

### Source Code (repository root)

```text
src/
├── main.cpp
├── network/
│   ├── MarantzClient.h/.cpp          # Add explicit live status verification helper if needed
│   ├── ReceiverDiscovery.h/.cpp      # New SSDP discovery state machine
│   └── WiFiManager.h/.cpp            # Existing Wi-Fi connection state remains unchanged
├── storage/
│   └── ConfigStore.h/.cpp            # Persist verified receiverIp in DeviceConfig
└── ui/
    ├── ScreenManager.h/.cpp
    └── Screens/
        ├── HomeScreen.h/.cpp         # Route receiver-required state to receiver setup
        ├── SettingsScreen.h/.cpp     # Route Receiver Setup menu entry
        ├── ReceiverListScreen.h/.cpp # New discovery results and retry/manual actions
        ├── ReceiverIpScreen.h/.cpp   # New IPv4-only manual entry screen
        └── ReceiverStatusScreen.h/.cpp # New verification and save feedback screen
```

**Structure Decision**: Extend the existing layered `src/` structure. Receiver discovery and verification belong in `src/network`, receiver persistence stays in `src/storage`, and setup flow screens stay under `src/ui/Screens`.

## Complexity Tracking

No constitution violations or exceptional complexity are required.

## Phase 0 Research Summary

See [research.md](research.md). All planning unknowns are resolved:

- Use SSDP M-SEARCH over UDP for local receiver discovery.
- Use the existing Marantz live status endpoint as the verification rule.
- Keep IPv4-only manual entry for touchscreen simplicity and spec compliance.
- Preserve saved receiver configuration when later status checks fail.

## Phase 1 Design Summary

See [data-model.md](data-model.md), [contracts/config_schema.md](contracts/config_schema.md), [contracts/receiver_setup_flow.md](contracts/receiver_setup_flow.md), and [quickstart.md](quickstart.md).

## Post-Design Constitution Check

- **I. Visibility-First UI**: PASS. The UI contract requires name plus IPv4 address and explicit progress/failure states.
- **II. Passive Appliance Stability**: PASS. Discovery and verification contracts require timeout-bounded, non-blocking behavior.
- **III. Strict Layered Architecture**: PASS. Contracts keep UI, network, and storage responsibilities separate.
- **IV. Deterministic Dependency Management**: PASS. No unmanaged dependency is introduced.
- **V. Intuitive On-Device Setup**: PASS. Automatic discovery is primary and manual IPv4 entry remains available on-device.
