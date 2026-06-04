# Feature Specification: Bitmap Icon Assets

**Feature Branch**: `feature/005-bitmap-icons`

**Created**: 2026-06-04

**Status**: Draft

**Input**: User description: "Add bitmap assets for icons"

## Clarifications

### Session 2026-06-04

- Q: Should the keyboard password-visibility eye and backspace controls also move to bitmap icons in
  this feature? → A: Include both keyboard password-visibility eye and backspace icons in this
  feature.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Recognize Primary Controls at a Distance (Priority: P1)

As a person using or glancing at the volume monitor from across the room, I want common controls and
status indicators to use clear icon artwork so I can recognize actions and device states without
reading small labels.

**Why this priority**: The project depends on fast, high-contrast recognition from a living-room
viewing distance. Icons that are unclear, missing, or inconsistent weaken the main appliance
experience.

**Independent Test**: Review the home, settings, Wi-Fi setup, and receiver setup screens with the icon
asset set available. The story is complete when every primary control and persistent status indicator
has a matching icon that remains recognizable on the device display.

**Acceptance Scenarios**:

1. **Given** a screen has a primary action such as settings, back, confirm, cancel, retry, scan, or
   save, **When** that action is shown, **Then** the action is represented by a matching high-contrast
   icon asset.
2. **Given** the home screen settings entry is visible, **When** the user views the top-right control,
   **Then** the control uses a bitmap settings icon rather than a manually drawn gear.
3. **Given** a screen shows device or connection state, **When** the user views the screen from a
   normal room distance, **Then** the state icon is distinguishable from other icons without relying
   on color alone.
4. **Given** an icon appears on both the home screen and setup screens, **When** the screens are
   compared, **Then** the icon retains consistent meaning and visual style.

---

### User Story 2 - Show Audio Family Icons on the Home Screen (Priority: P1)

As a listener, I want the Dolby, DTS, PCM, and Other audio-family indicators to appear as icons rather
than textual buttons so the bottom row reads like status lighting instead of a row of labels.

**Why this priority**: The audio family row is always visible on the primary appliance screen and is a
key target for the bitmap icon migration.

**Independent Test**: Display the home screen with each audio family active and inactive. The story is
complete when all four family areas render as icons, the active family appears lit in green, and the
inactive families appear in the same dark gray visual weight as the current inactive button
background.

**Acceptance Scenarios**:

1. **Given** the active audio mode belongs to Dolby, **When** the home screen is shown, **Then** the
   Dolby family area uses the Dolby logo as a green lit icon while DTS, PCM, and Other remain dark
   gray.
2. **Given** the active audio mode belongs to DTS, **When** the home screen is shown, **Then** the DTS
   family area uses the DTS logo as a green lit icon while Dolby, PCM, and Other remain dark gray.
3. **Given** the active audio mode belongs to PCM, **When** the home screen is shown, **Then** the PCM
   family area uses a custom non-brand icon that communicates digital or uncompressed audio while the
   other family icons remain dark gray.
4. **Given** the active audio mode does not match Dolby, DTS, or PCM, **When** the home screen is
   shown, **Then** the Other family area uses a custom generic audio icon while the other family icons
   remain dark gray.
5. **Given** no audio family is active or the receiver status is unavailable, **When** the home screen
   is shown, **Then** all audio family icons appear in the inactive dark gray state.

---

### User Story 3 - Replace Setup and Keyboard Glyphs (Priority: P2)

As a user navigating settings and entering Wi-Fi credentials, I want navigation arrows and keyboard
function controls to use polished bitmap icons so the setup experience feels consistent with the home
screen.

**Why this priority**: The settings and keyboard screens currently rely on text arrows and manually
drawn glyphs. Replacing these with bitmap icons removes a visible style mismatch before the broader
Material Design refresh.

**Independent Test**: Open the settings screen and keyboard screen. The story is complete when
settings navigation arrows, keyboard caps lock, keyboard password visibility, keyboard backspace,
keyboard OK, and keyboard Cancel are rendered from bitmap icons that match the intended foreground
color.

**Acceptance Scenarios**:

1. **Given** the settings screen lists Touch Calibration, Wi-Fi Setup, and Receiver Setup, **When** the
   list is displayed, **Then** each row uses a bitmap forward-arrow icon that matches the row text
   color instead of a text arrow.
2. **Given** the keyboard caps lock key is inactive, **When** the keyboard is displayed, **Then** caps
   lock uses a bitmap icon in the normal function-key foreground color.
3. **Given** the keyboard caps lock key is active, **When** the keyboard is displayed, **Then** caps
   lock uses the selected-state foreground treatment and remains recognizable as caps lock.
4. **Given** the keyboard password-visibility and backspace keys are displayed, **When** the keyboard
   is displayed, **Then** each key uses a bitmap icon instead of a manually drawn eye glyph or text
   arrow.
