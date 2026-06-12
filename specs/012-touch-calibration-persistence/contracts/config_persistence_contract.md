# Contract: Configuration Persistence

## Purpose

Define how persisted calibration data and selective resets behave inside `config.json`.

## Config Shape Rules

- `config.json` continues to store existing persisted settings for `wifiSsid`, `wifiPassword`, `receiverIp`, `brightness`, and `useDbScale`.
- The file may also contain an optional `touchCalibration` object representing a saved user calibration profile.
- Missing `touchCalibration` means the device uses the shipped default calibration profile.

## Load Rules

- `ConfigStore` loads the rest of the config even if `touchCalibration` is missing.
- If `touchCalibration` is present but unreadable or unusable, the device ignores it and falls back to the shipped default calibration profile.
- Startup applies the active calibration profile before normal touch interaction is relied upon.

## Save Rules

- Successful calibration overwrites only the persisted `touchCalibration` section and keeps unrelated saved settings intact.
- Saving any other settings continues to preserve the most recent valid calibration profile.
- Resetting `calibration` removes the saved `touchCalibration` section and reapplies the shipped default profile immediately.

## Selective Reset Rules

- Resetting `wifi` clears `wifiSsid` and `wifiPassword` only.
- Resetting `receiver` clears `receiverIp` only.
- Resetting `calibration` clears only the saved calibration section.
- No selective reset operation may mutate unrelated settings fields.

## Acceptance Rules

- A saved calibration survives reboot.
- Clearing calibration does not clear Wi-Fi or receiver settings.
- Clearing Wi-Fi or receiver settings does not change the active calibration profile.
