# Feature Specification: Display Current Settings

**Feature Branch**: `feature/010-display-current-settings`

**Created**: 2026-06-11

**Status**: Draft

**Input**: User description: "Display current settings"

## Clarifications

### Session 2026-06-11

- Q: Where should current settings appear? → A: Add a new `Current Settings` entry to the `Settings` menu that opens a separate read-only screen.
- Q: What information must the read-only screen show? → A: Show Wi-Fi SSID, volume monitor IP address, current Wi-Fi signal strength, receiver name or type when available, receiver IP address, and receiver power on/off status.
- Q: How should the user leave the read-only screen? → A: Provide a single `OK` action that dismisses the screen and returns to `Settings`.
- Q: Where should the new menu entry appear? → A: `Current Settings` is the first entry in the `Settings` menu.
- Q: How should the settings menu adapt if four entries no longer fit well? → A: Follow the existing Material-compatible patterns and preserve readable labeled navigation; use a Material-compatible overflow approach such as settings pagination rather than forcing an icon-only menu by default.
- Q: Should the `Current Settings` screen keep live status updated while it is visible? → A: Refresh when the screen opens and continue refreshing live status while the screen remains visible.
- Q: How should Wi-Fi signal strength be shown? → A: Show a Wi-Fi strength graphic like the Wi-Fi selection screen and include a numeric value when available.

## User Scenarios And Testing *(mandatory)*

### User Story 1 - Open A Current Settings Overview (Priority: P1)

As a user in `Settings`, I want a dedicated `Current Settings` entry that opens a read-only overview screen so I can review the monitor's current configuration and live status without entering setup flows.

**Why this priority**: This is the core value of the feature. Without a dedicated overview screen, users still need to guess or enter setup flows just to inspect current state.

**Independent Test**: Open `Settings`, select `Current Settings`, and verify that the device shows a separate overview screen instead of opening an editable setup flow.

**Acceptance Scenarios**:

1. **Given** the user opens `Settings`, **When** the settings menu is shown, **Then** `Current Settings` appears as the first settings entry.
2. **Given** the user selects `Current Settings`, **When** the new screen opens, **Then** it presents a read-only overview rather than editable controls.
3. **Given** the user leaves the overview screen, **When** they press `OK`, **Then** the device returns to `Settings`.

---

### User Story 2 - Review Wi-Fi And Receiver Details Together (Priority: P1)

As a user troubleshooting or confirming setup, I want the overview screen to show both Wi-Fi details and receiver details together so I can understand the current device state at a glance.

**Why this priority**: The main recovery questions are whether the monitor is connected correctly and what receiver it is targeting. Splitting these details across multiple flows slows recovery.

**Independent Test**: Open the `Current Settings` screen on a configured device and verify that all required Wi-Fi and receiver items are visible together on the same read-only screen and continue to reflect current live status while the screen remains open.

**Acceptance Scenarios**:

1. **Given** Wi-Fi is configured and connected, **When** the user opens `Current Settings`, **Then** the screen shows the saved Wi-Fi network name, the monitor IP address, and the current Wi-Fi signal strength using the established Wi-Fi strength graphic plus a numeric value when available.
2. **Given** a receiver is configured, **When** the user opens `Current Settings`, **Then** the screen shows the receiver IP address and the current receiver power status.
3. **Given** a receiver name or type is available, **When** the user opens `Current Settings`, **Then** the screen shows that identifying information with the receiver details.
4. **Given** the `Current Settings` screen remains visible while live device or receiver status changes, **When** fresh status becomes available, **Then** the visible values update without requiring the user to leave and reopen the screen.

---

### User Story 3 - Understand Missing Or Unavailable Data (Priority: P2)

As a user diagnosing a problem, I want the overview screen to clearly distinguish configured values from unavailable live status so I can tell whether the issue is missing setup or a temporary connection problem.

**Why this priority**: Recovery depends on understanding whether a value was never configured, is currently unreachable, or is simply not provided by the receiver.

**Independent Test**: View the `Current Settings` screen with missing Wi-Fi configuration, missing receiver configuration, disconnected Wi-Fi, and receiver-unavailable states. The story is complete when each unavailable item is explained clearly without looking like stale confirmed data.

**Acceptance Scenarios**:

1. **Given** Wi-Fi has not been configured, **When** the user opens `Current Settings`, **Then** the Wi-Fi section clearly indicates that Wi-Fi setup is still required.
2. **Given** Wi-Fi is configured but the monitor is not currently connected, **When** the user opens `Current Settings`, **Then** the saved Wi-Fi network remains identifiable and live Wi-Fi status items that cannot be determined are shown as unavailable.
3. **Given** a receiver is configured but unavailable or powered off, **When** the user opens `Current Settings`, **Then** the screen continues to show the configured receiver target and reports the current receiver power or availability state without implying that live status is confirmed when it is not.

---

### User Story 4 - Keep Settings Navigation Readable (Priority: P3)

As a user navigating the `Settings` menu, I want the added `Current Settings` entry to fit within the same Material-compatible navigation language so the menu stays readable and easy to use.

**Why this priority**: Adding another settings destination should not degrade readability or force a confusing one-off navigation pattern.

**Independent Test**: Review the `Settings` menu after adding `Current Settings`. The story is complete when all settings destinations remain discoverable, labeled, and easy to tap, whether they appear on one screen or use a documented overflow pattern.

**Acceptance Scenarios**:

1. **Given** all settings destinations fit clearly on one menu screen, **When** the menu is displayed, **Then** each destination remains labeled and directly tappable without reducing readability.
2. **Given** all settings destinations do not fit clearly on one menu screen, **When** the menu is displayed, **Then** the device uses a Material-compatible overflow pattern that preserves labeled navigation and touch clarity.
3. **Given** the user is browsing the `Settings` menu, **When** they look for setup destinations, **Then** the new `Current Settings` entry does not make existing destinations harder to recognize or reach.

