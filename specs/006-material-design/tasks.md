# Tasks: Material Design UI

**Input**: Design documents from `/specs/006-material-design/`

**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/, quickstart.md

**Tests**: Automated test tasks are not included because the specification does not request TDD or automated tests for this embedded UI feature. Static validation and hardware visual verification tasks are included in the final phase.

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel with other marked tasks in the same phase when assigned to different files.
- **[Story]**: Maps task to a user story from `spec.md`.
- Every task includes an exact file path.

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Create the shared style files and implementation audit location used by every story.

- [X] T001 Create shared Material style API declarations in `src/ui/MaterialStyle.h`
- [X] T002 Create shared Material style implementation shell in `src/ui/MaterialStyle.cpp`
- [X] T003 Add `src/ui/MaterialStyle.cpp` to the PlatformIO build by placing it under `src/ui/MaterialStyle.cpp`
- [X] T004 [P] Add a Material Design UI section outline in `docs/ui-reference.md`
- [X] T005 [P] Add a screen coverage audit table for all current screens in `docs/ui-reference.md`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Define shared roles, geometry, state primitives, and drawing helpers before screen work begins.

**Critical**: No user story work should begin until this phase is complete.

- [X] T006 Define typography role constants and text drawing helpers in `src/ui/MaterialStyle.h`
- [X] T007 Implement typography role helpers using existing TFT font numbers in `src/ui/MaterialStyle.cpp`
- [X] T008 Define shared spacing, corner radius, icon gap, button height, list row height, and touch target constants in `src/ui/MaterialStyle.h`
- [X] T009 Define button variant, component state, status kind, and list row option structs in `src/ui/MaterialStyle.h`
- [X] T010 Implement standard icon-with-text button rendering in `src/ui/MaterialStyle.cpp`
- [X] T011 Implement keyboard icon-only button rendering in `src/ui/MaterialStyle.cpp`
- [X] T012 Implement selection list row rendering with primary and secondary text handling in `src/ui/MaterialStyle.cpp`
- [X] T013 Implement searching/status panel rendering helpers with non-blocking progress frame input in `src/ui/MaterialStyle.cpp`
- [X] T014 Implement long text truncation helpers for constrained controls in `src/ui/MaterialStyle.cpp`
- [X] T015 [P] Document typography roles, spacing, geometry, and color roles in `docs/ui-reference.md`
- [X] T016 [P] Document standard button, keyboard button, list row, searching screen, and state treatment patterns in `docs/ui-reference.md`

**Checkpoint**: Foundation ready. User story work can now call shared style primitives instead of hand-rolling repeated styling.

---

## Phase 3: User Story 1 - Recognize the Interface as a Coherent Appliance (Priority: P1) MVP

**Goal**: Every current on-device screen shares coherent spacing, typography hierarchy, icon treatment, component states, and touch affordances while preserving home-screen readability from 15 feet.

**Independent Test**: Review every current on-device screen as a complete UI set. Confirm screens share consistent visual hierarchy, the home screen remains volume-first, and project appliance constraints override generic Material conventions where needed.

### Implementation for User Story 1

- [X] T017 [US1] Apply shared home typography and hierarchy rules in `src/ui/Screens/HomeScreen.cpp`
- [X] T018 [US1] Apply shared settings row and title styling in `src/ui/Screens/SettingsScreen.cpp`
- [X] T019 [US1] Apply shared calibration instruction and touch target styling in `src/ui/Screens/CalibrationScreen.cpp`
- [X] T020 [US1] Apply shared receiver IP entry title, input, and action styling in `src/ui/Screens/ReceiverIpScreen.cpp`
- [X] T021 [US1] Apply shared setup status title and status block styling in `src/ui/Screens/SetupStatusScreen.cpp`
- [X] T022 [US1] Apply shared receiver verification title and status block styling in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [X] T023 [US1] Apply shared network list title and surface spacing in `src/ui/Screens/NetworkListScreen.cpp`
- [X] T024 [US1] Apply shared receiver list title and surface spacing in `src/ui/Screens/ReceiverListScreen.cpp`
- [X] T025 [US1] Apply shared keyboard title and input field styling in `src/ui/Screens/KeyboardScreen.cpp`
- [X] T026 [US1] Record screen-by-screen pattern coverage and any justified exceptions in `docs/ui-reference.md`

