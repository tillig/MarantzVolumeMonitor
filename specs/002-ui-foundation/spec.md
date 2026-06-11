# Feature Specification: UI Foundation Integration

**Feature Branch**: `feature/002-ui-foundation`

**Created**: 2026-05-31

**Status**: Draft

**Input**: User description: "Integrate spike logic into UI architecture"

## Clarifications

### Session 2026-05-31

- Q: Should we expand the foundation scope to include layout skeletons from `docs/ui-reference.md`? → A: Yes, include Volume bar, Source/Mode labels, and Audio Tile placeholders.
- Q: How should the system behave in an "unconfigured" state (no WiFi/Receiver set)? → A: Show a prominent "UNCONFIGURED" message with "Tap for Settings" instructions.

## User Scenarios and Testing

### User Story 1 - Clean UI Layer Initialization (Priority: P1)

As a developer, I want the display and touch hardware to be initialized within the dedicated `DisplayManager` and `TouchManager` classes so that the application logic is decoupled from low-level driver configuration.

**Why this priority**: Fundamental requirement for moving from a "spike" to a maintainable application architecture.

**Independent Test**: Build and upload. Success if the screen initializes and touch coordinates are correctly mapped to pixels using the new classes.

**Acceptance Scenarios**:

1. **Given** the system is powered on, **When** `DisplayManager::begin()` is called, **Then** the TFT display is initialized with the correct ST7796 driver and VSPI pins.
2. **Given** the system is powered on, **When** `TouchManager::begin()` is called, **Then** the touch controller is initialized and raw values are mapped to 480x320 screen coordinates.

---

### User Story 2 - Final UI Layout Skeleton (Priority: P1)

As a user, I want to see the real application layout (Volume, Bar, Labels, Tiles) after boot so I can see the final design intent even before the receiver is connected.

**Why this priority**: Confirms the `ScreenManager` and `HomeScreen` classes can handle complex layouts defined in `docs/ui-reference.md`.

**Independent Test**: Boot the device. Success if the `HomeScreen` renders all the structural elements (Volume bar, labels, tile boxes) on a black background.

**Acceptance Scenarios**:

1. **Given** the hardware is initialized, **When** the application starts, **Then** the `ScreenManager` displays the `HomeScreen` with placeholders for Volume, Source, and Mode.
2. **Given** the `HomeScreen` is active, **When** rendered, **Then** it uses the color palette (e.g., #101010 background, #3DAEFF accent) defined in `docs/ui-reference.md`.

---

### User Story 3 - Unconfigured State Messaging (Priority: P2)

As a first-time user, I want a clear message when the device is not yet set up so I know I need to access the settings menu.

**Why this priority**: Ensures the "passive appliance" feel by communicating state clearly.

**Independent Test**: Power on the device for the first time. Success if the screen shows "UNCONFIGURED" in the center.

**Acceptance Scenarios**:

1. **Given** no configuration exists, **When** the `HomeScreen` loads, **Then** a prominent "UNCONFIGURED" message is displayed with "Tap for Settings" instructions.
2. **Given** the unconfigured screen is visible, **When** the user touches the screen, **Then** the system provides feedback that it is transitioning or acknowledging the setup requirement.

## Requirements

### Functional Requirements

- **FR-001**: `DisplayManager` MUST handle all `TFT_eSPI` initialization using the ESP32 VSPI pin mapping.
- **FR-002**: `TouchManager` MUST implement the calibration mapping (X: 350-3750, Y: 400-3750) to provide screen pixel coordinates.
- **FR-003**: `ScreenManager` MUST manage the lifecycle of the active screen and handle the background clearing.
- **FR-004**: `HomeScreen` MUST implement the layout skeleton from `docs/ui-reference.md`, including placeholders for:
  - Volume Number (72-96px)
  - Volume Bar (70% width)
  - Source and Mode labels
  - Audio Family Tiles (Dolby, DTS, PCM, Other)
- **FR-005**: `HomeScreen` MUST display a prominent "UNCONFIGURED" message if WiFi or Receiver IP is missing.
- **FR-006**: `main.cpp` MUST only interact with high-level managers, not raw drivers.

### Key Entities

- **DisplayManager**: Singleton responsible for low-level display hardware.
- **TouchManager**: Singleton responsible for low-level touch hardware and coordinate mapping.
- **ScreenManager**: Orchestrator for switching between different UI screens.
- **HomeScreen**: The primary UI state for this feature.

## Success Criteria

### Measurable Outcomes

- **SC-001**: System renders the initial screen within 2 seconds of power-on.
- **SC-002**: Touch coordinates reported by `TouchManager` are accurate within 5 pixels of the physical touch point.
- **SC-003**: All layout elements from `docs/ui-reference.md` are visually represented (even if data is hardcoded/placeholder).
- **SC-004**: No driver-specific code (TFT_eSPI calls) exists in `main.cpp`.

## Assumptions

- The calibration values derived in the hardware spike are stable for all Elegoo ESP32 boards of this model.
- The `no_ota.csv` partition scheme provides sufficient space for the initial UI code.
- Power is supplied via USB-C or VIN to ensure 5V for the display.