### Edge Cases

- Wi-Fi is configured, but the device is currently disconnected, so the saved SSID is known while live network values may be unavailable.
- The device has no current Wi-Fi IP address because it is not connected to Wi-Fi.
- Current Wi-Fi signal strength cannot be determined at the moment the screen is shown.
- Current Wi-Fi signal strength graphic is available, but the numeric value is temporarily unavailable.
- A receiver IP address is saved, but the receiver is currently unavailable.
- Receiver name or type is not available even though the receiver IP address is known.
- The receiver is reachable and reports a powered-off state distinct from a general unavailable state.
- Four settings destinations do not fit clearly on one screen using the established settings row treatment.
- Long SSID or receiver-identifying text competes with labels, section headings, or the dismiss action.
- Wi-Fi or receiver status changes while the `Current Settings` screen is still open.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST add a new `Current Settings` entry to the `Settings` menu.
- **FR-002**: `Current Settings` MUST be the first entry in the `Settings` menu order.
- **FR-003**: Selecting `Current Settings` MUST open a separate read-only screen rather than a setup or editing flow.
- **FR-004**: The `Current Settings` screen MUST include a Wi-Fi section and a receiver section on the same screen.
- **FR-005**: The Wi-Fi section MUST show the saved Wi-Fi network name when one exists.
- **FR-006**: The Wi-Fi section MUST show the monitor's current IP address when that address is available.
- **FR-007**: The Wi-Fi section MUST show the current Wi-Fi signal strength when that value is available.
- **FR-008**: When Wi-Fi signal strength is shown, it MUST use the established Wi-Fi strength graphic and include a numeric value when that value is available.
- **FR-009**: The receiver section MUST show the configured receiver IP address when one exists.
- **FR-010**: The receiver section MUST show the current receiver power status and distinguish powered off from a more general unavailable state when that distinction is known.
- **FR-011**: The receiver section MUST show receiver name or type when that identifying information is available.
- **FR-012**: When a required configured value does not exist, the `Current Settings` screen MUST clearly indicate that setup is still required rather than leaving the value blank.
- **FR-013**: When a live status value cannot be determined, the `Current Settings` screen MUST clearly indicate that the value is unavailable rather than presenting stale confirmed data as current.
- **FR-014**: The `Current Settings` screen MUST be read-only and MUST NOT allow editing, saving, or launching configuration changes directly from that screen.
- **FR-015**: The `Current Settings` screen MUST load current live status when it opens and continue refreshing live status while it remains visible.
- **FR-016**: The `Current Settings` screen MUST provide a single `OK` action that returns the user to `Settings`.
- **FR-017**: The added `Current Settings` entry MUST preserve readable labeled navigation in the `Settings` menu and MUST NOT require an icon-only settings menu by default.
- **FR-018**: If all settings destinations cannot be shown clearly with the established settings-row treatment on one screen, the system MUST use a Material-compatible overflow pattern that preserves labeled touch targets and clear navigation between settings destinations.
- **FR-019**: Existing destinations for Wi-Fi setup, receiver setup, and touch calibration MUST remain reachable after adding `Current Settings`.
- **FR-020**: The `Current Settings` screen MUST keep labels, values, and actions readable without overlapping or clipping at the device's normal display size.

### Key Entities *(include if feature involves data)*

- **CurrentSettingsScreen**: A read-only screen that presents the monitor's current Wi-Fi and receiver configuration and live status information together.
- **WiFiStatusSummary**: The user-facing set of Wi-Fi details shown on the current settings screen, including saved network identity and available live network status.
- **ReceiverStatusSummary**: The user-facing set of receiver details shown on the current settings screen, including configured receiver identity and available live receiver status.
- **SettingsMenuDestination**: A labeled entry in the `Settings` menu that opens a distinct destination while preserving the menu's navigation order and readability.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: In interaction testing, users can open `Current Settings` from `Settings` in 2 taps or fewer.
- **SC-002**: In validation testing, 100% of required overview items are visible on the `Current Settings` screen when the corresponding data is available.
- **SC-003**: In unavailable-data testing, 100% of missing or unknown live values are labeled clearly enough that reviewers can distinguish `not configured`, `unavailable`, and `powered off` states without opening another screen.
- **SC-004**: In navigation testing, 100% of users can dismiss the `Current Settings` screen and return to `Settings` using the single `OK` action on their first attempt.
- **SC-005**: In layout review, all settings destinations remain readable and tappable after adding `Current Settings`, whether shown on one screen or through the approved overflow pattern.
- **SC-006**: In live-status testing, changes to Wi-Fi or receiver state that occur while `Current Settings` is visible are reflected on the screen within 3 seconds.
- **SC-007**: In Wi-Fi status display review, the screen uses the established Wi-Fi strength graphic consistently and shows the numeric strength value whenever that value is available.

## Assumptions

- This feature adds a dedicated overview screen instead of repurposing Wi-Fi setup or receiver setup screens.
- The screen is informational only; editing and recovery actions remain in the existing setup flows.
- Current Wi-Fi and receiver values may include a mix of saved configuration and live status, and the UI must label unavailable live values clearly.
- The preferred settings-menu adaptation is to keep text-labeled Material-compatible navigation. If space becomes too tight, a paged or similar overflow pattern is preferable to collapsing the menu into ambiguous icon-only navigation.
- Touch calibration remains a maintenance destination and stays available even though the new `Current Settings` entry is placed first.
