# Feature Specification: Volume Display Scale

**Feature Branch**: `feature/011-volume-scale-setting`

**Created**: 2026-06-11

**Status**: Draft

**Input**: User description: "I would like to add a setting to the settings menu so you can select to change from the 0 - 100 scale to the dB scale when displaying volume. This should affect the number displayed on the home screen, but the gauge displays the same."

## Clarifications

### Session 2026-06-12

- Q: Should `Volume Display Scale` change immediately inside `Settings` or open its own screen? → A: It is a settings-menu entry that opens a dedicated screen.
- Q: How should changes be confirmed or discarded? → A: The dedicated screen provides `OK` to apply the selected option and `Cancel` to leave the saved setting unchanged.
- Q: What happens if the user selects a different option and then presses `Cancel`? → A: No change occurs.
- Q: What is the default scale? → A: `0-100`.
- Q: How should the active option be shown? → A: The user must be able to visually identify the currently selected option.
- Q: How much numeric precision should the home-screen volume readout use in each scale? → A: Show one decimal place in both `0-100` and `dB` modes.
- Q: Where should the user return after pressing `OK` or `Cancel` on the selection screen? → A: Both `OK` and `Cancel` return to `Settings`.

## User Scenarios And Testing *(mandatory)*

### User Story 1 - Choose The Home Volume Number Format (Priority: P1)

As a user in `Settings`, I want `Volume Display Scale` to open a dedicated selection screen where I can choose whether the home screen shows the volume number as `0-100` or `dB` so the monitor matches the scale I use mentally when checking listening level.

**Why this priority**: The feature's main value is letting the user pick the numeric format that is most meaningful to them without changing the rest of the product behavior.

**Independent Test**: Open `Settings`, enter `Volume Display Scale`, select an option, press `OK`, return to the home screen, and confirm that the numeric volume readout uses the selected format.

**Acceptance Scenarios**:

1. **Given** the user opens `Settings`, **When** they browse the available options, **Then** they can find a `Volume Display Scale` menu entry.
2. **Given** the user opens `Volume Display Scale`, **When** the selection screen is shown, **Then** they can see both `0-100` and `dB`, and the currently selected option is visually clear.
3. **Given** the user chooses a different option and presses `OK`, **When** the home screen shows a live volume value, **Then** the numeric readout uses the newly selected scale with one decimal place.
4. **Given** the user chooses a different option and presses `Cancel`, **When** they leave the selection screen, **Then** the saved scale remains unchanged and they return to `Settings`.

---

### User Story 2 - Keep The Gauge Behavior Unchanged (Priority: P1)

As a user who already understands the home-screen gauge, I want the gauge to behave the same no matter which number format I choose so I do not need to relearn the visual indicator.

**Why this priority**: The user explicitly wants the feature to change only the number presentation. Altering the gauge would change existing behavior and expand the feature beyond its intended scope.

**Independent Test**: Compare the home screen at the same receiver volume after applying each scale option and confirm that only the numeric readout changes while the gauge presentation remains the same.

**Acceptance Scenarios**:

1. **Given** a live volume value is displayed, **When** the user switches between `0-100` and `dB`, **Then** the gauge continues to represent the same volume level with the same visual range and position while the numeric readout keeps one decimal place in either mode.
2. **Given** a gauge animation is in progress because the receiver volume changed, **When** the user applies a different display scale, **Then** the gauge continues its existing motion without adopting a different scale behavior.

---

### User Story 3 - Keep The Preference For Future Use (Priority: P2)

As a user, I want the selected volume number scale to stay in effect after I leave `Settings` or restart the device so I do not need to reapply the same preference repeatedly.

**Why this priority**: A scale preference is part of the user's ongoing setup. Requiring repeated changes would make the feature feel incomplete.

**Independent Test**: Verify that a new device defaults to `0-100`, then change the scale, leave and re-enter the home screen, restart the device, and confirm that the chosen format is still active when live volume is available again.

**Acceptance Scenarios**:

1. **Given** no saved preference has been changed yet, **When** the home screen first shows live volume, **Then** the numeric readout uses the `0-100` scale by default.
2. **Given** the user changes the volume display scale and presses `OK`, **When** they return to the home screen later in the same session, **Then** the selected scale remains active.
3. **Given** the user previously chose a volume display scale and applied it with `OK`, **When** the device restarts and later shows live volume again, **Then** the same scale remains active.
4. **Given** the user reopens the volume display scale setting, **When** the options are shown, **Then** the currently active saved scale is clearly indicated.
5. **Given** the user presses `OK` or `Cancel` on the volume display scale selection screen, **When** the screen closes, **Then** the device returns to `Settings`.

### Edge Cases