5. **Given** the keyboard OK and Cancel keys are displayed, **When** the keyboard is displayed, **Then**
   each key uses a bitmap icon instead of a manually drawn check or X glyph.

---

### User Story 4 - Cover Setup and Receiver States (Priority: P2)

As a user completing device setup, I want icons for Wi-Fi, receiver discovery, receiver connection,
manual entry, success, warning, and failure states so setup progress and recoverable errors are easy
to scan.

**Why this priority**: Setup screens are more text-heavy than the home screen. Icons help users
understand where they are in the flow and what action or state matters next.

**Independent Test**: Walk through Wi-Fi setup and receiver setup, including success and failure
paths. The story is complete when each major state has an appropriate icon and no fallback placeholder
is visible.

**Acceptance Scenarios**:

1. **Given** the device is searching for networks or receivers, **When** progress is displayed,
   **Then** the screen includes an icon that communicates discovery or scanning.
2. **Given** a setup step succeeds or fails, **When** the result is displayed, **Then** the screen
   includes a distinct success, warning, or failure icon as appropriate.
3. **Given** manual entry is offered as a fallback, **When** the option is displayed, **Then** the
   screen includes an icon that differentiates manual entry from automatic discovery.

---

### User Story 5 - Maintain Asset Consistency Over Time (Priority: P3)

As a project contributor, I want a documented icon inventory and usage mapping so future UI work can
reuse the same assets rather than introducing duplicate or mismatched artwork.

**Why this priority**: The icon set will become part of the durable UI language. A clear inventory
prevents drift as new screens and setup states are added.

**Independent Test**: Compare the delivered icon inventory against the UI screens and documentation.
The story is complete when every asset has a stable name, intended meaning, usage guidance, and a
visible sample or reference.

**Acceptance Scenarios**:

1. **Given** an icon asset exists, **When** a contributor inspects the icon inventory, **Then** the
   asset has a stable name, intended meaning, and expected usage context.
2. **Given** two icons represent related actions or states, **When** they are reviewed together,
   **Then** their shapes, contrast, and visual weight appear intentionally related.
3. **Given** a future screen needs an existing concept such as settings, retry, or disconnected,
   **When** a contributor checks the inventory, **Then** they can identify the existing asset to reuse.

### Edge Cases

- An icon is scaled down for a compact touch target and becomes hard to distinguish.
- A state depends on color, but the icon must still communicate meaning to users who cannot reliably
  distinguish the colors.
- Multiple actions have similar meanings, such as retry, refresh, and scan, and must remain visually
  distinct.
- An icon appears on a dark background and on a highlighted control state.
- A brand logo cannot be sourced with acceptable license or usage terms and must be flagged before a
  substitute is used.
- A future contributor adds a screen before knowing whether a suitable icon already exists.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST include a complete icon asset set for the current home, settings, Wi-Fi
  setup, and receiver setup user flows.
- **FR-002**: The icon asset set MUST cover primary controls including settings, back, confirm,
  cancel, retry, scan, manual entry, and save.
- **FR-003**: The icon asset set MUST cover major status concepts including Wi-Fi, receiver,
  connected, disconnected, searching, success, warning, and failure.
- **FR-004**: The home screen audio family row MUST use icon assets for Dolby, DTS, PCM, and Other
  instead of textual buttons.
- **FR-005**: Dolby and DTS audio family icons MUST use the respective brand logo artwork when
  acceptable source assets and usage terms are available.
- **FR-006**: PCM and Other audio family icons MUST use custom non-brand artwork that fits the same
  visual system as Dolby and DTS.
- **FR-007**: Audio family icons MUST use a lit green active state and an inactive dark gray state,
  and active/inactive state MUST be communicated by more than color alone where practical.
- **FR-008**: The home screen settings entry MUST use a bitmap settings icon rather than a manually
  drawn gear.
- **FR-009**: Settings screen navigation rows MUST use bitmap forward-arrow icons that match the row
  text color rather than text arrows.
- **FR-010**: Keyboard caps lock, password visibility, backspace, OK, and Cancel controls MUST use
  bitmap icons rather than manually drawn glyphs or text arrows.
- **FR-011**: The icon style MUST align with Google Material Design concepts, using Material Symbols
  or Material-compatible custom artwork for non-brand actions and states.
- **FR-012**: Every icon asset MUST have one intended meaning and MUST NOT be reused for unrelated
  concepts.
- **FR-013**: Every icon asset MUST be readable against the project's dark UI background and against
  any highlighted control state where it is expected to appear.
- **FR-014**: Every icon asset MUST remain recognizable at the smallest size used by the on-device UI.
- **FR-015**: Icon assets MUST use a consistent visual style, including comparable stroke or shape
  weight, spacing, contrast, and level of detail.
- **FR-016**: Icon assets MUST avoid relying on color as the only way to communicate meaning.
- **FR-017**: The project MUST provide an icon inventory that records each asset name, meaning,
  source, license or usage note, and intended screen or state usage.
