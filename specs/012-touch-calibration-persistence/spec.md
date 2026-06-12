# Feature Specification: Touch Calibration Persistence

**Feature Branch**: `feature/012-touch-calibration-persistence`

**Created**: 2026-06-12

**Status**: Draft

**Input**: User description: "I would like to implement the touch calibration for real. We currently have a calibration screen where the whole premise of the screen is to connect to the serial monitor, get the data, and feed it into the system so manual changes to the code can be made. I would like to make it so the calibration makes actual changes to the system and persists configuration so people with different setups don't have to connect to the serial monitor to have the calibration work."

## Clarifications

### Session 2026-06-12

- Q: How should users recover from a bad saved calibration? → A: Add a `Reset to defaults` settings menu option as the last item in the list, allowing users to reset `Wi-Fi`, `Receiver`, and `Calibration` individually based on selection.
- Q: How should destructive reset actions be confirmed? → A: Require a confirmation step for each reset selection, with explicit `Reset` and `Cancel` actions before anything is cleared.

## User Scenarios And Testing *(mandatory)*

### User Story 1 - Complete Calibration On The Device (Priority: P1)

As a user setting up the monitor on my own hardware, I want the calibration flow to produce a working touch alignment directly on the device so I do not need a serial monitor or source-code changes before I can use the touchscreen accurately.

**Why this priority**: This is the core user value. The current workflow still depends on developer tools and manual code updates, which breaks the goal of intuitive on-device setup.

**Independent Test**: Start calibration from the device UI, complete the required taps, return to normal navigation, and confirm that touch targets line up correctly without using any external tools.

**Acceptance Scenarios**:

1. **Given** the user opens touch calibration from an available device entry point, **When** the calibration flow starts, **Then** the screen explains the guided tap process without requiring an external computer or serial monitor.
2. **Given** the user completes the full calibration flow with valid taps, **When** the flow finishes, **Then** the device applies the new touch alignment during the same session.
3. **Given** the user finishes a successful calibration, **When** they return to settings or home navigation, **Then** touch interactions align with visible targets closely enough for normal use.
4. **Given** the user produces an accidental extra tap or a tap outside the active target area, **When** calibration is waiting for the current step, **Then** the device rejects that sample and keeps the session on the same target until a valid tap is captured.

---

### User Story 2 - Keep Calibration After Restart (Priority: P1)

As a user with a specific display and touch panel combination, I want the calibration result to stay with my device after reboot so I do not have to recalibrate every time power is lost.

**Why this priority**: A calibration result is part of device setup. If it does not persist, the feature is incomplete and users with hardware variation still face recurring setup friction.

**Independent Test**: Complete calibration successfully, restart the device, and confirm that touch alignment remains active without rerunning calibration.

**Acceptance Scenarios**:

1. **Given** the user has completed a successful calibration, **When** the device restarts, **Then** the same calibration result remains active automatically.
2. **Given** the device has no user-saved calibration yet, **When** it boots normally, **Then** it uses the shipped default touch alignment derived from the current proven baseline for this product until a successful calibration is saved.
3. **Given** a valid saved calibration exists, **When** the user reopens touch calibration later, **Then** the device starts from the saved alignment rather than forgetting it.

---

### User Story 3 - Recover From A Bad Or Incomplete Calibration (Priority: P2)

As a user, I want calibration to fail safely so an incomplete or poor tap sequence does not leave the touchscreen unusable or trap me outside the calibration flow.

**Why this priority**: Touch calibration is a high-risk setup action. The device must remain recoverable if the user mis-taps, abandons the flow, or produces an invalid result.

**Independent Test**: Start calibration and cancel, interrupt, or provide unusable input, then confirm that the prior working alignment remains available and the user can re-enter calibration.

**Acceptance Scenarios**:

