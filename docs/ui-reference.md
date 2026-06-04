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
|   ( Dolby )      (  DTS  )      (  PCM  )      ( Other )   |
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

- A small gear icon in the top-right corner opens Settings from the normal Home Screen.
- Settings exposes touch calibration, Wi-Fi setup, and receiver setup.
- Flows launched from Settings should return to Settings when complete; the Settings OK button
  returns to the Home Screen.

### Source And Mode Text

- Typography: white, Font 4
- Vertical position: centered near `Y=255`
- Source: left-aligned near `X=40`
- Mode: right-aligned near `X=440`
- Show raw receiver values without labels such as `SRC:`

### Audio Family Tiles

- Families: Dolby, DTS, PCM, Other
- Typography: secondary gray, Font 2
- Layout: bottom row near `Y=278`
- Shape: rounded pills, approximately 102x32 px
- Active state: accent blue background with black text

## Color Palette

| Purpose          | Color     |
| ---------------- | --------- |
| Background       | `#101010` |
| Text primary     | `#FFFFFF` |
| Text secondary   | `#A0A0A0` |
| Text dimmed      | `#606060` |
| Accent/fill      | `#3DAEFF` |
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
