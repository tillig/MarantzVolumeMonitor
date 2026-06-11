# Data Model: Display Current Settings

## SettingsMenuPageState

Represents the visible page of the `Settings` menu after adding `Current Settings`.

### Menu Page Fields

| Field | Type | Description |
| --- | --- | --- |
| `pageIndex` | Integer | Zero-based visible page index. |
| `pageCount` | Integer | Total number of settings pages available. |
| `visibleEntries` | Ordered list | The labeled settings destinations shown on the active page. |

### Menu Page Validation Rules

- `Current Settings` must be the first entry in the overall destination order.
- Each page must keep labeled row navigation and the existing bottom `OK` action.
- Pagination controls appear only when more than one page exists.
- Existing destinations for `Wi-Fi Setup`, `Receiver Setup`, and `Touch Calibration` must remain reachable.

## CurrentSettingsViewModel

Represents the complete read-only overview shown on `Current Settings`.

### View Model Fields

| Field | Type | Description |
| --- | --- | --- |
| `wifiSsid` | String or unavailable state | Saved Wi-Fi SSID from configuration. |
| `monitorIpAddress` | String or unavailable state | Current device IP address while connected to Wi-Fi. |
| `wifiSignalGraphicLevel` | Integer or unavailable state | Strength level used to render the established Wi-Fi icon treatment. |
| `wifiSignalRssi` | Integer or unavailable state | Current Wi-Fi RSSI in dBm when available. |
| `receiverDisplayName` | String or unavailable state | Receiver friendly name or model text when available. |
| `receiverIpAddress` | String or unavailable state | Saved configured receiver IP address. |
| `receiverPowerState` | Enum | Current receiver state for display. |
| `lastRefreshAtMs` | Integer | Timestamp of the most recent successful live refresh. |

### View Model Validation Rules

- The screen must combine saved configuration and live state without implying that unavailable live values are confirmed current.
- `wifiSsid` and `receiverIpAddress` use a `setup required` representation when no saved value exists.
- `monitorIpAddress`, `wifiSignalGraphicLevel`, and `wifiSignalRssi` use an `unavailable` representation when Wi-Fi is not currently connected or those live values cannot be read.
- `receiverDisplayName` is optional and may remain unavailable even when `receiverIpAddress` is known.
- `receiverPowerState` must distinguish `poweredOn`, `poweredOff`, `unavailable`, and `notConfigured`.

## ReceiverPowerState

Represents the receiver state shown on the read-only overview.

### Values

- `poweredOn`: Fresh receiver status confirms the configured receiver is on.
- `poweredOff`: Fresh receiver status confirms the configured receiver is off.
- `unavailable`: The receiver is configured but current live status cannot be confirmed.
- `notConfigured`: No saved receiver IP exists.

### Receiver Power Validation Rules

- `poweredOff` must remain distinct from `unavailable`.
- `notConfigured` must not reuse the same label as `unavailable`.
- `poweredOn` and `poweredOff` require fresh receiver-status data from the network layer.

## CurrentSettingsRefreshSession

Represents the live refresh behavior while the screen remains visible.

### Refresh Session Fields

| Field | Type | Description |
| --- | --- | --- |
| `refreshIntervalMs` | Integer | Live refresh cadence. Planned value is 1000 ms. |
| `isVisible` | Boolean | Whether the screen is active and should continue refreshing. |
| `receiverIdentityCached` | Boolean | Whether receiver name or type has already been resolved for the current screen session. |
| `awaitingNextRefreshAtMs` | Integer | Next scheduled live refresh time. |

### State Transitions

- `screenOpened -> firstRefreshPending`: The screen becomes active and immediately requests current data.
- `firstRefreshPending -> refreshing`: The screen reads current Wi-Fi and receiver status.
- `refreshing -> visibleIdle`: A refresh completes and the screen waits for the next interval.
- `visibleIdle -> refreshing`: The 1-second refresh interval elapses while the screen is still visible.
- `visibleIdle -> screenClosed`: The user presses `OK` and returns to `Settings`.

### Refresh Session Validation Rules

- Live refresh must continue only while the screen is visible.
- Receiver identity lookup may be cached for the screen session and should not trigger every second once resolved.
- Refresh logic must not block touch handling or settings dismissal.
