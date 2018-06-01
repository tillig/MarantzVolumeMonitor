# Power Specification

## Power Options

### USB Power (Development)

- USB charger (2A recommended)
- NodeMCU USB → 5V rail → TFT VCC

### Dedicated 5V Supply (Production)

- 5V 2A regulated supply
- Parallel wiring to NodeMCU 5V and TFT VCC

## Requirements

- Shared ground between all components
- No constant animations to reduce power draw
- Backlight may be PWM-controlled
