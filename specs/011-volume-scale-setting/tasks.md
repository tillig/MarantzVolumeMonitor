# Tasks: Volume Display Scale

**Input**: Design documents from `/specs/011-volume-scale-setting/`

**Prerequisites**: `plan.md`, `spec.md`, `research.md`, `data-model.md`, `contracts/`, `quickstart.md`

**Tests**: No separate automated TDD task set was requested. Static validation and hardware verification tasks are included from `quickstart.md` and `contracts/validation_contract.md`.

**Organization**: Tasks are grouped by user story so each story can be implemented and validated as an independent increment. The feature reuses the existing persisted `useDbScale` config flag, adds a dedicated selection screen under `Settings`, and keeps the home-screen gauge behavior unchanged.

## Phase 1: Setup (Shared Context)

**Purpose**: Confirm the implementation surface and validation scope before code changes begin.

- [X] T001 Review `specs/011-volume-scale-setting/spec.md`, `specs/011-volume-scale-setting/plan.md`, and `specs/011-volume-scale-setting/research.md`
- [X] T002 [P] Review existing settings navigation, persisted `useDbScale` handling, and home volume formatting in `src/ui/Screens/SettingsScreen.cpp`, `src/storage/ConfigStore.cpp`, and `src/ui/Screens/HomeScreen.cpp`
- [X] T003 [P] Review selection-screen, home-format, and validation contracts in `specs/011-volume-scale-setting/contracts/volume_display_scale_screen_contract.md`, `specs/011-volume-scale-setting/contracts/home_volume_format_contract.md`, and `specs/011-volume-scale-setting/contracts/validation_contract.md`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Add the shared UI scaffolding and helpers required before any user story can be completed cleanly.

**⚠️ CRITICAL**: No user story work should begin until this phase is complete.

- [X] T004 Create the `VolumeScaleScreen` class scaffold and pending-selection state declarations in `src/ui/Screens/VolumeScaleScreen.h` and `src/ui/Screens/VolumeScaleScreen.cpp`
- [X] T005 [P] Add shared single-choice row indicator declarations and supporting drawing helpers in `src/ui/MaterialStyle.h`
- [X] T006 [P] Implement shared single-choice row indicator and selection-row presentation helpers in `src/ui/MaterialStyle.cpp`
- [X] T007 Confirm and, if needed, tighten `useDbScale` load/save reuse paths without changing the config schema in `src/storage/ConfigStore.h` and `src/storage/ConfigStore.cpp`

**Checkpoint**: Shared screen and Material-style primitives are ready for story work.

---

## Phase 3: User Story 1 - Choose The Home Volume Number Format (Priority: P1) 🎯 MVP

**Goal**: Let the user open `Volume Display Scale` from `Settings`, choose `0-100` or `dB` on a dedicated screen, and apply or discard the change with `OK` and `Cancel`.

**Independent Test**: Open `Settings`, enter `Volume Display Scale`, confirm both options and the selected-state indicator are visible, select a different option, press `OK`, and confirm the home-screen number format changes. Repeat with `Cancel` and confirm no change is applied.

### Implementation for User Story 1

- [X] T008 [US1] Add the `Volume Display Scale` destination label, ordering, and open-entry wiring in `src/ui/Screens/SettingsScreen.h` and `src/ui/Screens/SettingsScreen.cpp`
- [X] T009 [US1] Implement the `VolumeScaleScreen` layout with title, two selectable options, and bottom `OK` and `Cancel` actions in `src/ui/Screens/VolumeScaleScreen.cpp`
- [X] T010 [US1] Implement pending-choice selection behavior and visible selected-state indication without relying on color alone in `src/ui/Screens/VolumeScaleScreen.cpp`
- [X] T011 [US1] Implement `OK` apply and `Cancel` discard flows returning to `Settings` in `src/ui/Screens/VolumeScaleScreen.cpp`
- [X] T012 [US1] Validate the menu-entry, selection, apply, and cancel flows against `specs/011-volume-scale-setting/contracts/settings_menu_navigation_contract.md`, `specs/011-volume-scale-setting/contracts/volume_display_scale_screen_contract.md`, and `specs/011-volume-scale-setting/quickstart.md`

**Checkpoint**: User Story 1 delivers the full settings flow for changing the displayed volume scale.

---

## Phase 4: User Story 2 - Keep The Gauge Behavior Unchanged (Priority: P1)

**Goal**: Switch only the home-screen numeric volume format while preserving the existing normalized gauge mapping, animation behavior, and non-live state handling.

**Independent Test**: At the same live receiver volume, switch between `0-100` and `dB` and confirm the number changes format with one decimal place while the gauge position and animation remain unchanged.

### Implementation for User Story 2

- [X] T013 [US2] Update home-screen numeric formatting to branch between normalized `0-100` and raw receiver `dB` text in `src/ui/Screens/HomeScreen.h` and `src/ui/Screens/HomeScreen.cpp`
- [X] T014 [US2] Preserve normalized gauge targets, gauge motion, and redraw behavior regardless of the selected numeric mode in `src/ui/Screens/HomeScreen.cpp`
- [X] T015 [US2] Ensure non-live states continue suppressing numeric volume after a scale change in `src/ui/Screens/HomeScreen.cpp`
- [X] T016 [US2] Validate number-format switching and unchanged gauge behavior against `specs/011-volume-scale-setting/contracts/home_volume_format_contract.md` and `specs/011-volume-scale-setting/contracts/validation_contract.md`

**Checkpoint**: User Story 2 keeps the home-screen visual gauge stable while the numeric format changes.

---

## Phase 5: User Story 3 - Keep The Preference For Future Use (Priority: P2)

**Goal**: Keep the selected scale as a durable user preference with the correct default, persisted save behavior, and correct re-entry state on the selector screen.

