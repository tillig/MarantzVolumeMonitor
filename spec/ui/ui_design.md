# UI Design Specification

Target Display: 4" 480×320 SPI TFT (Dark Mode)

Status: Final (Updated with Circular Gauge)

---

## 1. Overview

This document defines the complete UI design for MarantzVolumeMonitor v2.
It includes layout, typography, color palette, touch behavior, animation rules, and mockups for all screens.

The UI is designed to be:

- Readable from 10–15 feet away
- Calm and non-distracting
- Appliance-like (no constant animations)
- Touch-friendly
- Dark-mode optimized

---

## 2. Home Screen (Final Layout)

```text
+------------------------------------------------------------+
|                                                            |
|                           /  \                             |
|                          | 45 |                            |
|                           \__/                             |
|                                                            |
|      Blu-ray                           Dolby TrueHD        |
|                                                            |
|   ( Dolby )      (  DTS  )      (  PCM  )      ( Other )   |
|                                                            |
|                                                [ ⚙ ]        |
+------------------------------------------------------------+
```

### 2.1 Home Screen Elements

#### Volume Number

- Centered inside the circular gauge.
- Large, bold white text.
- Size: 96 px (Font 8).

#### Volume Arc Gauge

- A circular arc that wraps around the volume number.
- **Inner Radius**: ~80 px.
- **Outer Radius**: ~100 px.
- **Arc Span**: -225° to +45° (approx 270 degree sweep).
- **Color**: Gradient from Green (low volume) to Yellow (medium) to Red (high).
- **Unfilled**: Dark gray shadow color (#303030).

#### Source & Mode Text

- Positioned below the volume gauge.
- **Left**: Actual source value (e.g., "Blu-ray"). No prefix.
- **Right**: Actual mode value (e.g., "Dolby TrueHD"). No prefix.
- **Size**: 24 px (Font 4) for high visibility.

#### Audio Family Tiles

- Dolby, DTS, PCM, Other.
- **Shape**: Highly rounded edges (pills).
- **Background**: Slight gray shaded background (#303030).
- **Active State**: Accent color background (#3DAEFF) with black text.
- **Inactive State**: Gray background with secondary text color.

#### Settings Gear

- Small, subtle.
- Bottom-right corner.
- Touch target: 48×48 px.

---

## 3. Color Palette (Dark Mode)

### Background Color

- Primary: `#101010`
- Panel: `#181818`

### Text Color

- Primary: `#FFFFFF`
- Secondary: `#A0A0A0`
- Dimmed: `#606060`

### Arc Gauge Colors

- Low: `#00FF00` (Green)
- High: `#FF0000` (Red)
- Shadow: `#303030`

---

## 4. Typography

### Volume Number Typography

- Size: 96 px (Font 8)
- Weight: Bold
- Alignment: Center

### Labels (Source/Mode) Typography

- Size: 24 px (Font 4)
- Weight: Medium
- Alignment: Left/Right

---

## 5. Animation Rules

- Volume changes should update the arc length smoothly.
- No idle animations.

---

## 6. Implementation Notes

- Use `_tft.drawArc()` for the circular volume gauge.
- Calibration values from `TouchManager` must be used for the settings button hit box.
