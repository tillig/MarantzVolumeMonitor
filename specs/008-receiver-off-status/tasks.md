# Tasks: Receiver Off Screen Sleep

**Input**: Design documents from `/specs/008-receiver-off-status/`

**Prerequisites**: `plan.md`, `spec.md`, `research.md`, `data-model.md`, `contracts/`, `quickstart.md`

**Tests**: No automated TDD task set was requested. Validation tasks are included from `quickstart.md` and `contracts/validation_contract.md`.

**Organization**: Tasks are grouped by user story so each story can be implemented and validated as an independent increment. Hardware backlight switching and receiver power control remain out of scope.

## Phase 1: Setup (Shared Context)

**Purpose**: Confirm the current implementation surface and validation scope before making code changes.

- [X] T001 Review receiver-off requirements in `specs/008-receiver-off-status/spec.md` and current Home Screen receiver status handling in `src/ui/Screens/HomeScreen.cpp`
- [X] T002 [P] Review receiver-off validation expectations in `specs/008-receiver-off-status/contracts/validation_contract.md` and `specs/008-receiver-off-status/quickstart.md`
- [X] T003 [P] Review user-facing documentation impact in `docs/ui-reference.md` and `README.md`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Add shared Home Screen state primitives required by every user story.

**CRITICAL**: No user story work should begin until this phase is complete.

- [X] T004 Add receiver-off blank display state, receiver-off timeout fields, and helper declarations in `src/ui/Screens/HomeScreen.h`
- [X] T005 Add receiver-off timeout helper definitions for start, restart, stop, and expiration checks in `src/ui/Screens/HomeScreen.cpp`
- [X] T006 Add Home Screen display-state transition helper logic that centralizes receiver-off timer changes and live-volume animation reset behavior in `src/ui/Screens/HomeScreen.cpp`
- [X] T007 Initialize the new receiver-off timer and blank-state fields in `src/ui/Screens/HomeScreen.cpp`

**Checkpoint**: Home Screen has the shared state and timer primitives needed by all stories.

---

## Phase 3: User Story 1 - Let The Monitor Go Dark When Receiver Is Off (Priority: P1) MVP

**Goal**: Confirmed receiver-off status shows `Receiver off` briefly, then software-blanks the monitor display without showing stale live content.

**Independent Test**: With a saved receiver that responds with confirmed powered-off status, `Receiver off` appears, no stale live volume is shown, and the display blanks to black after 3 seconds +/-0.5 seconds.

### Implementation for User Story 1

- [X] T008 [US1] Update `HomeScreen::draw()` to render visible `Receiver off` with the Settings entry and render the receiver-off blank state as a black screen in `src/ui/Screens/HomeScreen.cpp`
- [X] T009 [US1] Update `HomeScreen::update()` to start the 3000 ms receiver-off timeout when Home enters visible receiver-off state in `src/ui/Screens/HomeScreen.cpp`
- [X] T010 [US1] Update receiver-off timeout handling to enter the blank state only when current status still confirms receiver powered off in `src/ui/Screens/HomeScreen.cpp`
- [X] T011 [US1] Ensure entering visible or blank receiver-off state clears live volume, source, mode, and audio-family rendering artifacts in `src/ui/Screens/HomeScreen.cpp`
- [X] T012 [US1] Validate receiver-active, confirmed receiver-off, and 3-second blanking scenarios from `specs/008-receiver-off-status/quickstart.md`
- [X] T013 [US1] Confirm User Story 1 acceptance rules against `specs/008-receiver-off-status/contracts/receiver_off_display_cycle.md`

**Checkpoint**: User Story 1 is complete and independently demonstrable as the MVP.

---

## Phase 4: User Story 2 - Wake The Monitor For Recovery Actions (Priority: P2)

**Goal**: A tap on the blank receiver-off display wakes the monitor without also activating Settings or setup, then the user can intentionally open Settings from the visible screen.

**Independent Test**: Let the receiver-off display blank, tap once, verify visible `Receiver off` returns without opening Settings, then tap the Settings icon and verify Settings opens.

### Implementation for User Story 2

