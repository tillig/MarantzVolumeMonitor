# Tasks: Receiver Detection and Configuration

**Input**: Design documents from `/specs/004-receiver-detection-config/`

**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/

**Tests**: No separate test-code tasks are generated because the feature specification requests hardware validation, not TDD. Validation tasks are included in the final phase.

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (US1, US2, US3)
- Include exact file paths in descriptions

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Confirm the existing project structure and dependencies support receiver discovery without new unmanaged libraries.

- [x] T001 [P] Verify ESP32 networking dependencies include `WiFi.h`, `WiFiUdp.h`, and `HTTPClient` availability through `platformio.ini`
- [x] T002 [P] Review durable architecture and UI references in `docs/architecture.md` and `docs/ui-reference.md`
- [x] T003 [P] Review receiver setup contracts in `specs/004-receiver-detection-config/contracts/receiver_setup_flow.md`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core models, validation helpers, and routing hooks that MUST be complete before user story implementation.

**CRITICAL**: No user story work can begin until this phase is complete.

- [x] T004 Create `ReceiverCandidate`, `ReceiverSource`, `ReceiverDiscoveryState`, and `VerificationResult` declarations in `src/network/ReceiverDiscovery.h`
- [x] T005 Create empty `ReceiverDiscovery` implementation shell in `src/network/ReceiverDiscovery.cpp`
- [x] T006 Add IPv4 validation helper declaration to `src/network/ReceiverDiscovery.h`
- [x] T007 Implement IPv4 validation helper in `src/network/ReceiverDiscovery.cpp`
- [x] T008 Add live receiver-status verification helper declaration to `src/network/MarantzClient.h`
- [x] T009 Implement live receiver-status verification helper using existing status request behavior in `src/network/MarantzClient.cpp`
- [x] T010 [P] Add `ReceiverListScreen` class skeleton in `src/ui/Screens/ReceiverListScreen.h`
- [x] T011 [P] Add `ReceiverIpScreen` class skeleton in `src/ui/Screens/ReceiverIpScreen.h`
- [x] T012 [P] Add `ReceiverStatusScreen` class skeleton in `src/ui/Screens/ReceiverStatusScreen.h`

**Checkpoint**: Foundation ready - receiver discovery, manual entry, and verification stories can now be implemented.

---

## Phase 3: User Story 1 - Discover Available Receivers (Priority: P1) MVP

**Goal**: Let the user discover compatible local receivers and select one from a list showing receiver name and IPv4 address.

**Independent Test**: Open receiver setup on a network with a compatible receiver. Success if discovery shows progress, results list each receiver name and IPv4 address, and no-results state offers retry and manual entry.

### Implementation for User Story 1

- [x] T013 [US1] Implement SSDP M-SEARCH start logic in `src/network/ReceiverDiscovery.cpp`
- [x] T014 [US1] Implement non-blocking UDP response polling and 15-second timeout in `src/network/ReceiverDiscovery.cpp`
- [x] T015 [US1] Parse receiver name and IPv4 address from SSDP responses in `src/network/ReceiverDiscovery.cpp`
- [x] T016 [US1] Deduplicate discovered receivers by IPv4 address in `src/network/ReceiverDiscovery.cpp`
- [x] T017 [US1] Implement discovery progress, results, empty state, retry action, and manual-entry action rendering in `src/ui/Screens/ReceiverListScreen.cpp`
- [x] T018 [US1] Implement touch selection, retry, manual-entry, and back handling in `src/ui/Screens/ReceiverListScreen.cpp`
- [x] T019 [US1] Route Wi-Fi-connected receiver-required Home Screen state to `ReceiverListScreen` in `src/ui/Screens/HomeScreen.cpp`
- [x] T020 [US1] Route Settings Screen Receiver Setup item to `ReceiverListScreen` in `src/ui/Screens/SettingsScreen.cpp`

**Checkpoint**: User Story 1 delivers automatic receiver discovery and selectable discovered candidates.

---

## Phase 4: User Story 2 - Configure a Receiver Manually (Priority: P1)

**Goal**: Let the user manually enter an IPv4 receiver address, reject invalid values, and continue to verification.

**Independent Test**: Open manual receiver setup, enter invalid values, and confirm they are rejected before verification. Enter a valid IPv4 address and confirm the flow proceeds to receiver verification.

### Implementation for User Story 2

- [x] T021 [US2] Implement IPv4 input rendering and cursor display in `src/ui/Screens/ReceiverIpScreen.cpp`
- [x] T022 [US2] Implement numeric keypad touch handling for octets, dots, backspace, cancel, and submit in `src/ui/Screens/ReceiverIpScreen.cpp`
- [x] T023 [US2] Reject empty, malformed, hostname, URL, custom-port, and out-of-range octet input using validation from `src/network/ReceiverDiscovery.cpp`
- [x] T024 [US2] Show recoverable validation errors without leaving manual entry in `src/ui/Screens/ReceiverIpScreen.cpp`
- [x] T025 [US2] Route valid manual IPv4 submissions to `ReceiverStatusScreen` in `src/ui/Screens/ReceiverIpScreen.cpp`
- [x] T026 [US2] Ensure `ReceiverListScreen` manual-entry action opens `ReceiverIpScreen` in `src/ui/Screens/ReceiverListScreen.cpp`

**Checkpoint**: User Story 2 delivers IPv4-only manual receiver configuration.

---

## Phase 5: User Story 3 - Save and Update Receiver Configuration (Priority: P2)

