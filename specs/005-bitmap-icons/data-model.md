# Data Model: Bitmap Icon Assets

## IconAsset

Represents one source icon and its generated firmware outputs.

### IconAsset Fields

- `id`: Stable kebab-case identifier, for example `settings`, `audio-dolby`, or `keyboard-caps-lock`.
- `meaning`: Single user-facing concept represented by the icon.
- `category`: One of `control`, `status`, `audio-family`, or `setup`.
- `sourceType`: One of `material-symbol`, `brand-logo`, or `custom`.
- `sourcePath`: Repository path to the source asset after implementation.
- `sourceUrl`: Original upstream URL when the source came from outside the repository.
- `licenseOrUsageNote`: License, trademark note, or owner approval note.
- `targetSizes`: One or more generated display sizes, such as `24x24`, `32x32`, `48x48`, or `102x32`.
- `minimumDisplaySize`: Smallest approved rendered size.
- `generatedSymbol`: Firmware symbol name for the generated bitmap mask.
- `foregroundModes`: Approved foreground treatments, such as active green, inactive dark gray, text primary, text secondary, selected dark text, warning, or error.

### IconAsset Validation Rules

- `id` must be unique.
- `meaning` must describe exactly one concept.
- `sourceType=brand-logo` must include `sourceUrl` and `licenseOrUsageNote`.
- `sourceType=material-symbol` must name the selected Material Symbol in the inventory.
- `sourceType=custom` must include the design direction used to create the asset.
- Every generated icon must have an inventory row before it is used by a screen.

## IconSet

Represents the initial Material-compatible icon set delivered by this feature.

### IconSet Fields

- `name`: Human-readable icon set name.
- `styleGuide`: Material-compatible style guidance used for the set.
- `assets`: Ordered collection of `IconAsset` records.
- `generatedAt`: Asset generation timestamp or version note.
- `generatorVersion`: Script or process version used to generate firmware masks.

### IconSet Validation Rules

- The set must include all required home, settings, keyboard, and setup/status icons from the spec.
- All non-brand icons should share comparable geometry, optical weight, and padding.
- Brand logos may keep brand geometry but must fit the same display canvas and state colors.

## AudioFamilyIcon

Specialized icon mapping for the home screen audio-family row.

### AudioFamilyIcon Fields

- `family`: One of `Dolby`, `DTS`, `PCM`, or `Other`.
- `assetId`: IconAsset id used for this family.
- `matchRules`: Receiver mode text patterns or classification rules that activate this family.
- `activeColor`: Lit green foreground treatment.
- `inactiveColor`: Dark gray foreground treatment.
- `canvasSize`: `102x32`.
- `safeContentBox`: `94x24`.

### AudioFamilyIcon State Transitions

```text
Unknown/Unavailable -> Inactive all
Receiver mode contains Dolby -> Dolby active, others inactive
Receiver mode contains DTS -> DTS active, others inactive
Receiver mode indicates PCM -> PCM active, others inactive
Receiver mode is known but unmatched -> Other active, others inactive
```

### AudioFamilyIcon Validation Rules

- Exactly one family may be active for a valid receiver mode.
- All families must render as icons, not text labels.
- The active state must be green and visually stronger than inactive dark gray.

## IconUsageMapping

Connects icon assets to screen locations and intended colors.

### IconUsageMapping Fields

- `screen`: Screen or UI flow where the icon appears.
- `controlOrState`: Specific control, row, key, or state.
- `assetId`: IconAsset id.
- `displaySize`: Rendered size.
- `foregroundMode`: Named color/treatment from the inventory.
- `textRetained`: Whether adjacent text remains visible for clarity.

### IconUsageMapping Validation Rules

- Every screen use must reference an existing IconAsset.
- Icons replacing glyphs must remove the old text arrow or manual primitive drawing.
- Setup and keyboard text needed for comprehension must remain visible.
