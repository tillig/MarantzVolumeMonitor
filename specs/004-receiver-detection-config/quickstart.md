# Quickstart: Receiver Detection and Configuration

## Prerequisites

- ESP32 firmware builds and uploads with PlatformIO.
- Device has valid Wi-Fi credentials and can join the same local network as the receiver.
- A compatible Marantz/Denon receiver is powered on and reachable on the local network.

## Automatic Discovery Scenario

1. Flash firmware with `pio run -t upload`.
2. Ensure Wi-Fi setup is complete and `receiverIp` is absent from `config.json`.
3. Boot the device and wait for Wi-Fi connected state.
4. If no receiver is configured, tap the Home Screen to open receiver setup. If a receiver is already configured, tap the top-right gear icon, choose Receiver Setup, and return to Home with OK when testing is complete.
5. Confirm the screen shows discovery progress.
6. Confirm discovered receiver rows show receiver name and IPv4 address.
7. Select the target receiver.
8. Confirm verification shows progress, succeeds only after live receiver status is returned, saves `receiverIp`, and returns to the expected screen: Home for first-run setup or Settings when launched from Settings.
9. Reboot and confirm the saved receiver is reused without repeating setup.

## Manual IPv4 Scenario

1. Open Settings from the Home Screen gear icon, choose Receiver Setup, and choose manual entry.
2. Enter a malformed value such as `receiver.local` or `192.168.1`.
3. Confirm the UI rejects the value before verification.
4. Enter a valid reachable receiver IPv4 address.
5. Confirm verification succeeds, saves `receiverIp`, and returns to Settings.

## Failure and Recovery Scenario

1. Start with a saved working receiver IP.
2. Power off or disconnect the receiver.
3. Confirm the device shows an offline or disconnected state without clearing the saved receiver IP.
4. Open Settings from the Home Screen gear icon, choose Receiver Setup, and enter an unreachable replacement IP.
5. Confirm verification fails and the previous saved receiver IP remains unchanged.
6. Restore the receiver and confirm normal status polling resumes.

## Validation Commands

```bash
pre-commit run --all-files
pio run
```

## Hardware Notes

- Discovery depends on local-network multicast/UPnP behavior. Some routers may block multicast between wireless clients.
- The test network should use the same subnet for the ESP32 and receiver.
- If a Marantz/Denon receiver is expected to appear while in standby, enable its network/IP control standby mode. On many receivers this is named "Network Control" or "IP Control" and must be set to an always-on behavior.
- If discovery finds no receiver while the receiver is powered on, use the serial log to confirm SSDP responses are received and whether their UPnP device description identifies the receiver.