**Independent Test**: Verify a fresh device defaults to `0-100`, save `dB`, reopen the selector and confirm `dB` is selected, restart the device, and confirm the same saved format remains active.

### Implementation for User Story 3

- [X] T017 [US3] Load the saved `useDbScale` choice on `VolumeScaleScreen` entry and map missing config to the default `0-100` selection in `src/ui/Screens/VolumeScaleScreen.cpp`
- [X] T018 [US3] Persist the confirmed `useDbScale` value without mutating unrelated config fields in `src/ui/Screens/VolumeScaleScreen.cpp` and `src/storage/ConfigStore.cpp`
- [X] T019 [US3] Rehydrate the saved scale into `HomeScreen` refresh and selector re-entry behavior in `src/ui/Screens/HomeScreen.cpp` and `src/ui/Screens/VolumeScaleScreen.cpp`
- [X] T020 [US3] Validate default, reopen, and restart persistence behavior against `specs/011-volume-scale-setting/contracts/volume_display_scale_screen_contract.md`, `specs/011-volume-scale-setting/contracts/validation_contract.md`, and `specs/011-volume-scale-setting/quickstart.md`

**Checkpoint**: User Story 3 makes the chosen format durable across screen changes and restart.

---

## Phase 6: Polish and Cross-Cutting Concerns

**Purpose**: Update durable documentation and run the full validation sequence.

- [X] T021 [P] Update the durable volume-format and selection-screen guidance in `docs/ui-reference.md`
- [X] T022 [P] Update user-facing settings and volume-format notes in `README.md`
- [X] T023 Run `pre-commit run --all-files` from the repository root and resolve any issues reported for `src/ui/Screens/VolumeScaleScreen.cpp`, `src/ui/Screens/SettingsScreen.cpp`, `src/ui/Screens/HomeScreen.cpp`, `src/ui/MaterialStyle.cpp`, `docs/ui-reference.md`, `README.md`, or `specs/011-volume-scale-setting/`
- [X] T024 Run `git diff --check` from the repository root and resolve whitespace issues in `src/ui/Screens/VolumeScaleScreen.cpp`, `src/ui/Screens/SettingsScreen.cpp`, `src/ui/Screens/HomeScreen.cpp`, `src/ui/MaterialStyle.cpp`, `docs/ui-reference.md`, `README.md`, or `specs/011-volume-scale-setting/`
- [X] T025 Run `/Users/travis.illig/.platformio/penv/bin/platformio run` to validate the firmware build defined by `platformio.ini`
- [X] T026 Perform the complete hardware verification matrix from `specs/011-volume-scale-setting/contracts/validation_contract.md`
- [X] T027 Review the final diff against `specs/011-volume-scale-setting/plan.md`, `specs/011-volume-scale-setting/research.md`, and `specs/011-volume-scale-setting/contracts/home_volume_format_contract.md` to confirm the gauge stayed unchanged, no config migration was added, and the settings flow stayed within scope

---

## Dependencies and Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies.
- **Foundational (Phase 2)**: Depends on Setup completion and blocks all user stories.
- **User Story 1 (Phase 3)**: Depends on Foundational and delivers the MVP settings flow.
- **User Story 2 (Phase 4)**: Depends on Foundational and is safest after User Story 1 because it relies on the new selector being able to change the active mode.
- **User Story 3 (Phase 5)**: Depends on Foundational and builds on the selection flow from User Story 1 plus the formatting behavior from User Story 2.
- **Polish (Phase 6)**: Depends on all desired user stories being complete.

### User Story Dependencies

- **US1 (P1)**: No dependency on other user stories; delivers the complete settings selection flow and is the MVP.
- **US2 (P1)**: Depends on the saved selection from US1 to exercise live numeric mode changes while preserving gauge behavior.
- **US3 (P2)**: Depends on US1 for selector entry and confirm/cancel flow, and on US2 for verifying the persisted mode in live home-screen presentation.

### Parallel Opportunities

- T002 and T003 can run in parallel during Setup.
- T005 and T006 can run in parallel during Foundational because they affect different shared UI helper declarations and implementations.
- T021 and T022 can run in parallel during Polish because they update different documentation files.
- Most US1 through US3 tasks touch `src/ui/Screens/VolumeScaleScreen.cpp` or `src/ui/Screens/HomeScreen.cpp`, so do not run those tasks concurrently in the same worktree.

---

## Parallel Example: Foundational Helpers

```text
Task: "Add shared single-choice row indicator declarations and supporting drawing helpers in src/ui/MaterialStyle.h"
Task: "Implement shared single-choice row indicator and selection-row presentation helpers in src/ui/MaterialStyle.cpp"
```

## Parallel Example: Documentation Polish

```text
Task: "Update the durable volume-format and selection-screen guidance in docs/ui-reference.md"
Task: "Update user-facing settings and volume-format notes in README.md"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup.
2. Complete Phase 2: Foundational.
3. Complete Phase 3: User Story 1.
4. Stop and validate that `Volume Display Scale` opens from `Settings`, shows the saved selection clearly, applies with `OK`, discards with `Cancel`, and returns to `Settings`.

### Incremental Delivery

1. Add US1 to deliver the dedicated selection-screen workflow.
2. Add US2 to switch the home-screen numeric format while preserving the gauge.
3. Add US3 to harden defaulting, save persistence, and restart behavior.
4. Complete documentation, static validation, firmware build, and hardware verification.

### Scope Guard

- Do not change the home-screen gauge range, mapping, or animation behavior.
- Do not add a `config.json` migration or replace the existing `useDbScale` field with a new storage shape.
- Do not add immediate-apply behavior when the user taps an option; `OK` remains the only apply action.
- Do not switch `Settings` to icon-only navigation or remove existing labeled destinations.