**Goal**: Verify selected receivers with a live status request, save successful receiver configuration, preserve existing configuration on failures, and show offline state later without clearing saved data.

**Independent Test**: Select or enter a reachable receiver and confirm it is saved after verification. Then attempt an unreachable replacement and confirm the previous receiver IP remains saved. Power off the saved receiver and confirm offline state appears without clearing config.

### Implementation for User Story 3

- [x] T027 [US3] Implement verification progress and result state rendering in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [x] T028 [US3] Start live receiver-status verification for selected discovered candidates in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [x] T029 [US3] Start live receiver-status verification for manual IPv4 candidates in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [x] T030 [US3] Persist verified `receiverIp` through `ConfigStore` only after verification success in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [x] T031 [US3] Preserve existing `receiverIp` when replacement verification fails in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [x] T032 [US3] Show recoverable verification failure actions for retry, manual entry, and discovery return in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [x] T033 [US3] Update Home Screen offline receiver copy and touch routing for saved unreachable receivers in `src/ui/Screens/HomeScreen.cpp`
- [x] T034 [US3] Ensure `ConfigStore` continues loading and saving existing `receiverIp` without migration changes in `src/storage/ConfigStore.cpp`

**Checkpoint**: User Story 3 delivers verified persistence, replacement safety, and offline saved-receiver behavior.

---

## Phase 6: Polish & Cross-Cutting Concerns

**Purpose**: Final validation, documentation alignment, and hardware verification across the complete receiver setup flow.

- [x] T035 [P] Update durable reference documentation in `docs/` if receiver setup changes architecture, UI, or hardware assumptions
- [x] T036 [P] Update user-facing setup documentation if receiver setup behavior changes in `README.md`
- [x] T037 Run `pre-commit run --all-files` from the repository root
- [x] T038 Run `platformio run` from the repository root
- [x] T039 Perform automatic discovery hardware validation using `specs/004-receiver-detection-config/quickstart.md`
- [x] T040 Perform manual IPv4 and replacement-failure hardware validation using `specs/004-receiver-detection-config/quickstart.md`

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies - can start immediately.
- **Foundational (Phase 2)**: Depends on Setup completion and blocks all user stories.
- **User Stories (Phase 3+)**: Depend on Foundational completion.
- **Polish (Phase 6)**: Depends on all desired user stories being complete.

### User Story Dependencies

- **User Story 1 (P1)**: Starts after Foundational and provides discovered receiver selection.
- **User Story 2 (P1)**: Starts after Foundational and can be implemented independently of discovery results except for the manual-entry link task.
- **User Story 3 (P2)**: Depends on candidate input from US1 or US2 and should follow at least one P1 path.

### Within Each User Story

- Network helpers before UI integration.
- Input validation before verification routing.
- Verification before persistence.
- Story checkpoint validation before moving to the next priority.

### Parallel Opportunities

- T001, T002, and T003 can run in parallel.
- T010, T011, and T012 can run in parallel after T004.
- US1 rendering and Settings/Home routing can proceed after `ReceiverListScreen` skeleton exists, while SSDP parsing is implemented in `ReceiverDiscovery`.
- US2 keypad rendering and validation-error UI can proceed in parallel with Foundational IPv4 helper integration once `ReceiverIpScreen` exists.
- T035 and T036 can run in parallel after implementation behavior is stable.

---

## Parallel Example: User Story 1

```text
Task: "Implement SSDP M-SEARCH start logic in src/network/ReceiverDiscovery.cpp"
Task: "Implement discovery progress, results, empty state, retry action, and manual-entry action rendering in src/ui/Screens/ReceiverListScreen.cpp"
Task: "Route Settings Screen Receiver Setup item to ReceiverListScreen in src/ui/Screens/SettingsScreen.cpp"
```

---

## Parallel Example: User Story 2

```text
Task: "Implement IPv4 input rendering and cursor display in src/ui/Screens/ReceiverIpScreen.cpp"
Task: "Implement numeric keypad touch handling for octets, dots, backspace, cancel, and submit in src/ui/Screens/ReceiverIpScreen.cpp"
Task: "Show recoverable validation errors without leaving manual entry in src/ui/Screens/ReceiverIpScreen.cpp"
```

---

## Parallel Example: User Story 3

```text
Task: "Implement verification progress and result state rendering in src/ui/Screens/ReceiverStatusScreen.cpp"
Task: "Update Home Screen offline receiver copy and touch routing for saved unreachable receivers in src/ui/Screens/HomeScreen.cpp"
Task: "Ensure ConfigStore continues loading and saving existing receiverIp without migration changes in src/storage/ConfigStore.cpp"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup.
2. Complete Phase 2: Foundational.
3. Complete Phase 3: User Story 1.
4. Stop and validate discovery progress, results, no-results, retry, and manual-entry routing.

### Incremental Delivery

1. Add US1 automatic discovery and selection.
2. Add US2 manual IPv4 fallback.
3. Add US3 verification, persistence, replacement safety, and offline behavior.
4. Run Polish validation and hardware quickstart scenarios.

### Single-Developer Strategy

Work sequentially by phase and checkpoint. Do not start persistence behavior until at least one candidate source from US1 or US2 can reach `ReceiverStatusScreen`.

---

## Notes

- [P] tasks use different files or can proceed without depending on incomplete tasks.
- Each user story remains traceable to the spec and contracts.
- Screens must delegate network work to `ReceiverDiscovery` or `MarantzClient`.
- Preserve the existing Wi-Fi setup flow while adding receiver setup.
- Commit after each phase or logical group.
