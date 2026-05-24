# UI Design Specification — MarantzVolumeMonitor v2

Target: 4" 480×320 SPI TFT (Dark Mode)
Status: Finalized

---

## 1. Home Screen (Final Mockup)

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

### Behavior Notes

- Volume number is large, centered, high contrast.
- Volume bar animates only when volume changes.
- MODE text shows the exact string from the receiver.
- Tiles represent audio families:
  - Dolby → highlight if MODE contains “Dolby”
  - DTS → highlight if MODE contains “DTS”
  - PCM → highlight if MODE contains “PCM”
  - Other → highlight if none of the above
- All tiles are always visible (no layout shifting).
- Settings gear is small, subtle, and in the corner.

---

## 2. Color Palette (Dark Mode)

### Background

- Primary background: `#101010`
- Panel background: `#181818`

### Text

- Primary text: `#FFFFFF`
- Secondary text: `#A0A0A0`
- Dimmed text: `#606060`

### Volume Bar

- Bar fill: `#3DAEFF`
- Bar background: `#303030`
- Peak (optional): `#FF7043`

### Tiles

- Active tile background: `#3DAEFF`
- Active tile text/icon: `#000000`
- Inactive tile background: `#303030`
- Inactive tile text/icon: `#A0A0A0`

### Settings Gear

- Icon color: `#A0A0A0`
- Highlight on tap: `#3DAEFF`

---

## 3. Typography

### Volume Number

- Size: 72–96 px
- Weight: Bold
- Alignment: Center

### Labels (SRC, MODE)

- Size: 20–24 px
- Weight: Medium
- Alignment: Left/Right as shown

### Tile Labels

- Size: 18–20 px
- Weight: Medium
- Alignment: Center

---

## 4. Spacing & Layout

### Margins

- Outer margin: 12–16 px
- Between major sections: 20–28 px

### Volume Bar

- Height: 20–24 px
- Width: ~70% of screen width
- Corner radius: 4–6 px

### Tiles

- Tile size: ~90×50 px
- Tile spacing: 10–12 px
- Touch target minimum: 48×48 px

### Settings Gear

- Size: ~32 px
- Touch target: 48×48 px
- Padding from edges: 12 px

---

## 5. Animation Rules

### Volume Change

- Number transitions smoothly over 150–250 ms.
- Bar animates to new value over same duration.
- No idle animation.

### Screen Transitions

- Settings panel slides in from right over 150–200 ms.
- Ease-in-out curve.
- Background dims slightly (5–10% opacity overlay).

### Tile Highlight

- Instant or 100 ms fade.
- No pulsing or looping animations.

---

## 6. UI Flow Diagram

```text
[ Home Screen ]
       |
       v
[ Settings Panel ]  <-- slide-in from right
       |
       +--> WiFi Setup
       |        |
       |        +--> Scan Networks
       |        +--> Enter Password (On-Screen Keyboard)
       |
       +--> Receiver Setup
       |        |
       |        +--> Auto Discovery (SSDP)
       |        +--> Manual IP Entry (Numeric Keyboard)
       |
       +--> Display Settings
       |        |
       |        +--> Brightness
       |        +--> Theme (Dark Only for Now)
       |
       +--> Back (slide-out)
```

---

## 7. Settings Screen Mockup

```text
+------------------------------------------------------------+
| Settings                                         [ ✕ ] |
| ------------------------------------------------------ |
|                                                        |
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
|                                                        |
+------------------------------------------------------------+
```

---

## 8. On-Screen Keyboard Mockup (QWERTY)

```text
+------------------------------------------------------------+
|  Q W E R T Y U I O P                                       |
|  A S D F G H J K L                                         |
|  Z X C V B N M                                             |
|                                                            |
|  [123]   [ Space ]   [← Back]   [Enter]                    |
+------------------------------------------------------------+
```

### Numeric Mode (for IP entry)

```text
+------------------------------------------------------------+
|  1 2 3                                                     |
|  4 5 6                                                     |
|  7 8 9                                                     |
|  . 0 ← Back   [Enter]                                      |
+------------------------------------------------------------+
```

---

## 9. Implementation Notes for Copilot CLI

- Save this file as `docs/ui/UI_Design.md`
- Save mockups as separate `.txt` files if preferred
- The CLI can read these files and generate:
  - `HomeScreen.cpp`
  - `SettingsScreen.cpp`
  - `Keyboard.cpp`
  - `DisplayManager.cpp`
- The CLI can also generate the drawing code using TFT_eSPI.

---

## 10. Final Home Screen Mockup (480×320 Proportions)

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

This is the final, approved design.
