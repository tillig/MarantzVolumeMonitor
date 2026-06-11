# Quickstart: Receiver Off Hardware Handling

## Scope

Implement hardware backlight handling for the existing receiver-off screen sleep flow. The feature
controls only the TFT `LED/BL` backlight/illumination path; it must not switch TFT `VCC`, touch power,
ESP32 power, or receiver power. Preserve the current power topology: the USB 5V 2A supply plugs into
the ESP32, and TFT/touch plus backlight-switch load power comes from ESP32 power pins rather than a
separate direct supply.

## References

- Feature spec: `specs/009-receiver-off-hardware/spec.md`
- Plan: `specs/009-receiver-off-hardware/plan.md`
- Contracts:
  - `specs/009-receiver-off-hardware/contracts/backlight_control_contract.md`
  - `specs/009-receiver-off-hardware/contracts/hardware_upgrade_guide_contract.md`
  - `specs/009-receiver-off-hardware/contracts/validation_contract.md`
- Durable hardware reference: `docs/hardware.md`
- Durable UI reference: `docs/ui-reference.md`

## Implementation Outline

1. Review the existing receiver-off visible/blank state in `HomeScreen`.
2. Add a tracked backlight-control pin definition for GPIO13 if not already defined.
3. Add `DisplayManager` behavior to initialize the pin as an output and default the backlight on.
4. Add a `DisplayManager` API to set the backlight on or off.
5. Call the API from `HomeScreen` so all visible states drive backlight on and only
   `ReceiverOffBlank` drives backlight off.
6. Preserve wake-only touch behavior from the blank state.
7. Preserve receiver polling while blanked.
8. Do not add Settings UI, persistent config, receiver protocol changes, or separate firmware builds.
9. Update `docs/hardware.md` with canonical circuit, BOM, wiring, safety warnings, and validation.
10. Update `docs/ui-reference.md` and `README.md` with concise hardware-capable behavior notes.

## Canonical Hardware Direction

Use a high-side backlight switch on the TFT `LED/BL` feed:

```text
USB 5V 2A supply ----> ESP32 USB

ESP32 5V/VIN pin ----+----> TFT VCC
                     |
                     +----> Pololu 2810 VIN

Pololu 2810 VOUT ----> TFT LED/BL

ESP32 GPIO13 ---------------> Pololu 2810 ON

ESP32 GND -------------------+----> TFT GND
                             |
                             +----> Pololu 2810 GND
```

Keep this as reversible inline wiring. If the installed display cannot expose the backlight path
without module modification, document the modification as last-resort work only.

The duplicated `VIN`, `VOUT`, and `GND` pads on the Pololu board are electrically common; use any
matching pad positions that make the wiring clean. Leave the optional switch-contact pads open for the
canonical ESP32-controlled wiring.

Do not add a separate external power feed to the TFT/touch assembly or Pololu `VIN` pads. The
hardware guide must include a current-budget check showing the ESP32 USB input and `5V`/`VIN` output
path can support the existing TFT/touch load plus the switched backlight load.

Do not use `2N7000FS-ND` / onsemi `2N7000` as the planned backlight switch. It is useful to mention
in the hardware documentation only as an evaluated-but-rejected part: it is an N-channel low-side
small-signal MOSFET, not the canonical high-side switch, and its current/on-resistance margins are
not appropriate for a trusted unknown TFT backlight load.

## Validation

Run static validation:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

Run hardware verification:

1. With GPIO13 unconnected, confirm software-only receiver-off blanking and touch wake still work.
2. Wire the canonical backlight circuit with power disconnected.
3. Inspect that USB power enters through the ESP32 and no separate external supply feeds TFT/touch or
   the Pololu `VIN` pads directly.
4. Inspect that TFT `VCC` remains powered from ESP32 `5V`/`VIN`, the Pololu `VIN` pad is fed from the
   same ESP32 power pin, and only `LED/BL` is switched through `VOUT`.
5. Confirm GPIO13 is wired to the Pololu `ON` pin and shared ground exists across ESP32, TFT, and the
   Pololu board.
6. Confirm the ESP32-fed current-budget check passes before accepting the wiring.
7. Power on and confirm setup, Settings, receiver unavailable, and live states are visible.
8. Turn the receiver off while reachable and confirm `Receiver off` appears.
9. Confirm the backlight turns off after 3 seconds, +/-0.5 seconds.
10. Tap the dark display and confirm the backlight turns on and the wake tap opens no second action.
11. Turn the receiver active while blanked and confirm live volume returns without reboot.
12. While blanked, make receiver-off confirmation unavailable, then tap the dark display and confirm
    the monitor shows the latest current visible state rather than assuming the receiver is still off.
13. Restart the monitor while the receiver is already off and confirm it remains recoverable without a
    forced power disconnect.
14. Open Settings while receiver is off and confirm it remains visible for at least 60 seconds.
