# Research: Receiver Off Hardware Handling

## Decision: Use A High-Side Backlight Switch As The Canonical Circuit

Route the TFT `LED/BL` positive backlight feed through a Pololu Mini MOSFET Slide Switch LV, item
`2810`, controlled by ESP32 GPIO13. Keep USB power entering through the ESP32, keep TFT `VCC`,
TFT `GND`, touch power, touch SPI, and ESP32 power wired through the existing ESP32-fed topology, and
source the switch load input from an ESP32 power pin rather than a separate direct supply.

Reference source: <https://www.pololu.com/product/2810>

**Rationale**: The existing hardware reference already separates TFT `VCC` from `LED/BL`, and the
feature explicitly requires backlight/illumination control only. A high-side switch supports
non-destructive inline wiring between 5V and `LED/BL`, which is more likely to be externally
accessible than an isolated backlight ground return. It also preserves the TFT controller and touch
controller state while the display appears off.

**Alternatives considered**:

- Full TFT `VCC` switching: rejected because it power-cycles display logic, risks display
  reinitialization problems, and conflicts with touch-wake recovery.
- Low-side backlight switching as the canonical path: rejected because many TFT modules do not expose
  an isolated backlight cathode/return externally. It remains acceptable only as a documented
  substitute when the module exposes the required return path.
- Driving `LED/BL` directly from ESP32 GPIO: rejected because the TFT backlight can draw far more
  current than a GPIO should provide.

## Decision: Preserve ESP32-Fed Display And Touch Power

Keep the current single-supply topology: the USB 5V 2A adapter plugs into the ESP32, and the display,
touch hardware, and backlight-switch load input are powered from ESP32 power pins such as `5V`/`VIN`
or `3V3` when a specific load requires 3.3V. Do not add a second power input or wire the TFT/touch
assembly or switch load input directly to an external supply.

**Rationale**: The installed device already works with the screen/touch powered from the ESP32. The
hardware upgrade should only add controllable backlight switching, not change the system power-entry
model or require a new power harness. Routing display/touch and switch-load power from ESP32 pins also
keeps one obvious disconnect point: unplugging USB from the ESP32 removes power from the monitor
hardware.

**Alternatives considered**:

- Split the 5V adapter output so one branch feeds the ESP32 and another branch feeds TFT/touch or the
  switch load input directly: rejected because it violates the requested power topology and creates a
  second direct power path to the display assembly.
- Add a separate power supply for the TFT/touch assembly: rejected because it complicates grounding,
  startup, and service without adding value for this feature.
- Default all display and switch power to ESP32 `3V3`: rejected because the current TFT/backlight path
  is 5V-powered; `3V3` should be used only for loads explicitly documented and validated as requiring
  3.3V.

## Decision: Document Adafruit 5648 Only As A Conditional Low-Side Substitute

Document Adafruit STEMMA MOSFET Driver, product `5648`, as a non-canonical substitute only when the
specific display module exposes an isolated backlight return that can be switched low-side without
affecting TFT logic, touch, or shared system ground.

Reference source: <https://www.adafruit.com/product/5648>

**Rationale**: The board is useful for logic-level low-side load switching, but making it canonical
would push maintainers toward cutting or modifying display-module internals if only the positive
`LED/BL` feed is exposed. That conflicts with the clarified preference for reversible inline wiring.

**Alternatives considered**:

- Treat Adafruit 5648 and Pololu 2810 as equal canonical options: rejected because the spec requires
  one canonical reference circuit.
- Omit substitutions entirely: rejected because maintainers may have a module with an externally
  accessible backlight return where a low-side driver is electrically appropriate.

## Decision: Do Not Use 2N7000FS-ND As The Backlight Switch

Do not use the user's existing Digi-Key `2N7000FS-ND` parts as the canonical backlight switch or as a
recommended substitute for this feature.

Reference sources:

- Digi-Key 2N7000FS-ND / onsemi 2N7000 product page:
  <https://www.digikey.com/en/products/detail/onsemi/2N7000/244278>
- onsemi 2N7000 product page:
  <https://www.onsemi.com/products/discrete-power-modules/mosfets/small-signal-mosfets/2n7000>

**Rationale**: The 2N7000FS-ND is an N-channel small-signal MOSFET, so it is a low-side switch part
and does not match the canonical non-destructive high-side switch on the TFT `LED/BL` positive feed.
It is also rated for 200 mA continuous drain current at ambient and has high on-resistance compared
with a power/load-switch module. Digi-Key lists 5 ohm maximum RDS(on) at 500 mA with 10V gate drive,
and the part is not specified as a robust 3.3V logic-level power switch for an unknown TFT backlight
load. That makes it unsuitable as the trusted hardware-guide recommendation.

**Alternatives considered**:

- Use 2N7000FS-ND as the canonical high-side switch: rejected because an N-channel low-side part
  cannot switch the positive `LED/BL` feed directly from a 3.3V ESP32 GPIO.
- Use 2N7000FS-ND as the conditional low-side substitute: rejected for the hardware guide because it
  would require an externally isolated backlight return, measured load current comfortably below the
  part limit, and acceptance of uncertain 3.3V gate-drive performance.
- Use 2N7000FS-ND only as an experimental bench test: acceptable outside the canonical guide only if
  the installed display exposes a low-current logic-enable `LED/BL` input or a measured isolated
  backlight return below the part rating. This does not satisfy the feature deliverable.

## Decision: Use GPIO13 As The Backlight-Control Signal

Use ESP32 GPIO13 as the documented backlight-control signal. Track the pin in source or
`platformio.ini` as `BACKLIGHT_CONTROL_PIN=13` so hardware guidance and firmware stay aligned.

**Rationale**: GPIO13 is not used by the current TFT SPI, touch SPI, receiver/network, or setup
wiring in `platformio.ini`, and it is physically closer to the existing display/power wiring than
GPIO32. It also avoids the ESP32 GPIO12 strapping-pin risk while keeping one documented pin in the
hardware guide and avoiding a Settings toggle or stored hardware capability flag.

**Alternatives considered**:

- Reuse an existing TFT/touch SPI pin: rejected because it would conflict with display or touch
  operation.
- Use GPIO12 because it is physically nearby: rejected because GPIO12 is an ESP32 strapping pin and
  is a weaker choice for a startup-sensitive control signal.
- Make the pin configurable in Settings: rejected because hardware capability is determined by wiring,
  not user preference.
- Use a separate firmware build per wiring style: rejected because unmodified hardware can safely
  ignore the unconnected signal.

## Decision: Firmware Always Drives The Signal

Initialize the backlight-control pin during display startup, drive it on for all visible states, and
drive it off only while Home is in receiver-off blank state. Do not add a stored configuration flag or
on-device Settings toggle.

**Rationale**: With no backlight circuit attached, the GPIO is simply unconnected and the existing
software blank behavior remains the fallback. With the circuit attached, the same firmware controls
the hardware backlight. This keeps one firmware path and avoids hidden setup state.

**Alternatives considered**:

- Build-time enable flag only: rejected because it creates two firmware behaviors for one documented
  device.
- Settings toggle: rejected because it adds persistent state and a user-facing choice for something
  that should be determined by wiring and validation.
- Auto-detect hardware: rejected because the proposed circuit does not provide feedback and detection
  would add unnecessary hardware complexity.

## Decision: Keep Backlight API In DisplayManager

Add a small `DisplayManager` API for enabling/disabling hardware display backlight. `HomeScreen`
continues to own receiver-off state transitions and calls `DisplayManager` when entering or leaving
receiver-off blank state.

**Rationale**: `HomeScreen` already implements the receiver-off visible/blank state machine and
Settings wake behavior. `DisplayManager` already owns TFT initialization and is the natural layer for
display hardware pins. This preserves the constitution's layered separation.

**Alternatives considered**:

- Put GPIO calls directly in `HomeScreen`: rejected because it couples screen behavior to hardware
  control details.
- Add a global power manager: rejected because the feature only controls one display backlight signal.
- Add hardware control to `main.cpp`: rejected because the main loop should remain touch/update
  dispatch, not screen-state logic.

## Decision: Prefer Non-Destructive Inline Wiring

Document the canonical circuit as reversible inline wiring between the 5V backlight feed and TFT
`LED/BL`. Any display-module trace cutting, jumper removal, or onboard-part modification is
last-resort guidance only when the module cannot expose the backlight path externally.

**Rationale**: The user needs trustworthy hardware instructions that minimize risk. Inline wiring can
be inspected, bypassed, and reversed. Destructive module modifications are harder to verify and can
permanently damage the display.

**Alternatives considered**:

- Treat module modification as a normal path: rejected because it increases risk and reduces
  maintainability.
- Forbid all module modification: rejected because some display variants may tie backlight power in a
  way that cannot be controlled externally.

## Decision: Expand Hardware Documentation Before Feature Completion

Make `docs/hardware.md` a deliverable, not a follow-up. The guide must include BOM, canonical
connections, equivalent-part ratings, unsafe wiring warnings, first-power-on checks, receiver-off
hardware validation, software-only fallback validation, and rollback guidance.

**Rationale**: This feature changes physical wiring. Without durable, specific documentation, the
firmware behavior cannot be safely reproduced or maintained.

**Alternatives considered**:

- Keep wiring notes only in `specs/009-receiver-off-hardware`: rejected because durable hardware
  references belong under `docs/`.
- Put detailed wiring only in source comments: rejected because the maintainer needs a build guide
  before editing or flashing firmware.
