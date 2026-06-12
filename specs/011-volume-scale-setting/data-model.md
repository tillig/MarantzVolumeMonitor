# Data Model: Volume Display Scale

## VolumeDisplayScalePreference

Represents the persisted user preference that controls the home-screen numeric volume format.

### Preference Fields

| Field | Type | Description |
| --- | --- | --- |
| `useDbScale` | Boolean | Saved config flag where `false` means `0-100` and `true` means `dB`. |
| `defaultMode` | Boolean | Default saved value used when no config file or field exists. Planned value is `false`. |

### Preference Validation Rules

- `useDbScale = false` maps to the normalized `0-100` home-screen number format.
- `useDbScale = true` maps to the raw receiver `dB` home-screen number format.
- Missing or unreadable saved config falls back to `useDbScale = false`.
- Saving the preference must not change unrelated config fields such as Wi-Fi credentials, receiver IP, or brightness.

## VolumeScaleSelectionState

Represents the user-visible state of the dedicated `Volume Display Scale` screen while the user is reviewing or changing the selection.

### Selection State Fields

| Field | Type | Description |
| --- | --- | --- |
| `savedChoice` | Enum | The currently persisted scale when the screen opens. |
| `pendingChoice` | Enum | The option currently highlighted on the selection screen. |
| `hasPendingChange` | Boolean | Whether `pendingChoice` differs from `savedChoice`. |
| `selectedIndicatorVisible` | Boolean | Whether the dedicated selected-state indicator is shown for the active option. Planned value is always true for the current option. |
| `exitTarget` | Enum | The destination screen after `OK` or `Cancel`. Planned value is `Settings`. |

### Selection State Values

- `normalized`: Show the home-screen number on the `0-100` scale.
- `db`: Show the home-screen number on the raw receiver dB scale.

### Selection State Transitions

- `screenOpened -> reviewingSavedChoice`: The screen loads `savedChoice` and copies it into `pendingChoice`.
- `reviewingSavedChoice -> pendingChanged`: The user taps the other option, changing `pendingChoice` while leaving `savedChoice` untouched.
- `pendingChanged -> savedApplied`: The user presses `OK`, persisting `pendingChoice` and returning to `Settings`.
- `pendingChanged -> discarded`: The user presses `Cancel`, discarding `pendingChoice` and returning to `Settings`.
- `reviewingSavedChoice -> discarded`: The user presses `Cancel` without changing the option and returns to `Settings`.

### Selection State Validation Rules

- The screen must show exactly one selected-state indicator at a time.
- The selected-state indicator must remain visible even if the row is also styled as focused or pressed.
- `Cancel` must never mutate `savedChoice`.
- `OK` must persist `pendingChoice` only when the value differs or when saving the current value is harmlessly idempotent.

## HomeVolumePresentation

Represents the home-screen volume data after applying the user’s preferred numeric format.

### Presentation Fields

| Field | Type | Description |
| --- | --- | --- |
| `receiverVolumeDb` | Float | Raw live receiver volume from `MarantzClient`. |
| `normalizedVolume` | Float | Existing normalized value derived from `receiverVolumeDb + 80`, constrained to `0-100`. |
| `numericDisplayMode` | Enum | The active home-screen number format, `normalized` or `db`. |
| `visibleNumberText` | String | The formatted home-screen number shown to the user. |
| `gaugeVolume` | Float | The value used for gauge positioning and animation. Planned source is always `normalizedVolume`. |
| `hasLiveVolume` | Boolean | Whether the home screen is in a live state that should show a number at all. |

### Presentation Validation Rules

- `visibleNumberText` shows one decimal place in both `normalized` and `db` modes.
- `gaugeVolume` always uses `normalizedVolume`, regardless of `numericDisplayMode`.
- Non-live states such as `not configured`, `unavailable`, and `powered off` suppress `visibleNumberText`.
- Changing `numericDisplayMode` must not change the gauge position for the same underlying receiver volume.
- Live animation retargeting continues to operate on `gaugeVolume`, not on the selected text format.

## SettingsMenuDestinationOrder

Represents the relevant `Settings` menu ordering after adding the new destination.

### Destination Fields

| Field | Type | Description |
| --- | --- | --- |
| `entryIndex` | Integer | Zero-based destination order in `Settings`. |
| `label` | String | User-facing menu label. |
| `supportsReturnToSettings` | Boolean | Whether the destination flow returns to `Settings` when finished. |

### Destination Validation Rules

- `Volume Display Scale` must appear as a labeled `Settings` destination.
- Existing destinations remain reachable after adding the new entry.
- The `Settings` screen continues to preserve labeled pagination if multiple pages are needed.