- **FR-018**: The icon inventory MUST distinguish between sourced Material icons, sourced brand logo
  assets, and custom-generated assets.
- **FR-019**: The icon inventory MUST include candidate directions for the custom PCM and Other icons
  before final artwork is accepted.
- **FR-020**: The UI reference documentation MUST identify the icon usage rules and the durable place
  where contributors can find the icon inventory.
- **FR-021**: The delivered asset set MUST not replace readable text where text is still needed for
  setup clarity; icons supplement concise labels and status messages.
- **FR-022**: The delivered asset set MUST avoid placeholder, decorative, or ambiguous artwork that
  does not communicate a specific action or state.
- **FR-023**: The delivered asset set MUST provide a clear path for future Material Design display
  styling without requiring the icons to be redesigned.

### Design Direction

- **Material controls**: Prefer official Material Symbols for settings, forward arrows, caps lock,
  check/OK, cancel/close, scan, manual entry, retry, and common setup states when the symbol meaning is
  clear.
- **Dolby and DTS**: Use official or otherwise acceptable brand logo artwork and document the source
  and usage terms before adding the assets to the project.
- **Provided Dolby source**: Evaluate the Wikimedia Commons `Dolby_logo.svg` asset supplied by the
  project owner as the Dolby family icon source candidate, with final usage recorded in the icon
  inventory.
- **Provided DTS source**: Evaluate the Wikimedia Commons `DTS_logo.svg` asset supplied by the
  project owner as the DTS family icon source candidate, with final usage recorded in the icon
  inventory.
- **Brand approval**: Record that the project owner approved use of the supplied Dolby and DTS logo
  sources for this project on 2026-06-04.
- **PCM candidate directions**: Consider a stylized digital waveform, pulse-code step waveform, or
  binary/sample-grid icon. The selected PCM icon should read as clean digital audio and should not look
  like a brand logo.
- **Other candidate directions**: Consider a generic audio waveform, speaker-with-spark, or
  waveform-with-question-mark icon. The selected Other icon should communicate uncategorized audio
  without implying an error.
- **Home audio family states**: Active family icons should read as illuminated green status lights;
  inactive family icons should recede into the dark gray bottom-row treatment.
- **Future Material refresh**: Icons should use Material-compatible geometry, optical size, spacing,
  and visual weight so they can survive a later Material Design screen style update.

### Key Entities *(include if feature involves data)*

- **IconAsset**: A single bitmap icon representing one control, object, brand, or state, with a
  stable name, intended meaning, source, usage note, and expected minimum display size.
- **IconSet**: The collection of icon assets that share a consistent visual style and cover the
  current on-device UI flows.
- **IconUsageMapping**: The inventory that links each icon asset to its intended screen, control,
  state, or user-facing meaning.
- **AudioFamilyIcon**: One of the home screen bottom-row family icons for Dolby, DTS, PCM, or Other,
  including active and inactive visual states.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: 100% of current home, settings, Wi-Fi setup, and receiver setup primary controls have a
  matching icon asset or a documented reason why text-only presentation is preferred.
- **SC-002**: 100% of home screen Dolby, DTS, PCM, and Other audio family areas render as icons rather
  than text labels.
- **SC-003**: Inactive audio family icons match the intended dark gray inactive treatment, and the
  active audio family icon is visibly green and identifiable within 1 second during visual review.
- **SC-004**: 100% of settings row arrows and keyboard caps lock, password visibility, backspace, OK,
  and Cancel controls render as bitmap icons rather than text arrows or manually drawn glyphs.
- **SC-005**: 100% of major setup states for searching, success, warning, and failure have distinct
  icon assets with no placeholder artwork shown to users.
- **SC-006**: In a visual review at normal room distance, at least 90% of reviewed icons are correctly
  identified by their intended meaning without reading adjacent labels.
- **SC-007**: Every delivered icon asset has an inventory entry with name, meaning, source, usage note,
  intended usage, and minimum display size.
- **SC-008**: No delivered icon requires color alone to distinguish its meaning from another icon in
  the set.
- **SC-009**: Adding or updating a screen can identify the correct reusable icon from the inventory in
  under 2 minutes.

## Assumptions

- The first icon set targets the UI flows already present in the project: home, settings, Wi-Fi setup,
  and receiver setup.
- Icons are supplemental to the existing readable screen text and do not remove labels needed for
  setup comprehension.
- The icon set should prioritize the 4-inch touchscreen appliance experience rather than general
  marketing or app-store artwork.
- The project will keep durable icon usage guidance with the existing UI reference documentation.
- Material Symbols are the preferred source for non-brand control and state icons when a suitable
  symbol exists.
- Dolby and DTS require logo assets rather than Material Symbols; acceptable source and usage terms
  must be documented during planning or implementation.
- The project owner supplied Dolby and DTS SVG source candidates and approved use of both supplied
  logo sources for this project on 2026-06-04.
