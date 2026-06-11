# Tasks: Receiver Off Hardware Handling

**Input**: Design documents from `/specs/009-receiver-off-hardware/`

**Prerequisites**: `plan.md`, `spec.md`, `research.md`, `data-model.md`, `contracts/`, `quickstart.md`

**Tests**: No automated TDD task set was requested. Validation tasks are included from `quickstart.md`
and `contracts/validation_contract.md`.

**Organization**: Tasks are grouped by user story so each story can be implemented and validated as
an independent increment. The canonical hardware path is high-side `LED/BL` backlight switching via
Pololu item `2810`; `2N7000FS-ND` is documented as evaluated but rejected for the planned switch.

## Phase 1: Setup (Shared Context)

**Purpose**: Confirm the current implementation and hardware documentation surface before making
changes.

- [X] T001 Review receiver-off hardware requirements in `specs/009-receiver-off-hardware/spec.md` and current receiver-off state handling in `src/ui/Screens/HomeScreen.cpp`
- [X] T002 [P] Review backlight signal rules in `specs/009-receiver-off-hardware/contracts/backlight_control_contract.md` and current display initialization in `src/ui/DisplayManager.cpp`
- [X] T003 [P] Review hardware guide requirements in `specs/009-receiver-off-hardware/contracts/hardware_upgrade_guide_contract.md` and current wiring notes in `docs/hardware.md`
- [X] T004 [P] Review validation scenarios in `specs/009-receiver-off-hardware/contracts/validation_contract.md` and `specs/009-receiver-off-hardware/quickstart.md`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Add the shared backlight-control API and tracked pin definition required by every
hardware behavior story.

**CRITICAL**: No user story work should begin until this phase is complete.

- [X] T005 Add a tracked GPIO13 backlight-control pin definition such as `BACKLIGHT_CONTROL_PIN=13` in `platformio.ini`
- [X] T006 Add backlight-control method declarations and state tracking to `src/ui/DisplayManager.h`
- [X] T007 Implement GPIO13 initialization as an active-high output that defaults backlight on in `src/ui/DisplayManager.cpp`
- [X] T008 Implement `DisplayManager` backlight on/off behavior that is safe when GPIO13 is unconnected in `src/ui/DisplayManager.cpp`
- [X] T009 Update `DisplayManager::begin()` to initialize the backlight-control signal before visible UI drawing in `src/ui/DisplayManager.cpp`

**Checkpoint**: Display hardware control exists behind `DisplayManager`; Home Screen can call it
without owning GPIO details.

---

## Phase 3: User Story 1 - Physically Quiet The Display When Receiver Is Off (Priority: P1) MVP

**Goal**: Confirmed receiver-off status shows `Receiver off`, then turns off hardware
backlight/illumination instead of leaving display glow.

**Independent Test**: With the canonical backlight circuit wired, confirmed receiver-off status shows
`Receiver off`, then the backlight turns off after 3 seconds +/-0.5 seconds while no stale content or
glow remains visible from 15 feet.

### Implementation for User Story 1

- [X] T010 [US1] Call `DisplayManager` to keep the backlight on for live, setup, unavailable, and visible receiver-off states in `src/ui/Screens/HomeScreen.cpp`
- [X] T011 [US1] Call `DisplayManager` to turn the backlight off only when entering `ReceiverOffBlank` in `src/ui/Screens/HomeScreen.cpp`
- [X] T012 [US1] Ensure receiver-off blank drawing still clears visible content before the hardware backlight is turned off in `src/ui/Screens/HomeScreen.cpp`
- [X] T013 [US1] Verify receiver-off blank entry still requires fresh confirmed powered-off status in `src/ui/Screens/HomeScreen.cpp`
- [X] T014 [US1] Validate hardware-capable receiver-off visible timeout and backlight-off behavior using `specs/009-receiver-off-hardware/contracts/validation_contract.md`

**Checkpoint**: User Story 1 is complete and independently demonstrable as the MVP.

---

## Phase 4: User Story 2 - Wake Reliably From Hardware-Off Display State (Priority: P2)

**Goal**: A tap while the backlight is off restores visible output without triggering a second action,
then Settings can be opened by a separate visible tap.

**Independent Test**: Let receiver-off hardware display-off state occur, tap once, verify the
backlight turns on and the current visible state appears without opening Settings; then tap Settings
and verify Settings opens.

### Implementation for User Story 2

- [X] T015 [US2] Turn the backlight on before drawing the wake result from `ReceiverOffBlank` in `src/ui/Screens/HomeScreen.cpp`
- [X] T016 [US2] Preserve wake-only touch consumption so the wake touch cannot also open Settings in `src/ui/Screens/HomeScreen.cpp`
- [X] T017 [US2] Keep receiver polling active while the backlight is off so fresh active status restores live display in `src/ui/Screens/HomeScreen.cpp`
- [X] T018 [US2] Ensure wake from `ReceiverOffBlank` after receiver-off confirmation is lost shows the latest current visible state instead of assuming receiver-off in `src/ui/Screens/HomeScreen.cpp`
- [X] T019 [US2] Validate touch wake, wake-only behavior, receiver-active restore, and wake after lost receiver-off confirmation using `specs/009-receiver-off-hardware/quickstart.md`

