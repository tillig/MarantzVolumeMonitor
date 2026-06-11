# Contract: Settings Menu Navigation

## Purpose

Define how `Settings` changes after adding the `Current Settings` destination.

## Destination Order

- `Current Settings` is the first destination in the overall settings order.
- `Wi-Fi Setup`, `Receiver Setup`, and `Touch Calibration` remain available after the new destination is added.
- The `Settings` screen keeps its existing `OK` return action to leave `Settings`.

## Menu Presentation

- Settings destinations remain labeled rows using the established Material-compatible settings treatment.
- The menu must not switch to icon-only navigation as the default solution for the extra destination.
- If all destinations do not fit clearly on one screen, the menu uses a paginated overflow pattern with labeled `PREV` and `NEXT` controls consistent with constrained list screens.

## Pagination Rules

- Pagination controls appear only when more than one settings page exists.
- The active page must keep direct touch access to every visible destination row.
- Page transitions must preserve the same title, row treatment, and bottom `OK` action pattern.
- `Current Settings` remains first in the overall order even when pagination is used.

## Acceptance Rules

- A user can reach `Current Settings` from `Settings` in one selection after opening `Settings`.
- A user can still reach Wi-Fi setup, receiver setup, and touch calibration without hidden gestures.
- No destination becomes ambiguous because of pagination or reduced labeling.
