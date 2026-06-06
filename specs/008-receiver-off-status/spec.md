# Feature Specification: Receiver Off Screen Sleep

**Feature Branch**: `feature/008-receiver-off-status`

**Created**: 2026-06-05

**Status**: Draft

**Input**: User description: "Improve Receiver Off status handling"

## Clarifications

### Session 2026-06-05

- This feature changes the volume monitor's receiver-off experience; it does not add receiver power
  control.
- Hardware backlight switching is out of scope for this feature. Until a separate hardware feature
  adds controllable backlight wiring, "monitor display off" means a software-blanked dark screen.
- When the receiver is confirmed off, the monitor should show `Receiver off` briefly, starting with a
  3-second timeout, then turn the monitor display off so no status message remains visible in the
  room.
- While the monitor display is off because the receiver is off, tapping the screen should wake the
  monitor and show the `Receiver off` screen again.
- After wake, the user can tap the Settings icon to configure the monitor. Settings screens should
  remain visible while the user works.
- When the user exits Settings and the receiver is still off, the monitor should return to the
  `Receiver off` screen, wait the timeout, then turn the monitor display off again.
- Q: After the monitor has blanked from confirmed receiver-off status, what should happen if later
  polling can no longer confirm the receiver is still off? → A: Stay blank until touch or active
  receiver status; touch wake shows the latest current state.
- Q: What should a non-Settings tap on the visible `Receiver off` screen do before the 3-second
  timeout expires? → A: Reset the 3-second timer and keep `Receiver off` visible.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Let The Monitor Go Dark When Receiver Is Off (Priority: P1)

As a person in the room, I want the volume monitor display to turn off after briefly confirming that
the receiver is off, so the monitor does not leave a distracting status message visible when the
audio system is not in use.

**Why this priority**: The device is meant to be a passive appliance. When the receiver is off, a
persistent `Receiver off` message adds visual noise without providing ongoing value.

**Independent Test**: With a saved receiver that responds with confirmed powered-off status, observe
the home display after the receiver turns off. The story is complete when `Receiver off` appears
briefly, then the monitor display becomes visually off within the configured timeout.

**Acceptance Scenarios**:

1. **Given** the saved receiver responds with fresh status confirming it is powered off, **When** the
   home display refreshes, **Then** the monitor shows `Receiver off` and does not show the last live
   volume as current.
2. **Given** the `Receiver off` screen is visible, **When** 3 seconds pass without user interaction
   and the receiver is still off, **Then** the monitor display turns off completely.
3. **Given** the monitor display is off because the receiver is off, **When** the receiver remains
   off and the user does nothing, **Then** no persistent status message or live-volume content remains
   visible.

---

### User Story 2 - Wake The Monitor For Recovery Actions (Priority: P2)

As a user, I want to tap the dark monitor display to wake it, so I can confirm the monitor is still
alive and access Settings if I need to recover or change configuration.

**Why this priority**: The display should be quiet when the receiver is off, but the monitor still
needs to be reachable for setup, calibration, or receiver replacement without a reboot.

**Independent Test**: Let the monitor display turn off in the receiver-off state, tap the screen, and
verify that the `Receiver off` screen returns with Settings available. The story is complete when the
first tap wakes the monitor and a Settings tap can open the settings menu.

**Acceptance Scenarios**:

1. **Given** the monitor display is off because the receiver is off, **When** the user taps anywhere
   on the screen, **Then** the monitor wakes and shows the `Receiver off` screen.
2. **Given** the monitor has woken to the `Receiver off` screen, **When** the user taps the Settings
   icon, **Then** the Settings screen opens.
3. **Given** the monitor has woken to the `Receiver off` screen and the receiver remains off,
   **When** the user does not open Settings or otherwise interact, **Then** the monitor display turns
   off again after the receiver-off timeout.

---

### User Story 3 - Keep Settings Usable While Receiver Is Off (Priority: P3)

As a user working in Settings, I want the monitor display to stay on while I am configuring it, so the
receiver-off screen timeout does not interrupt setup or recovery work.

**Why this priority**: Settings is the recovery path for monitor configuration. Letting the display go
dark during settings work would make the recovery path unreliable.

**Independent Test**: Wake the monitor from the receiver-off display-off state, open Settings, remain
in Settings longer than 3 seconds, then exit Settings while the receiver is still off. The story is
complete when Settings remains visible during use and the receiver-off timeout resumes only after
returning to the home receiver-off state.

