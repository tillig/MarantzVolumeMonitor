# Data Model: Touch Calibration Persistence

## DeviceConfig

Represents the persisted user configuration stored in `config.json`.

### DeviceConfig Fields

| Field | Type | Description |
| --- | --- | --- |
| `wifiSsid` | String | Saved Wi-Fi network name. |
| `wifiPassword` | String | Saved Wi-Fi password. |
| `receiverIp` | String | Saved receiver IP address. |
| `brightness` | Integer | Existing brightness setting. |
| `useDbScale` | Boolean | Existing home-volume number format setting. |
| `touchCalibration` | Object or missing | Optional saved user calibration profile. Missing means the device uses the shipped default calibration. |

### DeviceConfig Validation Rules

- Existing fields remain backward-compatible and must keep their current defaults.
- Missing `touchCalibration` means the shipped default coefficients remain active.
- Invalid or unreadable `touchCalibration` data must be ignored without preventing the rest of the config from loading.
- Selective reset must clear only the requested config fields or sections.

## TouchCalibrationProfile

Represents the optional persisted affine transform that overrides the shipped default touch mapping.

### TouchCalibrationProfile Fields

| Field | Type | Description |
| --- | --- | --- |
| `version` | Integer | Schema version for saved calibration data. Planned initial value is `1`. |
| `xFromRawX` | Float | Affine coefficient used to compute mapped X from raw X. |
| `xFromRawY` | Float | Affine coefficient used to compute mapped X from raw Y. |
| `xOffset` | Float | Affine X offset. |
| `yFromRawX` | Float | Affine coefficient used to compute mapped Y from raw X. |
| `yFromRawY` | Float | Affine coefficient used to compute mapped Y from raw Y. |
| `yOffset` | Float | Affine Y offset. |
| `source` | Enum | Runtime provenance for the active profile, `default` or `saved`. |

### TouchCalibrationProfile Validation Rules

- All coefficients must be finite numeric values.
- The fitted transform must map calibration targets into the screen bounds closely enough to be considered usable.
- The affine determinant must remain far enough from zero to avoid a degenerate transform.
- A saved profile replaces the shipped default only after the candidate profile passes validation.

## CalibrationSessionState

Represents the in-memory state of an active on-device calibration run.

### CalibrationSessionState Fields

| Field | Type | Description |
| --- | --- | --- |
| `activePointIndex` | Integer | Zero-based index of the target currently awaiting a valid sample. |
| `acceptedSamples` | List of CalibrationSample | Raw and mapped samples accepted so far. |
| `sessionStatus` | Enum | `collecting`, `rejected`, `readyToSave`, `saved`, or `canceled`. |
| `requiresRelease` | Boolean | Whether the user must fully release touch before the next point can arm. |
| `rejectionReason` | Enum or empty | `outsideBoundary`, `tooCloseToPrevious`, `duplicatePress`, `invalidFit`, or empty. |
| `returnTarget` | Enum | Destination after success or cancellation, `Home` or `Settings`. |

### State Transitions

- `collecting -> collecting`: A valid tap is accepted and the next target becomes active.
- `collecting -> rejected`: The candidate sample fails spatial or fit validation and the current target remains active.
- `collecting -> readyToSave`: All nine targets are accepted and a valid affine profile is computed.
- `readyToSave -> saved`: The profile is applied and persisted.
- `collecting -> canceled`: The user leaves before completion and no saved profile changes.
- `rejected -> collecting`: The user retries the same target after the rejection message or guard condition clears.

### CalibrationSessionState Validation Rules

- Only one target is active at a time.
- The current target's accepted capture boundary must not overlap any other target boundary in the same session.
- A second touch from the same intended press must not advance to the next point.
- A rejected sample must never mutate the active saved profile.

## CalibrationSample

Represents one accepted calibration point.

### CalibrationSample Fields

| Field | Type | Description |
| --- | --- | --- |
| `targetLabel` | String | User-visible calibration point label such as `Top Left`. |
| `targetX` | Integer | Expected screen X for the target. |
| `targetY` | Integer | Expected screen Y for the target. |
| `mappedX` | Integer | Current mapped touch X at capture time. |
| `mappedY` | Integer | Current mapped touch Y at capture time. |
| `rawX` | Integer | Raw controller X at capture time. |
| `rawY` | Integer | Raw controller Y at capture time. |
| `rawZ` | Integer | Raw controller pressure at capture time. |

### CalibrationSample Validation Rules

- `mappedX` and `mappedY` must land within the active target boundary before the sample is accepted.
- The raw sample must be sufficiently distinct from previously accepted raw samples for other targets.
- Samples are accepted in the fixed nine-point order defined by the calibration screen.

## ResetDefaultsSelection

Represents the in-memory state of the `Reset to defaults` settings flow.

### ResetDefaultsSelection Fields

| Field | Type | Description |
| --- | --- | --- |
| `selectedType` | Enum | The settings type chosen for reset: `wifi`, `receiver`, or `calibration`. |
| `confirmationState` | Enum | `choosingType`, `confirming`, `applied`, or `canceled`. |
| `returnTarget` | Enum | Planned destination after completion. Planned value is `Settings`. |

### ResetDefaultsSelection Validation Rules

- `Reset to defaults` appears as the last settings destination.
- Resetting `calibration` clears only `touchCalibration` and reapplies the shipped default profile immediately.
- Resetting `wifi` clears Wi-Fi credentials only.
- Resetting `receiver` clears the saved receiver IP only.
- `Cancel` from the confirmation step leaves all persisted values unchanged.
