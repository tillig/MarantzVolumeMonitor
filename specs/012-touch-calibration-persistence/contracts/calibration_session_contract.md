# Contract: Calibration Session

## Purpose

Define the user-visible and runtime behavior of the on-device touch calibration flow.

## Screen Content

- The screen title remains `Touch Calibration`.
- The screen explains that calibration is performed directly on the device and no serial monitor or code update is required.
- The active point index and target label remain visible throughout the session.
- Success and rejection states use clear status messaging and preserve the return path to either `Settings` or `Home`.

## Capture Rules

- The screen collects points in the fixed nine-point order already used by the current calibration layout.
- A sample is accepted only if the mapped tap falls inside the active target boundary.
- Target boundaries for different points must not overlap.
- The session must require a full touch release before arming the next point.
- A second touch from the same intended press must not satisfy the next point.
- Samples that are implausibly close to previously accepted raw points for different targets are rejected and do not advance the session.

## Validation And Apply Rules

- The screen computes a candidate affine profile after all nine valid points are accepted.
- The candidate profile must pass numeric sanity checks before it can be applied or saved.
- If validation fails, the active saved profile remains unchanged and the user is told to retry.
- If validation succeeds, the new profile is applied immediately and persisted through `ConfigStore`.

## Exit Rules

- Leaving the screen before successful completion preserves the previously active profile.
- A successful calibration returns to the caller with the new profile already active.
- A failed or canceled calibration leaves the existing profile active and keeps calibration reachable for another attempt.

## Acceptance Rules

- Users can complete calibration on-device without external tools.
- Out-of-bounds taps and duplicate-like taps do not advance the point index.
- A bad calibration attempt cannot overwrite a working saved or default profile.
