# Feature Specification: WiFi Setup UI

**Feature Branch**: `feature/003-wifi-setup-ui`

**Created**: 2026-06-02

**Status**: Draft

**Input**: User description: "wifi-setup-ui"

## Clarifications

### Session 2026-06-02

- Q: Should we provide a "Manual Entry" option for hidden SSIDs? → A: Yes, provide a "Manual Entry" button.
- Q: Which security types should be supported? → A: WPA2-PSK Only.

## User Scenarios and Testing *(mandatory)*

### User Story 1 - Network Scanning and Selection (Priority: P1)

As a user, I want to see a list of available WiFi networks so I can select the correct one for my home theater setup.

**Why this priority**: Core blocker for connecting the device to the network.

**Independent Test**: Open the WiFi setup screen. Success if a list of nearby SSIDs is displayed and I can select one.

**Acceptance Scenarios**:

1. **Given** the device is in unconfigured mode, **When** I tap the settings/setup area, **Then** the device scans for networks and displays them in a list.
2. **Given** a list of networks, **When** I tap an SSID, **Then** that network is selected for the next step.

---

### User Story 2 - Password Entry (Priority: P1)

As a user, I want to enter my WiFi password using an on-screen keyboard so I can securely connect to my chosen network.

**Why this priority**: Required for connecting to most modern home networks.

**Independent Test**: Select a network. Success if a keyboard appears and I can type and submit a password.

**Acceptance Scenarios**:

1. **Given** a network is selected, **When** I tap the password field or continue, **Then** a full on-screen keyboard (QWERTY or similar) is displayed.
2. **Given** the keyboard is visible, **When** I type characters, **Then** they appear in the password field (optionally masked).

---

### User Story 3 - Connection Feedback (Priority: P2)

As a user, I want to see the status of the connection attempt so I know if I typed my password correctly.

**Why this priority**: Vital for troubleshooting setup issues.

**Independent Test**: Submit a password. Success if a "Connecting..." message is shown followed by a "Success" or "Failed" indicator.

**Acceptance Scenarios**:

1. **Given** a password has been submitted, **When** the device attempt to connect, **Then** a "Connecting..." status is displayed.
2. **Given** a connection attempt completes, **When** successful, **Then** the user is returned to the Home Screen with a "Connected" indicator.

---

### Edge Cases

- **Hidden SSIDs**: System MUST provide a "Manual Entry" option to allow the user to type in a hidden SSID.
- **Wrong Password**: System must allow the user to retry without starting the entire flow over.
- **No Networks Found**: System should show a "No networks detected" message and a "Retry" button.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST scan for 2.4GHz WiFi networks.
- **FR-002**: System MUST display a scrollable list of SSIDs with signal strength indicators.
- **FR-003**: System MUST provide a full on-screen keyboard for password entry.
- **FR-004**: System MUST securely persist WiFi credentials once connected.
- **FR-005**: System MUST provide visual feedback during the connection process (busy indicator).
- **FR-006**: System MUST support standard WPA2-PSK security.

### Key Entities *(include if feature involves data)*

- **WiFiNetwork**: Represents a detected SSID, signal strength (RSSI), and security type.
- **NetworkCredentials**: The SSID and Password pair used for connection.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Users can complete the WiFi setup flow in under 60 seconds.
- **SC-002**: On-screen keyboard keys have a minimum touch target size of 40x40px for accuracy.
- **SC-003**: Network list populates within 5 seconds of starting a scan.
- **SC-004**: 100% of successfully connected credentials are persisted across reboots.

## Assumptions

- Users are within range of a 2.4GHz WiFi network (ESP32 standard).
- Most users use WPA2-PSK (Personal) security.
- The 4" display is large enough for a usable touch keyboard.
- Calibration values are accurate enough for typing.
