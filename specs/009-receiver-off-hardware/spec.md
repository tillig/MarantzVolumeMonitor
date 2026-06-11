# Feature Specification: Receiver Off Hardware Handling

**Feature Branch**: `feature/009-receiver-off-hardware`

**Created**: 2026-06-05

**Status**: Draft

**Input**: User description: "Enable hardware handling for receiver off state"

## Clarifications

### Session 2026-06-05

- Q: Should hardware documentation require one canonical backlight-control reference circuit, or
  document several options equally? -> A: Require one canonical reference circuit; equivalent
  substitutions are allowed only if they meet documented ratings.
- Q: Should hardware handling switch only the display backlight/illumination path, or the entire TFT
  module power rail? -> A: Switch only the display backlight/illumination path; TFT logic and touch
  remain powered.
- Q: How should software-only or unmodified hardware handle the backlight-control signal? -> A:
  Firmware always drives the documented control signal; hardware-capable wiring responds, while
  unmodified hardware ignores the unconnected signal and keeps software blank fallback behavior.
- Q: Should the hardware guide prefer non-destructive inline wiring, allow normal display-module
  modification, or forbid module modification entirely? -> A: Prefer non-destructive inline wiring;
  destructive display-module modification is a last resort only.

### Session 2026-06-06

- Q: Should the hardware upgrade preserve the current USB-powered ESP32 power path for the
  screen/touch? -> A: Yes. USB power continues to enter through the ESP32, and screen/touch plus
  backlight-switch load power must come from ESP32 power pins, not a separate direct supply.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Physically Quiet The Display When Receiver Is Off (Priority: P1)

As a person in the room, I want the monitor to physically turn off display backlight/illumination
after the receiver is confirmed off, so the device does not leave a visible glow or status message in
the room when the audio system is not in use.

**Why this priority**: The software blanking behavior removes content, but hardware illumination can
still create distraction. The receiver-off idle state should be visually quiet at the appliance level.

**Independent Test**: With receiver-off screen sleep already working and hardware display control
available, turn the receiver off and observe the monitor from normal room distance. The story is
complete when the monitor first confirms `Receiver off`, then physical display backlight/illumination
turns off within the expected timeout.

**Acceptance Scenarios**:

1. **Given** the saved receiver is confirmed powered off and hardware display control is available,
   **When** the receiver-off timeout expires, **Then** the monitor turns off physical display
   backlight/illumination rather than only drawing a black screen.
2. **Given** the monitor has turned off physical display backlight/illumination because the receiver
   is off, **When** the receiver remains off and the user does nothing, **Then** no visible glow,
   stale volume, or persistent status message remains visible from normal room distance.
3. **Given** the receiver is active, unavailable, unknown, or requires setup, **When** the home
   display refreshes, **Then** the monitor does not turn off physical display backlight/illumination
   as if the receiver were confirmed off.

---

### User Story 2 - Wake Reliably From Hardware-Off Display State (Priority: P2)

As a user, I want a touch to wake the monitor after physical display backlight/illumination has
turned off, so I can confirm the device is still running and reach Settings without rebooting or
unplugging it.

**Why this priority**: Hardware display-off behavior is only acceptable if the appliance remains
recoverable through the same touch-first interaction model.

**Independent Test**: Allow the monitor to enter the receiver-off hardware display-off state, then tap
the screen. The story is complete when the first tap restores visible output without triggering a
second action, and a later visible tap can open Settings.

**Acceptance Scenarios**:

1. **Given** physical display backlight/illumination is off because the receiver is confirmed off,
   **When** the user taps the screen, **Then** the monitor restores visible output and shows the
   current receiver state.
2. **Given** the wake tap restored visible output while the receiver is still off, **When** the user
   wants to change configuration, **Then** Settings is available from the visible receiver-off screen.
3. **Given** the monitor was woken from the hardware display-off state, **When** the receiver remains
   off and the user does not open Settings, **Then** the receiver-off timeout can turn physical
   display backlight/illumination off again.

---

### User Story 3 - Preserve Safe Operation When Hardware Control Is Unavailable (Priority: P3)

As an installer or maintainer, I want the monitor to remain safe and usable when hardware display
control is not wired, so a software-only or unmodified installation does not break setup, touch, or
receiver monitoring.

**Why this priority**: Hardware display control depends on the installed wiring. The feature must
degrade predictably instead of assuming every device has the same physical control capability.

**Independent Test**: Run the receiver-off flow on a monitor without hardware display control
available. The story is complete when the existing software-blank receiver-off behavior still works,
and the device remains touch-wakeable and configurable.

**Acceptance Scenarios**:

1. **Given** hardware display control is not wired, **When** the receiver-off timeout expires, **Then**
   the monitor uses the existing visual blanking behavior and remains responsive.
