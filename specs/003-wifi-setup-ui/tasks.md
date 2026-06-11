# Tasks: WiFi Setup UI

**Input**: Design documents from `/specs/003-wifi-setup-ui/`

**Prerequisites**: plan.md, spec.md

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization and basic structure

- [x] T001 [P] Verify project structure per implementation plan
- [x] T002 [P] Ensure WiFi.h and ArduinoJson are correctly referenced in platformio.ini

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core infrastructure that MUST be complete before ANY user story can be implemented

- [x] T003 [P] Implement `WiFiManager::scanNetworks()` support in `src/network/WiFiManager.cpp`
- [x] T004 [P] Update `ConfigStore` to support `wifiSsid` and `wifiPassword` fields in `src/storage/ConfigStore.cpp`
- [x] T005 [P] Create `NetworkListScreen` skeleton in `src/ui/Screens/NetworkListScreen.h`
- [x] T006 [P] Create `KeyboardScreen` skeleton in `src/ui/Screens/KeyboardScreen.h`

**Checkpoint**: Foundation ready - user story implementation can now begin

---

## Phase 3: User Story 1 - Network Scanning and Selection (Priority: P1) 🎯 MVP

**Goal**: Discover and select nearby SSIDs

**Independent Test**: Open the WiFi setup flow. Success if a scrollable list of SSIDs appears and a selection can be made.

### Implementation for User Story 1

- [x] T007 [US1] Implement `NetworkListScreen::draw()` with SSID list in `src/ui/Screens/NetworkListScreen.cpp`
- [x] T008 [US1] Implement non-blocking scan trigger and "Scanning..." spinner in `src/ui/Screens/NetworkListScreen.cpp`
- [x] T009 [US1] Implement touch selection logic for network items in `src/ui/Screens/NetworkListScreen.cpp`
- [x] T010 [US1] Implement "Manual Entry" button on `NetworkListScreen` as per clarification
- [x] T011 [US1] Trigger `NetworkListScreen` from the "UNCONFIGURED" Home Screen area in `src/ui/Screens/HomeScreen.cpp`

**Checkpoint**: User Story 1 delivers a functional network discovery list.

---

## Phase 4: User Story 2 - Password Entry (Priority: P1)

**Goal**: Enter credentials via on-screen keyboard

**Independent Test**: Select a network. Success if a QWERTY keyboard appears and typed characters populate the field.

### Implementation for User Story 2

- [x] T012 [P] [US2] Implement `KeyboardScreen` QWERTY layout in `src/ui/Screens/KeyboardScreen.cpp`
- [x] T013 [P] [US2] Ensure keyboard touch targets are >= 40x40px for accuracy (SC-002) in `src/ui/Screens/KeyboardScreen.cpp`
- [x] T014 [US2] Implement text input buffer and field display in `src/ui/Screens/KeyboardScreen.cpp`
- [x] T015 [US2] Implement "Show/Hide" password toggle as decided in research in `src/ui/Screens/KeyboardScreen.cpp`
- [x] T016 [US2] Pass selected SSID from `NetworkListScreen` to `KeyboardScreen`

**Checkpoint**: User Story 2 delivers a functional credential entry system.

---

## Phase 5: User Story 3 - Connection Feedback (Priority: P2)

**Goal**: Attempt connection and show success/fail status

**Independent Test**: Submit password. Success if "Connecting..." is shown followed by a return to Home Screen on success.

### Implementation for User Story 3

- [x] T017 [US3] Create `SetupStatusScreen` to display connection progress in `src/ui/Screens/SetupStatusScreen.cpp`
- [x] T018 [US3] Call `WiFiManager::connect()` and monitor status asynchronously in `src/ui/Screens/SetupStatusScreen.cpp`
- [x] T019 [US3] On SUCCESS: Persist credentials using `ConfigStore` and return to `HomeScreen`
- [x] T020 [US3] On FAILURE: Display error message and provide "Retry" button back to keyboard

---

## Phase 6: Polish and Cross-Cutting Concerns

- [x] T021 [P] Implement scroll scrolling/paging for long network lists in `src/ui/Screens/NetworkListScreen.cpp`
- [x] T022 [P] Add signal strength (RSSI) icons to the network list in `src/ui/Screens/NetworkListScreen.cpp`
- [x] T023 Run `pre-commit run --all` for final validation
- [x] T024 Perform final hardware-in-the-loop verification of the entire setup flow
