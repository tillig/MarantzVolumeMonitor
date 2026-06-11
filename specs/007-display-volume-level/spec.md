# Feature Specification: Display Volume Level

**Feature Branch**: `feature/007-display-volume-level`

**Created**: 2026-06-05

**Status**: Draft

**Input**: User description: "Display volume level"

## Clarifications

### Session 2026-06-05

- Q: When the receiver is powered off, should the home screen still show the last known volume? → A: Show `Receiver off` instead of a live volume value; do not show the last known volume as current.
- Q: How should the home screen format the displayed volume value? → A: Show one decimal place at all times.
- Q: Should the powered-off state also keep Settings accessible? → A: Powered-off state should also keep Settings accessible.

## User Scenarios and Testing *(mandatory)*

### User Story 1 - Read The Current Volume At A Glance (Priority: P1)

As a person looking at the device from across the room, I want the current receiver volume level to be visible immediately so I can confirm the listening level without opening menus or interacting with the device.

**Why this priority**: The product's main purpose is passive, at-a-glance visibility. If the current volume is not clearly visible, the device fails its primary use case.

**Independent Test**: Place the device in its normal viewing position with a configured receiver and confirm that a person can identify the current volume level from the main display within a few seconds without touching the screen.

**Acceptance Scenarios**:

1. **Given** the device has live receiver status, **When** the home display is visible, **Then** the current volume level is shown as the most prominent information on the screen with one decimal place.
2. **Given** the volume level changes on the receiver, **When** the display refreshes, **Then** the shown volume level matches the latest receiver volume.
3. **Given** the volume level changes from one value to another, **When** the home display updates, **Then** the colored gauge bar animates toward the new position and the numeric volume display counts toward the new value instead of jumping instantly, using one consistent motion style.

---

### User Story 2 - Understand Why Live Volume Is Not Shown (Priority: P2)

As a user, I want the device to make it clear why the current live volume is not being shown so I do not mistake a turned-off receiver for an unreachable one or assume stale information is current.

**Why this priority**: The display is only useful if people can trust what it shows. Clear degraded states prevent false confidence and help the user choose the right recovery action.

**Independent Test**: Verify the home display once with the receiver unreachable and once with the receiver reachable but powered off. The story is complete when those two conditions are presented as different non-live states and neither is mistaken for live volume.

**Acceptance Scenarios**:

1. **Given** the receiver is unavailable or not yet configured, **When** the home display is shown, **Then** the screen communicates that the current volume is unavailable.
2. **Given** the receiver responds but reports that it is powered off, **When** the home display is shown, **Then** the screen communicates a powered-off receiver state, does not label that state as unavailable, and does not present the last known volume as current.
3. **Given** live status resumes after an unavailable period or power-off period, **When** the device receives fresh active receiver data, **Then** the current volume level returns to the normal prominent display state.
4. **Given** the receiver is unavailable, **When** the user needs to point the device at a working receiver, **Then** the user can still access Settings from the home display and reopen receiver configuration.
5. **Given** the receiver is powered off, **When** the user wants to reconfigure or replace the saved receiver target, **Then** the user can still access Settings from the home display.

---

### User Story 3 - Keep The Display Calm While Volume Changes (Priority: P3)

As a user, I want volume updates to remain easy to notice without making the screen distracting so the device still behaves like a passive appliance in the room.

**Why this priority**: The device lives in a shared viewing space. Volume feedback should be visible without becoming visually noisy.

**Independent Test**: Trigger several volume changes in normal use and confirm the updated value is visible while the display stays stable, legible, and free of distracting idle behavior.

**Acceptance Scenarios**:

1. **Given** the volume changes during normal playback, **When** the home display updates, **Then** the updated value is easy to notice and remains readable from the normal viewing distance.
2. **Given** the volume remains unchanged, **When** the home display is idle, **Then** it does not introduce constant motion or visual noise unrelated to the latest volume state.
3. **Given** the volume change animation is nearing its final value, **When** the motion approaches the new target, **Then** the movement slows down smoothly instead of stopping abruptly.
4. **Given** another volume change happens before the previous animation finishes, **When** the home display retargets to the newest value, **Then** the animation remains smooth and settles on the latest volume without a jarring reset.
5. **Given** the device uses Material-compatible motion, **When** volume feedback animates, **Then** the gauge and numeric animation use the same functional motion style instead of unrelated motion behaviors.
6. **Given** a volume animation is in progress, **When** the display updates each animation step, **Then** only the affected screen regions are redrawn so the screen does not flash.

### Edge Cases

