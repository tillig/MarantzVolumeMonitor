# Tasks: Bitmap Icon Assets

**Input**: Design documents from `/specs/005-bitmap-icons/`

**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/, quickstart.md

**Tests**: Automated test tasks are not included because the specification does not request TDD or
automated tests for this embedded UI asset feature. Validation tasks are included in the final phase.

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of
each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel with other marked tasks in the same phase when assigned to different
  files.
- **[Story]**: Maps task to a user story from `spec.md`.
- Every task includes an exact file path.

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Create the durable asset and generator structure used by every story.

- [X] T001 Create icon asset directories in `assets/icons/source`, `assets/icons/generated`, `scripts/icons`, and `src/ui/assets`
- [X] T002 Create initial icon inventory table with required columns in `assets/icons/inventory.md`
- [X] T003 [P] Add empty generated icon data shell in `src/ui/assets/IconBitmaps.h`
- [X] T004 [P] Add empty generated icon data implementation shell in `src/ui/assets/IconBitmaps.cpp`
- [X] T005 [P] Create deterministic icon generation entry point in `scripts/icons/generate-icons.py`
- [X] T006 [P] Create generated asset README with regeneration rules in `assets/icons/generated/README.md`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Establish icon rendering, source asset conventions, and inventory records before screen
work begins.

**Critical**: No user story work should begin until this phase is complete.

- [X] T007 Define shared icon metadata structures and bitmap declarations in `src/ui/assets/IconBitmaps.h`
- [X] T008 Implement placeholder-safe generated bitmap definitions in `src/ui/assets/IconBitmaps.cpp`
- [X] T009 Create UI-layer icon renderer API in `src/ui/IconRenderer.h`
- [X] T010 Implement monochrome bitmap mask rendering in `src/ui/IconRenderer.cpp`
- [X] T011 Add icon color constants for lit green and inactive dark gray in `src/ui/DisplayManager.h`
- [X] T012 Add Material Symbol source acquisition list for required generic icons in `assets/icons/inventory.md`
- [X] T013 Add owner-approved Dolby and DTS source records with approval note in `assets/icons/inventory.md`
- [X] T014 Implement deterministic conversion of SVG sources into firmware bitmap masks in `scripts/icons/generate-icons.py`
- [X] T015 Generate initial firmware icon masks for required assets in `src/ui/assets/IconBitmaps.h` and `src/ui/assets/IconBitmaps.cpp`
- [X] T016 Generate visual preview outputs for required assets in `assets/icons/generated/`

**Checkpoint**: Foundation ready. Each user story can now draw inventory-backed bitmap icons.

---

## Phase 3: User Story 1 - Recognize Primary Controls at a Distance (Priority: P1)

**Goal**: Users can recognize primary controls and status indicators from normal room distance, with
the home settings control using a bitmap icon instead of a manually drawn gear.

**Independent Test**: Review the home, settings, Wi-Fi setup, and receiver setup screens. Confirm
every primary control and persistent status indicator has a high-contrast inventory-backed icon, and
the top-right home settings control is no longer manually drawn.

### Implementation for User Story 1

- [X] T017 [P] [US1] Add `settings` Material Symbol source asset in `assets/icons/source/settings.svg`
- [X] T018 [P] [US1] Add settings icon inventory row and generated symbol mapping in `assets/icons/inventory.md`
- [X] T019 [US1] Replace the manually drawn home gear with the `settings` bitmap icon in `src/ui/Screens/HomeScreen.cpp`
- [X] T020 [US1] Remove obsolete home gear primitive drawing helper code in `src/ui/Screens/HomeScreen.cpp`
- [X] T021 [US1] Confirm home settings touch target remains unchanged in `src/ui/Screens/HomeScreen.cpp`
- [X] T022 [P] [US1] Document home settings bitmap icon usage in `docs/ui-reference.md`

**Checkpoint**: User Story 1 is independently testable on the home screen.

---

## Phase 4: User Story 2 - Show Audio Family Icons on the Home Screen (Priority: P1)

**Goal**: Dolby, DTS, PCM, and Other render as home-screen audio-family icons with green lit active
state and dark gray inactive state.

**Independent Test**: Display the home screen with Dolby, DTS, PCM, Other, and unavailable receiver
states. Confirm all four audio-family areas render as icons, not text, with exactly one green active
icon for a valid classified mode and all icons inactive when status is unavailable.

### Implementation for User Story 2

