# Contract: Volume Motion And Redraw

## Purpose

Define the required home-screen motion behavior for gauge and numeric volume updates.

## Live Volume Presentation

- The numeric volume display shows one decimal place whenever live receiver status is available.
- The gauge arc and numeric value move together as one interaction.

## Motion Rules

- Use one calm, Material-compatible standard motion treatment for both the gauge and numeric value.
- Motion decelerates into the final value instead of stopping abruptly.
- Motion avoids decorative bounce or attention-seeking overshoot.
- If a new volume target arrives during motion, the animation retargets smoothly to the latest value.

## Redraw Rules

- Per-frame motion updates redraw only directly affected regions:
  - gauge arc
  - gauge endpoint caps
  - numeric value
  - any live labels that actually changed
- Per-frame motion updates must not fill and repaint the full screen.
- Full-screen redraws are reserved for coarse state changes such as switching between live,
  powered-off, unavailable, and setup-required layouts.

## Responsiveness Rules

- Motion must not block the home-screen update loop.
- Motion must not interfere with the existing receiver polling cadence.
- Motion must remain readable during both small steps and larger jumps.

## Acceptance Rules

- Rapid consecutive receiver changes settle on the newest value without visibly restarting from stale
  positions.
- Unaffected screen regions do not flash during motion.
- The final displayed value matches the latest live receiver value with one decimal place.
