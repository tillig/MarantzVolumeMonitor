# Research: Display Current Settings

## Settings Menu Overflow Pattern

- Decision: Use paginated settings navigation with labeled rows and `PREV`/`NEXT` controls if all settings destinations do not fit clearly on one page.
- Rationale: The current `SettingsScreen` layout fits 3 full-height Material-style rows plus the bottom `OK` action cleanly. Adding a fourth row with the same treatment would crowd the bottom action area and weaken touch clarity. The existing Wi-Fi list already establishes a Material-compatible pagination pattern for constrained list screens, so reusing that pattern reduces one-off UI behavior.
- Alternatives considered:
  - Shrink row height or spacing to force 4 rows onto one page: rejected because it compromises the established settings-row treatment and touch clarity.
  - Switch to icon-only settings navigation: rejected because the spec explicitly prefers readable labeled navigation and this repo uses icon-plus-text for standard actions.

## Live Refresh Cadence

- Decision: Refresh the `Current Settings` screen immediately on open and continue refreshing live status at a 1-second cadence while the screen remains visible.
- Rationale: `HomeScreen` already refreshes receiver and Wi-Fi state on a 1-second loop, and the specification requires visible updates within 3 seconds. Matching the existing cadence keeps the appliance stable, avoids unnecessary network traffic, and preserves responsive touch handling.
- Alternatives considered:
  - Snapshot only on entry: rejected because the clarified spec requires live current status while the screen remains open.
  - Faster repeated polling: rejected because the feature does not need higher-frequency data and extra refreshes would add redraw and network churn.

## Receiver Identity Source

- Decision: Resolve receiver name or type through a network-layer metadata lookup based on the configured receiver IP and cache the result for the screen session instead of persisting the name in `config.json`.
- Rationale: The current config stores only `receiverIp`, while `ReceiverDiscovery` already contains the description-XML parsing needed to derive a friendly name or model. Keeping identity lookup in the network layer preserves architecture boundaries and avoids a storage-schema change for data the spec marks as optional.
- Alternatives considered:
  - Persist receiver name during setup: rejected because it changes the config schema and creates stale-data migration issues for an optional display field.
  - Show only receiver IP and omit name/type completely: rejected because the spec requires name or type when available.

## Wi-Fi Signal Presentation

- Decision: Reuse the existing Wi-Fi selection screen's signal-strength graphic and show numeric RSSI when available.
- Rationale: The repo already encodes a consistent 1-to-3 bar visual language for Wi-Fi strength in `MaterialStyle::drawListRow`. Reusing that graphic keeps the current-settings screen consistent with the rest of the UI while the numeric RSSI supports troubleshooting.
- Alternatives considered:
  - Numeric RSSI only: rejected because it is less glanceable for the on-device appliance UI.
  - Qualitative label only: rejected because it removes the numeric detail the user explicitly asked to retain when available.

## Current Settings Screen Layout

- Decision: Use a dedicated read-only screen with separate Wi-Fi and Receiver sections plus one bottom `OK` action.
- Rationale: The overview must show mixed saved and live values, unavailable states, and the Wi-Fi strength graphic without becoming visually dense. A sectioned status layout is clearer than reusing tappable list-row styling for non-interactive fields.
- Alternatives considered:
  - Reuse list rows for every status field: rejected because list rows visually imply navigation and make read-only state harder to scan.
  - Split Wi-Fi and receiver details across separate screens: rejected because the feature value is reviewing both categories together.
