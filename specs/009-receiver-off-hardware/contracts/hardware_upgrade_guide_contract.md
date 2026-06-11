# Contract: Hardware Upgrade Guide

## Purpose

Define the required durable content for `docs/hardware.md` so the hardware portion of this feature is reproducible and safe.

## Required Bill Of Materials

The guide must include:

- Canonical switch module: Pololu Mini MOSFET Slide Switch LV, item `2810`.
- Hookup wire or jumper wires suitable for the installed enclosure and display.
- Optional connectors or terminal blocks if needed for reversible inline wiring.
- Multimeter for continuity and voltage validation.
- Current-budget worksheet or checklist for the existing USB-to-ESP32 power path and ESP32 power pin feeding the display/touch and switch load.
- Equivalent-substitution minimum ratings: 5V load support, measured backlight current support with margin, 3.3V logic-compatible control, and shared-ground compatibility.
- Conditional low-side substitute: Adafruit STEMMA MOSFET Driver, item `5648`, only when the display exposes an isolated backlight return.
- Explicit non-recommendation: `2N7000FS-ND` / onsemi `2N7000` must not be listed as a recommended backlight switch for this feature because it is a small-signal N-channel low-side MOSFET with insufficient margin for the canonical high-side `LED/BL` feed.

## Required Connection Map

The guide must include a connection table covering:

- USB 5V 2A supply plugged into the ESP32 USB power input.
- ESP32 `5V`/`VIN` power pin to TFT `VCC`.
- ESP32 `5V`/`VIN` power pin to Pololu `VIN`.
- Pololu `VOUT` to TFT `LED/BL`.
- Shared ground between ESP32 `GND`, TFT `GND`, and switch module `GND`.
- ESP32 GPIO13 to Pololu `ON`.
- Explicit instruction that TFT `VCC` is not switched.
- Explicit instruction not to add a separate direct external power feed to TFT/touch hardware or the Pololu `VIN` pads.
- Explicit instruction that duplicated `VIN`, `VOUT`, and `GND` pads are common and the optional switch-contact pads are left open in the canonical wiring.

## Required Safety Warnings

The guide must warn that:

- ESP32 GPIO must not directly drive the backlight current.
- Small-signal MOSFETs such as 2N7000 must not be treated as drop-in substitutes for the canonical backlight switch unless a separate bench measurement proves the actual display input is only a low-current logic-enable signal.
- ESP32 GPIO is 3.3V logic and is not 5V tolerant.
- TFT `VCC` and touch-controller power stay connected.
- TFT/touch and switch-load power must come from ESP32 power pins after USB power enters the ESP32.
- Directly feeding the TFT/touch assembly or Pololu `VIN` pads from a separate external supply is out of scope for this guide.
- All components must share ground.
- Power must be disconnected before rewiring.
- Destructive display-module modification is last-resort only.

## Required Validation Steps

The guide must include:

- Visual wiring inspection before power.
- Continuity check for common ground.
- Check that USB power enters through the ESP32 and no separate external supply is wired directly to TFT/touch hardware or the Pololu `VIN` pads.
- Check that TFT `VCC` remains connected to ESP32 `5V`/`VIN`.
- Check that a Pololu `VIN` pad is connected to ESP32 `5V`/`VIN`.
- Check that only TFT `LED/BL` is routed through a Pololu `VOUT` pad.
- Check that GPIO13 is connected to Pololu `ON`.
- Check that the optional switch-contact pads are left open unless a manual switch is intentionally added outside the canonical design.
- Current-budget review for the ESP32-fed power path before first power-on acceptance.
- First power-on with receiver active or setup visible.
- Receiver-off visible timeout and backlight-off verification.
- Touch wake verification.
- Receiver-active restore verification.
- Software-only fallback verification with GPIO13 unconnected.
- Rollback or bypass instructions for the switch module.

## Acceptance Rules

- Every documented connection has at least one inspection or functional validation step.
- The canonical path is non-destructive inline wiring.
- The guide preserves one USB power entry through the ESP32 and does not document a separate direct display/touch supply as the planned path.
- Current-budget validation is required before the hardware upgrade is accepted.
- Last-resort module modification includes prerequisites, risks, and rollback limits.
- A maintainer can identify required parts and wiring changes in under 10 minutes from the guide.