1. **Given** the user exits calibration before finishing, **When** the flow closes, **Then** the previously active touch alignment remains unchanged.
2. **Given** the calibration input is incomplete or fails validation, **When** the device rejects the attempt, **Then** it keeps the prior working alignment and tells the user to try again.
3. **Given** a saved calibration is inaccurate for the current hardware or setup, **When** the user needs to recalibrate, **Then** the device still provides a reachable way to launch calibration again.
4. **Given** the user accidentally double-taps during calibration, **When** the second touch does not qualify as a new valid sample for the active target, **Then** the device ignores it and does not advance or corrupt the captured set.
5. **Given** the user needs to recover from a bad saved setup value, **When** they open `Reset to defaults` from the bottom of the settings list, **Then** they can choose `Wi-Fi`, `Receiver`, or `Calibration` and reset only the selected type without affecting the others.
6. **Given** the user selects a reset type from `Reset to defaults`, **When** the destructive action is about to occur, **Then** the device requires an explicit `Reset` confirmation and offers `Cancel` without changing any saved values.

### Edge Cases

- The user taps outside a target, double-taps, or releases too quickly during one or more calibration steps.
- The user taps close enough to a previously captured point that the new sample looks inconsistent with the expected target layout.
- The allowed capture area around the active target must not overlap the allowed area for any other target in the same session.
- The calibration flow is interrupted by navigation away from the screen, a temporary input issue, or a restart before completion.
- The device has no previously saved user calibration and the first calibration attempt fails.
- The user saves a calibration that is valid enough to accept but still needs to be redone for better accuracy.
- A stored calibration record is missing, unreadable, or otherwise unusable during startup.
- Resetting `Calibration` must restore the shipped default touch alignment without also clearing saved `Wi-Fi` or `Receiver` values.
- Canceling a pending reset must leave all saved values unchanged.
- Touch calibration must remain reachable from the setup path and from `Settings` without hidden gestures.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST let users launch touch calibration from the existing on-device calibration entry points.
- **FR-002**: The touch calibration flow MUST provide plain-language guidance that tells the user how to complete calibration entirely on the device.
- **FR-003**: The touch calibration flow MUST collect enough touch samples to calculate a full-device alignment result suitable for normal navigation across the screen.
- **FR-004**: After a successful calibration, the system MUST apply the new touch alignment without requiring source-code changes, a serial monitor, or a device restart.
- **FR-005**: After a successful calibration, the system MUST save the new touch alignment as part of the device's persistent configuration.
- **FR-006**: On startup, the system MUST restore the most recently saved valid touch alignment automatically.
- **FR-007**: If no saved calibration exists, the system MUST use the shipped default touch alignment until a successful user calibration is saved.
- **FR-008**: The shipped default touch alignment MUST be based on the currently proven baseline values already captured for this product's default hardware setup.
- **FR-009**: The system MUST validate calibration input before replacing the active saved calibration.
- **FR-010**: During each calibration step, the system MUST accept a tap only when it falls within the allowed capture boundary for the active target.
- **FR-011**: The allowed capture boundary for one target MUST NOT overlap the allowed capture boundary for any other target in the same calibration session.
- **FR-012**: The system MUST reject taps that fall outside the active target's allowed capture boundary and MUST keep the session on the current step until a valid tap is captured.
- **FR-013**: The system MUST require a clearly separate new touch event before capturing the next calibration point so an accidental double-tap cannot satisfy two steps.
- **FR-014**: The system MUST detect and reject suspicious samples that are implausibly close to previously accepted calibration points for different targets.
- **FR-015**: If calibration input is incomplete, invalid, or abandoned, the system MUST keep the previously active touch alignment unchanged.
- **FR-016**: The system MUST present a clear success state when a new calibration has been applied and saved.
- **FR-017**: The system MUST present a clear retry path when calibration does not produce a usable result.
- **FR-018**: The system MUST keep a reachable way to relaunch touch calibration even after a failed or poor calibration attempt.
- **FR-019**: Completing calibration successfully from the setup path MUST let the user continue normal setup without external tooling.
- **FR-020**: Completing calibration successfully from `Settings` MUST return the user to normal settings navigation with the new alignment active.
- **FR-021**: The saved calibration data MUST be specific enough to distinguish a completed valid calibration from default or missing state.
- **FR-022**: The system MUST ignore unreadable or unusable saved calibration data at startup and fall back to the shipped default touch alignment instead of leaving touch behavior undefined.
- **FR-023**: The `Settings` menu MUST include a `Reset to defaults` entry as the last item in the settings list.
- **FR-024**: Opening `Reset to defaults` MUST let the user choose among `Wi-Fi`, `Receiver`, and `Calibration` reset types.
- **FR-025**: Resetting one settings type from `Reset to defaults` MUST clear or restore only the selected type and MUST NOT affect the others.
- **FR-026**: Resetting `Calibration` from `Reset to defaults` MUST remove the saved user calibration and immediately restore the shipped default touch alignment.
- **FR-027**: Before any selected reset type is cleared, the system MUST present a confirmation step with explicit `Reset` and `Cancel` actions.
- **FR-028**: Pressing `Cancel` in the reset confirmation step MUST leave all saved values unchanged and return the user to the reset flow.
- **FR-029**: The feature MUST replace the current serial-log-only expectation in the user-facing calibration flow with a self-contained on-device outcome.

