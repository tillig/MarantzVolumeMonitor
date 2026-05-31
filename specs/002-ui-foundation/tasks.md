# Tasks: UI Foundation Integration

**Input**: Design documents from `/specs/002-ui-foundation/`

**Prerequisites**: plan.md, spec.md

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization and basic structure

- [x] T001 [P] Verify project structure per implementation plan
- [x] T002 [P] Configure PlatformIO for ESP32 and VSPI pins (Done in spike cleanup)

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core infrastructure that MUST be complete before ANY user story can be implemented

- [x] T003 [P] Define UI Constants (Colors) in `src/ui/DisplayManager.h` per project standards
- [x] T004 [P] Update `Screen` base class in `src/ui/Screens/Screen.h` for ESP32 compatibility
- [x] T005 Update `ScreenManager` in `src/ui/ScreenManager.cpp` to handle initial screen setup

**Checkpoint**: Foundation ready - user story implementation can now begin

---

## Phase 3: User Story 1 - Clean UI Layer Initialization (Priority: P1) 🎯 MVP

**Goal**: Encapsulate hardware initialization in manager classes

**Independent Test**: Build and upload. Success if Serial logs show successful initialization of both managers.

### Implementation for User Story 1

- [x] T006 [P] [US1] Implement ESP32-specific `TFT_eSPI` initialization in `src/ui/DisplayManager.cpp`
- [x] T007 [P] [US1] Implement calibration mapping logic in `src/ui/TouchManager.cpp` using derived constants
- [x] T008 [US1] Refactor `main.cpp` to call `DisplayManager::getInstance().begin()` and `TouchManager::getInstance().begin()`

**Checkpoint**: At this point, the hardware is initialized via the architecture, but nothing is drawn yet.

---

## Phase 4: User Story 2 - Basic HomeScreen Rendering (Priority: P1)

**Goal**: Display "Hello World" using the Screen architecture

**Independent Test**: Boot the device. Success if "Hello World" appears on a black background.

### Implementation for User Story 2

- [x] T009 [P] [US2] Implement `HomeScreen::draw()` in `src/ui/Screens/HomeScreen.cpp`
- [x] T010 [US2] Implement 3 Layout Presets in `HomeScreen` (Classic, Modern, Minimal)
- [x] T011 [US2] Create touch-based "Prototyping Loop" to cycle presets for visual review
- [x] T012 [US2] Configure `ScreenManager` to set `HomeScreen` as the active screen on boot
- [x] T013 [US2] Update `main.cpp` to call `ScreenManager::getInstance().update()` in the loop

**Checkpoint**: User Story 2 delivers a functional visual output using the formal architecture.

---

## Phase 5: User Story 3 - Interactive Feedback (Priority: P2)

**Goal**: Restore touch interactivity within the HomeScreen class

**Independent Test**: Touch the screen. Success if the text color changes.

### Implementation for User Story 3

- [x] T012 [P] [US3] Implement `HomeScreen::handleTouch()` to detect touches and cycle colors
- [x] T013 [US3] Ensure `ScreenManager` correctly forwards touch events to the active screen
- [x] T014 [US3] Final hardware verification of end-to-end flow

---

## Phase 6: Polish & Cross-Cutting Concerns

- [x] T015 [P] Remove all temporary hardware spike code from `main.cpp`
- [x] T016 [P] Update `README.md` with the new architecture status
- [x] T017 Run `pre-commit run --all` for final validation