**Acceptance Scenarios**:

1. **Given** the user is in Settings while the receiver is still off, **When** more than 3 seconds
   pass, **Then** the monitor display remains on and usable.
2. **Given** the user exits Settings while the receiver is still confirmed off, **When** the home
   display returns, **Then** the `Receiver off` screen is shown again before the monitor display turns
   off after the timeout.
3. **Given** the user exits Settings after the receiver has become active, **When** fresh active
   receiver status is available, **Then** the monitor shows the normal live volume display instead of
   entering the receiver-off display-off cycle.

---

### User Story 4 - Avoid Misleading Screen-Off Behavior (Priority: P4)

As a user, I want the monitor to go dark only for confirmed receiver-off status, so network failures,
setup gaps, or unknown receiver state do not look like an intentional receiver-off condition.

**Why this priority**: Screen-off behavior removes visible information. It must not hide conditions
that require setup or troubleshooting.

**Independent Test**: Compare confirmed receiver-off status with unreachable receiver, missing
receiver configuration, Wi-Fi disconnected, and unknown power-state cases. The story is complete when
only confirmed receiver-off status enters the receiver-off display-off cycle.

**Acceptance Scenarios**:

1. **Given** the saved receiver does not respond or cannot be reached, **When** the home display
   refreshes, **Then** the monitor does not enter the receiver-off display-off cycle.
2. **Given** the receiver responds but does not provide enough current status to confirm power state,
   **When** the home display refreshes, **Then** the monitor does not enter the receiver-off
   display-off cycle.
3. **Given** Wi-Fi or receiver setup is required, **When** the setup-required screen is shown,
   **Then** the monitor remains visible for setup rather than turning off as if the receiver were
   confirmed off.

### Edge Cases

- The receiver was previously live, then turns off and remains reachable long enough to confirm
  powered-off status.
- The receiver was previously live, then becomes unreachable before any powered-off status is
  confirmed.
- The receiver is powered off while network standby is disabled, making the saved receiver
  unreachable.
- The monitor display is off because the receiver is off, then the receiver powers on without any
  user touching the monitor.
- The monitor display is off because the receiver is off, then the user taps the screen repeatedly.
- The user wakes the monitor and taps Settings before the receiver-off timeout expires.
- The user wakes the monitor but does not tap Settings.
- The user taps the visible `Receiver off` screen outside the Settings icon before the timeout
  expires.
- The user remains in Settings longer than the receiver-off timeout.
- The user exits Settings while the receiver is still off.
- The user exits Settings after the receiver has become active.
- The saved receiver configuration exists, but Wi-Fi is disconnected.
- The saved receiver configuration is missing or intentionally replaced through Settings.
- A status refresh temporarily fails during receiver power-on or power-off transitions.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST enter the receiver-off display-off cycle only when current receiver
  status confirms the saved receiver is powered off.
- **FR-002**: The system MUST show `Receiver off` before turning the monitor display off.
- **FR-003**: The receiver-off screen MUST remain visible for 3 seconds before the monitor display
  turns off, unless the receiver becomes active, the user opens Settings, or the user touches the
  visible receiver-off screen before the timeout.
- **FR-004**: When the user touches the visible receiver-off screen outside the Settings entry before
  the timeout expires, the system MUST restart the 3-second receiver-off timeout and keep
  `Receiver off` visible.
- **FR-005**: After the receiver-off timeout expires and the receiver is still off, the monitor
  display MUST turn off completely so no persistent status message remains visible.
- **FR-006**: While the monitor display is off because the receiver is off, the system MUST continue
  monitoring receiver status.
- **FR-007**: While the monitor display is off because the receiver is off, a screen tap MUST wake the
  monitor and show the `Receiver off` screen.
- **FR-008**: A wake tap from the receiver-off display-off state MUST NOT by itself activate Settings
  or any other setup action; Settings access occurs from the visible `Receiver off` screen.
- **FR-009**: The visible `Receiver off` screen MUST provide a Settings entry.
- **FR-010**: While the user is in Settings or settings-launched setup screens, the receiver-off
  timeout MUST NOT turn the monitor display off.
- **FR-011**: When the user exits Settings and the receiver is still confirmed off, the system MUST
  show the `Receiver off` screen again and restart the receiver-off timeout.
- **FR-012**: When the receiver becomes active while the monitor display is off because the receiver
  was off, the system MUST leave the display-off state and return to the normal live volume display.