### Key Entities *(include if feature involves data)*

- **Touch Calibration Session**: The guided on-device process in which the user taps the requested calibration targets so the device can derive a new alignment result.
- **Touch Calibration Profile**: The saved calibration result that represents the device's active touch alignment and persists across restarts.
- **Default Touch Alignment**: The shipped touch behavior based on the current proven baseline values for the default hardware setup, used before any valid user calibration is saved or when saved calibration data cannot be used.
- **Target Capture Boundary**: The non-overlapping acceptable area around the active calibration target within which a tap may be counted for that step.
- **Calibration Validation Result**: The pass or fail outcome that determines whether a calibration session can replace the currently active alignment.
- **Reset To Defaults Selection**: The user's explicit choice of which saved settings type to reset from the `Reset to defaults` settings flow.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: In hardware validation, users can start touch calibration, complete it, and return to normal navigation without any external tools in one uninterrupted session.
- **SC-002**: In hardware validation, at least 90% of first-attempt taps on primary navigation targets register on the intended control immediately after a successful calibration.
- **SC-003**: In restart testing, 100% of successful calibration results remain active after device reboot until the user recalibrates or resets configuration.
- **SC-004**: In failure-path testing, 100% of incomplete or invalid calibration attempts leave the previously active alignment unchanged.
- **SC-005**: In calibration-guard testing, 100% of taps outside the active target boundary are rejected without advancing the calibration step.
- **SC-006**: In calibration-guard testing, 100% of accidental double-taps fail to capture two different calibration steps from one intended press.
- **SC-007**: In recovery testing, users can relaunch touch calibration from an available device entry point after a failed attempt without connecting a development computer.
- **SC-008**: In reset-flow testing, 100% of `Calibration` resets restore the shipped default touch alignment without clearing saved `Wi-Fi` or `Receiver` settings.
- **SC-009**: In reset-flow testing, users can find `Reset to defaults` as the last item in the settings list and select the intended reset type on their first attempt.
- **SC-010**: In reset-confirmation testing, 100% of canceled reset actions leave the selected settings type unchanged.
- **SC-011**: In usability testing, users can understand from the on-device text alone that calibration applies directly to the device and no manual code update is required.

## Assumptions

- The existing product already exposes touch calibration from the setup flow and from `Settings`, and those entry points remain the correct places to start calibration.
- The product continues to target the current 480x320 touch display hardware family rather than adding support for unrelated input devices.
- Touch calibration remains a local device-setup feature and does not add remote calibration, cloud sync, or per-user profiles.
- The device's persistent settings store can be extended to hold calibration data alongside other saved setup values.
- A calibration result should only replace the prior saved result when the device determines the new result is usable.
- The current measured default alignment from the existing setup is considered a sensible shipped baseline and should remain in effect unless a user completes and saves a better calibration for their hardware.