- [X] T014 [US2] Update `HomeScreen::handleTouch()` to consume any touch in the receiver-off blank state as wake-only input in `src/ui/Screens/HomeScreen.cpp`
- [X] T015 [US2] Add wake resolution logic that shows visible `Receiver off` when off remains confirmed or the latest current visible state when off can no longer be confirmed in `src/ui/Screens/HomeScreen.cpp`
- [X] T016 [US2] Restart the 3000 ms receiver-off timeout after wake returns to visible `Receiver off` in `src/ui/Screens/HomeScreen.cpp`
- [X] T017 [US2] Preserve separate Settings-icon activation after wake while preventing the wake touch from opening Settings in `src/ui/Screens/HomeScreen.cpp`
- [X] T018 [US2] Update `HomeScreen::handleTouch()` so taps on visible `Receiver off` outside the Settings icon restart the 3000 ms receiver-off timeout and keep `Receiver off` visible in `src/ui/Screens/HomeScreen.cpp`
- [X] T019 [US2] Validate touch wake, wake-only behavior, and timeout restart scenarios from `specs/008-receiver-off-status/quickstart.md`

**Checkpoint**: User Story 2 can be validated without relying on Settings timeout behavior.

---

## Phase 5: User Story 3 - Keep Settings Usable While Receiver Is Off (Priority: P3)

**Goal**: Settings and settings-launched setup flows remain visible while the receiver is off, and the receiver-off timeout resumes only after returning Home.

**Independent Test**: Wake from blank receiver-off state, open Settings, remain in Settings longer than 3 seconds, then return Home and confirm receiver-off blanking resumes only after Home is shown.

### Implementation for User Story 3

- [X] T020 [US3] Verify the Settings OK return path creates a fresh Home Screen without applying receiver-off timeout while Settings is active in `src/ui/Screens/SettingsScreen.cpp` and `src/ui/Screens/HomeScreen.cpp`
- [X] T021 [US3] Update Home Screen re-entry behavior to show visible `Receiver off` and restart the timeout when returning from Settings while receiver-off remains confirmed in `src/ui/Screens/HomeScreen.cpp`
- [X] T022 [US3] Verify settings-launched Wi-Fi, receiver setup, receiver status, and calibration flows remain visible while active in `src/ui/Screens/NetworkListScreen.cpp`, `src/ui/Screens/ReceiverListScreen.cpp`, `src/ui/Screens/SetupStatusScreen.cpp`, and `src/ui/Screens/CalibrationScreen.cpp`
- [X] T023 [US3] Validate Settings entry, 60-second Settings visibility, and return-from-Settings scenarios from `specs/008-receiver-off-status/quickstart.md`

**Checkpoint**: User Story 3 preserves the receiver-off recovery path through Settings.

---

## Phase 6: User Story 4 - Avoid Misleading Screen-Off Behavior (Priority: P4)

**Goal**: The monitor blanks only for confirmed receiver-off status and does not hide unavailable, unknown, Wi-Fi, or setup-required states.

**Independent Test**: Compare confirmed receiver-off status with unreachable receiver, missing receiver configuration, Wi-Fi disconnected, and unknown power state; only confirmed receiver-off status enters the receiver-off blank cycle.

### Implementation for User Story 4

- [X] T024 [US4] Guard receiver-off cycle entry so only fresh `isValid && powerKnown && !power` receiver status can enter visible or blank receiver-off states in `src/ui/Screens/HomeScreen.cpp`
- [X] T025 [US4] Keep the monitor blank if receiver-off confirmation is lost after blanking, until user touch or fresh active receiver status, in `src/ui/Screens/HomeScreen.cpp`
- [X] T026 [US4] Exit receiver-off visible or blank state to normal live volume when fresh active receiver status with volume resumes in `src/ui/Screens/HomeScreen.cpp`
- [X] T027 [US4] Preserve visible unavailable, Wi-Fi connecting, Wi-Fi setup-required, and receiver setup-required states instead of entering receiver-off blanking in `src/ui/Screens/HomeScreen.cpp`
- [X] T028 [US4] Validate unreachable receiver, unknown power state, Wi-Fi disconnected, setup-required, and active-status recovery scenarios from `specs/008-receiver-off-status/contracts/validation_contract.md`

