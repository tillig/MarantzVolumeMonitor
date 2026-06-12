# Research: Touch Calibration Persistence

## Persisted Calibration Profile Shape

- Decision: Extend `config.json` with an optional nested touch-calibration profile that stores a schema version and the six affine coefficients, while treating the current hardcoded coefficients as the shipped default when no saved profile exists.
- Rationale: A nested calibration object groups related data cleanly, makes selective reset of `Calibration` straightforward, and keeps the existing default coefficients available even if the saved profile is missing or invalid. This also avoids a second file and keeps all user configuration ownership inside `ConfigStore`.
- Alternatives considered:
  - Keep calibration hardcoded only: rejected because the feature requires per-device persistence without source edits.
  - Store calibration in a separate file: rejected because it complicates reset behavior and adds more file-lifecycle cases for no user-facing benefit.
  - Flatten six new top-level config keys: rejected because it makes selective reset and config readability worse than a grouped calibration object.

## Runtime Calibration Application

- Decision: `TouchManager` keeps the shipped default coefficients as compile-time constants, exposes a runtime calibration profile API, and allows `main.cpp`, calibration save, and reset flows to apply either the saved profile or the default profile immediately without reboot.
- Rationale: `TouchManager` already owns the mapping from raw touch points to screen coordinates, so runtime profile switching belongs there. Applying the active profile immediately satisfies the spec without forcing reboot-only behavior or screen-specific offsets.
- Alternatives considered:
  - Apply saved calibration only after reboot: rejected because the spec requires successful calibration to take effect in the same session.
  - Let each screen load calibration separately: rejected because it violates the repo's central touch-ownership model and risks per-screen drift.

## Guarded Calibration Capture

- Decision: Replace the current serial-capture-only tap handling with a non-blocking calibration session state machine that accepts a sample only when the mapped tap lands inside the active target radius, requires a release before arming the next target, and rejects samples that are implausibly close to previously accepted raw points for different targets.
- Rationale: The current `TouchManager` confirmation window and main-loop debounce are not enough to protect the calibration workflow from double-taps or mis-targeted samples. Step-local spatial validation and explicit re-arm rules directly address the failure mode the user described without weakening normal touch behavior elsewhere.
- Alternatives considered:
  - Rely only on the existing global debounce timing: rejected because calibration errors can still pass when a bad sample looks like a separate valid press.
  - Validate only against expected target radius in screen space: rejected because raw-point proximity checks add a second layer of defense against accidental duplicate samples.
  - Allow overlapping target capture regions: rejected because it weakens the guarantee that one intended target cannot satisfy another step.

## On-Device Affine Solve

- Decision: Compute the six affine coefficients on-device from the existing 9-point calibration dataset using a small deterministic least-squares solve, then validate the candidate profile before saving or applying it.
- Rationale: The repo already uses a 9-point affine model because the panel shows cross-axis skew. Solving on-device removes the serial/manual step while preserving the calibration model that already matches the hardware.
- Alternatives considered:
  - Revert to axis min/max calibration: rejected because prior hardware validation showed it was not accurate enough.
  - Reduce to a 3-point solve: rejected because it throws away the redundancy that helps reject bad samples and fit the observed skew.
  - Keep serial export for offline recompute: rejected because it does not satisfy the feature goal.

## Reset To Defaults Flow

- Decision: Add a dedicated `Reset to defaults` settings child flow as the last settings item, present `Wi-Fi`, `Receiver`, and `Calibration` as separate reset types, and require an explicit `Reset` or `Cancel` confirmation before clearing the selected type.
- Rationale: The clarified spec intentionally broadens recovery beyond calibration only, and selective reset is safer than a monolithic wipe. A dedicated settings child flow keeps destructive behavior visible, labeled, and consistent with the repo's explicit confirm/discard patterns.
- Alternatives considered:
  - Add a calibration-only restore action inside the calibration screen: rejected because the clarified scope requires Wi-Fi and receiver reset too.
  - Reset immediately on selection: rejected because the clarified spec requires explicit destructive confirmation.
  - Use a full config reset only: rejected because it is broader than requested and creates unnecessary setup rework.
