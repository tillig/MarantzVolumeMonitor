# Tasks: Display Volume Level

**Input**: Design documents from `/specs/007-display-volume-level/`

**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, contracts/

**Tests**: No separate automated test tasks were generated because the feature specification does not explicitly request TDD or new automated tests. Static validation and hardware verification remain required.

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3)
- Include exact file paths in descriptions

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Confirm the feature workspace and review existing home-screen contracts before editing implementation files.

- [X] T001 Review `specs/007-display-volume-level/spec.md`, `specs/007-display-volume-level/plan.md`, and `specs/007-display-volume-level/contracts/volume_motion_contract.md`
- [X] T002 Inspect current home-screen and receiver-status implementation in `src/ui/Screens/HomeScreen.h`, `src/ui/Screens/HomeScreen.cpp`, `src/network/MarantzClient.h`, and `src/network/MarantzClient.cpp`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Establish shared home-screen state and animation scaffolding required by all user stories.

**⚠️ CRITICAL**: No user story work can begin until this phase is complete

- [X] T003 Add explicit home-display state and animation tracking members to `src/ui/Screens/HomeScreen.h`
- [X] T004 Implement receiver-state classification, shared redraw-region helpers, and non-blocking animation tick helpers in `src/ui/Screens/HomeScreen.cpp`
- [X] T005 Confirm or adjust `MarantzStatus` handling in `src/network/MarantzClient.h` and `src/network/MarantzClient.cpp` so `HomeScreen` can distinguish live, powered-off, and unavailable states

**Checkpoint**: Foundation ready - user story implementation can now begin in parallel

---

## Phase 3: User Story 1 - Read The Current Volume At A Glance (Priority: P1) 🎯 MVP

**Goal**: Present live volume as the dominant readable home-screen value with one decimal place and smooth synchronized gauge/value motion.

**Independent Test**: With a configured live receiver, confirm the home screen shows volume with one decimal place, and small/large volume changes animate the gauge and numeric value together without blocking the UI.

### Implementation for User Story 1

- [X] T006 [US1] Update the live home-screen layout and one-decimal value formatting in `src/ui/Screens/HomeScreen.cpp`
- [X] T007 [US1] Implement synchronized gauge arc and numeric count animation in `src/ui/Screens/HomeScreen.cpp`
- [X] T008 [US1] Implement mid-animation retargeting and end-of-motion deceleration behavior in `src/ui/Screens/HomeScreen.cpp`
- [X] T009 [US1] Restrict live-animation redraws to dirty regions in `src/ui/Screens/HomeScreen.cpp`

**Checkpoint**: At this point, User Story 1 should be fully functional and testable independently

---

## Phase 4: User Story 2 - Understand Why Live Volume Is Not Shown (Priority: P2)

**Goal**: Distinguish powered-off and unavailable receiver states without showing stale live volume, while keeping Settings accessible for receiver reconfiguration.

**Independent Test**: Verify the home screen once with a reachable powered-off receiver and once with an unavailable receiver; each state should show different messaging, hide stale live volume, and keep Settings reachable.

### Implementation for User Story 2

- [X] T010 [US2] Implement distinct powered-off and unavailable home-screen state rendering in `src/ui/Screens/HomeScreen.cpp`
- [X] T011 [US2] Preserve Settings access and touch behavior from powered-off and unavailable states in `src/ui/Screens/HomeScreen.cpp`
- [X] T012 [US2] Ensure state transitions back to live presentation occur automatically when valid active receiver status resumes in `src/ui/Screens/HomeScreen.cpp`
- [X] T013 [US2] Preserve the no-receiver-configured and Wi-Fi-setup-required home-screen entry paths in `src/ui/Screens/HomeScreen.cpp`

**Checkpoint**: At this point, User Stories 1 AND 2 should both work independently

---

## Phase 5: User Story 3 - Keep The Display Calm While Volume Changes (Priority: P3)