**Checkpoint**: User Story 4 prevents receiver-off blanking from masking setup or troubleshooting states.

---

## Phase 7: Polish and Cross-Cutting Concerns

**Purpose**: Update durable documentation and run the complete validation sequence.

- [X] T029 [P] Document receiver-off visible timeout, software blanking, touch wake, and Settings visibility behavior in `docs/ui-reference.md`
- [X] T030 [P] Add a concise user-facing receiver-off blank/wake note in `README.md`
- [X] T031 Run `pre-commit run --all-files` from the repository root and resolve any issues reported for `specs/008-receiver-off-status/tasks.md`, `src/ui/Screens/HomeScreen.cpp`, `docs/ui-reference.md`, or `README.md`
- [X] T032 Run `git diff --check` from the repository root and resolve whitespace issues in `specs/008-receiver-off-status/tasks.md`, `src/ui/Screens/HomeScreen.cpp`, `docs/ui-reference.md`, or `README.md`
- [X] T033 Run `/Users/travis.illig/.platformio/penv/bin/platformio run` to validate the firmware build defined by `platformio.ini`
- [X] T034 Perform the complete hardware verification matrix from `specs/008-receiver-off-status/contracts/validation_contract.md`
- [X] T035 Review the final diff to confirm no receiver power-control behavior, hardware backlight switching, storage schema change, network protocol change, or new dependency was added outside `specs/008-receiver-off-status/research.md` scope

---

## Dependencies and Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies.
- **Foundational (Phase 2)**: Depends on Setup completion and blocks all user stories.
- **User Story 1 (Phase 3)**: Depends on Foundational and is the MVP.
- **User Story 2 (Phase 4)**: Depends on Foundational; safest after User Story 1 because it wakes the blank state introduced there.
- **User Story 3 (Phase 5)**: Depends on Foundational; safest after User Story 2 because Settings entry is reached through wake.
- **User Story 4 (Phase 6)**: Depends on Foundational; can be developed alongside earlier stories with care, but should be validated after receiver-off blanking exists.
- **Polish (Phase 7)**: Depends on all desired user stories being complete.

### User Story Dependencies

- **US1 (P1)**: No dependency on other user stories; delivers the MVP blanking cycle.
- **US2 (P2)**: Requires the blank state from US1 to fully validate wake behavior.
- **US3 (P3)**: Requires Settings entry from visible receiver-off state; integrates naturally after US2.
- **US4 (P4)**: Strengthens classification and transition guards; can be implemented after US1 but should be final-validated against all prior stories.

### Parallel Opportunities

- T002 and T003 can run in parallel during Setup.
- T029 and T030 can run in parallel after behavior is implemented.
- Hardware validation preparation from T012, T019, T023, T028, and T034 can be split by scenario, but results should be reconciled against the single device behavior.
- Most code tasks touch `src/ui/Screens/HomeScreen.cpp`, so do not run them concurrently in the same worktree.

---

## Parallel Example: Documentation Polish

```text
Task: "Document receiver-off visible timeout, software blanking, touch wake, and Settings visibility behavior in docs/ui-reference.md"
Task: "Add a concise user-facing receiver-off blank/wake note in README.md"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup.
2. Complete Phase 2: Foundational.
3. Complete Phase 3: User Story 1.
4. Stop and validate `Receiver off` visible timing, black blanking, and stale live-content removal.

### Incremental Delivery

1. Add US1 to deliver the quiet receiver-off MVP.
2. Add US2 so the blank monitor can be woken and Settings can be reached intentionally.
3. Add US3 so Settings and settings-launched flows remain usable while the receiver is off.
4. Add US4 to harden unavailable/setup/unknown-state behavior.
5. Complete docs, static validation, firmware build, and hardware verification.

### Scope Guard

- Do not add receiver power-control UI or commands.
- Do not add hardware backlight switching or wiring guidance in this feature.
- Do not add storage schema, network protocol, dependency, or platform configuration changes unless a validation failure proves they are necessary for the specified behavior.
