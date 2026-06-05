# Data Model: Display Volume Level

## ReceiverStatusSnapshot

Represents the latest raw status fetched from the configured receiver.

### ReceiverStatusSnapshot Fields

- `isValid`: Whether the receiver responded with usable status data.
- `power`: Whether the receiver reports the main zone as on.
- `volume`: Current receiver volume as a floating-point value.
- `input`: Current source/input label.
- `mode`: Current listening mode label.

### ReceiverStatusSnapshot Validation Rules

- `isValid == false` means the home screen must not treat `volume` as live.
- `isValid == true` and `power == false` means the receiver is reachable but powered off.
- When live status is shown, `volume` must format to one decimal place on the home screen.

## HomeDisplayState

Represents the home screen's current top-level presentation mode.

### HomeDisplayState Variants

- `live`: Receiver status is valid and powered on; volume, source, mode, and audio-family indicators
  are shown normally.
- `poweredOff`: Receiver status is valid but reports power off; the home screen shows a powered-off
  message and keeps Settings accessible.
- `receiverUnavailable`: Receiver is configured but does not return usable live status; the home
  screen shows an unavailable message and keeps Settings accessible.
- `receiverSetupRequired`: Wi-Fi is available but no receiver is configured yet.
- `wifiConnecting`: Saved Wi-Fi exists but the device is not connected yet.
- `wifiSetupRequired`: No Wi-Fi configuration exists yet.

### HomeDisplayState Validation Rules

- `poweredOff` and `receiverUnavailable` must be visually distinct.
- `poweredOff` must not reuse the live volume presentation or show the last-known volume as if it
  were current.
- `receiverUnavailable` and `poweredOff` must both preserve a Settings path for reconfiguration.

## VolumePresentationState

Represents the live volume value as currently drawn and the target value the UI is moving toward.

### VolumePresentationState Fields

- `displayedVolume`: The current on-screen animated value.
- `targetVolume`: The latest live receiver volume that the UI should reach.
- `formattedVolumeText`: The one-decimal display string derived from `displayedVolume`.
- `displayedSweepDegrees`: The current gauge sweep derived from `displayedVolume`.
- `targetSweepDegrees`: The final gauge sweep derived from `targetVolume`.
- `animationActive`: Whether motion is still in progress.
- `lastAnimationTickMs`: Timestamp used to advance animation without blocking.

### VolumePresentationState Validation Rules

- `formattedVolumeText` must always render with one decimal place while in `live` state.
- `displayedVolume` must converge on `targetVolume` smoothly without abrupt snapping during normal
  motion.
- If a newer `targetVolume` arrives before animation completes, the state must retarget rather than
  finish toward the stale value first.

## DirtyRegionSet

Represents the screen regions that may be redrawn independently during live-volume motion.

### DirtyRegionSet Fields

- `gaugeRegion`: Bounding area covering the arc and endpoint caps.
- `valueRegion`: Bounding area covering the large numeric value and caption if needed.
- `sourceRegion`: Bounding area for source text when it changes.
- `modeRegion`: Bounding area for listening mode text when it changes.
- `stateRegion`: Bounding area used for powered-off or unavailable status content.

### DirtyRegionSet Validation Rules

- Per-frame animation updates should only repaint `gaugeRegion`, `valueRegion`, and any directly
  changed text regions.
- Switching between `live`, `poweredOff`, `receiverUnavailable`, and setup states may trigger a full
  layout redraw.
- Dirty regions must be sized to avoid leaving visual artifacts from previous frames.

## HomeInteractionRule

Represents the touch behavior available from each non-live state.

### HomeInteractionRule Fields

- `state`: One of the `HomeDisplayState` variants.
- `settingsAccess`: Whether the top-right Settings affordance or equivalent receiver-settings path is
  available.
- `primaryTapAction`: The fallback tap action when the screen is in a setup-required state.
- `supportingMessage`: User-facing explanation of the current state and next step.

### HomeInteractionRule Validation Rules

- `poweredOff` and `receiverUnavailable` must both expose Settings access for receiver reconfiguration.
- Setup-required states must continue to preserve their existing onboarding path.
- Supporting messages must not imply that stale volume is current.
