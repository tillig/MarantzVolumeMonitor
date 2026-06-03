# Contract: config.json Schema

## Description

Defines the structure of the JSON configuration file stored in LittleFS.

## Schema

```json
{
  "wifiSsid": "string",
  "wifiPassword": "string",
  "receiverIp": "string",
  "brightness": "integer (0-255)",
  "useDbScale": "boolean"
}
```

## Validation Rules

- `wifiSsid`: Mandatory for connection. Max 32 chars.
- `wifiPassword`: Mandatory for WPA2. Min 8, Max 63 chars.
- `receiverIp`: IPv4 format (e.g., 192.168.1.50).
- `brightness`: Default 255.
- `useDbScale`: Default false.
