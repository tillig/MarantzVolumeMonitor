# UI Reference

## Principles

The main UI is optimized for a 15-foot viewing distance. It uses a high-contrast dark palette, large
status text, and restrained animation so the device behaves like a passive appliance.

- Dark mode only.
- No idle animation.
- Volume changes may animate briefly.
- Setup and settings screens should stay touch-first and readable on the 4" display.

## Home Screen

```text
+------------------------------------------------------------+
|                                                    (gear)  |
|                           /  \                             |
|                          | 50 |                            |
|                           \__/                             |
|                                                            |
|      Blu-ray                           Dolby TrueHD        |
|                                                            |
|   [ Dolby ]      [  DTS  ]      [  PCM  ]      [ Other ]   |
|                                                            |
+------------------------------------------------------------+
```

### Volume Number And Gauge

- Center point: `(240, 140)`
- Volume number: white, Font 8, centered near `Y=125`
- Volume caption: dimmed gray, Font 2, centered near `Y=185`
- Gauge arc: outer radius 125 px, inner radius 109 px, 240-degree span
- Gauge colors: green/yellow/red fill over `#303030` background

### Settings Entry

- A small bitmap settings icon in the top-right corner opens Settings from the normal Home Screen.
- Settings exposes touch calibration, Wi-Fi setup, and receiver setup.
- Flows launched from Settings should return to Settings when complete; the Settings OK button
  returns to the Home Screen.

### Source And Mode Text

- Typography: white, Font 4
- Vertical position: centered near `Y=255`
- Source: left-aligned near `X=40`
- Mode: right-aligned near `X=440`
- Show raw receiver values without labels such as `SRC:`

### Audio Family Icons

- Families: Dolby, DTS, PCM, Other
- Layout: bottom row near `Y=278`
- Canvas: 102x32 px per family
- Active state: lit green icon
- Inactive state: dark gray icon matching the gauge background treatment
- Dolby and DTS use approved brand logo source assets.
- PCM uses a custom pulse-code step waveform icon.
- Other uses a custom uncategorized-audio waveform icon.
- If receiver status is unavailable, all family icons are inactive.

## Icon Assets

The durable icon inventory is `assets/icons/inventory.md`. It records each icon's source, usage note,
target size, generated firmware symbol, screen usage, and runtime colors.

Source artwork is stored in `assets/icons/source/`. Generated previews are stored in
`assets/icons/generated/`. Firmware bitmap masks are generated into `src/ui/assets/`.

Use Material-compatible geometry and visual weight for non-brand icons. Icons supplement setup text;
do not remove labels needed for setup comprehension.

Run icon generation from the repository root after changing source artwork:

```bash
python3 scripts/icons/generate-icons.py
```

The firmware build uses checked-in generated masks and does not decode SVG or PNG files at runtime.

## Settings Screen

- Settings navigation rows use bitmap chevron icons that match the row text color.
- Row labels remain visible and the existing row touch targets remain unchanged.

## Keyboard Icons

- Caps lock, password visibility, backspace, OK, and Cancel use bitmap icons.
- Caps lock inactive uses the normal function-key foreground color.
- Caps lock active uses selected-state foreground treatment.
- Password visibility swaps between visibility and visibility-off icons as masking changes.
- OK and Cancel retain their semantic button backgrounds.

## Color Palette

| Purpose          | Color     |
| ---------------- | --------- |
| Background       | `#101010` |
| Text primary     | `#FFFFFF` |
| Text secondary   | `#A0A0A0` |
| Text dimmed      | `#606060` |
| Accent/fill      | `#3DAEFF` |
| Icon active      | `#35D06E` |
| Gauge background | `#303030` |

## Touch Targets

Touch hitboxes should be sized for direct finger input, with setup keyboards and keypads using large
targets appropriate for the 480x320 display. Do not compensate for alignment problems with
screen-specific hitbox offsets. Touch coordinates are globally calibrated in `TouchManager` with the
9-point affine transform described in `docs/hardware.md`.

## Animation

- Volume arc updates should complete quickly, around 200 ms.
- Screen transitions may be used for settings/setup navigation when they do not distract from the
  passive display behavior.
