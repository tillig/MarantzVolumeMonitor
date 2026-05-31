# Feature Specification: UI Foundation Integration

**Feature Branch**: `002-ui-foundation`

**Created**: 2026-05-31

**Status**: Draft

**Input**: User description: "Integrate spike logic into UI architecture"

## User Scenarios & Testing

### User Story 1 - Clean UI Layer Initialization (Priority: P1)

As a developer, I want the display and touch hardware to be initialized within the dedicated `DisplayManager` and `TouchManager` classes so that the application logic is decoupled from low-level driver configuration.

**Why this priority**: Fundamental requirement for moving from a "spike" to a maintainable application architecture.

**Independent Test**: Build and upload. Success if the screen initializes and touch coordinates are correctly mapped to pixels using the new classes.

**Acceptance Scenarios**:

1. **Given** the system is powered on, **When** `DisplayManager::begin()` is called, **Then** the TFT display is initialized with the correct ST7796 driver and VSPI pins.
2. **Given** the system is powered on, **When** `TouchManager::begin()` is called, **Then** the touch controller is initialized and raw values are mapped to 480x320 screen coordinates.

---

### User Story 2 - Basic HomeScreen Rendering (Priority: P1)

As a user, I want to see the application's Home Screen after boot so I know the system is ready for use.

**Why this priority**: Confirms the `ScreenManager` and `Screen` base classes are functioning correctly on the new hardware.

**Independent Test**: Boot the device. Success if the `HomeScreen` is rendered with the "Hello World" text.

**Acceptance Scenarios**:

1. **Given** the hardware is initialized, **When** the application starts, **Then** the `ScreenManager` displays the `HomeScreen`.
2. **Given** the `HomeScreen` is active, **When** rendered, **Then** it uses the colors defined in the project constitution (Dark Mode).

---

### User Story 3 - Interactive Feedback (Priority: P2)

As a user, I want the UI to respond to my touch so I can verify the end-to-one interaction layer is working.

**Why this priority**: Required for all future interactive features like menu navigation.

**Independent Test**: Touch the screen. Success if the "Hello World" text changes color.

**Acceptance Scenarios**:

1. **Given** the `HomeScreen` is active, **When** the user touches the screen, **Then** the "Hello World" text cycles through the defined color list.

## Requirements

### Functional Requirements

- **FR-001**: `DisplayManager` MUST handle all `TFT_eSPI` initialization using the ESP32 VSPI pin mapping.
- **FR-002**: `TouchManager` MUST implement the calibration mapping (X: 350-3750, Y: 400-3750) to provide screen pixel coordinates.
- **FR-003**: `ScreenManager` MUST manage the lifecycle of the active screen and handle the background clearing.
- **FR-004**: `HomeScreen` MUST implement the `draw()` and `handleTouch()` methods.
- **FR-005**: `main.cpp` MUST only interact with high-level managers, not raw drivers.

### Key Entities

- **DisplayManager**: Singleton responsible for low-level display hardware.
- **TouchManager**: Singleton responsible for low-level touch hardware and coordinate mapping.
- **ScreenManager**: Orchestrator for switching between different UI screens.
- **HomeScreen**: The primary UI state for this feature.

## Success Criteria

### Measurable Outcomes

- **SC-001**: System renders the initial screen within 2 seconds of power-on.
- **SC-002**: Touch coordinates reported by `TouchManager` are accurate within 5 pixels of the physical touch point.
- **SC-003**: No driver-specific code (TFT_eSPI calls) exists in `main.cpp`.

## Assumptions

- The calibration values derived in the hardware spike are stable for all Elegoo ESP32 boards of this model.
- The `no_ota.csv` partition scheme provides sufficient space for the initial UI code.
- Power is supplied via USB-C or VIN to ensure 5V for the display.
