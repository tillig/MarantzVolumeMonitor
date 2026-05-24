# UI Design Specification

Target Display: 4" 480×320 SPI TFT (Dark Mode)

Status: Final

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

## 2. Home Screen (Final Mockup)

```text
+------------------------------------------------------------+
|                                                            |
|                          45.0 dB                           |
|                                                            |
|                 [====================----]                 |
|                                                            |
|   SRC: Blu-ray                     MODE: Dolby TrueHD       |
|                                                            |
|   [ 🎧 Dolby ]     [ 🔊 DTS ]     [ 🎼 PCM ]     [ … Other ]|
|                                                            |
|                                                [ ⚙ ]        |
+------------------------------------------------------------+
```

### 2.1 Home Screen Elements

#### Volume Number

- Centered, large, bold
- Size: 72–96 px
- High contrast

#### Volume Bar

- Horizontal bar beneath the number
- Width: ~70% of screen width
- Height: 20–24 px
- Rounded corners (4–6 px)
- Animates only when volume changes

#### Source & Mode Text

- SRC: <source>
- MODE: <exact string from receiver>
- Mode text is **verbatim** from Marantz API

#### Audio Family Tiles

Tiles represent **families**, not specific modes:

- Dolby
- DTS
- PCM
- Other (always visible)

#### Tile Behavior

- Highlight tile if MODE contains:
  - “Dolby” → Dolby tile
  - “DTS” → DTS tile
  - “PCM” → PCM tile
  - Otherwise → Other tile
- Only one tile is active at a time
- Active tile uses accent color
- Inactive tiles are dimmed

#### Settings Gear

- Small, subtle
- Top-right corner
- Touch target: 48×48 px
- Opens slide-in settings panel

---

## 3. Color Palette (Dark Mode)

### Background

- Primary: `#101010`
- Panel: `#181818`

### Text

- Primary: `#FFFFFF`
- Secondary: `#A0A0A0`
- Dimmed: `#606060`

### Volume Bar

- Fill: `#3DAEFF`
- Background: `#303030`
- Peak (optional): `#FF7043`

### Tiles

- Active background: `#3DAEFF`
- Active text/icon: `#000000`
- Inactive background: `#303030`
- Inactive text/icon: `#A0A0A0`

### Settings Gear

- Default: `#A0A0A0`
- On tap: `#3DAEFF`

---

## 4. Typography

### Volume Number

- Size: 72–96 px
- Weight: Bold
- Alignment: Center

### Labels (SRC, MODE)

- Size: 20–24 px
- Weight: Medium
- Alignment: Left/Right

### Tile Labels

- Size: 18–20 px
- Weight: Medium
- Alignment: Center

---

## 5. Spacing & Layout

### Margins

- Outer margin: 12–16 px
- Section spacing: 20–28 px

### Volume Bar

- Height: 20–24 px
- Width: ~70% of screen width

### Tiles

- Size: ~90×50 px
- Spacing: 10–12 px
- Touch target: ≥48×48 px

### Settings Gear

- Size: ~32 px
- Touch target: 48×48 px
- Padding: 12 px from edges

---

## 6. Animation Rules

### Volume Change

- Number transitions over 150–250 ms
- Bar animates to new value over same duration
- No idle animation

### Screen Transitions

- Settings panel slides in from right (150–200 ms)
- Ease-in-out curve
- Background dims 5–10%

### Tile Highlight

- Instant or 100 ms fade
- No pulsing or looping animations

---

## 7. UI Flow

```text
[ Home Screen ]
       |
       v
[ Settings Panel ]  <-- slide-in from right
       |
       +--> WiFi Setup
       |        |
       |        +--> Scan Networks
       |        +--> Enter Password (Keyboard)
       |
       +--> Receiver Setup
       |        |
       |        +--> Auto Discovery (SSDP)
       |        +--> Manual IP Entry (Numeric Keyboard)
       |
       +--> Display Settings
       |        |
       |        +--> Brightness
       |        +--> Theme (Dark)
       |
       +--> Back (slide-out)
```

---

## 8. Settings Screen Mockup

```text
+------------------------------------------------------------+
| Settings                                         [ ✕ ] |
| ------------------------------------------------------ |
| WiFi Setup →                                           |
| - Scan Networks                                        |
| - Enter Password                                       |
|                                                        |
| Receiver Setup →                                       |
| - Auto Discover                                        |
| - Manual IP Entry                                      |
|                                                        |
| Display Settings →                                     |
| - Brightness                                           |
| - Theme (Dark)                                         |
+------------------------------------------------------------+
```

---

## 9. On-Screen Keyboard Specification

## 9.1 Modes

- QWERTY (letters + punctuation)
- Symbols (full punctuation set)
- Numeric (for IP entry)

## 9.2 Requirements

- Touch targets ≥ 48×48 px
- Enter, Backspace, Space keys
- Mode switch keys:
  - [123]
  - [Symbols]
  - [ABC]

## 9.3 Character Sets

### QWERTY Mode

```text
Q W E R T Y U I O P
A S D F G H J K L
Z X C V B N M

[123]   [Symbols]   [ Space ]   [← Back]   [Enter]
```

### Symbols Mode

```text
! @ # $ % ^ & *
( ) - _ = + [ ]
{ } ; : ' " , .
/ ? \ | ~ `

[ABC]   [123]   [← Back]   [Enter]
```

### Numeric Mode (IP Entry)

```text
1 2 3
4 5 6
7 8 9
. 0 ← Back   [Enter]
```

---

## 10. Implementation Notes

- UI should be implemented using TFT_eSPI.
- All coordinates should be defined in a layout constants file.
- Animations should be time-based, not frame-based.
- Touch regions must be debounced.
- Settings panel should be a separate screen class.
- Keyboard should be modular and reusable.

---

## 11. Final Home Screen Mockup (480×320 Proportions)

```text
+------------------------------------------------------------+
|                                                            |
|                          45.0 dB                           |
|                                                            |
|                 [====================----]                 |
|                                                            |
|   SRC: Blu-ray                     MODE: Dolby TrueHD       |
|                                                            |
|   [ 🎧 Dolby ]     [ 🔊 DTS ]     [ 🎼 PCM ]     [ … Other ]|
|                                                            |
|                                                [ ⚙ ]        |
+------------------------------------------------------------+
```

This is the final, approved UI design.
