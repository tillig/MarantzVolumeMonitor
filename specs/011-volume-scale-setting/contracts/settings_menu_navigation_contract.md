# Contract: Settings Menu Navigation

## Purpose

Define how `Settings` changes after adding the `Volume Display Scale` destination.

## Destination Order

- `Volume Display Scale` appears as a labeled `Settings` destination.
- Existing destinations for `Current Settings`, `Wi-Fi Setup`, `Receiver Setup`, and `Touch Calibration` remain reachable after the new destination is added.
- The `Settings` screen keeps its existing `OK` action to return to `HomeScreen`.

## Menu Presentation

- Settings destinations remain labeled rows using the established Material-compatible settings treatment.
- The menu must not switch to icon-only navigation as the default solution for the extra destination.
- If all destinations do not fit clearly on one screen, the menu uses a paginated overflow pattern with labeled `PREV` and `NEXT` controls consistent with constrained list screens.

## Navigation Rules

- Selecting `Volume Display Scale` opens the dedicated selection screen.
- Flows launched from `Settings` continue to return to `Settings` when complete.
- The new destination must not make existing settings rows ambiguous or harder to reach.

## Acceptance Rules

- A user can open `Volume Display Scale` in one selection after opening `Settings`.
- A user can still reach Wi-Fi setup, receiver setup, touch calibration, and `Current Settings` without hidden gestures.
- No destination loses its label or becomes ambiguous because of pagination.
