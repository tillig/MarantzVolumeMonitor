# Contract: Screen Application

## Purpose

Define how the shared Material-compatible catalog applies to each current on-device screen.

## Required Screen Audit

Implementation must audit and update every current screen:

| Screen | Required Pattern Coverage |
| --- | --- |
| `HomeScreen` | Home hierarchy, settings icon button, audio-family state, text roles, restrained volume animation |
| `SettingsScreen` | Navigation rows, standard settings actions, typography, spacing, row affordances |
| `NetworkListScreen` | Searching pattern, selection list rows, pagination/overflow, Manual/Rescan/Cancel buttons |
| `KeyboardScreen` | Keyboard key grid, compact icon-only function buttons, OK/Cancel alignment, input field |
| `SetupStatusScreen` | Wi-Fi connecting, success/failure, retry action, progress feedback |
| `ReceiverListScreen` | Searching pattern, receiver selection rows, Retry/Manual/Cancel buttons |
| `ReceiverIpScreen` | Manual receiver entry, standard actions, validation feedback |
| `ReceiverStatusScreen` | Verification progress, success/failure, Retry/Manual/Discover buttons |
| `CalibrationScreen` | Calibration instructions, touch targets, status/result treatment |

## Shared Rendering Expectations

- Screens should call shared UI-layer style helpers for repeated buttons, rows, searching states, and status blocks.
- Screens may keep domain-specific content and flow logic, but must not hand-roll shared styling.
- Screens must not perform network or storage work directly as part of style standardization.
- Existing `IconRenderer` and icon bitmap assets should be reused for button and state icons.
- Long strings must be truncated, wrapped, or otherwise handled without changing the shared geometry.

## Button Application Rules

- Standard actions use `standard-icon-text`.
- Keyboard function actions may use `keyboard-icon-only`.
- OK and Cancel on the keyboard must share the same y-coordinate, height, visual center, and icon alignment.
- Equivalent actions across screens use the same label and icon unless a catalog exception is documented.

## Searching Application Rules

- Wi-Fi network scanning and receiver discovery use one title/message/icon/progress layout.
- Searching progress appears immediately when searching starts.
- Searching progress movement must be update-driven and non-blocking.
- Manual entry, retry/rescan, and cancel actions stay available where the current flow allows them.

## List Application Rules

- Wi-Fi network rows and receiver candidate rows use comparable row geometry and typography.
- Primary and secondary metadata must fit inside the row without overlap.
- Empty and unavailable states use catalog state treatments and recovery actions.
- Pagination or overflow indicators must not conflict with bottom action buttons.

## Acceptance Criteria

- A screen-by-screen review finds no square buttons on one screen and rounded buttons on another unless the exception is documented.
- Standard OK, Cancel, Retry, Manual, and Save-like actions use the same button variant across equivalent contexts.
- Wi-Fi and receiver searching screens match title, message, icon, progress, and action placement.
- Wi-Fi and receiver selection lists match row height, typography roles, spacing, and selection state treatment.
- No screen text overlaps neighboring controls or disappears outside its intended bounds.
