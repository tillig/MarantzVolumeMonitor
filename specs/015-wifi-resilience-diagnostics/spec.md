# Feature Specification: Wi-Fi Resilience and Diagnostics

**Feature Branch**: `feature/015-wifi-resilience-diagnostics`

**Created**: 2026-10-02

**Status**: Implemented

**Input**: [Issue #8](https://github.com/tillig/MarantzVolumeMonitor/issues/8): the monitor intermittently stops reconnecting to Wi-Fi, reports what looks like a bad password for known-good credentials, and offers no way to see why.

## User Scenarios and Testing

### User Story 1 - Recover Without Unplugging (Priority: P1)

As an owner, I want the monitor to keep retrying my saved network so a router restart or a weak-signal handshake failure does not leave it stuck on `Connecting Wi-Fi`.

**Independent Test**: Power off the access point for two minutes, then restore it. The monitor reconnects without being touched.

**Acceptance Scenarios**:

1. **Given** saved credentials and no connection for 20 seconds, **When** the Arduino core has stopped retrying, **Then** the firmware restarts the radio and tries again, waiting longer between attempts up to once a minute.
2. **Given** the boot-time connection attempt is still running, **When** I enter new credentials, **Then** the new attempt is not rejected because the driver is busy.

### User Story 2 - Honest Failure Messages (Priority: P1)

As an owner, I want the setup failure screen to say what the driver reported so I can tell a missing network from a rejected password or weak signal.

**Acceptance Scenarios**:

1. **Given** a failed setup attempt, **When** the failure screen appears, **Then** it shows a plain-language reason and the driver's numeric reason code.
2. **Given** a failed setup attempt and previously saved credentials, **When** the failure screen appears, **Then** the monitor resumes retrying the saved network rather than the failed one.

### User Story 3 - See What Happened (Priority: P1)

As an owner filing a bug, I want recent connectivity events on the device and over HTTP so I can report precise symptoms.

**Acceptance Scenarios**:

1. **Given** the monitor is running, **When** I open `Settings` > `Diagnostics`, **Then** I see the newest events first and the address of the web diagnostics page.
2. **Given** the monitor is on Wi-Fi, **When** I browse to its IP address, **Then** I see firmware version, reset reason, heap, Wi-Fi details including the last disconnect reason, receiver polling results, and the event log.
3. **Given** the receiver-off blank screen is showing, **When** the receiver is unreachable instead of off, **Then** the event log records the state the blank screen is hiding.

## Requirements

- **FR-001**: Disable Wi-Fi modem sleep.
- **FR-002**: Retry the target network with a full radio restart when disconnected for 20, 40, then 60 seconds, without interrupting a running scan.
- **FR-003**: Keep the last 50 events in RAM and mirror them to serial.
- **FR-004**: Serve read-only diagnostics on port 80 without exposing the Wi-Fi password.
- **FR-005**: Log only state transitions so a persistent failure does not push older events out of the log.

## Out of Scope

- Persisting the log across reboots; the reset reason is reported instead.
- Remote actions such as restart or reconfiguration over HTTP.