2. **Given** hardware display control is not wired, **When** the user touches the blanked monitor,
   **Then** the monitor wakes to the same visible state it would show on hardware-capable devices.
3. **Given** hardware display control is not wired, **When** the receiver becomes active, **Then** the
   monitor returns to the normal live display without requiring user setup, stored configuration, or
   reboot.

---

### User Story 4 - Keep Configuration And Recovery Visible (Priority: P4)

As a user working in Settings, I want the monitor to keep the display backlight/illumination on while
I configure Wi-Fi, receiver setup, or touch calibration, so receiver-off hardware handling does not
interrupt recovery work.

**Why this priority**: Settings is the on-device recovery path. Hardware display-off behavior must not
make setup screens disappear while the user is actively working.

**Independent Test**: Wake the monitor from receiver-off hardware display-off state, open Settings,
and remain in Settings beyond the receiver-off timeout. The story is complete when Settings stays
visible and hardware display-off handling resumes only after returning Home while the receiver is
still off.

**Acceptance Scenarios**:

1. **Given** the user is in Settings while the receiver is still off, **When** more than the
   receiver-off timeout passes, **Then** physical display backlight/illumination remains on and the
   Settings screen remains usable.
2. **Given** the user exits Settings while the receiver is still confirmed off, **When** Home is shown,
   **Then** the monitor shows `Receiver off` before physical display backlight/illumination turns off
   again.
3. **Given** the user exits Settings after the receiver becomes active, **When** fresh active status is
   available, **Then** the normal live display appears and physical display backlight/illumination
   remains on.

---

### User Story 5 - Follow A Specific Hardware Upgrade Guide (Priority: P5)

As the device builder, I want the hardware reference documentation to tell me exactly what parts to
buy, what connections to make, and how to validate the wiring safely, so I can perform the physical
display-control upgrade without guessing or relying on scattered notes.

**Why this priority**: The feature changes hardware behavior. A working firmware change is not
deliverable unless the builder has clear, durable, project-specific guidance for making the required
hardware change safely.

**Independent Test**: Review the hardware guide before touching the device. The story is complete
when the guide identifies one canonical non-destructive reference circuit, required parts, acceptable
substitutions, wiring points, safety constraints, validation steps, last-resort modification limits,
and rollback behavior clearly enough for a maintainer to execute the upgrade.

**Acceptance Scenarios**:

1. **Given** the maintainer is preparing to add hardware display control, **When** they read the
   hardware guide, **Then** they can identify the canonical reference circuit, recommended parts,
   quantities, ratings, and source links or part identifiers needed for the upgrade.
2. **Given** the maintainer is wiring the hardware display-control circuit, **When** they follow the
   hardware guide, **Then** the required connections, shared grounds, display backlight/control line,
   monitor control signal, and ESP32-fed power path are all specified without ambiguous pin names.
3. **Given** the maintainer has completed wiring, **When** they follow the validation section, **Then**
   they can verify safe off/on operation, touch wake, receiver polling, and software fallback behavior
   before treating the upgrade as complete.
4. **Given** the canonical inline wiring path is not possible on the specific display module, **When**
   the maintainer reviews the hardware guide, **Then** any destructive display-module modification is
   identified as last-resort work with explicit prerequisites, risks, and rollback limits.

### Edge Cases

- The monitor has hardware display control available, but the receiver becomes active while physical
  display backlight/illumination is off.
- The monitor has hardware display control available, but receiver-off confirmation is lost after
  physical display backlight/illumination has already turned off.
- The receiver is unreachable before any confirmed receiver-off status is observed.
- Wi-Fi or receiver setup is required while the display would otherwise be eligible for receiver-off
  handling.
- The user repeatedly taps the display while physical display backlight/illumination is off or during
  wake.
- The user opens Settings immediately after waking from hardware display-off state.
- The user remains in Settings longer than the receiver-off timeout.
- Hardware display control is not wired or not verified on the installed device.
- Hardware display control fails to change illumination state during receiver-off idle.
- The monitor restarts while the receiver is already off.
- The display is physically off while touch handling must remain available.
- A maintainer has an older software-only build or device wiring and needs to determine whether the
  hardware upgrade has been completed.
- A recommended part is unavailable and a maintainer needs minimum electrical ratings for selecting
  an equivalent.
- The display module cannot expose the backlight path through normal external pins or inline wiring.
- A maintainer assumes the TFT, touch, or backlight switch should be powered directly from a separate
  supply instead of through the ESP32 power pins.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST attempt physical display backlight/illumination off only after current
  receiver status confirms the saved receiver is powered off and the receiver-off visible timeout has
  expired.
- **FR-002**: The system MUST show `Receiver off` before turning off physical display
  backlight/illumination.
- **FR-003**: When hardware display control is available, the receiver-off display-off state MUST turn
  off physical display backlight/illumination rather than relying only on a visually black screen.
