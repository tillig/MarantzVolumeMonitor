# Tasks: Touch Calibration Persistence

**Input**: Design documents from `/specs/012-touch-calibration-persistence/`

**Prerequisites**: `plan.md`, `spec.md`, `research.md`, `data-model.md`, `contracts/`, `quickstart.md`

**Tests**: No separate automated TDD task set was requested. Static validation and hardware verification tasks are included from `quickstart.md` and `contracts/validation_contract.md`.

**Organization**: Tasks are grouped by user story so each story can be implemented and validated as an independent increment. The feature keeps the existing measured affine coefficients as the shipped default, adds a saved optional calibration profile in `config.json`, replaces the serial-capture-only calibration flow with an on-device flow, and adds a selective `Reset to defaults` settings child flow.

## Phase 1: Setup (Shared Context)

**Purpose**: Confirm the implementation surface, calibration constraints, and validation scope before code changes begin.

- [X] T001 Review `specs/012-touch-calibration-persistence/spec.md`, `specs/012-touch-calibration-persistence/plan.md`, and `specs/012-touch-calibration-persistence/research.md`
- [X] T002 [P] Review current calibration, boot-config load, and touch-mapping behavior in `src/ui/Screens/CalibrationScreen.cpp`, `src/main.cpp`, and `src/ui/TouchManager.cpp`
- [X] T003 [P] Review reset, persistence, and validation contracts in `specs/012-touch-calibration-persistence/contracts/calibration_session_contract.md`, `specs/012-touch-calibration-persistence/contracts/config_persistence_contract.md`, `specs/012-touch-calibration-persistence/contracts/settings_reset_defaults_contract.md`, and `specs/012-touch-calibration-persistence/contracts/validation_contract.md`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Add the shared storage and runtime touch-profile plumbing required before any user story can be completed cleanly.

**⚠️ CRITICAL**: No user story work should begin until this phase is complete.

- [X] T004 Extend calibration-profile and selective-reset declarations in `src/storage/ConfigStore.h` and `src/ui/TouchManager.h`
- [X] T005 Implement persisted `touchCalibration` serialization, deserialization, and selective-reset helpers in `src/storage/ConfigStore.cpp`
- [X] T006 Implement shipped-default versus active-profile application and validation helpers in `src/ui/TouchManager.cpp`

**Checkpoint**: Shared config and touch-profile primitives are ready for story work.

---

## Phase 3: User Story 1 - Complete Calibration On The Device (Priority: P1) 🎯 MVP

**Goal**: Let the user complete touch calibration entirely on the device, with guarded sample capture and immediate application of a valid result.

**Independent Test**: Start calibration from the device UI, complete all nine points without a serial monitor, confirm bad taps do not advance the target, and verify the new alignment is active when the flow returns to normal navigation.

### Implementation for User Story 1

- [X] T007 [P] [US1] Expand calibration session state, target-boundary constants, and fitted-profile fields in `src/ui/Screens/CalibrationScreen.h`
- [X] T008 [US1] Replace the serial-capture-only flow with guarded on-device sample capture, rejection messaging, and affine solve/apply logic in `src/ui/Screens/CalibrationScreen.cpp`
- [X] T009 [P] [US1] Update calibration entry copy to describe on-device behavior in `src/ui/Screens/SettingsScreen.cpp` and `src/ui/Screens/HomeScreen.cpp`
- [X] T010 [US1] Validate the on-device calibration flow against `specs/012-touch-calibration-persistence/contracts/calibration_session_contract.md` and `specs/012-touch-calibration-persistence/quickstart.md`

**Checkpoint**: User Story 1 delivers the full on-device calibration workflow with guarded capture and same-session application.

---

## Phase 4: User Story 2 - Keep Calibration After Restart (Priority: P1)

**Goal**: Restore the saved calibration profile automatically at boot and fall back safely to the shipped default profile when no valid saved calibration exists.

**Independent Test**: Complete calibration successfully, reboot, and confirm the saved profile is still active; then boot without a saved calibration and confirm touch still uses the shipped default profile without forcing recalibration.

