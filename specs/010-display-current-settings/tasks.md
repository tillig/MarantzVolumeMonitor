# Tasks: Display Current Settings

**Input**: Design documents from `/specs/010-display-current-settings/`

**Prerequisites**: `plan.md`, `spec.md`, `research.md`, `data-model.md`, `contracts/`, `quickstart.md`

**Tests**: No automated TDD task set was requested. Validation tasks are included from `quickstart.md` and `contracts/validation_contract.md`.

**Organization**: Tasks are grouped by user story so each story can be implemented and validated as an independent increment. The feature remains read-only, keeps labeled `Settings` navigation, and reuses existing network and storage managers.

## Phase 1: Setup (Shared Context)

**Purpose**: Confirm the implementation surface, validation scope, and documentation touchpoints before code changes.

- [X] T001 Review current-settings requirements in `specs/010-display-current-settings/spec.md` and implementation constraints in `specs/010-display-current-settings/plan.md`
- [X] T002 [P] Review current settings-navigation, Wi-Fi state, and receiver state sources in `src/ui/Screens/SettingsScreen.cpp`, `src/network/WiFiManager.cpp`, `src/network/MarantzClient.cpp`, and `src/network/ReceiverDiscovery.cpp`
- [X] T003 [P] Review validation and documentation expectations in `specs/010-display-current-settings/contracts/validation_contract.md`, `specs/010-display-current-settings/quickstart.md`, `docs/ui-reference.md`, and `README.md`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Add the shared screen primitives and data-access helpers required by all user stories.

**⚠️ CRITICAL**: No user story work should begin until this phase is complete.