- [X] T023 [P] [US2] Add supplied Dolby SVG source asset in `assets/icons/source/audio-dolby.svg`
- [X] T024 [P] [US2] Add supplied DTS SVG source asset in `assets/icons/source/audio-dts.svg`
- [X] T025 [P] [US2] Create custom PCM SVG source asset in `assets/icons/source/audio-pcm.svg`
- [X] T026 [P] [US2] Create custom Other SVG source asset in `assets/icons/source/audio-other.svg`
- [X] T027 [US2] Add Dolby, DTS, PCM, and Other inventory rows with source, usage, and runtime colors in `assets/icons/inventory.md`
- [X] T028 [US2] Generate `102x32` audio-family firmware masks in `src/ui/assets/IconBitmaps.h` and `src/ui/assets/IconBitmaps.cpp`
- [X] T029 [US2] Add audio-family classification helper declarations in `src/ui/Screens/HomeScreen.h`
- [X] T030 [US2] Implement Dolby, DTS, PCM, Other, and unavailable audio-family classification in `src/ui/Screens/HomeScreen.cpp`
- [X] T031 [US2] Replace bottom-row textual family pills with centered bitmap icons in `src/ui/Screens/HomeScreen.cpp`
- [X] T032 [US2] Apply lit green active and dark gray inactive icon colors in `src/ui/Screens/HomeScreen.cpp`
- [X] T033 [P] [US2] Update home audio-family icon state guidance in `docs/ui-reference.md`

**Checkpoint**: User Story 2 is independently testable on the home screen with simulated or live mode
values.

---

## Phase 5: User Story 3 - Replace Setup and Keyboard Glyphs (Priority: P2)

**Goal**: Settings navigation arrows and keyboard function glyphs use polished bitmap icons matching
their intended foreground colors.

**Independent Test**: Open Settings and Keyboard. Confirm settings rows use bitmap chevrons and the
keyboard caps lock, password visibility, backspace, OK, and Cancel controls use bitmap icons.

### Implementation for User Story 3

- [X] T034 [P] [US3] Add chevron-right Material Symbol source asset in `assets/icons/source/chevron-right.svg`
- [X] T035 [P] [US3] Add keyboard caps lock Material Symbol source asset in `assets/icons/source/keyboard-caps-lock.svg`
- [X] T036 [P] [US3] Add keyboard visibility and visibility-off Material Symbol source assets in `assets/icons/source/keyboard-visibility.svg` and `assets/icons/source/keyboard-visibility-off.svg`
- [X] T037 [P] [US3] Add keyboard backspace Material Symbol source asset in `assets/icons/source/keyboard-backspace.svg`
- [X] T038 [P] [US3] Add keyboard OK Material Symbol source asset in `assets/icons/source/keyboard-ok.svg`
- [X] T039 [P] [US3] Add keyboard Cancel Material Symbol source asset in `assets/icons/source/keyboard-cancel.svg`
- [X] T040 [US3] Add settings and keyboard icon inventory rows with source and runtime color rules in `assets/icons/inventory.md`
- [X] T041 [US3] Generate `24x24` settings and keyboard firmware masks in `src/ui/assets/IconBitmaps.h` and `src/ui/assets/IconBitmaps.cpp`
- [X] T042 [US3] Replace text arrows in Settings rows with chevron bitmap icons in `src/ui/Screens/SettingsScreen.cpp`
- [X] T043 [US3] Replace keyboard caps lock drawing with bitmap icon rendering in `src/ui/Screens/KeyboardScreen.cpp`
- [X] T044 [US3] Replace keyboard password visibility drawing with bitmap icon rendering in `src/ui/Screens/KeyboardScreen.cpp`
- [X] T045 [US3] Replace keyboard backspace text arrow with bitmap icon rendering in `src/ui/Screens/KeyboardScreen.cpp`
- [X] T046 [US3] Replace keyboard OK and Cancel manual glyph drawing with bitmap icon rendering in `src/ui/Screens/KeyboardScreen.cpp`
- [X] T047 [US3] Remove obsolete keyboard manual icon helper declarations in `src/ui/Screens/KeyboardScreen.h`
- [X] T048 [P] [US3] Document settings chevron and keyboard function icon usage in `docs/ui-reference.md`

**Checkpoint**: User Story 3 is independently testable by opening Settings and Keyboard.

---

## Phase 6: User Story 4 - Cover Setup and Receiver States (Priority: P2)

**Goal**: Wi-Fi and receiver setup screens use inventory-backed icons for searching, manual entry,
retry, success, warning, failure, Wi-Fi, and receiver states where those states appear.

