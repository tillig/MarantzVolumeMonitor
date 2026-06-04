# Contract: config.json Schema

## Description

Defines the receiver-related configuration persisted in LittleFS through `ConfigStore`.

## Schema

```json
{
  "brightness": "integer (0-255)",
  "receiverIp": "string",
  "useDbScale": "boolean",
  "wifiPassword": "string",
  "wifiSsid": "string"
}
```

## Receiver Validation Rules

- `receiverIp` is optional until receiver setup succeeds.
- `receiverIp` must be an IPv4 address when present.
- `receiverIp` must be written only after live receiver-status verification succeeds.
- If replacement verification fails, the previously saved `receiverIp` must remain unchanged.
- Temporary receiver offline state must not clear `receiverIp`.

## Compatibility

- Existing configurations without `receiverIp` remain valid and should route the user into receiver setup after Wi-Fi connects.
- Existing configurations with `receiverIp` should continue to load without migration.