- The receiver is not configured, unavailable, or powered off, so the home screen is showing a non-live state instead of a numeric volume.
- The user highlights a different option on the selection screen and then presses `Cancel`.
- The user changes the preferred scale while the home screen is already showing a live volume value.
- The user changes the preferred scale while a volume change animation is still settling on the home screen.
- The currently active saved scale must still be clear when the user reopens the setting later.
- The live volume is at the extreme low or high end of the supported display range.
- A saved preference exists before the receiver resumes live status after a temporary outage.
- Adding the new setting must not make existing `Settings` destinations unclear or hard to reach.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST add a `Volume Display Scale` entry within `Settings`.
- **FR-002**: Selecting `Volume Display Scale` from `Settings` MUST open a dedicated selection screen.
- **FR-003**: The `Volume Display Scale` selection screen MUST present exactly two options: `0-100` and `dB`.
- **FR-004**: The default volume display scale MUST be `0-100`.
- **FR-005**: The currently selected saved scale MUST be visually identifiable when the user views the selection screen.
- **FR-006**: The selection screen MUST use a single-choice selection pattern with a dedicated selected-state indicator and MUST NOT rely on color alone to show which option is selected.
- **FR-007**: The selection screen MUST provide an `OK` action that applies the selected option and saves it as the new preference.
- **FR-008**: The selection screen MUST provide a `Cancel` action that leaves the saved preference unchanged.
- **FR-009**: If the user selects a different option and then presses `Cancel`, the system MUST discard the pending change.
- **FR-010**: When the user changes the volume display scale and presses `OK`, the system MUST save the new preference.
- **FR-011**: Pressing `OK` or `Cancel` on the selection screen MUST return the user to `Settings`.
- **FR-012**: The selected volume display scale MUST control the numeric volume value shown on the home screen whenever live volume is available.
- **FR-013**: When `0-100` is selected, the home-screen numeric volume display MUST use the `0-100` scale.
- **FR-014**: When `dB` is selected, the home-screen numeric volume display MUST use the `dB` scale.
- **FR-015**: The home-screen numeric volume display MUST show one decimal place in both `0-100` and `dB` modes whenever live volume is available.
- **FR-016**: Changing the volume display scale MUST NOT change the gauge's range, position mapping, or visual behavior.
- **FR-017**: The gauge MUST continue to represent the same underlying receiver volume level regardless of which numeric display scale is selected.
- **FR-018**: The selected volume display scale MUST remain active after the user leaves `Settings` and returns to the home screen.
- **FR-019**: The selected volume display scale MUST remain active after the device restarts.
- **FR-020**: If the home screen is showing a non-live receiver state such as `not configured`, `unavailable`, or `powered off`, changing the volume display scale MUST NOT replace that state with a numeric value.
- **FR-021**: The system MUST apply a newly confirmed scale without requiring the user to repeat receiver setup or restart the device.
- **FR-022**: Adding the `Volume Display Scale` entry MUST preserve clear labeled navigation for all existing `Settings` destinations.

### Key Entities *(include if feature involves data)*

- **Volume Display Scale Preference**: The user's saved format choice for the home-screen numeric volume readout, with `0-100` as the default.
- **Home Volume Readout**: The numeric volume value presented on the home screen when live receiver volume is available.
- **Home Volume Gauge**: The existing visual gauge on the home screen that continues to represent volume without changing scale behavior.
- **Volume Display Scale Selection Screen**: The confirmation screen opened from `Settings` where the user can review the current choice, select a different option, and either apply or discard that pending choice.
- **Settings Destination**: A labeled option within `Settings`, including the new `Volume Display Scale` entry and the existing setup destinations.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: In interaction testing, users can reach the `Volume Display Scale` screen, choose an option, and apply it in 4 taps or fewer from `Settings`.
- **SC-002**: In validation testing, 100% of live-volume cases show the home-screen numeric readout in the selected format after the next normal screen refresh following `OK`.
- **SC-003**: In validation testing, 100% of live-volume cases show one decimal place in both `0-100` and `dB` modes.
- **SC-004**: In cancel-flow testing, 100% of pending changes are discarded when the user presses `Cancel`, including cases where a different option was highlighted before canceling.
- **SC-005**: In interaction testing, 100% of `OK` and `Cancel` exits from the selection screen return the user to `Settings`.
- **SC-006**: In comparison testing at the same receiver volume, 100% of scale changes leave the gauge visually unchanged apart from any normal live-volume updates already in progress.
- **SC-007**: In restart testing, 100% of saved scale preferences remain active after the device reboots, and devices with no saved preference use `0-100`.
- **SC-008**: In degraded-state testing, 100% of `not configured`, `unavailable`, and `powered off` home-screen states remain non-numeric after the user changes the preferred scale.
- **SC-009**: In usability review, users can identify the currently selected option on the `Volume Display Scale` screen on their first attempt without trial and error.

## Assumptions

- This feature changes only how the home-screen volume number is presented and does not add direct volume control.
- The existing gauge remains the authoritative visual indicator for gauge behavior and does not adopt different rules for the two number formats.
- The product already has access to the information needed to present the current volume in both supported number formats, so this feature focuses on user selection and presentation.
- The selected scale is a user preference that should persist like other on-device configuration choices.
- Existing settings navigation remains text-labeled and readable, even if the menu uses the established overflow or pagination behavior to accommodate additional destinations.
- The selection screen follows the existing settings-flow model of reviewing a pending choice and confirming it explicitly with `OK` rather than applying changes immediately on highlight.
- The selected option should follow Material's single-choice pattern, which favors radio-button-style indicators for one-of-many selection and an additional visual cue beyond color alone.