- The receiver reports a volume value at the extreme low or high end of its supported range.
- The receiver reports a value whose decimal precision must still render clearly at the minimum and maximum ends of the supported range.
- The receiver changes volume rapidly in several steps and the displayed value must still settle on the latest level.
- The displayed gauge animation is already in progress when a new volume update arrives.
- The numeric count animation would otherwise overshoot or skip the final target value.
- Animation updates redraw more of the home screen than necessary and cause visible flashing.
- The receiver is reachable but does not provide a valid current volume value.
- The receiver is reachable and explicitly reports that it is turned off.
- The receiver turns off after previously showing a live volume value.
- The receiver is powered off, but the user still needs a direct path into Settings to reconfigure the saved receiver target.
- The device has not been configured with a receiver yet.
- The device temporarily loses access to the receiver after previously showing a live volume.
- The receiver is unavailable, but the user still needs a direct path into Settings to reconfigure the saved receiver target.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST show the current receiver volume level on the primary home display when live receiver status is available.
- **FR-002**: The displayed volume level MUST be the most visually prominent status element on the home display.
- **FR-003**: The displayed volume value on the home screen MUST show one decimal place whenever live receiver status is available.
- **FR-004**: The displayed volume level MUST update to reflect the latest receiver volume after a user-visible change occurs on the receiver.
- **FR-005**: When the displayed volume level changes, the colored gauge bar MUST animate from the previously shown position to the new target position.
- **FR-006**: When the displayed volume level changes, the numeric volume display MUST count upward or downward toward the new target value instead of changing instantly.
- **FR-007**: Volume change animation MUST use one shared Material-compatible motion treatment for the gauge and numeric display so both elements feel like parts of the same interaction.
- **FR-008**: Volume change animation MUST use a Material-compatible standard motion style appropriate for a utilitarian product, settling smoothly at the final value and avoiding decorative or attention-seeking bounce.
- **FR-009**: Volume change animation MUST decelerate near the end of the motion so the gauge and numeric display settle on the final value without an abrupt stop.
- **FR-010**: Volume change animation MUST remain visually pleasing and non-jarring during both small adjustments and larger jumps.
- **FR-011**: If a new volume update arrives while a prior volume animation is still in progress, the home display MUST retarget the animation to the latest value and settle on the latest volume without reverting to stale intermediate values.
- **FR-012**: Volume change animation MUST complete quickly enough that the displayed value still feels responsive to normal receiver adjustments.
- **FR-013**: Volume animation updates MUST redraw only the gauge, numeric value, and any other directly affected regions instead of redrawing the full home screen.
- **FR-014**: The system MUST present the volume level in a format that is legible from the normal room viewing distance defined for the product.
- **FR-015**: The system MUST preserve stable screen layout while the shown volume level changes.
- **FR-016**: The system MUST communicate when the current volume level is not being shown because the receiver is not configured, not reachable, powered off, or not returning usable live status.
- **FR-017**: The home display MUST distinguish between a receiver that is unavailable and a receiver that is reachable but powered off.
- **FR-018**: The powered-off receiver state MUST be visually distinguishable from both the live-volume state and the unavailable-receiver state.
- **FR-019**: When the receiver is powered off, the home display MUST show a powered-off state instead of continuing to present the last known volume as current.
- **FR-020**: When the receiver is powered off, the home display MUST continue to provide access to Settings so the user can reconfigure receiver settings without restarting the device.
- **FR-021**: When the receiver is unavailable, the home display MUST continue to provide access to Settings so the user can reconfigure receiver settings without restarting the device.
- **FR-022**: When live receiver status resumes, the system MUST return to showing the current volume level without requiring the user to restart the device or reopen the screen.
- **FR-023**: The home display MUST avoid idle visual behavior unrelated to presenting the current volume state.
- **FR-024**: The system MUST ensure the displayed value reflects the latest known receiver volume and does not continue to imply that stale data is live after receiver status becomes unavailable or the receiver reports that it is powered off.

### Key Entities

- **Receiver Volume Level**: The current listening level reported by the configured receiver and shown as the primary value on the device.
- **Home Display State**: The main viewing state of the device, including live volume presentation and degraded or unavailable presentation.
- **Receiver Availability State**: The condition that determines whether the device can present a trustworthy current volume level.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: In a normal room setup, a viewer can identify the current volume level from the home display within 5 seconds or less from the product's intended viewing distance.
- **SC-002**: During normal use, the home display presents the live volume with one decimal place consistently whenever live receiver status is available.
- **SC-003**: During normal use, at least 95% of receiver volume changes are reflected on the home display before the next glance at the screen would reasonably occur.
- **SC-004**: In visual verification, volume changes use one consistent functional motion style for both the gauge and numeric display and settle smoothly at the final value without abrupt stops.
- **SC-005**: In verification testing, 100% of rapid consecutive volume changes settle on the latest target value without visibly snapping backward or restarting from the old starting point.
- **SC-006**: In visual verification, volume animation updates do not produce full-screen flashing and do not visibly redraw unaffected parts of the home screen.
- **SC-007**: In visual verification, volume motion remains calm and readable and does not introduce decorative bounce that competes with the passive appliance experience.
- **SC-008**: In verification testing, 100% of unavailable-receiver conditions show a clearly distinguishable non-live state instead of continuing to present the last value as current.
- **SC-009**: In verification testing, 100% of powered-off receiver conditions show a state that is distinct from the unavailable-receiver state and do not continue to present the last known volume as current.
- **SC-010**: In verification testing, 100% of unavailable-receiver and powered-off receiver conditions still allow the user to enter Settings and reopen receiver configuration from the home display.
- **SC-011**: In visual review, the home display remains stable while volume changes occur and shows no constant idle motion unrelated to live status changes.

## Assumptions

- The feature applies to the existing primary home display rather than introducing a separate screen.
- The device continues using the same configured receiver and existing receiver-status flow.
- Volume display behavior is limited to presenting current status and degraded-state messaging; direct volume control is out of scope.
- The home-screen volume animation should follow Material-compatible component motion principles rather than screen-to-screen transition patterns.
- For this device, Material-compatible motion means the standard motion scheme rather than the more expressive scheme because the product is a passive, utilitarian appliance.
- This feature defines the requirement to distinguish powered-off and unavailable receiver states on the home display, but broader receiver power-management behavior remains out of scope.
- The product's existing visibility target for normal room viewing distance remains unchanged.
