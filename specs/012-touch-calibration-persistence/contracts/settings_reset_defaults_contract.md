# Contract: Settings Reset To Defaults

## Purpose

Define how `Settings` exposes the new recovery flow and how reset choices behave.

## Settings Menu Rules

- `Reset to defaults` appears as the last item in the `Settings` menu.
- Existing settings destinations remain labeled and reachable with pagination if needed.
- `Touch Calibration` remains reachable as a normal settings destination before `Reset to defaults`.

## Reset Selection Rules

- Opening `Reset to defaults` presents exactly three reset types: `Wi-Fi`, `Receiver`, and `Calibration`.
- Choosing a reset type moves into a confirmation step for that specific selection.
- The confirmation step provides explicit `Reset` and `Cancel` actions.

## Reset Behavior Rules

- Pressing `Reset` applies only the selected reset type.
- Pressing `Cancel` leaves all persisted settings unchanged and returns to the reset flow.
- Resetting `Calibration` immediately restores the shipped default touch profile in the current session.
- Completed reset flows return to `Settings`.

## Acceptance Rules

- Users can find `Reset to defaults` as the last settings item.
- Users can reset only one settings type at a time.
- Destructive changes never happen without explicit confirmation.
