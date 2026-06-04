# Contract: Icon Asset Pipeline

## Purpose

Define how source artwork becomes firmware-ready bitmap icon assets.

## Source Inputs

Source assets must live under `assets/icons/source/` after implementation.

Accepted source categories:

- Material Symbol SVG sources for non-brand controls and states.
- Owner-approved Dolby and DTS SVG source candidates for brand family icons.
- Custom SVG source artwork for PCM and Other.

## Generated Outputs

Generated outputs must include:

- Firmware bitmap mask data under `src/ui/assets/IconBitmaps.h/.cpp`.
- Optional generated preview files under `assets/icons/generated/`.
- Updated `assets/icons/inventory.md` entries linking each source to generated outputs.

Generated firmware masks must be deterministic. Running the generation command twice from the same
inputs must produce byte-identical firmware asset files.

## Target Sizes

| Icon Group | Canvas | Safe Content Box | Notes |
| ---------- | ------ | ---------------- | ----- |
| Home audio family | `102x32` | `94x24` | Dolby, DTS, PCM, Other |
| Small controls | `24x24` | `20x20` | Settings, chevron, caps lock, visibility, visibility off, backspace, OK, Cancel, retry, manual entry |
| Setup/status | `32x32` or `48x48` | Proportional padding | Use larger size when the icon is the main state symbol |

## Color Handling

The generated bitmap data should be a monochrome mask. Runtime rendering selects foreground color
from the screen context:

- Active audio family: lit green.
- Inactive audio family: dark gray.
- Settings arrows: row text color.
- Keyboard OK/Cancel/Caps Lock/visibility/backspace: current key foreground treatment.
- Warning/error/success: screen-specific semantic color if used, with shape still carrying meaning.

## Acceptance Checks

- Every generated icon has a corresponding inventory entry.
- No generated firmware icon is orphaned from the inventory.
- Dolby and DTS entries include the owner approval note dated 2026-06-04.
- Re-running generation produces no uncommitted diff when inputs are unchanged.
- A normal `platformio run` succeeds without requiring image conversion tools.
