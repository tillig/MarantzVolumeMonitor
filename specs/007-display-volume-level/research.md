# Research: Display Volume Level

## Decision: Keep Volume Animation State Local To `HomeScreen`

Implement volume motion as `HomeScreen`-owned presentation state rather than introducing a new
cross-screen animation framework.

**Rationale**: This feature only animates one screen, one gauge, and one numeric value. A local state
object keeps the behavior easy to reason about and avoids spreading motion concerns into unrelated
screens.

**Alternatives considered**:

- Shared animation framework in `src/ui/`: rejected because only the home screen currently needs this
  behavior.
- Poll-triggered full redraws without persistent animation state: rejected because it would make
  smooth retargeting and partial redraws harder to implement correctly.

## Decision: Distinguish Live, Powered-Off, And Unavailable Receiver States

Treat the home screen as presenting three distinct receiver outcomes:

- Live receiver status
- Reachable receiver that reports powered off
- Unavailable or unverified receiver

**Rationale**: The spec explicitly separates powered-off from unavailable and assigns different user
expectations to each. The current `MarantzStatus` already exposes `power` and `isValid`, so the design
can classify those outcomes without inventing a new network contract.

**Alternatives considered**:

- Collapse powered-off into unavailable: rejected because it would contradict the clarified feature
  behavior.
- Show last-known volume while powered off: rejected by clarification because it implies live data.

## Decision: Use Material-Compatible Standard Motion With Smooth Retargeting

Animate both the gauge and numeric value using one calm, non-decorative motion treatment that settles
smoothly and can retarget when a newer volume arrives mid-animation.

**Rationale**: The spec requires shared motion treatment, deceleration near the end, and no jarring
reset on rapid changes. Standard motion better matches a passive appliance than expressive or bouncy
feedback.

**Alternatives considered**:

- Instant jumps to the new value: rejected because the clarified feature requires animated feedback.
- Decorative bounce or overshoot: rejected because it competes with the passive appliance experience.
- Separate motion styles for gauge and number: rejected because it would make the interaction feel
  inconsistent.

## Decision: Use Dirty-Region Redraws During Motion

Redraw only the gauge, numeric value, and directly affected status areas during volume animation.
Reserve full-screen redraws for state changes such as switching between live, powered-off, and
unavailable layouts.

**Rationale**: The current `draw()` path fills the entire screen. That is acceptable for coarse state
changes, but not for per-frame animation on a TFT where full redraws risk visible flashing and wasted
work.

**Alternatives considered**:

- Continue using full-screen redraws for every animation step: rejected because the spec explicitly
  forbids flashing and full-screen redraw during motion.
- Redraw only the numeric value and leave the gauge static: rejected because the feature requires both
  gauge and value motion.

## Decision: Preserve Existing Polling Ownership And Timing

Keep receiver polling in the existing home-screen update cycle and ensure animation progresses between
polls without adding new network requests or blocking delays.

**Rationale**: The constitution requires passive appliance stability and non-blocking UI behavior.
Animation should reflect the most recent polled target value, not create a separate fetch path.

**Alternatives considered**:

- Increase network polling frequency to drive smoother motion: rejected because motion should be a UI
  concern and tighter polling adds network churn without solving presentation cleanly.
- Use blocking delays to complete motion before the next update: rejected because it would hurt touch
  responsiveness.

## Decision: Document Durable Home-Screen Behavior In `docs/ui-reference.md` And `README.md`

Keep feature-specific contracts in this spec folder, but record stable home-screen behavior in durable
docs once implemented.

**Rationale**: One-decimal display rules, powered-off vs unavailable behavior, and home-screen motion
constraints are long-lived product behaviors that future contributors and users should be able to
find outside a single feature branch.

**Alternatives considered**:

- Keep all behavior notes only in the feature folder: rejected because that would hide durable product
  rules from normal contributor workflows.
