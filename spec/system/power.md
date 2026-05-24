# Power Specification

## Power Options

A 4" TFT display draws **300–800 mA**, which is too much for weak USB supplies or the NodeMCU’s onboard regulator. You have **two valid powering options**:

### Option A — Power via USB Charger (Simple, Good for Development)

- Plug a **USB wall charger (2A recommended)** into the NodeMCU’s USB port
- The NodeMCU’s USB 5V rail powers the board and the **5V pin**, which can power the TFT.

**Wiring:**

```text
USB Charger → NodeMCU USB Port
NodeMCU 5V → TFT VCC
NodeMCU GND → TFT GND
```

This works well **as long as the USB charger can supply at least 1A**.

### Option B — Dedicated 5V Power Supply (Recommended for Final Build)

Use a **5V 2A regulated supply** and wire it directly:

```text
[5V Supply +] ────────┬────────> NodeMCU 5V
                      │
                      └────────> TFT VCC

[5V Supply GND] ──────┬────────> NodeMCU GND
                      │
                      └────────> TFT GND
```

## Requirements

- **Shared Ground**: A common ground between all components is mandatory.
- **Power Draw**: No constant animations to reduce unnecessary current consumption.
- **Backlight Control**: If the TFT’s LED/BL pin is raw (no resistor), use a transistor + resistor for PWM brightness control.
- **Logic Level**: Most ESP-compatible TFT modules accept 5V power but use 3.3V logic.