**Independent Test**: Walk through Wi-Fi and receiver setup success and failure paths. Confirm each
major state has an appropriate readable icon and no placeholder artwork is visible.

### Implementation for User Story 4

- [X] T049 [P] [US4] Add scan Material Symbol source asset in `assets/icons/source/scan.svg`
- [X] T050 [P] [US4] Add manual-entry Material Symbol source asset in `assets/icons/source/manual-entry.svg`
- [X] T051 [P] [US4] Add retry Material Symbol source asset in `assets/icons/source/retry.svg`
- [X] T052 [P] [US4] Add success Material Symbol source asset in `assets/icons/source/success.svg`
- [X] T053 [P] [US4] Add warning Material Symbol source asset in `assets/icons/source/warning.svg`
- [X] T054 [P] [US4] Add failure Material Symbol source asset in `assets/icons/source/failure.svg`
- [X] T055 [P] [US4] Add Wi-Fi Material Symbol source asset in `assets/icons/source/wifi.svg`
- [X] T056 [P] [US4] Add receiver source asset in `assets/icons/source/receiver.svg`
- [X] T057 [US4] Add setup and status icon inventory rows with source and runtime color rules in `assets/icons/inventory.md`
- [X] T058 [US4] Generate setup and status firmware masks in `src/ui/assets/IconBitmaps.h` and `src/ui/assets/IconBitmaps.cpp`
- [X] T059 [US4] Add scanning and manual-entry icons to Wi-Fi network selection in `src/ui/Screens/NetworkListScreen.cpp`
- [X] T060 [US4] Add success, failure, and retry icons to Wi-Fi setup status in `src/ui/Screens/SetupStatusScreen.cpp`
- [X] T061 [US4] Add scanning, manual-entry, retry, and receiver icons to receiver discovery in `src/ui/Screens/ReceiverListScreen.cpp`
- [X] T062 [US4] Add receiver and manual-entry icons to manual receiver IP entry in `src/ui/Screens/ReceiverIpScreen.cpp`
- [X] T063 [US4] Add receiver verification success, warning, failure, retry, and discovery icons in `src/ui/Screens/ReceiverStatusScreen.cpp`
- [X] T064 [P] [US4] Document setup and status icon usage in `docs/ui-reference.md`

**Checkpoint**: User Story 4 is independently testable by walking through setup flows.

---

## Phase 7: User Story 5 - Maintain Asset Consistency Over Time (Priority: P3)

**Goal**: Contributors can audit and reuse the icon set without introducing duplicate or mismatched
assets.

**Independent Test**: Inspect the icon inventory and documentation. Confirm every delivered asset has
a stable name, meaning, source, usage note, target size, runtime colors, and screen usage.

### Implementation for User Story 5

- [X] T065 [US5] Complete inventory rows for every delivered asset in `assets/icons/inventory.md`
- [X] T066 [US5] Add generated symbol names and minimum display sizes to every inventory row in `assets/icons/inventory.md`
- [X] T067 [US5] Add source URLs and owner approval notes for Dolby and DTS in `assets/icons/inventory.md`
- [X] T068 [US5] Add candidate rationale for PCM and Other custom artwork in `assets/icons/inventory.md`
- [X] T069 [P] [US5] Add contributor regeneration instructions in `scripts/icons/README.md`
- [X] T070 [P] [US5] Update durable icon inventory pointer and reuse guidance in `docs/ui-reference.md`
- [X] T071 [P] [US5] Update contributor guidance for icon generation commands in `CONTRIBUTING.md`

**Checkpoint**: User Story 5 is independently testable by auditing documentation and inventory.

---

## Phase 8: Polish & Cross-Cutting Concerns

**Purpose**: Validate the full feature and clean up cross-story issues.

- [X] T072 [P] Run icon generation twice and confirm no second-run diff in `src/ui/assets/IconBitmaps.h`, `src/ui/assets/IconBitmaps.cpp`, and `assets/icons/generated/`
- [X] T073 [P] Run markdown and repository validation with `pre-commit run --all-files`
- [X] T074 [P] Run firmware build validation with `platformio run`
- [X] T075 Perform hardware visual verification from `specs/005-bitmap-icons/quickstart.md`
- [X] T076 Remove obsolete manual glyph helper implementations from `src/ui/Screens/HomeScreen.cpp` and `src/ui/Screens/KeyboardScreen.cpp`
- [X] T077 Verify all source and generated assets are represented in `assets/icons/inventory.md`
- [X] T078 Verify `docs/ui-reference.md` matches the final icon behavior and color states
- [X] T079 Verify no old text family labels or settings text arrows remain in `src/ui/Screens/HomeScreen.cpp` and `src/ui/Screens/SettingsScreen.cpp`

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies.
- **Foundational (Phase 2)**: Depends on Phase 1 and blocks all user stories.
- **User Stories (Phases 3-7)**: Depend on Phase 2 completion.
- **Polish (Phase 8)**: Depends on all desired user stories being complete.

