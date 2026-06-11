# Feature Specification: Material Design UI

**Feature Branch**: `feature/006-material-design`

**Created**: 2026-06-04

**Status**: Draft

**Input**: User description: "Use Material Design"

## Clarifications

### Session 2026-06-04

- Q: Which button variant should be the default for standard actions across screens? → A: Standard actions use icon-with-text buttons; icon-only is allowed only for documented compact contexts like keyboard controls.
- Q: Which on-device screens are in scope for the Material Design refresh? → A: Refresh all current on-device screens, including calibration, status, and result screens.
- Q: When should progress feedback appear for loading operations? → A: Searching screens show progress immediately; other operations show progress only after 500 ms.
- Q: Where should the shared Material-compatible styling rules live? → A: Add a shared component/style catalog to `docs/ui-reference.md`.

## User Scenarios and Testing *(mandatory)*

### User Story 1 - Recognize the Interface as a Coherent Appliance (Priority: P1)

As a person viewing the volume monitor from across the room, I want the home screen, setup screens, and settings screens to share a coherent Material Design visual language so the device feels intentional and easy to understand at a glance.

**Why this priority**: The main value of this device is fast, low-friction visibility in a living room. A Material Design refresh must improve clarity without weakening the dark, high-contrast appliance experience.

**Independent Test**: Review every current on-device screen as a complete UI set, including home, settings, setup, keyboard, calibration, status, and result screens. The story is complete when the screens share consistent spacing, typography hierarchy, icon treatment, component states, and touch affordances while remaining readable from 15 feet.

**Acceptance Scenarios**:

1. **Given** the home screen is visible from a normal room viewing position, **When** the user glances at the display, **Then** the volume, source, listening mode, and audio-family state remain the dominant information.
2. **Given** the user moves between home, settings, and setup screens, **When** each screen appears, **Then** the screens look like parts of the same product through consistent visual hierarchy, spacing, icons, and state styling.
3. **Given** a screen uses a Material Design pattern, **When** that pattern conflicts with the project's dark, distance-readable appliance constraints, **Then** the appliance constraints take precedence.

---

### User Story 2 - Complete Touch Setup with Material Controls (Priority: P1)

As a user configuring Wi-Fi or receiver settings on the device, I want touch controls, lists, dialogs, and keyboards to use familiar Material Design behavior so setup feels predictable and requires less interpretation.

**Why this priority**: Setup is the highest-friction interaction on the device. Material Design should make touch targets, screen progression, selection states, and confirmation actions easier to scan and use.

**Independent Test**: Walk through Wi-Fi setup and receiver setup from Settings using only the touchscreen. The story is complete when all required actions are available through clearly styled Material-compatible controls and the user can complete setup without relying on hidden gestures or ambiguous labels.

**Acceptance Scenarios**:

1. **Given** a settings row opens a setup flow, **When** the row is shown, **Then** it presents a clear touch target, label, and affordance consistent with other navigable rows.
2. **Given** the user must choose a network or receiver, **When** choices are listed, **Then** each choice is scannable, selected and focused states are clear, and long names remain legible.
3. **Given** the user confirms, cancels, retries, or saves setup information, **When** the action is shown, **Then** the action uses consistent Material-compatible button styling and communicates its consequence before activation.
4. **Given** the user enters text with the on-device keyboard, **When** key states change, **Then** pressed, selected, disabled, and error states are visually distinct and readable.
5. **Given** the same action appears on multiple screens, **When** the user compares those screens, **Then** the action uses the same approved button variant unless the documented pattern explicitly calls for icon-only treatment.
6. **Given** the keyboard shows OK and Cancel controls, **When** the keyboard is displayed, **Then** the controls are vertically aligned and use the documented keyboard button variant.
7. **Given** the Wi-Fi setup or receiver setup flow is shown, **When** the screen is scanning or presenting a selection list, **Then** the page title matches the Settings menu label, remains in a consistent centered position, and the screen includes a matching centered instruction line.

---

### User Story 3 - Understand State, Feedback, and Errors (Priority: P2)

As a user, I want connection states, progress, success, warnings, and failures to follow a consistent Material Design feedback model so I understand what the device is doing and what I should do next.

**Why this priority**: The monitor must remain calm and passive, but setup and connection failures need enough feedback to be recoverable without guesswork.

**Independent Test**: Exercise normal, loading, empty, success, warning, and failure states for Wi-Fi and receiver setup. The story is complete when every state has a clear message, matching visual treatment, and an appropriate next action where recovery is possible.

**Acceptance Scenarios**:

1. **Given** the device is scanning, connecting, or saving, **When** progress is shown, **Then** the screen communicates ongoing work without distracting idle animation.
2. **Given** a recoverable error occurs, **When** the error is displayed, **Then** the user sees a concise explanation and a clear next action such as retry, edit, or go back.
3. **Given** a step completes successfully, **When** success is displayed, **Then** the user can tell whether the device will continue automatically or expects a touch action.
4. **Given** receiver status is temporarily unavailable on the home screen, **When** the status is shown, **Then** the screen preserves the core layout and communicates degraded state without appearing broken.
5. **Given** Wi-Fi network search and receiver search are both in progress, **When** each searching screen is displayed, **Then** the title placement, explanatory text, icon placement, font roles, and progress treatment match the same searching-screen pattern.
6. **Given** an operation has an unknown wait time, **When** it remains in progress beyond the threshold where a loading state should be shown, **Then** the UI presents restrained movement that confirms the device is still working.

---

### User Story 4 - Preserve Contributor Design Consistency (Priority: P3)

As a project contributor, I want durable Material Design guidance for this device so future UI changes reuse the same visual decisions instead of creating one-off screen styles.

**Why this priority**: A design refresh only stays useful if later features can extend it without reinterpreting every spacing, color, typography, icon, and component decision.

**Independent Test**: Review the durable UI reference after the feature is complete. The story is complete when contributors can identify the approved component styles, state treatments, screen patterns, and exceptions that apply to this device.

**Acceptance Scenarios**:

1. **Given** a contributor adds or updates a screen, **When** they consult the UI reference, **Then** they can find the expected Material-compatible pattern for common controls and states.
2. **Given** a contributor needs a new action or status icon, **When** they consult the icon inventory and UI reference, **Then** they can determine whether to reuse an existing asset or create a new Material-compatible one.
3. **Given** a future design choice conflicts with the project's dark appliance principles, **When** the UI reference is consulted, **Then** the documented priority order makes the acceptable choice clear.

### Edge Cases

- A standard Material Design pattern uses light surfaces or dense text that does not remain readable on the dark 4-inch display.
- A component that looks clear in isolation competes with the home screen's large volume display.
- A long Wi-Fi network name, receiver name, source name, or listening mode label exceeds the normal space available.
- A touch target looks visually compact but still needs to remain easy to hit with a finger.
- Icon-only, icon-with-text, and text-only button variants are all available, but the same action must not switch variants inconsistently across equivalent contexts.
- Keyboard action controls can use icon-only buttons, but their shape, size, and alignment still need to match the documented keyboard pattern.
- Wi-Fi network lists and receiver lists contain different content but still need comparable row height, typography, selection state, and spacing.
- A disabled, selected, focused, pressed, loading, or error state is visible against both normal and highlighted surfaces.
- A state color is not distinguishable enough by itself and needs supporting shape, icon, or text treatment.
- A Material-compatible icon is unavailable for an audio-specific concept and custom artwork must still match the broader design language.
- A setup flow changes from scanning to a selection list and must not jump between centered and left-aligned headers.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The UI MUST adopt a Material Design visual language across all current on-device screens, including the home screen, settings, Wi-Fi setup, receiver setup, status screens, result screens, calibration screens, and on-device keyboard.
- **FR-002**: The UI MUST preserve the project's dark-mode-only, high-contrast presentation and 15-foot readability requirements.
- **FR-003**: The home screen MUST keep the volume display, source, listening mode, and audio-family state as the primary visual hierarchy.
- **FR-004**: The UI MUST define consistent Material-compatible typography roles for primary values, section labels, body text, button text, status text, and compact metadata.
- **FR-005**: The UI MUST define consistent spacing rules for screen margins, row spacing, button spacing, keyboard keys, icon placement, and status messages.
- **FR-006**: The UI MUST define Material-compatible control patterns for buttons, icon buttons, navigation rows, selection lists, dialogs or confirmation prompts, status messages, and keyboard keys.
- **FR-007**: The UI MUST define approved button variants where standard actions use icon-with-text buttons, icon-only buttons are allowed only for documented compact contexts such as keyboard controls, and text-only buttons are reserved for documented exceptions.
- **FR-008**: Equivalent actions MUST use the same approved button variant, label treatment, icon treatment, shape, size, color role, and font across equivalent contexts.
- **FR-009**: Standard OK, Cancel, Save, Retry, Back, and Confirm actions MUST use icon-with-text buttons across screens unless the feature documentation records a specific compact-context exception.
- **FR-010**: Keyboard action buttons MAY use icon-only treatment, but they MUST share consistent shape, sizing, vertical alignment, spacing, state treatment, and icon style.
- **FR-011**: The keyboard OK and Cancel controls MUST align to the same vertical baseline and visual center within the keyboard action area.
- **FR-012**: Selection lists, including Wi-Fi network lists and receiver lists, MUST use consistent row height, typography roles, spacing, selection state, focus state, empty state, and unavailable state treatment.
- **FR-013**: Searching screens, including Wi-Fi network search and receiver search, MUST use a shared layout pattern for title placement, explanatory text placement, icon placement, font roles, and action placement.
- **FR-014**: Searching screens MUST show progress feedback immediately when searching starts.
- **FR-015**: Non-searching loading operations MUST show progress feedback only when the operation remains active for at least 500 ms.
- **FR-016**: Progress feedback for unknown-duration searching MUST use restrained movement that communicates activity without distracting from the passive appliance experience.
- **FR-017**: Touch targets MUST remain large enough for direct finger input on every interactive control.
- **FR-018**: Interactive controls MUST provide visually distinct normal, pressed, selected, focused, disabled, loading, success, warning, and error states where those states apply.
- **FR-019**: Setup and settings screens MUST use consistent Material-compatible list, action, and feedback patterns so equivalent actions look and behave equivalently across flows.
- **FR-020**: The on-device keyboard MUST follow the same Material-compatible shape, spacing, typography, and state language as the rest of the setup UI.
- **FR-021**: Icons MUST follow the project's Material-compatible icon guidance, including existing bitmap icon inventory decisions where they apply.
- **FR-022**: Icons MUST supplement concise labels where labels are necessary for setup clarity.
- **FR-023**: The UI MUST communicate important states with more than color alone whenever state recognition affects setup, recovery, or safety of user action.
- **FR-024**: Progress, loading, and transition feedback MUST remain restrained and MUST NOT create distracting idle animation on the passive home display.
- **FR-025**: Error and warning states MUST include a concise user-facing explanation and at least one clear recovery action when recovery is possible.
- **FR-026**: Text content MUST fit within its intended control or screen area without overlapping neighboring content.
- **FR-027**: Long names and values MUST remain understandable through truncation, wrapping, or alternate presentation that preserves the surrounding layout.
- **FR-028**: The UI MUST define an explicit priority order for conflicts between Material Design conventions, project readability principles, display constraints, and setup usability.
- **FR-029**: `docs/ui-reference.md` MUST include a shared Material-compatible component/style catalog covering buttons, lists, searching screens, typography, spacing, icons, state treatments, and known exceptions for this device.
- **FR-030**: The feature MUST identify any existing screen styles that intentionally remain unchanged and explain why they are outside this refresh scope; no current on-device screen is excluded by default.
- **FR-031**: The refresh MUST avoid changing user-visible setup or monitoring workflows unless the change improves clarity or consistency while preserving the same user goal.
- **FR-032**: Wi-Fi setup and receiver setup screens MUST use page titles that match the Settings menu labels, and those titles MUST remain consistently centered during both scanning and selection states.
- **FR-033**: Wi-Fi setup and receiver setup screens MUST include a concise centered instruction line below the page title in both scanning and selection states.