**Checkpoint**: User Story 1 is independently testable by reviewing all screens for coherent style and confirming the home screen remains readable from 15 feet.

---

## Phase 4: User Story 2 - Complete Touch Setup with Material Controls (Priority: P1)

**Goal**: Wi-Fi and receiver setup controls, lists, dialogs/statuses, and keyboard controls use consistent Material-compatible behavior and presentation.

**Independent Test**: Walk through Wi-Fi setup and receiver setup from Settings using only the touchscreen. Confirm all required actions use documented controls, equivalent actions match across screens, and keyboard OK/Cancel are aligned.

### Implementation for User Story 2

- [X] T027 [US2] Replace local Wi-Fi action button drawing with shared standard icon-with-text buttons in `src/ui/Screens/NetworkListScreen.cpp`
- [X] T028 [US2] Replace local receiver discovery action button drawing with shared standard icon-with-text buttons in `src/ui/Screens/ReceiverListScreen.cpp`
- [X] T029 [US2] Replace receiver verification failure action buttons with shared standard icon-with-text buttons in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [X] T030 [US2] Replace Wi-Fi failure retry action with shared standard icon-with-text button rendering in `src/ui/Screens/SetupStatusScreen.cpp`
- [X] T031 [US2] Replace manual receiver IP action controls with shared standard icon-with-text buttons in `src/ui/Screens/ReceiverIpScreen.cpp`
- [X] T032 [US2] Migrate Wi-Fi network rows to shared selection list row rendering in `src/ui/Screens/NetworkListScreen.cpp`
- [X] T033 [US2] Migrate receiver candidate rows to shared selection list row rendering in `src/ui/Screens/ReceiverListScreen.cpp`
- [X] T034 [US2] Align keyboard OK and Cancel controls to the same y-coordinate, height, visual center, and icon center in `src/ui/Screens/KeyboardScreen.cpp`
- [X] T035 [US2] Migrate keyboard function keys to the shared keyboard icon-only button variant in `src/ui/Screens/KeyboardScreen.cpp`
- [X] T036 [US2] Update keyboard key layout constants to use shared spacing and touch target values in `src/ui/Screens/KeyboardScreen.cpp`
- [X] T037 [US2] Document setup button variants, list rows, keyboard compact exceptions, and long-name behavior in `docs/ui-reference.md`

**Checkpoint**: User Story 2 is independently testable by completing Wi-Fi and receiver setup on the touchscreen and comparing equivalent buttons, list rows, and keyboard controls.

---

## Phase 5: User Story 3 - Understand State, Feedback, and Errors (Priority: P2)

**Goal**: Loading, searching, empty, success, warning, failure, and unavailable states use consistent feedback and recovery patterns without distracting from passive appliance behavior.

**Independent Test**: Exercise normal, loading, empty, success, warning, and failure states for Wi-Fi and receiver setup. Confirm every state has matching visual treatment, clear text, and an appropriate next action where recovery is possible.

### Implementation for User Story 3

- [X] T038 [US3] Add progress frame state fields for Wi-Fi scanning to `src/ui/Screens/NetworkListScreen.h`
- [X] T039 [US3] Render Wi-Fi searching with the shared searching pattern and immediate progress movement in `src/ui/Screens/NetworkListScreen.cpp`
- [X] T040 [US3] Add progress frame state fields for receiver discovery to `src/ui/Screens/ReceiverListScreen.h`
- [X] T041 [US3] Render receiver searching with the shared searching pattern and immediate progress movement in `src/ui/Screens/ReceiverListScreen.cpp`
- [X] T042 [US3] Update Wi-Fi scan refresh logic so progress movement redraws without blocking touch handling in `src/ui/Screens/NetworkListScreen.cpp`
- [X] T043 [US3] Update receiver discovery refresh logic so progress movement redraws without blocking touch handling in `src/ui/Screens/ReceiverListScreen.cpp`
- [X] T044 [US3] Apply shared loading and failure treatments to Wi-Fi connection states, including the 500 ms non-search progress threshold, in `src/ui/Screens/SetupStatusScreen.cpp`
- [X] T045 [US3] Apply shared loading, success, and failure treatments to receiver verification states, including the 500 ms non-search progress threshold, in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [X] T046 [US3] Apply shared empty and unavailable state treatments to Wi-Fi and receiver list screens in `src/ui/Screens/NetworkListScreen.cpp`
- [X] T047 [US3] Apply shared empty and unavailable state treatments to receiver discovery results in `src/ui/Screens/ReceiverListScreen.cpp`
- [X] T048 [US3] Document searching, loading, success, warning, failure, unavailable, and recovery state rules in `docs/ui-reference.md`

