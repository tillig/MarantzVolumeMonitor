# Feature Specification: Web Flasher

**Feature Branch**: `feature/016-web-flasher`

**Created**: 2026-10-02

**Status**: Implemented

**Input**: [Issue #6](https://github.com/tillig/MarantzVolumeMonitor/issues/6): install the latest release from a browser, as `somfy-matter-remote` does.

## User Scenarios and Testing

### User Story 1 - Install From a Browser (Priority: P1)

As a builder, I want to flash the latest release from a web page so I don't need PlatformIO.

**Acceptance Scenarios**:

1. **Given** a blank ESP32 on USB and desktop Chrome or Edge, **When** I select `Connect and Install` on the flasher page, **Then** the latest release installs and the monitor boots to on-device setup.
2. **Given** a configured monitor, **When** I install without checking `Erase device`, **Then** Wi-Fi, receiver, and calibration settings survive.

### User Story 2 - Publish by Tagging (Priority: P1)

As the maintainer, I want pushing a `v*` tag to publish a release and refresh the flasher so releasing needs no manual uploads.

**Acceptance Scenarios**:

1. **Given** a pushed `v*` tag, **When** the workflows finish, **Then** the release has `firmware.factory.bin` and the flasher serves it.
2. **Given** a release build, **When** I open the diagnostics page, **Then** `firmwareVersion` matches the tag.

## Requirements

- **FR-001**: Serve the firmware from the Pages origin, since release asset redirects fail CORS.
- **FR-002**: Use one factory image at offset `0x0`. It ends before the LittleFS partition, so a separate update image is unnecessary.