- [X] T004 Create the `CurrentSettingsScreen` class scaffold and shared view-state declarations in `src/ui/Screens/CurrentSettingsScreen.h` and `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T005 [P] Extend current Wi-Fi state access with signal-strength retrieval helpers in `src/network/WiFiManager.h` and `src/network/WiFiManager.cpp`
- [X] T006 [P] Add receiver identity lookup helpers for the configured receiver IP in `src/network/ReceiverDiscovery.h` and `src/network/ReceiverDiscovery.cpp`
- [X] T007 Add shared read-only status presentation and settings-pagination helper declarations in `src/ui/MaterialStyle.h`
- [X] T008 Implement the shared read-only status presentation and settings-pagination helpers in `src/ui/MaterialStyle.cpp`

**Checkpoint**: Shared screen, Wi-Fi, receiver identity, and Material-style helpers are ready for story work.

---

## Phase 3: User Story 1 - Open A Current Settings Overview (Priority: P1) 🎯 MVP

**Goal**: Add a first-entry `Current Settings` destination in `Settings` that opens a separate read-only overview screen and returns to `Settings` with a single `OK` action.

**Independent Test**: Open `Settings`, confirm `Current Settings` appears first, open it, confirm a read-only overview screen appears, and press `OK` to return to `Settings`.

### Implementation for User Story 1

- [X] T009 [US1] Add `Current Settings` as the first settings destination and preserve the existing `Settings` title and bottom `OK` action in `src/ui/Screens/SettingsScreen.cpp`
- [X] T010 [US1] Wire `Settings` touch handling to open `CurrentSettingsScreen` and return from `CurrentSettingsScreen` to `Settings` in `src/ui/Screens/SettingsScreen.cpp` and `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T011 [US1] Implement the initial read-only `Current Settings` screen structure with separate Wi-Fi and Receiver sections and a single `OK` action in `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T012 [US1] Validate the open, read-only, and dismiss behavior against `specs/010-display-current-settings/contracts/current_settings_screen_contract.md` and `specs/010-display-current-settings/quickstart.md`

**Checkpoint**: User Story 1 is complete and the MVP overview flow is independently demonstrable.

---

## Phase 4: User Story 2 - Review Wi-Fi And Receiver Details Together (Priority: P1)

**Goal**: Show saved and live Wi-Fi and receiver details together on the overview screen and keep them updated while the screen remains visible.

**Independent Test**: Open `Current Settings` on a configured device and verify that Wi-Fi SSID, monitor IP, Wi-Fi signal, receiver IP, receiver power, and receiver identity appear together and update while the screen remains open.

### Implementation for User Story 2

- [X] T013 [US2] Load saved Wi-Fi SSID and receiver IP into `CurrentSettingsScreen` view state from existing configuration access in `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T014 [US2] Render current Wi-Fi IP address, Wi-Fi strength graphic, and numeric RSSI in `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T015 [US2] Load current receiver power status and receiver identity on screen open using `src/network/MarantzClient.h`, `src/network/MarantzClient.cpp`, `src/network/ReceiverDiscovery.h`, and `src/network/ReceiverDiscovery.cpp` from `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T016 [US2] Add the 1-second live refresh loop and receiver-identity session cache behavior in `src/ui/Screens/CurrentSettingsScreen.h` and `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T017 [US2] Validate configured-device content and live-update scenarios against `specs/010-display-current-settings/contracts/current_settings_screen_contract.md` and `specs/010-display-current-settings/contracts/validation_contract.md`

**Checkpoint**: User Story 2 delivers the full read-only current-state overview for configured devices.

---

## Phase 5: User Story 3 - Understand Missing Or Unavailable Data (Priority: P2)

**Goal**: Make missing setup, unavailable live status, and powered-off receiver state explicit so the screen remains trustworthy during troubleshooting.

**Independent Test**: Open `Current Settings` with missing Wi-Fi configuration, missing receiver configuration, disconnected Wi-Fi, and receiver-unavailable or powered-off states, then verify each state is labeled correctly without stale confirmed data.

### Implementation for User Story 3

- [X] T018 [US3] Implement `setup required` and `unavailable` formatting for Wi-Fi SSID, monitor IP, Wi-Fi graphic, and RSSI values in `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T019 [US3] Implement receiver `not configured`, `unavailable`, `powered off`, and `powered on` state presentation in `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T020 [US3] Clear or replace stale live values whenever refreshes become unavailable so the screen does not imply outdated confirmed state in `src/ui/Screens/CurrentSettingsScreen.cpp`
- [X] T021 [US3] Validate missing-configuration and unavailable-state scenarios against `specs/010-display-current-settings/contracts/current_settings_screen_contract.md`, `specs/010-display-current-settings/contracts/validation_contract.md`, and `specs/010-display-current-settings/quickstart.md`

**Checkpoint**: User Story 3 makes the overview reliable for recovery and troubleshooting states.

---

## Phase 6: User Story 4 - Keep Settings Navigation Readable (Priority: P3)

**Goal**: Keep the expanded `Settings` menu readable and reachable with labeled Material-compatible navigation even when pagination is required.

**Independent Test**: Review the `Settings` menu with the new destination present and verify that all destinations remain labeled, tappable, and reachable, using `PREV` and `NEXT` if multiple pages are needed.

### Implementation for User Story 4

- [X] T022 [US4] Add settings-page state and destination ordering helpers for the expanded menu in `src/ui/Screens/SettingsScreen.h` and `src/ui/Screens/SettingsScreen.cpp`
- [X] T023 [US4] Render paginated labeled settings rows and `PREV`/`NEXT` controls while preserving the bottom `OK` action in `src/ui/Screens/SettingsScreen.cpp`
- [X] T024 [US4] Update `Settings` touch hit-testing so `Current Settings`, `Wi-Fi Setup`, `Receiver Setup`, and `Touch Calibration` remain reachable across pages in `src/ui/Screens/SettingsScreen.cpp`
- [X] T025 [US4] Validate settings-menu readability and destination-reachability scenarios against `specs/010-display-current-settings/contracts/settings_menu_navigation_contract.md` and `specs/010-display-current-settings/contracts/validation_contract.md`

**Checkpoint**: User Story 4 preserves readable labeled navigation for the expanded `Settings` menu.

---

## Phase 7: Polish and Cross-Cutting Concerns

**Purpose**: Update durable documentation and run the complete validation sequence for the feature.

- [X] T026 [P] Document the `Current Settings` screen pattern, Wi-Fi signal presentation rule, and paginated `Settings` navigation in `docs/ui-reference.md`
- [X] T027 [P] Add a concise user-facing note about the `Current Settings` overview in `README.md`
- [X] T028 Run `pre-commit run --all-files` from the repository root and resolve any issues reported for `specs/010-display-current-settings/`, `src/ui/Screens/CurrentSettingsScreen.cpp`, `src/ui/Screens/SettingsScreen.cpp`, `src/network/WiFiManager.cpp`, `src/network/ReceiverDiscovery.cpp`, `docs/ui-reference.md`, or `README.md`
- [X] T029 Run `git diff --check` from the repository root and resolve whitespace issues in `specs/010-display-current-settings/`, `src/ui/Screens/CurrentSettingsScreen.cpp`, `src/ui/Screens/SettingsScreen.cpp`, `src/network/WiFiManager.cpp`, `src/network/ReceiverDiscovery.cpp`, `docs/ui-reference.md`, or `README.md`
- [X] T030 Run `/Users/travis.illig/.platformio/penv/bin/platformio run` to validate the firmware build defined by `platformio.ini`
- [X] T031 Perform the complete hardware verification matrix from `specs/010-display-current-settings/contracts/validation_contract.md`
- [X] T032 Review the final diff against `specs/010-display-current-settings/plan.md` and `specs/010-display-current-settings/research.md` to confirm no editable settings, config schema changes, icon-only settings navigation, or extra dependencies were added outside scope

---

## Dependencies and Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies.
- **Foundational (Phase 2)**: Depends on Setup completion and blocks all user stories.
- **User Story 1 (Phase 3)**: Depends on Foundational and delivers the MVP overview flow.
- **User Story 2 (Phase 4)**: Depends on Foundational and is safest after User Story 1 because it fills the overview with live and saved data.
- **User Story 3 (Phase 5)**: Depends on Foundational and builds on the live-data display from User Story 2.
- **User Story 4 (Phase 6)**: Depends on Foundational and can be developed after User Story 1, but final validation should include the full overview destination set.
- **Polish (Phase 7)**: Depends on all desired user stories being complete.

### User Story Dependencies

- **US1 (P1)**: No dependency on other user stories; delivers the MVP `Current Settings` navigation flow.
- **US2 (P1)**: Requires the overview screen from US1 to display the combined Wi-Fi and receiver details.
- **US3 (P2)**: Requires the overview data presentation from US2 so missing and unavailable states can replace or refine live values correctly.
- **US4 (P3)**: Requires the expanded settings destination set from US1 and final validation against the completed menu.

### Parallel Opportunities

- T002 and T003 can run in parallel during Setup.
- T005 and T006 can run in parallel during Foundational because they modify different network-layer files.
- T026 and T027 can run in parallel during Polish.
- Most implementation tasks in US1 through US4 touch `src/ui/Screens/SettingsScreen.cpp` or `src/ui/Screens/CurrentSettingsScreen.cpp`, so avoid running those tasks concurrently in the same worktree.

---

## Parallel Example: Foundational Helpers

```text
Task: "Extend current Wi-Fi state access with signal-strength retrieval helpers in src/network/WiFiManager.h and src/network/WiFiManager.cpp"
Task: "Add receiver identity lookup helpers for the configured receiver IP in src/network/ReceiverDiscovery.h and src/network/ReceiverDiscovery.cpp"
```

## Parallel Example: Polish Documentation

```text
Task: "Document the Current Settings screen pattern, Wi-Fi signal presentation rule, and paginated Settings navigation in docs/ui-reference.md"
Task: "Add a concise user-facing note about the Current Settings overview in README.md"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup.
2. Complete Phase 2: Foundational.
3. Complete Phase 3: User Story 1.
4. Stop and validate that `Current Settings` appears first, opens as a read-only screen, and returns to `Settings` with `OK`.

### Incremental Delivery

1. Add US1 to deliver the core overview navigation flow.
2. Add US2 to populate the overview with combined current Wi-Fi and receiver details plus live refresh.
3. Add US3 to harden missing and unavailable state handling.
4. Add US4 to finalize readable labeled navigation for the expanded `Settings` menu.
5. Complete documentation, static validation, firmware build, and hardware verification.

### Scope Guard

- Do not add editable settings or setup-launch actions inside `Current Settings`.
- Do not change the persisted `config.json` schema just to store receiver identity text.
- Do not switch `Settings` to icon-only navigation.
- Do not add new libraries or hardware configuration changes unless validation proves they are necessary for the planned behavior.
