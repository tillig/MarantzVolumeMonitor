# Contract: Validation

## Static Validation

Run from the repository root:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

## Documentation Validation

Review `docs/hardware.md` and confirm it includes:

1. Canonical circuit with Pololu item `2810`.
2. BOM with quantities, part identifiers, links, and equivalent minimum ratings.
3. Connection table for USB input, ESP32 `5V`/`VIN`, ground, TFT `VCC`, TFT `LED/BL`, ESP32 GPIO13,
   and Pololu `VIN`/`VOUT`/`ON`/`GND` pins.
4. Warning not to drive backlight current directly from ESP32 GPIO.
5. Warning not to switch TFT `VCC`.
6. Software-only fallback description for unmodified hardware.
7. Last-resort display-module modification limits.
8. 2N7000FS-ND / onsemi 2N7000 documented as evaluated but rejected for the planned switch.
9. Validation steps matching every documented connection.
10. Explicit instruction that USB power enters the ESP32 and TFT/touch plus switch-load power comes
    from ESP32 power pins, not a separate direct external supply.
11. Pass/fail current-budget check for the ESP32-fed display/touch and switch-load power path.

## Hardware Verification Matrix

Verify on the 480x320 touchscreen with a saved receiver target.

### Software-Only Wiring

1. Leave GPIO13 unconnected.
2. Keep the existing USB-to-ESP32 power topology and ESP32-fed TFT/touch power wiring.
3. Build and flash the firmware.
4. Confirm setup, Settings, receiver unavailable, and live display remain visible.
5. Confirm receiver-off flow still shows `Receiver off`, then software-blanks after 3 seconds.
6. Confirm touch wakes the blank state.
7. Confirm active receiver status restores live display.
8. Restart the monitor while the receiver is already off and confirm software blanking, touch wake,
   and Settings access remain recoverable.

### Hardware-Capable Wiring

1. Inspect wiring with power disconnected.
2. Confirm USB power enters through the ESP32 and there is no separate direct external supply wired
   to TFT/touch hardware or the Pololu `VIN` pads.
3. Confirm TFT `VCC` is still connected to ESP32 `5V`/`VIN` and not routed through the switch.
4. Confirm a Pololu `VIN` pad is connected to ESP32 `5V`/`VIN`.
5. Confirm TFT `LED/BL` is routed through a Pololu `VOUT` pad.
6. Confirm GPIO13 is connected to Pololu `ON` and the optional switch-contact pads are left open.
7. Confirm ESP32, TFT, and Pololu share ground.
8. Confirm the documented current-budget check passes for the ESP32-fed power path.
9. Power on with receiver active or setup visible and confirm the display is readable.
10. Turn receiver off while it remains reachable and confirm `Receiver off` appears.
11. Confirm the backlight turns off after 3 seconds, +/-0.5 seconds.
12. Confirm the room-distance display has no visible backlight glow at 15 feet.
13. Tap the dark display and confirm the backlight turns on and wakes to the current visible state.
14. Confirm the wake tap does not also open Settings.
15. While blanked, turn the receiver active and confirm live volume returns without reboot.
16. While blanked, make receiver-off confirmation unavailable, then tap the dark display and confirm
    the monitor shows the latest current visible state rather than assuming the receiver is still off.
17. Restart the monitor after receiver-off hardware display-off and confirm it remains recoverable
    without a forced power disconnect.
18. Open Settings while receiver is off and confirm Settings remains visible for at least 60 seconds.

## Pass Criteria

- Backlight off occurs only after confirmed receiver-off status and timeout expiration.
- Unavailable, unknown, Wi-Fi disconnected, and setup-required states do not turn off the backlight.
- Backlight on/off transitions occur within 0.5 seconds of receiver-off display state transitions.
- Touch wake and receiver polling continue while the backlight is off.
- Settings and settings-launched setup screens remain visible while active.
- Hardware-capable and software-only installations both remain recoverable without forced power
  disconnect.
- Hardware-capable validation preserves the single USB-to-ESP32 power entry and ESP32-fed display,
  touch, and switch-load power path.