- **FR-004**: While physical display backlight/illumination is off because the receiver is off, the
  system MUST continue monitoring receiver status.
- **FR-005**: While physical display backlight/illumination is off because the receiver is off, a
  touch MUST restore visible output.
- **FR-006**: A wake touch from the hardware display-off state MUST NOT by itself open Settings or
  trigger any setup action; Settings access MUST require a separate visible action.
- **FR-007**: When the receiver becomes active while physical display backlight/illumination is off,
  the system MUST restore visible output and show the normal live display without requiring user setup
  or reboot.
- **FR-008**: If receiver-off confirmation is lost after physical display backlight/illumination has
  already turned off, the system MUST keep the display quiet until user touch or fresh active receiver
  status becomes available.
- **FR-009**: When the user wakes the monitor after receiver-off confirmation is lost, the system MUST
  show the latest current visible state rather than assuming the receiver is still off.
- **FR-010**: The system MUST NOT turn off physical display backlight/illumination solely because of
  missing volume data, failed status refresh, network timeout, missing receiver configuration, missing
  Wi-Fi connection, or unknown receiver power state.
- **FR-011**: When hardware display control is not wired or not verified, the system MUST preserve the
  existing receiver-off software blanking and touch-wake behavior.
- **FR-012**: Hardware display control availability MUST be externally verifiable by maintainers so
  hardware-capable and software-only installations can be distinguished during setup or validation.
- **FR-013**: The system MUST keep Settings and settings-launched setup screens visible while active,
  even if the receiver remains off beyond the receiver-off timeout.
- **FR-014**: Hardware display-off handling MUST preserve existing Wi-Fi setup, receiver setup,
  Settings, touch calibration, and live volume display behavior.
- **FR-015**: Hardware display-off handling MUST NOT add user-facing receiver power control.
- **FR-016**: Hardware display-off handling MUST avoid unsafe or unsupported display power behavior
  that prevents touch wake, blocks receiver monitoring, or requires a power cycle for recovery.
- **FR-017**: The hardware reference documentation MUST be updated as part of this feature before the
  feature is considered complete.
- **FR-018**: The hardware guide MUST include a bill of materials with recommended parts, quantities,
  minimum electrical ratings, and part numbers or source links where practical.
- **FR-019**: The hardware guide MUST include an explicit connection map covering display power,
  backlight or illumination control, monitor control signal, and shared ground.
- **FR-020**: The hardware guide MUST warn against unsafe wiring, including direct high-current
  display illumination drive from a monitor control signal.
- **FR-021**: The hardware guide MUST include validation steps for wiring inspection, first power-on,
  receiver-off display-off behavior, touch wake, receiver-active restore, and software-only fallback.
- **FR-022**: The hardware guide MUST document how maintainers can identify whether a device is
  hardware-control capable or should remain in software-blank fallback mode.
- **FR-023**: The hardware guide MUST define one canonical reference circuit for the hardware upgrade;
  substitute parts or circuit modules MAY be documented only when they meet the same minimum ratings
  and preserve the same user-visible behavior.
- **FR-024**: The hardware upgrade MUST control only the display backlight/illumination path and MUST
  keep TFT logic power, touch-controller power, and monitor power connected during receiver-off idle.
- **FR-025**: The system MUST drive the documented backlight-control signal during receiver-off
  display-off and wake transitions without requiring an on-device Settings toggle, stored preference,
  or separate software build for hardware-capable installations.
- **FR-026**: Unmodified or software-only installations MUST remain safe and usable when the documented
  backlight-control signal is unconnected.
- **FR-027**: The canonical hardware guide MUST prefer non-destructive inline wiring that can be
  inspected, reversed, or bypassed without modifying the display module itself.
- **FR-028**: Destructive display-module modification MUST be documented only as a last-resort path
  when non-destructive inline wiring cannot safely expose the backlight path; any such guidance MUST
  state prerequisites, risks, and rollback limits.
- **FR-029**: The hardware upgrade MUST preserve the existing single USB power input to the ESP32 and
  MUST power the TFT, touch hardware, and backlight-switch load path from ESP32 power pins rather than
  wiring them directly to a separate external power supply.
- **FR-030**: The hardware guide MUST identify which ESP32 power pin supplies each display/touch or
  switch power connection and MUST include current-budget validation before accepting the wiring.

### Key Entities *(include if feature involves data)*

- **Hardware Display-Off State**: The receiver-off idle state where physical display
  backlight/illumination is turned off while the monitor remains powered, touch-wakeable, and able to
  monitor receiver status.
- **Hardware Display Control Availability**: The installation capability determined by whether the
  documented backlight-control circuit is wired and verified.