**Checkpoint**: User Story 3 is independently testable by forcing searching, loading, empty, success, warning, failure, and unavailable states in setup flows.

---

## Phase 6: User Story 4 - Preserve Contributor Design Consistency (Priority: P3)

**Goal**: Contributors can use durable Material-compatible guidance to extend the UI without reintroducing one-off screen styles.

**Independent Test**: Review `docs/ui-reference.md` and confirm a contributor can identify approved component styles, state treatments, screen patterns, and exceptions for future work.

### Implementation for User Story 4

- [X] T049 [US4] Complete the shared component/style catalog scope and priority rules in `docs/ui-reference.md`
- [X] T050 [US4] Complete the screen application matrix for every current screen in `docs/ui-reference.md`
- [X] T051 [US4] Complete documented exception entries for keyboard icon-only controls and any remaining screen-specific deviations in `docs/ui-reference.md`
- [X] T052 [US4] Add a hardware visual verification checklist for Material Design consistency in `docs/ui-reference.md`
- [X] T053 [US4] Cross-check `docs/ui-reference.md` against `specs/006-material-design/contracts/component_style_catalog.md`
- [X] T054 [US4] Cross-check `docs/ui-reference.md` against `specs/006-material-design/contracts/screen_application.md`
- [X] T055 [US4] Update `CONTRIBUTING.md` only if implementation adds validation commands beyond current pre-commit, PlatformIO, and diff checks in `CONTRIBUTING.md`

**Checkpoint**: User Story 4 is independently testable by using `docs/ui-reference.md` to choose the correct pattern for common future controls within 2 minutes.

---

## Phase 7: Polish and Cross-Cutting Concerns

**Purpose**: Validate the full feature and clean up cross-story issues.

- [X] T056 [P] Run markdown and repository validation with `pre-commit run --all-files`
- [X] T057 [P] Run firmware build validation with `platformio run` or record unavailable PlatformIO tooling in the final implementation validation notes
- [X] T058 [P] Run whitespace validation with `git diff --check`
- [ ] T059 Perform hardware visual verification using `specs/006-material-design/quickstart.md`, including the 15-foot/5-second home review and 5-minute setup completion review
- [X] T060 Verify at least 90% of reviewed interactive controls use documented component/state patterns and all current screens listed in `specs/006-material-design/contracts/screen_application.md` were updated or have documented exceptions in `docs/ui-reference.md`
- [X] T061 Verify repeated standard actions use icon-with-text buttons across `src/ui/Screens/NetworkListScreen.cpp`, `src/ui/Screens/ReceiverListScreen.cpp`, `src/ui/Screens/ReceiverStatusScreen.cpp`, `src/ui/Screens/SetupStatusScreen.cpp`, and `src/ui/Screens/ReceiverIpScreen.cpp`
- [X] T062 Verify Wi-Fi and receiver searching screens match title, message, icon, progress, and action placement in `src/ui/Screens/NetworkListScreen.cpp` and `src/ui/Screens/ReceiverListScreen.cpp`
- [X] T063 Verify no visible text overlap or clipped primary labels remain across all `src/ui/Screens/*.cpp`
- [X] T064 Verify `docs/ui-reference.md` satisfies `specs/006-material-design/contracts/validation_contract.md`

---

## Dependencies and Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies.
- **Foundational (Phase 2)**: Depends on Phase 1 and blocks all user stories.
- **User Stories (Phases 3-6)**: Depend on Phase 2 completion.
- **Polish (Phase 7)**: Depends on all desired user stories being complete.

### User Story Dependencies

