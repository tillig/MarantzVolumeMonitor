# Data Model: Material Design UI

## UISurface

Represents one current on-device screen or major screen region that participates in the refresh.

### UISurface Fields

- `id`: Stable screen identifier, such as `home`, `settings`, `network-list`, or `keyboard`.
- `titleRole`: Typography role used for the screen title, if present.
- `primaryContentRole`: The dominant content role for the screen.
- `componentPatterns`: Ordered list of component patterns used on the surface.
- `stateTreatments`: States the surface can present, such as loading, empty, success, warning, error, inactive, or unavailable.
- `documentedExceptions`: Any design exceptions approved for the surface.

### UISurface Validation Rules

- Every current screen in `src/ui/Screens` must have an entry in the implementation audit.
- A screen may diverge from the shared catalog only through a documented design exception.
- The home screen must preserve volume, source, listening mode, and audio family as the dominant visual hierarchy.

## ComponentPattern

Represents one reusable Material-compatible control or content treatment.

### ComponentPattern Fields

- `id`: Stable identifier, such as `standard-action-button`, `keyboard-icon-button`, `selection-list-row`, or `searching-screen`.
- `purpose`: User-facing reason the pattern exists.
- `geometry`: Shape, corner radius, minimum size, and alignment rules.
- `typography`: Text roles and font usage.
- `iconUsage`: Whether icons are required, optional, or disallowed.
- `stateRules`: Allowed states and their visual treatments.
- `touchRules`: Minimum touch target and hitbox expectations.

### ComponentPattern Validation Rules

- Equivalent actions must reference the same component pattern across equivalent contexts.
- Component text and icons must fit without overlapping neighboring content.
- Component patterns must use existing UI-layer colors unless implementation explicitly documents a new color role in `docs/ui-reference.md`.

## ButtonVariant

Represents a specific button treatment.

### ButtonVariant Fields

- `id`: One of `standard-icon-text`, `keyboard-icon-only`, or `documented-exception`.
- `actions`: Actions that may use the variant.
- `labelRule`: Required, optional, or disallowed label behavior.
- `iconRule`: Required, optional, or disallowed icon behavior.
- `shapeRule`: Corner radius and outline/fill expectations.
- `alignmentRule`: Text/icon group alignment and vertical centering.
- `stateRules`: Normal, pressed, selected, disabled, loading, success, warning, and error treatments.

### ButtonVariant Validation Rules

- OK, Cancel, Save, Retry, Back, Confirm, Manual, Rescan, and Discover use `standard-icon-text` outside documented compact contexts.
- Keyboard OK and Cancel may use `keyboard-icon-only`, but must share shape, size, baseline, center, spacing, and icon style.
- Text-only buttons require a documented exception.

## SelectionListPattern

Represents selectable rows used for Wi-Fi networks, receiver candidates, and future setup choices.

### SelectionListPattern Fields

- `rowHeight`: Approved row height.
- `rowShape`: Corner radius and outline/fill treatment.
- `primaryTextRole`: Typography role for the main row label.
- `secondaryTextRole`: Typography role for metadata such as IP address or signal strength.
- `leadingVisual`: Optional icon or signal indicator.
- `trailingVisual`: Optional page number, status, or affordance.
- `overflowRule`: Truncation or wrapping behavior for long values.
- `stateRules`: Normal, focused, selected, disabled, empty, unavailable, and error treatments.

### SelectionListPattern Validation Rules

- Wi-Fi and receiver lists must use comparable row height, spacing, typography, and state treatment.
- Long network and receiver names must remain understandable without changing row geometry.
- Empty and unavailable states must provide a clear recovery action when recovery is possible.

## SearchingPattern

Represents unknown-duration discovery or search screens.

### SearchingPattern Fields

- `titlePlacement`: Position and typography role for the title.
- `messagePlacement`: Position and typography role for explanatory text.
- `iconPlacement`: Position and color role for the search icon.
- `progressPlacement`: Position and movement treatment for activity feedback.
- `actionPlacement`: Position and variants for cancel, manual, retry, or rescan actions.
- `startRule`: When progress appears.

### SearchingPattern Validation Rules

- Wi-Fi search and receiver discovery must use the same pattern.
- Searching progress appears immediately when searching starts.
- Progress movement must be non-blocking and restrained.

## StateTreatment

Represents a reusable visual treatment for a UI state.

### StateTreatment Fields

- `id`: State identifier such as `normal`, `pressed`, `selected`, `disabled`, `loading`, `success`, `warning`, `error`, `inactive`, or `unavailable`.
- `colorRole`: Approved foreground/background color role.
- `iconRule`: Required or optional icon treatment.
- `textRule`: Required or optional text treatment.
- `motionRule`: Whether movement is allowed and how it is constrained.

### StateTreatment Validation Rules

- Setup and recovery states must not rely on color alone.
- Loading and progress movement must not block touch or screen updates.
- Error and warning states must include a concise message and recovery action when recoverable.

## DocumentationCatalog

Represents the durable style catalog added to `docs/ui-reference.md`.

### DocumentationCatalog Fields

- `scope`: Current on-device screens covered by the catalog.
- `componentPatterns`: Shared patterns documented for implementation and future contributors.
- `screenApplications`: Mapping from each screen to catalog patterns.
- `exceptions`: Approved design exceptions and rationale.
- `validationChecklist`: Hardware and review checks used to confirm consistency.

### DocumentationCatalog Validation Rules

- The catalog must cover buttons, lists, searching screens, typography, spacing, icons, state treatments, and exceptions.
- The catalog must identify icon-only keyboard controls as compact-context exceptions.
- The catalog must remain durable guidance outside the feature-specific artifacts.
