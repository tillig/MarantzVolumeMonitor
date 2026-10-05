# Feature Specification: Background Receiver Polling

**Feature Branch**: `feature/014-background-receiver-polling`

**Created**: 2026-10-02

**Status**: Implemented

**Input**: [Issue #7](https://github.com/tillig/MarantzVolumeMonitor/issues/7): on the `Receiver unavailable` screen, tapping the Settings icon sometimes does nothing.

## User Scenarios and Testing

### User Story 1 - Settings Responds While the Receiver Is Unreachable (Priority: P1)

As an owner, I want the Settings icon to respond on the first tap even when the receiver is slow or offline, so I can fix the receiver setup without rebooting.

**Independent Test**: Unplug the receiver's network cable, wait for `Receiver unavailable`, and tap Settings.

**Acceptance Scenarios**:

1. **Given** an unreachable receiver, **When** I tap the Settings icon, **Then** Settings opens without waiting for a status request to time out.
2. **Given** Wi-Fi is connected and the first status request is still running, **When** Home is visible, **Then** it shows `Connecting to receiver` with the Settings icon available.

### User Story 2 - Status Stays Current (Priority: P1)

As an owner, I want the volume to follow the receiver promptly and never show old values as current.

**Acceptance Scenarios**:

1. **Given** the live Home Screen, **When** I change the volume, **Then** the display follows each completed status request rather than waiting for the next screen refresh.
2. **Given** the live Home Screen, **When** the receiver stops answering, **Then** Home switches to `Receiver unavailable` within a few seconds instead of holding the last volume.

## Requirements

- **FR-001**: Fetch receiver status on a background task, not the UI loop.
- **FR-002**: Poll about once per second, and only while a screen has read status in the last few seconds.
- **FR-003**: Treat a cached status older than a few seconds as unavailable.
- **FR-004**: Discard results for a receiver address that changed while the request was running.

## Out of Scope

- Moving receiver identity lookup and receiver verification off the UI loop.
- Interrupting a status request that is already running.
