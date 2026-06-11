# Feature Specification: Receiver Detection and Configuration

**Feature Branch**: `feature/004-receiver-detection-config`

**Created**: 2026-06-03

**Status**: Draft

**Input**: User description: "Add receiver detection and configuration"

## Clarifications

### Session 2026-06-03

- Q: What verification rule confirms a selected receiver is valid? → A: Verification succeeds only if the device responds to a live receiver-status request.
- Q: What manual entry format should receiver configuration accept? → A: Manual configuration accepts an IPv4 address only.
- Q: What should happen if a saved receiver cannot be reached later? → A: Keep the saved receiver configuration and show a disconnected or offline state until the user changes it.
- Q: What identifying details should the discovered receiver list show? → A: Show the receiver name and IPv4 address.

## User Scenarios and Testing *(mandatory)*

### User Story 1 - Discover Available Receivers (Priority: P1)

As a user setting up the device, I want the system to find compatible receivers on my local network so I can choose the correct one without manually entering network details.

**Why this priority**: Automatic discovery is the fastest and least error-prone path for first-time setup, and it aligns with the project's on-device setup requirements.

**Independent Test**: Open the receiver setup flow on a network with at least one compatible receiver. The story is complete when the device presents one or more selectable receiver candidates with enough information to tell them apart.

**Acceptance Scenarios**:

1. **Given** no receiver is currently configured, **When** the user opens receiver setup, **Then** the system starts searching for compatible receivers and shows search progress.
2. **Given** one or more compatible receivers are found, **When** the search completes, **Then** the system displays a selectable list showing each receiver's name and IPv4 address.
3. **Given** no compatible receivers are found, **When** the search completes, **Then** the system informs the user and offers retry and manual configuration options.

---

### User Story 2 - Configure a Receiver Manually (Priority: P1)

As a user whose receiver is not discovered automatically, I want to enter receiver details manually so I can still complete setup.

**Why this priority**: Manual configuration is the required fallback when discovery is unavailable or incomplete, preventing setup from becoming a dead end.

**Independent Test**: Start receiver setup on a network where discovery returns no results. The story is complete when the user can enter receiver details manually, validate them, and continue to connection verification.

**Acceptance Scenarios**:

1. **Given** the user is in receiver setup, **When** they choose manual configuration, **Then** the system prompts for the receiver's IPv4 address required to reach the device.
2. **Given** the user enters an invalid or incomplete address, **When** they attempt to continue, **Then** the system explains the problem and keeps the user in the setup flow.
3. **Given** the user enters a valid receiver address, **When** they continue, **Then** the system verifies that receiver by requesting live receiver status before setup can complete.

---

### User Story 3 - Save and Update Receiver Configuration (Priority: P2)

As a user, I want a verified receiver choice to be saved and reusable so the device reconnects to the same receiver on future boots, while still allowing me to replace that choice later.

**Why this priority**: The setup flow is only valuable if a successful choice persists across restarts and can be changed without redoing unrelated device setup.

**Independent Test**: Select or manually enter a compatible receiver, complete verification, restart the device, and confirm the same receiver configuration is reused. Repeat by changing the configuration to a different receiver and confirm the new choice replaces the old one.

**Acceptance Scenarios**:

1. **Given** the user selects a discovered receiver or enters one manually, **When** verification succeeds, **Then** the system saves that receiver as the active configuration and exits setup.
2. **Given** a receiver configuration has already been saved, **When** the user returns to receiver setup and selects a different verified receiver, **Then** the system replaces the previous receiver configuration without clearing unrelated device settings.
3. **Given** verification fails for a new receiver choice, **When** the user is shown the failure, **Then** the existing working configuration remains unchanged.
4. **Given** a previously saved receiver later becomes unreachable, **When** the device starts or refreshes receiver status, **Then** the saved receiver configuration remains in place and the user sees that the receiver is offline until they retry or change it.

---

### Edge Cases

- No compatible receivers are discovered on the local network.
- Multiple compatible receivers share similar or identical display names.
- A receiver appears during discovery but becomes unreachable before verification completes.
- The user enters a malformed, incomplete, or unreachable IPv4 address.
- A previously saved receiver configuration exists, but the user wants to replace it without changing Wi-Fi or touch calibration settings.
- A previously saved receiver is temporarily offline during startup or a later status refresh.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST provide an on-device receiver setup flow that can be started when no receiver is configured and when a user wants to replace an existing receiver configuration.
- **FR-002**: The system MUST search the local network for compatible receivers and present search progress to the user.
- **FR-003**: The system MUST display each discovered receiver with its receiver name and IPv4 address so the user can distinguish between available options.
- **FR-004**: The system MUST allow the user to retry discovery without leaving the receiver setup flow.
- **FR-005**: The system MUST provide a manual configuration path when discovery does not find the target receiver or the user prefers to enter details directly.
- **FR-006**: The system MUST accept only an IPv4 address for manual configuration and validate that address before attempting to verify the receiver.
- **FR-007**: The system MUST allow the user to select exactly one discovered receiver or one manually entered receiver target as the candidate configuration.
- **FR-008**: The system MUST verify communication with the selected or manually entered receiver before marking setup complete, and verification MUST succeed only when the receiver responds to a live receiver-status request.
- **FR-009**: The system MUST show clear in-progress, success, and failure states during discovery and verification.
- **FR-010**: The system MUST persist a successfully verified receiver configuration so it remains available after restart.
- **FR-011**: The system MUST reuse the saved receiver configuration on later startups until the user intentionally changes it.
- **FR-012**: The system MUST preserve an existing working receiver configuration when a replacement attempt fails verification.
- **FR-013**: The system MUST retain the saved receiver configuration when that receiver is later unreachable and present an offline or disconnected state until the receiver becomes reachable again or the user changes the configuration.

### Key Entities *(include if feature involves data)*

- **ReceiverCandidate**: A compatible receiver discovered on the local network, including its receiver name and IPv4 address so the user can choose the correct device.
- **ReceiverConfiguration**: The saved receiver IPv4 address and related receiver details the device uses on future boots until changed.
- **VerificationResult**: The outcome of testing communication with a selected or manually entered receiver, including whether a live receiver-status request succeeded and any recoverable failure reason.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Users can complete receiver setup through automatic discovery in under 60 seconds when a compatible receiver is available on the local network.
- **SC-002**: Users can complete receiver setup through manual configuration in under 90 seconds when the receiver's network address is known.
- **SC-003**: At least 95% of setup attempts with a reachable compatible receiver end with a verified receiver selection on the first pass.
- **SC-004**: 100% of successfully verified receiver configurations remain available after a device restart until the user intentionally replaces them.
- **SC-005**: When discovery or verification cannot reach a compatible receiver, users receive a clear recoverable outcome within 15 seconds.
- **SC-006**: When a previously saved receiver is temporarily unavailable, users see its offline state without losing the saved configuration.

## Assumptions

- The user and receiver are on the same local network during setup.
- Only one active receiver configuration is needed at a time for this device.
- Receiver setup is performed entirely on the device touchscreen rather than through an external app.
- Manual receiver configuration is limited to IPv4 entry and does not include hostnames or custom ports.
- Cloud account linking, remote management outside the local network, and multi-receiver switching are out of scope for this feature.
