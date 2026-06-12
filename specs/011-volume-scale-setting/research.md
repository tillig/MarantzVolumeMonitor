# Research: Volume Display Scale

## Persisted Preference Source

- Decision: Reuse the existing `useDbScale` field in `ConfigStore` and `config.json` as the saved source of truth for the user's scale choice.
- Rationale: The repository already loads and saves `useDbScale` with a default of `false`, which maps naturally to the clarified default `0-100` mode. Reusing that field avoids storage migration work and keeps persistence ownership in the storage layer.
- Alternatives considered:
  - Add a new enum or string field for the scale: rejected because the current two-option requirement is already covered by the existing boolean and does not justify a schema change.
  - Keep the setting in UI memory only: rejected because the spec requires the choice to survive leaving `Settings` and device restart.

## Home-Screen Value Mapping

- Decision: Keep the home-screen gauge and animation on the existing normalized `0.0` to `100.0` scale while switching only the displayed number between normalized value and raw receiver dB value.
- Rationale: `HomeScreen` already derives a normalized display value with `receiver dB + 80`, while `MarantzClient` exposes raw receiver dB volume. Preserving the current normalized gauge path minimizes regression risk and matches the spec's explicit rule that the gauge stays the same.
- Alternatives considered:
  - Recompute the gauge on a dB scale when `dB` is selected: rejected because it would change the visual behavior the spec explicitly preserves.
  - Convert both modes from a single cached formatted string: rejected because the gauge and the number serve different display contracts and should stay decoupled.

## Selection-Screen Interaction Pattern

- Decision: Use a dedicated `Volume Display Scale` screen with a single-choice radio-style indicator, a pending selection state, and explicit `OK` and `Cancel` actions.
- Rationale: The clarified spec requires a menu entry that opens its own screen, keeps `Cancel` non-destructive, and makes the current choice clearly visible. A pending-choice model satisfies those rules without mutating stored config until `OK`.
- Alternatives considered:
  - Apply immediately when the user taps an option: rejected because it contradicts the accepted `OK`/`Cancel` confirmation flow.
  - Use color-only row highlighting: rejected because the spec requires a visible single-choice indicator and the selected state must not rely on color alone.

## Return Path And Settings Flow

- Decision: Return to `Settings` after both `OK` and `Cancel`.
- Rationale: `Settings` is the parent menu for this feature, and the clarified flow keeps it as the stable return target regardless of whether the pending change is saved or discarded. This matches the existing repo pattern where flows launched from `Settings` return there when complete.
- Alternatives considered:
  - Return to `HomeScreen` after `OK`: rejected because it breaks the clarified return-path rule and creates asymmetry between `OK` and `Cancel`.
  - Return to `HomeScreen` after both actions: rejected because it would make the selector behave unlike the rest of the settings menu and slow follow-up changes to other settings.