### Implementation for User Story 2

- [X] T011 [US2] Load and apply the active saved-or-default touch calibration during startup in `src/main.cpp`
- [X] T012 [US2] Implement missing-or-invalid saved calibration fallback behavior in `src/storage/ConfigStore.cpp` and `src/ui/TouchManager.cpp`
- [X] T013 [US2] Keep reopened calibration sessions and live setup flows aligned with the active saved-or-default profile in `src/ui/Screens/CalibrationScreen.cpp` and `src/ui/Screens/HomeScreen.cpp`
- [X] T014 [US2] Validate default, reboot, and saved-profile restore behavior against `specs/012-touch-calibration-persistence/contracts/config_persistence_contract.md` and `specs/012-touch-calibration-persistence/contracts/validation_contract.md`

**Checkpoint**: User Story 2 makes calibration durable across reboot while preserving a usable shipped baseline when no saved profile exists.

---

## Phase 5: User Story 3 - Recover From A Bad Or Incomplete Calibration (Priority: P2)

**Goal**: Preserve the previous working alignment on cancel or failure and provide a selective `Reset to defaults` flow for `Wi-Fi`, `Receiver`, and `Calibration`.

**Independent Test**: Cancel or fail a calibration attempt and confirm the prior profile remains active, then open `Reset to defaults`, verify `Cancel` is non-destructive, and confirm each reset type affects only the selected settings type.

### Implementation for User Story 3

- [X] T015 [P] [US3] Create the `ResetDefaultsScreen` selection and confirmation flow scaffold in `src/ui/Screens/ResetDefaultsScreen.h` and `src/ui/Screens/ResetDefaultsScreen.cpp`
- [X] T016 [US3] Wire `Reset to defaults` as the last paginated settings destination in `src/ui/Screens/SettingsScreen.h` and `src/ui/Screens/SettingsScreen.cpp`
- [X] T017 [US3] Implement selective `Wi-Fi`, `Receiver`, and `Calibration` reset actions with explicit `Reset` and `Cancel` behavior in `src/ui/Screens/ResetDefaultsScreen.cpp`, `src/storage/ConfigStore.cpp`, and `src/ui/TouchManager.cpp`
- [X] T018 [US3] Preserve the previously active calibration profile on cancel, retry, and failed-fit exits in `src/ui/Screens/CalibrationScreen.cpp` and `src/ui/Screens/ResetDefaultsScreen.cpp`
- [X] T019 [US3] Validate recovery, selective reset, and confirmation behavior against `specs/012-touch-calibration-persistence/contracts/settings_reset_defaults_contract.md` and `specs/012-touch-calibration-persistence/contracts/validation_contract.md`

**Checkpoint**: User Story 3 prevents bad calibration attempts from corrupting the active profile and adds a visible selective reset recovery path.

---

## Phase 6: Polish and Cross-Cutting Concerns

**Purpose**: Update durable documentation and run the full validation sequence.

- [X] T020 [P] Update user-facing calibration and reset guidance in `README.md` and `docs/hardware.md`
- [X] T021 [P] Update durable UI and architecture references in `docs/ui-reference.md` and `docs/architecture.md`
- [X] T022 Run `pre-commit run --all-files` from the repository root and resolve issues affecting `src/storage/ConfigStore.*`, `src/ui/TouchManager.*`, `src/ui/Screens/CalibrationScreen.*`, `src/ui/Screens/SettingsScreen.*`, `src/ui/Screens/ResetDefaultsScreen.*`, `README.md`, `docs/hardware.md`, `docs/ui-reference.md`, and `docs/architecture.md`
- [X] T023 Run `git diff --check` from the repository root and resolve whitespace issues in `src/storage/ConfigStore.*`, `src/ui/TouchManager.*`, `src/ui/Screens/CalibrationScreen.*`, `src/ui/Screens/SettingsScreen.*`, `src/ui/Screens/ResetDefaultsScreen.*`, `README.md`, `docs/hardware.md`, `docs/ui-reference.md`, and `docs/architecture.md`
- [X] T024 Run `/Users/travis.illig/.platformio/penv/bin/platformio run` to validate the firmware build defined by `platformio.ini`
- [X] T025 Perform the complete hardware verification matrix from `specs/012-touch-calibration-persistence/contracts/validation_contract.md`
- [X] T026 Review the final diff against `specs/012-touch-calibration-persistence/plan.md`, `specs/012-touch-calibration-persistence/research.md`, and `specs/012-touch-calibration-persistence/data-model.md` to confirm the shipped default profile, saved-profile persistence, guarded capture rules, and selective reset scope all stayed within plan

