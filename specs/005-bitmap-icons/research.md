# Phase 0 Research: Bitmap Icon Assets

## Decision: Keep SVG source artwork and generate firmware bitmap masks

**Rationale**: Source SVGs preserve auditability and allow resizing or recoloring later, while generated monochrome bitmap masks keep firmware rendering small and deterministic. The ESP32 should not spend UI time decoding SVG or PNG data at runtime.

**Alternatives considered**:

- Runtime PNG files from LittleFS: rejected because it adds filesystem dependence and image decoding complexity to normal screen rendering.
- Hand-drawn TFT primitives: rejected because the feature is explicitly moving away from manual glyphs.
- Full-color bitmap sprites for every state: rejected because active/inactive states can be produced more compactly by recoloring monochrome masks.

## Decision: Use Material Symbols for generic control and state icons

**Rationale**: Material Symbols match the requested Material Design direction and provide common icons for settings, chevrons, caps lock, check, close/cancel, Wi-Fi, search/scan, refresh, warning, error, and success. The implementation should keep downloaded/source symbols in the icon inventory and generate target firmware masks from those sources.

**Alternatives considered**:

- Mixed third-party icon packs: rejected because style consistency and licensing would be harder to audit.
- Custom drawing all icons: rejected because common actions already have well-understood Material forms.

## Decision: Treat Dolby and DTS as brand-logo sources, not Material icons

**Rationale**: Dolby and DTS should remain recognizable brand marks. The project owner supplied SVG source candidates for both and explicitly approved their use for this project on 2026-06-04. The icon inventory must retain source URLs and the approval note because these marks are brand assets.

**Alternatives considered**:

- Generate approximate brand logos: rejected because approximate logos are likely to be visually wrong and harder to justify.
- Replace brand logos with generic audio icons: rejected because the feature requires Dolby and DTS logo treatment on the home audio-family row.

## Decision: Create custom PCM and Other icons in a Material-compatible style

**Rationale**: PCM and Other are not brands, so custom assets avoid implying a nonexistent brand mark. PCM should read as digital/uncompressed audio; Other should read as uncategorized audio without looking like an error.

**Alternatives considered**:

- Text labels for PCM and Other: rejected because the home audio-family row must become icon based.
- Reusing a generic speaker icon for both: rejected because every icon must have one intended meaning.

## Decision: Compile icon masks into firmware

**Rationale**: Firmware-compiled masks keep boot and screen rendering independent from LittleFS asset availability. They also preserve passive appliance stability because icon drawing becomes a small, predictable UI-layer operation.

**Alternatives considered**:

- Store icons on LittleFS: rejected for this feature because the icons are static UI chrome, not user configuration.
- Fetch or update icons dynamically: rejected because the appliance should not depend on network access for UI chrome.

## Decision: Add an icon inventory as the durable asset contract

**Rationale**: The inventory is the durable record for name, source, license or usage note, owner approval, generated target sizes, and screen usage. It lets future Material Design work reuse assets without guessing.

**Alternatives considered**:

- Keep source notes only in code comments: rejected because source/license data belongs with assets and documentation, not generated firmware data.
- Keep source notes only in the feature spec: rejected because durable project references should live outside feature-specific Spec Kit artifacts after implementation.

## External References

- Material Symbols documentation: `https://developers.google.com/fonts/docs/material_symbols`
- Material Symbols catalog: `https://fonts.google.com/icons`
- Google Material Design icons license: `https://github.com/google/material-design-icons/blob/master/LICENSE`
- Dolby source candidate supplied by project owner: `https://upload.wikimedia.org/wikipedia/commons/3/32/Dolby_logo.svg`
- DTS source candidate supplied by project owner: `https://upload.wikimedia.org/wikipedia/commons/3/3d/DTS_logo.svg`