**Checkpoint**: User Story 2 can be validated without relying on Settings timeout behavior.

---

## Phase 5: User Story 3 - Preserve Safe Operation When Hardware Control Is Unavailable (Priority: P3)

**Goal**: Software-only or unmodified hardware remains safe and usable when GPIO13 is unconnected.

**Independent Test**: Leave GPIO13 unconnected, run the receiver-off flow, and verify the existing
software blanking, touch wake, active receiver restore, setup, and Settings behavior still work.

### Implementation for User Story 3

- [X] T020 [US3] Confirm the backlight API has no runtime dependency on hardware detection, Settings, storage, or alternate builds in `src/ui/DisplayManager.cpp`
- [X] T021 [US3] Confirm software-only receiver-off blanking still works when GPIO13 is unconnected in `src/ui/Screens/HomeScreen.cpp`
- [X] T022 [US3] Validate setup, unavailable, live, receiver-off blank, touch wake, and active restore behavior with GPIO13 unconnected using `specs/009-receiver-off-hardware/contracts/validation_contract.md`

**Checkpoint**: User Story 3 proves the same firmware remains safe on unmodified hardware.

---

## Phase 6: User Story 4 - Keep Configuration And Recovery Visible (Priority: P4)

**Goal**: Settings and settings-launched setup flows stay visible with the backlight on while the
receiver is off.

**Independent Test**: Wake from receiver-off hardware display-off state, open Settings, remain there
for at least 60 seconds, and verify the backlight stays on until returning Home.

### Implementation for User Story 4

- [X] T023 [US4] Ensure Settings navigation from visible receiver-off state turns or keeps the backlight on in `src/ui/Screens/HomeScreen.cpp`
- [X] T024 [US4] Verify Settings and settings-launched setup screens do not call receiver-off backlight-off behavior in `src/ui/Screens/SettingsScreen.cpp`, `src/ui/Screens/NetworkListScreen.cpp`, `src/ui/Screens/ReceiverListScreen.cpp`, `src/ui/Screens/SetupStatusScreen.cpp`, and `src/ui/Screens/CalibrationScreen.cpp`
- [X] T025 [US4] Validate Settings visibility for at least 60 seconds while receiver remains off using `specs/009-receiver-off-hardware/contracts/validation_contract.md`

**Checkpoint**: User Story 4 preserves the on-device recovery path.

---

## Phase 7: User Story 5 - Follow A Specific Hardware Upgrade Guide (Priority: P5)

**Goal**: Durable hardware documentation tells the builder exactly what to buy, how to wire it, what
not to use, and how to validate the upgrade safely.

**Independent Test**: Review `docs/hardware.md` and confirm it includes the required BOM, canonical
circuit, wiring table, safety warnings, validation steps, 2N7000 rejection, and last-resort
modification limits.

### Implementation for User Story 5

- [X] T026 [US5] Add the canonical Pololu 2810 high-side `LED/BL` backlight-control circuit and connection diagram to `docs/hardware.md`, showing USB power entering the ESP32 and Pololu `VIN`/`VOUT` pin usage from ESP32 `5V`/`VIN`
- [X] T027 [US5] Add a bill of materials with Pololu item `2810`, required wiring/connector materials, multimeter, source links, equivalent minimum ratings, pad-name guidance, and an ESP32-fed current-budget worksheet to `docs/hardware.md`
- [X] T028 [US5] Add an explicit connection table for USB input, ESP32 `5V`/`VIN`, shared ground, TFT `VCC`, TFT `LED/BL`, ESP32 GPIO13, and Pololu `VIN`/`VOUT`/`ON`/`GND` pins to `docs/hardware.md`
- [X] T029 [US5] Add safety warnings for GPIO current limits, 3.3V logic, no direct backlight drive, no TFT `VCC` switching, disconnected power while wiring, shared ground, and no separate direct TFT/touch or switch-load supply to `docs/hardware.md`
- [X] T030 [US5] Document Adafruit item `5648` only as a conditional low-side substitute and document `2N7000FS-ND` / onsemi `2N7000` as evaluated but rejected in `docs/hardware.md`
- [X] T031 [US5] Add wiring inspection, ESP32-fed power-origin check, current-budget check, first-power-on, receiver-off backlight-off, touch wake, receiver-active restore, software-only fallback, and rollback validation steps to `docs/hardware.md`
- [X] T032 [US5] Add last-resort display-module modification prerequisites, risks, and rollback limits to `docs/hardware.md`
- [X] T033 [US5] Validate `docs/hardware.md` against `specs/009-receiver-off-hardware/contracts/hardware_upgrade_guide_contract.md`

**Checkpoint**: User Story 5 provides the durable hardware guide needed before physical modification.

---