### User Story Dependencies

- **User Story 1 (P1)**: Can start after Foundation; no dependency on other stories.
- **User Story 2 (P1)**: Can start after Foundation; no dependency on other stories.
- **User Story 3 (P2)**: Can start after Foundation; no dependency on other stories.
- **User Story 4 (P2)**: Can start after Foundation; benefits from US1/US3 renderer usage but is
  independently testable.
- **User Story 5 (P3)**: Can start after Foundation but should finish after all chosen assets are
  known.

### Within Each User Story

- Add source assets before generating masks.
- Update inventory before screen integration.
- Generate bitmap data before rendering icons from screens.
- Update durable documentation after behavior is implemented.
- Validate each story at its checkpoint before moving to lower-priority work.

### Parallel Opportunities

- Setup file shells T003-T006 can run in parallel after T001-T002.
- Source asset tasks within US2, US3, and US4 can run in parallel.
- Documentation tasks marked [P] can run in parallel with screen code after behavior is known.
- US1, US2, and US3 can be implemented in parallel after Foundation if the shared generated bitmap
  files are coordinated.

---

## Parallel Example: User Story 2

```text
Task: "Add supplied Dolby SVG source asset in assets/icons/source/audio-dolby.svg"
Task: "Add supplied DTS SVG source asset in assets/icons/source/audio-dts.svg"
Task: "Create custom PCM SVG source asset in assets/icons/source/audio-pcm.svg"
Task: "Create custom Other SVG source asset in assets/icons/source/audio-other.svg"
Task: "Update home audio-family icon state guidance in docs/ui-reference.md"
```

---

## Parallel Example: User Story 3

```text
Task: "Add chevron-right Material Symbol source asset in assets/icons/source/chevron-right.svg"
Task: "Add keyboard caps lock Material Symbol source asset in assets/icons/source/keyboard-caps-lock.svg"
Task: "Add keyboard visibility Material Symbol source asset in assets/icons/source/keyboard-visibility.svg"
Task: "Add keyboard backspace Material Symbol source asset in assets/icons/source/keyboard-backspace.svg"
Task: "Document settings chevron and keyboard function icon usage in docs/ui-reference.md"
```

---

## Parallel Example: User Story 4

```text
Task: "Add scan Material Symbol source asset in assets/icons/source/scan.svg"
Task: "Add manual-entry Material Symbol source asset in assets/icons/source/manual-entry.svg"
Task: "Add retry Material Symbol source asset in assets/icons/source/retry.svg"
Task: "Add success Material Symbol source asset in assets/icons/source/success.svg"
Task: "Document setup and status icon usage in docs/ui-reference.md"
```

---

## Implementation Strategy

### MVP First

1. Complete Phase 1: Setup.
2. Complete Phase 2: Foundational.
3. Complete Phase 3: User Story 1.
4. Stop and validate the home settings bitmap icon independently.
5. Continue to Phase 4 for the visible home audio-family row.

### Incremental Delivery

1. Foundation ready: source inventory, generator, renderer, and generated bitmap shells exist.
2. US1: primary settings bitmap icon is visible and renderer is proven.
3. US2: home audio-family row becomes icon based.
4. US3: settings and keyboard glyphs become icon based.
5. US4: setup/status flows gain icons.
6. US5: inventory and documentation become durable enough for future Material Design work.

### Validation Strategy

1. Run per-story checkpoint verification after each story phase.
2. Run `pre-commit run --all-files`.
3. Run `platformio run`.
4. Run hardware visual verification from `quickstart.md`.
5. Commit after each completed story or logical group.

## Notes

- [P] tasks use different files or source assets and can run in parallel with coordination.
- User story labels map directly to `spec.md`.
- Generated firmware bitmap files are shared; coordinate edits before parallel story work touches
  `src/ui/assets/IconBitmaps.h` or `src/ui/assets/IconBitmaps.cpp`.
- Do not add runtime SVG or PNG decoding to firmware.
