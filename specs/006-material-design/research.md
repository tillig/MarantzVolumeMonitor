# Research: Material Design UI

## Decision: Add Shared UI-Layer Style Primitives

Use a small UI-layer helper, planned as `src/ui/MaterialStyle.h/.cpp`, for shared drawing primitives and constants used by buttons, lists, progress/searching screens, typography roles, spacing, and state treatments.

**Rationale**: The current screens duplicate button, row, and status drawing decisions. A shared UI-layer helper keeps styling consistent without moving behavior into network or storage layers.

**Alternatives considered**:

- Per-screen cleanup only: rejected because it would preserve the root cause of inconsistent buttons, lists, and searching screens.
- A full widget framework: rejected as too heavy for the current embedded display and feature scope.
- Extending `DisplayManager` with all component helpers: rejected because `DisplayManager` should stay focused on display access and shared colors.

## Decision: Standard Actions Use Icon-With-Text Buttons

Use icon-with-text buttons for standard OK, Cancel, Save, Retry, Back, Confirm, Manual, Rescan, and Discover actions. Icon-only buttons are allowed only for documented compact contexts such as keyboard function keys.

**Rationale**: The spec clarifies this as the default. It preserves readability and makes equivalent actions look equivalent across setup, status, and result screens while allowing the keyboard to remain space-efficient.

**Alternatives considered**:

- Text-only standard buttons: rejected because the prior bitmap icon work established icons as part of the device language.
- Context-by-context button variants: rejected because it would keep the inconsistency this feature is meant to remove.
- Icon-only standard buttons: rejected because setup clarity still depends on concise labels.

## Decision: Use One Selection List Pattern

Use one shared pattern for Wi-Fi networks and receiver candidates: consistent row height, corner radius, typography roles, icon/metadata placement, selected/focused/unavailable states, truncation, and pagination or overflow affordances.

**Rationale**: Wi-Fi and receiver lists differ in content, but the user task is the same: scan a list, choose an entry, or recover when the list is empty. A shared pattern makes setup predictable.

**Alternatives considered**:

- Preserve separate Wi-Fi and receiver list layouts: rejected because the current inconsistent row sizes and placement are explicitly in scope.
- Make receiver rows richer than Wi-Fi rows: deferred unless required by future receiver metadata.

## Decision: Use One Searching Screen Pattern

Use one shared searching pattern for Wi-Fi network search and receiver discovery. The pattern defines title placement, explanatory text placement, scan icon placement, progress indicator placement, and bottom action placement.

**Rationale**: Both screens communicate unknown-duration discovery. Matching layout and progress treatment reduces perceived inconsistency and gives users a clear signal that the device is still working.

**Alternatives considered**:

- Keep static searching screens: rejected because the spec requires visible activity during non-immediate searching.
- Use different search layouts per domain: rejected because the two flows are functionally similar.
- Use prominent full-screen animation: rejected because the constitution forbids distracting idle animation and prioritizes passive appliance stability.

## Decision: Use Restrained Non-Blocking Progress Movement

Show restrained progress movement immediately when explicit searching starts. For other loading operations such as saving or connecting, show progress only when the operation is not effectively immediate.

**Rationale**: Searching screens have unknown duration and need immediate feedback. Other operations can complete quickly, so delaying progress avoids flicker and unnecessary visual noise.

**Alternatives considered**:

- Always animate every loading operation immediately: rejected because it can add visual noise.
- Never animate progress: rejected because searching screens need an activity signal.
- Determinate progress values: rejected because Wi-Fi scan and receiver discovery do not expose meaningful completion percentages.

## Decision: Scope Covers All Current On-Device Screens

Apply the refresh to every current screen in `src/ui/Screens`, including home, settings, network list, keyboard, setup status, receiver list, receiver IP entry, receiver verification/status, setup result, and calibration.

**Rationale**: The feature goal is cross-screen consistency. Leaving status, result, or calibration screens out would create visible exceptions immediately after the refresh.

**Alternatives considered**:

- Only update the named setup screens: rejected because the clarification selected all current on-device screens.
- Only update screens with known defects: rejected because it would miss subtle typography, spacing, and state inconsistencies.

## Decision: Durable Style Catalog Lives in `docs/ui-reference.md`

Document the shared Material-compatible component/style catalog in `docs/ui-reference.md`. Keep feature-specific contracts and validation notes in this feature directory.

**Rationale**: `docs/ui-reference.md` is the durable UI reference for this project. Future contributors need stable guidance outside a single feature plan.

**Alternatives considered**:

- Keep all guidance in the feature folder: rejected because implemented UI guidance would be hard to find after the feature completes.
- Document only exceptions: rejected because contributors need positive rules for common components, not just a list of what differs.