## Phase 8: Polish & Cross-Cutting Concerns

**Purpose**: Align user-facing references and run the full validation sequence.

- [X] T034 [P] Update receiver-off hardware backlight behavior notes in `docs/ui-reference.md`
- [X] T035 [P] Add a concise user-facing hardware-capable receiver-off behavior note in `README.md`
- [X] T036 Run `pre-commit run --all-files` from the repository root and resolve issues in changed files
- [X] T037 Run `git diff --check` from the repository root and resolve whitespace issues in changed files
- [X] T038 Run `/Users/travis.illig/.platformio/penv/bin/platformio run` to validate the firmware build defined by `platformio.ini`
- [X] T039 Perform software-only hardware verification with GPIO13 unconnected, including restart while receiver is already off, using `specs/009-receiver-off-hardware/contracts/validation_contract.md`
- [X] T040 Perform hardware-capable verification with the canonical backlight circuit, including restart after receiver-off hardware display-off, using `specs/009-receiver-off-hardware/contracts/validation_contract.md`
- [X] T041 Review the final diff to confirm no receiver power-control behavior, TFT `VCC` switching, Settings toggle, storage schema change, network protocol change, or new dependency was added outside `specs/009-receiver-off-hardware/research.md` scope

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies.
- **Foundational (Phase 2)**: Depends on Setup completion and blocks all user stories.
- **User Story 1 (Phase 3)**: Depends on Foundational and is the MVP.
- **User Story 2 (Phase 4)**: Depends on Foundational; safest after User Story 1 because it wakes the
  hardware-off state introduced there.
- **User Story 3 (Phase 5)**: Depends on Foundational; can be validated after User Story 1 because it
  verifies unconnected GPIO fallback.
- **User Story 4 (Phase 6)**: Depends on Foundational; safest after User Story 2 because Settings is
  reached after wake.
- **User Story 5 (Phase 7)**: Depends on plan/research/contracts and can proceed in parallel with
  code stories, but final acceptance depends on actual implementation choices.
- **Polish (Phase 8)**: Depends on all desired user stories being complete.

### User Story Dependencies

- **US1 (P1)**: No dependency on other user stories; delivers the MVP hardware quieting behavior.
- **US2 (P2)**: Requires US1's hardware-off state for full validation.
- **US3 (P3)**: Requires Foundational and US1 behavior; proves safe fallback without hardware.
- **US4 (P4)**: Requires US2 wake path and visible Settings entry.
- **US5 (P5)**: Can be drafted from plan artifacts independently but should be final-reviewed after
  implementation and validation.

### Parallel Opportunities

- T002, T003, and T004 can run in parallel during Setup.
- US5 documentation tasks T026-T032 can run in parallel with firmware tasks after Phase 2 if one
  person owns reconciliation in T033.
- T034 and T035 can run in parallel during Polish.
- Most firmware tasks touch `src/ui/DisplayManager.*` or `src/ui/Screens/HomeScreen.cpp`, so do not
  run those concurrently in the same worktree.

---

## Parallel Example: User Story 5 Documentation

```text
Task: "Add the canonical Pololu 2810 high-side LED/BL backlight-control circuit and connection diagram to docs/hardware.md, preserving USB power into the ESP32 and using VIN/VOUT/ON labels"
Task: "Add a bill of materials with Pololu item 2810, required wiring/connector materials, multimeter, source links, equivalent minimum ratings, pad-name guidance, and current-budget worksheet to docs/hardware.md"
Task: "Add safety warnings for GPIO current limits, 3.3V logic, no direct backlight drive, no TFT VCC switching, disconnected power while wiring, shared ground, and no separate direct display/touch supply to docs/hardware.md"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup.
2. Complete Phase 2: Foundational.
3. Complete Phase 3: User Story 1.
4. Stop and validate receiver-off visible timing, hardware backlight-off transition, and no stale
   visible content.

### Incremental Delivery

1. Add US1 to deliver hardware backlight-off behavior for confirmed receiver-off idle.
2. Add US2 so the hardware-off monitor can be woken and Settings can be reached intentionally.
3. Add US3 so the same firmware remains safe on unmodified hardware.
4. Add US4 so Settings and setup flows stay visible while receiver remains off.
5. Add US5 so the hardware guide is specific enough for safe physical upgrade.
6. Complete cross-cutting docs, static validation, firmware build, and hardware verification.

### Scope Guard

- Do not switch TFT `VCC`, touch power, ESP32 power, or receiver power.
- Do not add a separate direct power feed to TFT/touch hardware or the Pololu `VIN` pads;
  USB power enters through the ESP32 and display/touch plus switch-load power comes from ESP32 power
  pins.
- Do not add receiver power-control UI or commands.
- Do not add Settings toggles, stored preferences, or separate firmware builds for hardware control.
- Do not use `2N7000FS-ND` as the planned switch; keep it documented as evaluated but rejected.
- Do not add storage schema, network protocol, dependency, or platform changes unless a validation
  failure proves they are necessary for the specified behavior.