### Key Entities

- **UI Surface**: A device screen or screen region that presents information or accepts touch input, such as Home, Settings, Wi-Fi setup, Receiver setup, calibration, status feedback, result feedback, or the keyboard.
- **Component Pattern**: A reusable visual and behavioral treatment for a control or content unit, including its purpose, visual hierarchy, touch affordance, and allowed states.
- **State Treatment**: The visible presentation for a condition such as normal, pressed, selected, disabled, loading, success, warning, error, unavailable, or inactive.
- **Design Exception**: A documented case where the device intentionally diverges from standard Material Design conventions to preserve readability, passive appliance behavior, or touch setup usability.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: In a visual review from 15 feet, reviewers can correctly identify the current volume, source, listening mode, and active audio family on the home screen in 5 seconds or less.
- **SC-002**: A first-time user can complete the Wi-Fi setup and receiver setup flows using only the touchscreen without external instructions in 5 minutes or less.
- **SC-003**: At least 90% of reviewed interactive controls across covered screens use the documented Material-compatible component and state patterns with no one-off styling.
- **SC-004**: All covered screens pass a readability review with no overlapping text, clipped primary labels, or ambiguous primary actions at the target display size.
- **SC-005**: Every recoverable setup error reviewed includes a plain-language message and a clear next action.
- **SC-006**: Contributors can locate the durable Material-compatible UI guidance and identify the correct pattern for common controls within 2 minutes.
- **SC-007**: In a screen-by-screen review, every repeated action uses the same documented button variant across equivalent contexts, with all documented exceptions approved in the UI reference.
- **SC-008**: Wi-Fi searching and receiver searching screens match the documented searching-screen pattern and visibly communicate ongoing work immediately after searching starts.

## Assumptions

- The refresh applies to all current screens in the existing on-device touchscreen UI rather than adding a separate mobile, desktop, or web interface.
- Material Design is used as a design language and interaction convention, not as permission to weaken the project's dark, high-contrast, 15-foot appliance requirements.
- Existing monitoring and setup workflows remain in scope, but wholesale product workflow redesign is out of scope unless needed to make the Material-compatible UI coherent.
- The bitmap icon work from the previous feature is the expected baseline for icon source, inventory, and visual-weight decisions.
- Material Design progress guidance is treated as in scope for active searching and saving states. Searching screens show restrained progress movement immediately; other operations show progress only when they remain active for at least 500 ms.
- Durable design guidance belongs in a shared component/style catalog in `docs/ui-reference.md`; feature-specific planning artifacts stay under this feature directory.