- **FR-013**: If receiver-off status can no longer be confirmed while the monitor is already blanked
  from a previously confirmed receiver-off state, the system MUST keep the monitor blank until the
  user touches the screen or fresh active receiver status becomes available.
- **FR-014**: When the user touches the blank monitor after receiver-off status can no longer be
  confirmed, the system MUST wake to the latest current visible state rather than assuming the
  receiver is still off.
- **FR-015**: The system MUST NOT infer receiver-off display-off behavior solely from a missing volume
  value, failed refresh, network timeout, missing receiver configuration, missing Wi-Fi connection, or
  unknown power state.
- **FR-016**: When receiver status is unavailable before the monitor has entered receiver-off
  display-off state, the system MUST show or preserve an
  unavailable/setup-related state rather than turning off as if receiver-off status were confirmed.
- **FR-017**: When `Receiver off` is shown, the system MUST NOT present the last known live volume,
  source, listening mode, or audio-format details as current.
- **FR-018**: The feature MUST NOT add any user-facing receiver power-control capability.
- **FR-019**: The system MUST preserve existing Wi-Fi setup, receiver setup, Settings, and touch
  calibration entry paths while adding receiver-off display sleep.

### Key Entities *(include if feature involves data)*

- **Receiver-Off Display Cycle**: The sequence where confirmed receiver-off status shows a visible
  `Receiver off` screen, waits for the configured timeout, then turns the monitor display off.
- **Monitor Display-Off State**: The quiet state entered only after confirmed receiver-off status and
  timeout expiration, where the display is visually off but touch wake and status monitoring remain
  available.
- **Wake Interaction**: A screen tap while the monitor display is off that returns the monitor to the
  visible `Receiver off` screen.
- **Settings Session**: Any settings or settings-launched setup flow during which the monitor display
  must remain on regardless of receiver-off timeout.
- **Receiver Power State**: The user-visible condition that determines whether the monitor shows live
  volume, receiver-off display sleep, unavailable status, or setup-required status.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: In verification testing, 100% of confirmed receiver-off cases show `Receiver off`
  before the monitor display turns off.
- **SC-002**: In verification testing, the monitor display turns off 3 seconds after showing
  `Receiver off`, with an acceptable tolerance of 0.5 seconds.
- **SC-003**: In visual review, the receiver-off display-off state leaves no persistent visible status
  message or live-volume content in the room.
- **SC-004**: In interaction testing, 100% of taps on the receiver-off display-off state wake the
  monitor to the visible `Receiver off` screen.
- **SC-005**: In interaction testing, 100% of wake taps require a separate visible Settings action
  before Settings opens.
- **SC-006**: In interaction testing, 100% of non-Settings taps on the visible `Receiver off` screen
  keep `Receiver off` visible for a fresh 3-second timeout.
- **SC-007**: In interaction testing, Settings remains visible and usable for at least 60 seconds
  while the receiver is still off.
- **SC-008**: In verification testing, 100% of Settings exits while the receiver is still off return
  to `Receiver off` and then turn the monitor display off again after the timeout.
- **SC-009**: In verification testing, 100% of transitions from receiver-off display-off state to
  fresh active receiver status return to the normal live volume display without user setup or reboot.
- **SC-010**: In verification testing, 100% of unreachable, setup-required, and unknown-power-state
  conditions avoid entering the receiver-off display-off cycle.
- **SC-011**: In verification testing, 100% of receiver-off presentations avoid showing stale live
  volume, source, mode, or audio-format details as current.
- **SC-012**: In verification testing, when receiver-off confirmation is lost after the monitor has
  already blanked, the monitor remains blank until user touch or active receiver status in 100% of
  attempts.

## Assumptions

- The receiver-off timeout starts at 3 seconds and may be tuned after hardware review.
- "Monitor display off" means the volume monitor display is visually dark/blank enough not to
  distract in the room while the device remains powered and responsive. This feature does not require
  turning off the physical display backlight.
- The monitor continues normal background status checks while its display is off due to confirmed
  receiver-off status.
- This feature refines monitor presentation and wake behavior only; direct receiver power control is
  out of scope.
- If the receiver is powered off and cannot respond because standby network access is disabled, the
  user-visible state is receiver unavailable rather than confirmed receiver off.
- Existing live volume formatting, motion, settings access, setup entry, and calibration behavior
  remain in scope only to prevent regressions.
- No new user accounts, permissions, or persistent user data are introduced by this feature.
