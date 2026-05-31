# UI Design Specification

Target Display: 4" 480×320 SPI TFT (Dark Mode)

Status: Final (Hardware Verified)

---

## 1. Overview

This document defines the complete UI design for MarantzVolumeMonitor v2.
The UI is optimized for a 15-foot viewing distance using a high-contrast dark palette.

---

## 2. Home Screen (Final Layout)

```text
+------------------------------------------------------------+
|                                                            |
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

### 2.1 Home Screen Elements

#### Volume Number & Gauge

- **Center Point**: (240, 140)
- **Volume Number**: White, Font 8 (approx 72pt), centered at **Y=125**.
- **Volume Caption**: Dimmed Gray, Font 2, "VOLUME" centered at **Y=185**.
- **Gauge Arc**:
  - **Radii**: Outer 125px, Inner 109px (Thickness 16px).
  - **Span**: 240 degrees total.
  - **Angles**: Starts at 60° (8:00), ends at 300° (4:00) wrapping over top.
  - **Ends**: Rounded "pill" caps (Radius 8px) at Start, End, and Current Fill.
  - **Colors**: Gradient Fill (Green → Yellow → Red) over Shadow Background (#303030).

#### Source & Mode Text

- **Typography**: White, Font 4 (approx 26px).
- **Vertical Position**: Centered at **Y=255**.
- **Horizontal**: Left-aligned at X=40 (Source), Right-aligned at X=440 (Mode).
- **Formatting**: Raw value only (No prefixes like "SRC:").

#### Audio Family Tiles

- **Typography**: Secondary Gray, Font 2.
- **Vertical Position**: **Y=278** (10px margin from bottom).
- **Shape**: Rounded pills (Size: 102x32px, Radius: 16px).
- **Active State**: Accent Blue background (#3DAEFF) with Black text.

---

## 3. Color Palette (Dark Mode)

- **Background**: `#101010` (0x0841)
- **Text Primary**: `#FFFFFF` (TFT_WHITE)
- **Text Secondary**: `#A0A0A0` (0xA514)
- **Text Dimmed**: `#606060` (0x632C)
- **Accent/Fill**: `#3DAEFF` (0x3D7F)
- **Gauge Background**: `#303030` (0x3186)

---

## 4. Animation Rules

- **Volume Update**: Arc length and end-cap position update over 200ms.
- **Screen Transition**: Settings panel (future) slides from right.

---

## 5. Implementation Notes

- Coordinate system for `drawArc`: 0° is Bottom (6:00).
- Coordinate system for `fillCircle` caps: Math radians (Arc Angle + 90° offset).
- Touch hitboxes for pills: (X, 278, 102, 32).
