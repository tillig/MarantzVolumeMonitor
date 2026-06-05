# Contract: Component Style Catalog

## Purpose

Define the durable Material-compatible UI catalog that implementation must add to
`docs/ui-reference.md` and mirror in shared UI-layer style helpers.

## Required Catalog Sections

### Scope

- Covers all current on-device screens in `src/ui/Screens`.
- Excludes mobile, desktop, and web interfaces.
- States that project appliance rules override generic Material Design choices when they conflict.

### Typography Roles

The catalog must name roles for:

- Home primary value
- Screen title
- Section label
- Body text
- Button label
- List primary text
- List secondary text
- Status message
- Compact metadata

Each role must include intended usage, font choice, color role, and alignment expectations.

### Spacing And Geometry

The catalog must define:

- Screen margins
- Row spacing
- Button spacing
- Keyboard key spacing
- Icon-to-label gap
- Standard corner radius
- Compact keyboard corner radius when different
- Minimum touch target expectations

### Button Variants

The catalog must define these variants:

| Variant | Required Use |
| --- | --- |
| `standard-icon-text` | OK, Cancel, Save, Retry, Back, Confirm, Manual, Rescan, Discover, and equivalent standard actions outside compact contexts |
| `keyboard-icon-only` | Keyboard OK, Cancel, backspace, visibility, and caps-lock compact controls |
| `documented-exception` | Any approved case that cannot use the two standard variants |

For each variant, the catalog must document shape, fill/outline treatment, icon placement, label
placement, font, color roles, and state treatments.

### Selection Lists

The catalog must define one selection-list row pattern for Wi-Fi networks, receiver candidates, and
future setup selections. It must cover row height, leading visual, primary text, secondary metadata,
trailing visual, long-value handling, pagination/overflow affordances, and empty/unavailable states.

### Searching Screens

The catalog must define one searching-screen pattern for Wi-Fi scanning and receiver discovery. It
must cover title placement, message placement, scan icon placement, progress placement, progress start
rule, and bottom actions.

### State Treatments

The catalog must define normal, pressed, selected, focused, disabled, loading, success, warning,
error, inactive, and unavailable treatments. Loading and progress treatments must specify that
movement is restrained and non-blocking.

### Exceptions

Every exception must include:

- Screen or component
- Shared rule being bypassed
- Reason the exception is needed
- Visual and test constraints that still apply

## Acceptance Criteria

- A contributor can identify the correct pattern for a common control within 2 minutes.
- Every current screen can be mapped to catalog patterns without relying on implicit per-screen style
  decisions.
- Standard button actions do not change between text-only, icon-only, and icon-with-text variants
  except through documented exceptions.
- Keyboard icon-only buttons are documented as compact controls and remain aligned with each other.