- **Wake Interaction**: A user touch while physical display backlight/illumination is off that
  restores visible output without activating another control in the same gesture.
- **Receiver Power State**: The current user-visible receiver condition that gates whether hardware
  display-off behavior is allowed.
- **Settings Session**: Any configuration or calibration flow that must remain visible regardless of
  receiver-off hardware handling.
- **Hardware Upgrade Guide**: The durable hardware documentation that explains required parts,
  non-destructive wiring, last-resort modification limits, safety limits, validation steps, and
  fallback behavior for receiver-off physical display control.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: In hardware-capable verification, 100% of confirmed receiver-off cases show
  `Receiver off` before physical display backlight/illumination turns off.
- **SC-002**: In hardware-capable verification, physical display backlight/illumination turns off
  within 0.5 seconds of the receiver-off timeout expiring in at least 95% of attempts.
- **SC-003**: In room-distance visual review, hardware-capable receiver-off idle leaves no visible
  display glow, status message, or stale live-volume content from 15 feet in 100% of reviewed cases.
- **SC-004**: In interaction testing, 100% of touches while physical display backlight/illumination is
  off restore visible output without opening Settings or triggering another action from the same
  touch.
- **SC-005**: In receiver power-transition testing, 100% of transitions from hardware display-off
  receiver-off idle to fresh active receiver status restore the normal live display without reboot.
- **SC-006**: In negative-case testing, 100% of unreachable, setup-required, unknown-power-state, and
  missing-configuration states avoid physical display-off behavior unless receiver-off status was
  previously confirmed and already quieted.
- **SC-007**: In software-only installation testing with the documented backlight-control signal
  unconnected, 100% of receiver-off flows preserve the existing visual blanking and touch-wake
  behavior.
- **SC-008**: In Settings testing, Settings and settings-launched setup screens remain visible and
  usable for at least 60 seconds while the receiver remains off.
- **SC-009**: In maintainer validation, hardware display control availability can be confirmed or
  ruled out without changing receiver configuration in 100% of tested devices.
- **SC-010**: In recovery testing, 100% of hardware display-off attempts leave the monitor
  recoverable through touch, active receiver status, or normal restart without requiring a forced
  power disconnect.
- **SC-011**: In documentation review, the hardware guide contains a complete bill of materials,
  canonical reference circuit, connection map, safety warnings, validation steps, and fallback
  guidance before implementation is accepted.
- **SC-012**: In maintainer review, a person with the existing hardware reference can identify the
  required parts and wiring changes in under 10 minutes without consulting source code.
- **SC-013**: In validation review, every documented hardware connection has a corresponding
  inspection or functional test step.
- **SC-014**: In documentation review, the canonical hardware path can be completed without
  destructive display-module modification unless the specific module lacks a safe externally
  accessible backlight path.
- **SC-015**: In documentation review, any last-resort destructive display-module modification
  includes explicit prerequisites, risks, and rollback limits before it is accepted.
- **SC-016**: In documentation and hardware review, 100% of display/touch and backlight-switch load
  power connections originate from ESP32 power pins after USB power enters the ESP32.
- **SC-017**: In validation review, the hardware guide includes a pass/fail current-budget check for
  the ESP32-fed power path before the upgrade is accepted.

## Assumptions

- This feature is the hardware follow-up to the existing receiver-off screen sleep behavior; it does
  not replace the visible `Receiver off`, timeout, wake-only tap, Settings, or software fallback
  semantics already specified for receiver-off idle.
- "Physical display backlight/illumination" means the part of the monitor display that remains
  visibly lit even when screen content is black. It does not mean the full TFT module power rail.
- Touch input and receiver monitoring must remain available while physical display
  backlight/illumination is off.
- Installations without verified hardware display control remain supported through the existing
  software blanking behavior.
- Hardware display control is determined by wiring and validation, not by an on-device Settings
  option, stored preference, or separate hardware-specific firmware build.
- Hardware design, wiring, and maintainer documentation will be captured during planning and
  implementation, but the user-facing feature is limited to receiver-off display backlight/illumination
  handling and recovery behavior.
- The durable hardware guide should prefer specific orderable part identifiers and vendor/manufacturer
  links when they are stable enough to be useful, while also documenting minimum ratings for
  equivalent substitutions.
- Equivalent hardware substitutions should be allowed only when they satisfy the canonical circuit's
  documented electrical ratings and validation steps.
- The preferred hardware path should be reversible inline wiring; modifying display-module traces,
  jumpers, or onboard parts is a last-resort path only when the module cannot expose the backlight
  path safely any other way.
- The device remains powered by a single 5V 2A USB supply plugged into the ESP32. The hardware guide
  may use ESP32 `5V`/`VIN` or `3V3` output pins as appropriate for connected loads, but must not add a
  separate direct power feed to the TFT, touch hardware, or backlight switch load input.