**Goal**: Make motion feel calm and appliance-appropriate, with no full-screen flashing and no decorative bounce.

**Independent Test**: Trigger rapid consecutive volume updates and visually confirm calm motion, smooth retargeting, no visible full-screen flashing, and stable unaffected regions during animation.

### Implementation for User Story 3

- [X] T014 [US3] Tune motion timing and settling behavior for calm Material-compatible standard motion in `src/ui/Screens/HomeScreen.cpp`
- [X] T015 [US3] Refine dirty-region redraw bounds for the gauge, value, and changed text regions in `src/ui/Screens/HomeScreen.cpp`
- [X] T016 [US3] Verify that idle live state and non-live state rendering do not introduce unnecessary motion in `src/ui/Screens/HomeScreen.cpp`

**Checkpoint**: All user stories should now be independently functional

---

## Phase 6: Polish and Cross-Cutting Concerns

**Purpose**: Update durable docs and run final validation across all stories

- [X] T017 [P] Update durable home-screen behavior guidance in `docs/ui-reference.md`
- [X] T018 [P] Update user-facing receiver-state behavior notes in `README.md`
- [X] T019 Run static validation with `pre-commit run --all-files`, `/Users/travis.illig/.platformio/penv/bin/platformio run`, and `git diff --check`
- [X] T020 Run the hardware verification steps from `specs/007-display-volume-level/quickstart.md`

---

## Dependencies and Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies - can start immediately
- **Foundational (Phase 2)**: Depends on Setup completion - BLOCKS all user stories
- **User Stories (Phase 3+)**: All depend on Foundational phase completion
  - User stories can then proceed in priority order; US2 may run alongside US1, while US3 follows US1 animation scaffolding
- **Polish (Phase 6)**: Depends on all desired user stories being complete

### User Story Dependencies

- **User Story 1 (P1)**: Can start after Foundational (Phase 2) - No dependencies on other stories
- **User Story 2 (P2)**: Can start after Foundational (Phase 2) - Builds on shared home-state scaffolding but should remain independently testable
- **User Story 3 (P3)**: Depends on User Story 1 completion because it refines the live animation behavior introduced there, but remains independently testable once that scaffolding exists

### Within Each User Story

- Shared scaffolding before story-specific rendering
- Live presentation before non-live state refinements
- State rendering before documentation and final validation

### Parallel Opportunities

- `T017` and `T018` can run in parallel after implementation stabilizes because they update different documentation files.
- Final validation tasks remain sequential because each depends on the complete implementation state.

---

## Parallel Example: Documentation Polish

```bash
# Launch durable docs updates together after implementation is complete:
Task: "Update durable home-screen behavior guidance in docs/ui-reference.md"
Task: "Update user-facing receiver-state behavior notes in README.md"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup
2. Complete Phase 2: Foundational
3. Complete Phase 3: User Story 1
4. **STOP and VALIDATE**: Verify live volume readability, one-decimal formatting, and synchronized animation on hardware

### Incremental Delivery

1. Complete Setup + Foundational → animation and state scaffolding ready
2. Add User Story 1 → Validate live volume display and motion
3. Add User Story 2 → Validate powered-off and unavailable state differentiation
4. Add User Story 3 → Validate motion calmness, partial redraw behavior, and no flashing
5. Finish documentation and full validation

### Parallel Team Strategy

With multiple developers:

1. Team completes Setup + Foundational together
2. Once Foundational is done:
   - Developer A: User Story 1 live volume presentation and animation
   - Developer B: User Story 2 non-live state rendering and Settings access
   - Developer C: Prepare follow-up polish for User Story 3, then implement it after User Story 1 animation scaffolding lands
3. Rejoin for documentation and final validation

---

## Notes

- All tasks follow the required checklist format with IDs and file paths.
- `[P]` is used only where tasks can be completed independently without file conflicts.
- Hardware verification is mandatory because the constitution requires it for UI and networking changes.