---

## Dependencies and Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies.
- **Foundational (Phase 2)**: Depends on Setup completion and blocks all user stories.
- **User Story 1 (Phase 3)**: Depends on Foundational and delivers the MVP on-device calibration flow.
- **User Story 2 (Phase 4)**: Depends on Foundational and is safest after User Story 1 because reboot persistence is easiest to verify once calibration can already produce a saved profile.
- **User Story 3 (Phase 5)**: Depends on Foundational and is safest after User Stories 1 and 2 because recovery and reset flows operate on the same saved/default profile behavior.
- **Polish (Phase 6)**: Depends on all desired user stories being complete.

### User Story Dependencies

- **US1 (P1)**: No dependency on other user stories; delivers the MVP calibration flow and same-session apply behavior.
- **US2 (P1)**: Depends on the shared persistence and runtime-profile plumbing from Phase 2 and is most meaningful once US1 can produce a saved calibration profile.
- **US3 (P2)**: Depends on the active-profile behavior from US1 and US2 so recovery can preserve or restore the correct profile.

### Parallel Opportunities

- T002 and T003 can run in parallel during Setup.
- T020 and T021 can run in parallel during Polish because they update different documentation files.
- T015 can proceed while T016 is being prepared because the new reset screen scaffold and settings-menu wiring start in different files.
- Most US1 through US3 tasks touch `src/ui/Screens/CalibrationScreen.cpp`, `src/storage/ConfigStore.cpp`, `src/ui/TouchManager.cpp`, or `src/ui/Screens/SettingsScreen.cpp`, so do not run those tasks concurrently in the same worktree.

---

## Parallel Example: Setup Review

```text
Task: "Review current calibration, boot-config load, and touch-mapping behavior in src/ui/Screens/CalibrationScreen.cpp, src/main.cpp, and src/ui/TouchManager.cpp"
Task: "Review reset, persistence, and validation contracts in specs/012-touch-calibration-persistence/contracts/calibration_session_contract.md, specs/012-touch-calibration-persistence/contracts/config_persistence_contract.md, specs/012-touch-calibration-persistence/contracts/settings_reset_defaults_contract.md, and specs/012-touch-calibration-persistence/contracts/validation_contract.md"
```

## Parallel Example: Documentation Polish

```text
Task: "Update user-facing calibration and reset guidance in README.md and docs/hardware.md"
Task: "Update durable UI and architecture references in docs/ui-reference.md and docs/architecture.md"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup.
2. Complete Phase 2: Foundational.
3. Complete Phase 3: User Story 1.
4. Stop and validate that calibration can be completed entirely on-device, that invalid taps do not advance the session, and that the new alignment applies immediately without reboot.

### Incremental Delivery

1. Add US1 to replace serial-only calibration with the on-device guarded flow.
2. Add US2 to restore saved calibration at boot and preserve the shipped default when no saved profile exists.
3. Add US3 to harden failure recovery and introduce selective reset behavior.
4. Complete documentation, static validation, firmware build, and hardware verification.

### Scope Guard

- Do not remove the current measured affine coefficients as the shipped default profile.
- Do not reintroduce screen-specific touch offsets or an external-tool dependency for normal calibration.
- Do not let one reset type clear unrelated saved settings.
- Do not make destructive reset actions happen without explicit confirmation.
