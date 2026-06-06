# Research: Receiver Off Screen Sleep

## Decision: Use Software Blank For This Feature

Render a black/blank screen when the receiver-off timeout expires. Do not require hardware backlight
control in this feature.

**Rationale**: The current hardware wiring keeps display power and backlight tied to 5V/VIN, and
hardware backlight switching requires separate circuit design and documentation. The requested user
experience can be validated now by blanking the TFT while keeping the device powered, polling, and
touch-responsive.

**Alternatives considered**:

- Physical backlight off: rejected for this feature because it requires hardware rewiring and should
  be planned independently.
- Full display module power cut: rejected because it risks disabling display logic and touch support
  and conflicts with the requirement that touch wake remains available.
- Leaving `Receiver off` visible indefinitely: rejected because it is the distracting state this
  feature is intended to remove.

## Decision: Keep Receiver-Off Sleep State In HomeScreen

Model receiver-off visible and blank states as Home Screen presentation states.

**Rationale**: The behavior applies only to the home receiver-off display. `HomeScreen` already
classifies live, receiver-off, unavailable, and setup-required conditions, and already handles the
top-right Settings entry.

**Alternatives considered**:

- Global screen sleep in `ScreenManager`: rejected because Settings and setup screens must remain
  visible while active, and the timeout is specific to confirmed receiver-off Home state.
- Main-loop sleep handling: rejected because `main.cpp` should stay focused on dispatching touch and
  update calls rather than owning screen-specific UX state.

## Decision: Wake Touch Is Wake-Only

The first touch while the monitor is blank because the receiver is off should wake the monitor and
stop there. If receiver-off status is still current, the wake result is visible `Receiver off`; if
receiver-off status can no longer be confirmed, the wake result is the latest current visible state.

**Rationale**: A dark display gives the user no target context. Wake-only behavior prevents an
accidental tap from both waking and activating Settings or a setup flow in one gesture.

**Alternatives considered**:

- Treat the touch position as a normal Home Screen tap after wake: rejected because the user cannot
  see the target before touching.
- Require a long press to wake: rejected because a simple tap is more discoverable and matches the
  feature request.

## Decision: Stay Blank If Receiver-Off Confirmation Is Lost After Blanking

If the monitor has already entered the receiver-off blank state, and later polling can no longer
confirm the receiver is still off, keep the monitor blank until the user touches it or fresh active
receiver status becomes available.

**Rationale**: The monitor already went quiet because receiver-off status had been confirmed. Staying
blank preserves the room-quiet goal and avoids surfacing a potentially transient unavailable message
without user intent. Touch wake remains the recovery path and must show the latest current state.

**Alternatives considered**:

- Immediately show `Receiver unavailable`: rejected because it reintroduces a visible idle message
  after the monitor was intentionally quieted.
- Use a grace timer before showing unavailable: rejected because it adds another timeout without a
  clear user benefit for this appliance.

## Decision: Continue Polling While Blanked

Keep normal receiver status refresh behavior active while the monitor is software-blanked.

**Rationale**: The monitor must automatically return to live volume display when the receiver becomes
active again. Continuing the existing polling cycle avoids adding a second state source or a manual
refresh requirement.

**Alternatives considered**:

- Stop polling while blanked and rely on user wake: rejected because the monitor could remain dark
  after the receiver powers on.
- Increase polling frequency while blanked: rejected because the existing 1-second home refresh
  cadence is already inside the constitution's 1-2 second polling target.

## Decision: Non-Settings Touches Reset Visible Receiver-Off Timeout

When the visible `Receiver off` screen is active, a touch outside the Settings entry should restart
the 3-second timeout and keep `Receiver off` visible.

**Rationale**: This makes visible receiver-off touch feedback simple: tapping the screen means "keep
the monitor awake briefly," while Settings remains an explicit icon tap. It avoids adding another
button or gesture.

**Alternatives considered**:

- Ignore non-Settings touches: rejected because the screen would go blank even after a user actively
  touched it.
- Immediately blank on non-Settings touch: rejected because it makes touch feel like a hidden
  command rather than a recovery/wake affordance.

## Decision: Settings Is Outside Receiver-Off Timeout

Do not apply the receiver-off timeout while Settings or settings-launched setup flows are active.

**Rationale**: Settings is the recovery path. A timeout while the user is configuring Wi-Fi, receiver
setup, or calibration would make the recovery flow unreliable.

**Alternatives considered**:

- Apply the timeout globally: rejected because it would interrupt setup and violate intuitive
  on-device setup.
- Add a separate Settings timeout: rejected as unrelated to the receiver-off screen sleep feature.