- **User Story 1 (P1)**: Can start after Foundation; no dependency on other stories.
- **User Story 2 (P1)**: Can start after Foundation; no dependency on other stories.
- **User Story 3 (P2)**: Can start after Foundation; depends on shared helpers but not on US1/US2.
- **User Story 4 (P3)**: Can start after Foundation but should finish after screen behavior and exceptions are known.

### Within Each User Story

- Update shared helpers before migrating screen-specific code.
- Update screen code before documenting final exceptions.
- Preserve screen flow behavior while changing visual treatment.
- Validate each story at its checkpoint before moving to lower-priority work.

### Parallel Opportunities

- T004 and T005 can run in parallel with style file setup after T001-T002.
- T015 and T016 can run in parallel after shared helper decisions are encoded.
- US1 screen migration tasks T017-T025 touch separate screen files and can run in parallel if helper contracts are stable.
- US2 button/list/keyboard tasks can run in parallel by screen file after T010-T012 complete.
- US3 Wi-Fi and receiver searching tasks can run in parallel by screen pair after T013 is complete.
- Final validation tasks T056-T058 can run in parallel.

---

## Parallel Example: User Story 1

```text
Task: "Apply shared home typography and hierarchy rules in src/ui/Screens/HomeScreen.cpp"
Task: "Apply shared settings row and title styling in src/ui/Screens/SettingsScreen.cpp"
Task: "Apply shared calibration instruction and touch target styling in src/ui/Screens/CalibrationScreen.cpp"
Task: "Apply shared keyboard title and input field styling in src/ui/Screens/KeyboardScreen.cpp"
```

---

## Parallel Example: User Story 2

```text
Task: "Replace local Wi-Fi action button drawing with shared standard icon-with-text buttons in src/ui/Screens/NetworkListScreen.cpp"
Task: "Replace local receiver discovery action button drawing with shared standard icon-with-text buttons in src/ui/Screens/ReceiverListScreen.cpp"
Task: "Replace receiver verification failure action buttons with shared standard icon-with-text buttons in src/ui/Screens/ReceiverStatusScreen.cpp"
Task: "Align keyboard OK and Cancel controls to the same y-coordinate, height, visual center, and icon center in src/ui/Screens/KeyboardScreen.cpp"
```

---

## Parallel Example: User Story 3

```text
Task: "Render Wi-Fi searching with the shared searching pattern and immediate progress movement in src/ui/Screens/NetworkListScreen.cpp"
Task: "Render receiver searching with the shared searching pattern and immediate progress movement in src/ui/Screens/ReceiverListScreen.cpp"
Task: "Apply shared loading and failure treatments to Wi-Fi connection states in src/ui/Screens/SetupStatusScreen.cpp"
Task: "Apply shared loading, success, and failure treatments to receiver verification states in src/ui/Screens/ReceiverStatusScreen.cpp"
```

---

## Parallel Example: User Story 4

```text
Task: "Complete the shared component/style catalog scope and priority rules in docs/ui-reference.md"
Task: "Complete documented exception entries for keyboard icon-only controls and any remaining screen-specific deviations in docs/ui-reference.md"
Task: "Cross-check docs/ui-reference.md against specs/006-material-design/contracts/component_style_catalog.md"
Task: "Cross-check docs/ui-reference.md against specs/006-material-design/contracts/screen_application.md"
```

---

## Implementation Strategy

### MVP First

1. Complete Phase 1: Setup.
2. Complete Phase 2: Foundational.
3. Complete Phase 3: User Story 1.
4. Stop and validate all screens for coherent Material-compatible styling and home-screen readability.

### Incremental Delivery

1. Foundation ready: shared UI style primitives and catalog outline exist.
2. US1: every current screen uses the shared visual language at a coarse level.
3. US2: setup controls, lists, and keyboard behavior become consistent and touch-first.
4. US3: searching, loading, success, warning, failure, and unavailable states become consistent.
5. US4: durable UI guidance becomes complete enough for future contributors.

### Validation Strategy

1. Run each user story checkpoint independently.
2. Run `pre-commit run --all-files`.
3. Run `platformio run` when PlatformIO is available.
4. Run `git diff --check`.
5. Run hardware visual verification from `specs/006-material-design/quickstart.md`.
