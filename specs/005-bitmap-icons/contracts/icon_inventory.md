# Contract: Icon Inventory

## Purpose

Define the durable table required for all icon assets.

## Location

Implementation must create or update:

```text
assets/icons/inventory.md
```

`docs/ui-reference.md` must point contributors to this inventory.

## Required Columns

| Column | Requirement |
| ------ | ----------- |
| Asset ID | Stable kebab-case id |
| Meaning | One user-facing concept |
| Category | `control`, `status`, `audio-family`, or `setup` |
| Source Type | `material-symbol`, `brand-logo`, or `custom` |
| Source | Repository path and upstream URL when applicable |
| License / Usage Note | License, trademark note, or owner approval |
| Target Size | Generated display size |
| Screen Usage | Screens or states using the icon |
| Runtime Colors | Allowed foreground treatments |

## Initial Required Assets

| Asset ID | Meaning | Source Type | Target Size | Screen Usage |
| -------- | ------- | ----------- | ----------- | ------------ |
| `audio-dolby` | Dolby audio family | `brand-logo` | `102x32` | Home audio family row |
| `audio-dts` | DTS audio family | `brand-logo` | `102x32` | Home audio family row |
| `audio-pcm` | PCM audio family | `custom` | `102x32` | Home audio family row |
| `audio-other` | Other audio family | `custom` | `102x32` | Home audio family row |
| `settings` | Open settings | `material-symbol` | `24x24` | Home top-right settings control |
| `chevron-right` | Navigate to listed setting | `material-symbol` | `24x24` | Settings rows |
| `keyboard-caps-lock` | Toggle caps lock | `material-symbol` | `24x24` | Keyboard function key |
| `keyboard-visibility` | Show or hide keyboard input | `material-symbol` | `24x24` | Keyboard function key |
| `keyboard-visibility-off` | Hide visible keyboard input | `material-symbol` | `24x24` | Keyboard function key when input is visible |
| `keyboard-backspace` | Delete previous character | `material-symbol` | `24x24` | Keyboard function key |
| `keyboard-ok` | Confirm keyboard input | `material-symbol` | `24x24` | Keyboard function key |
| `keyboard-cancel` | Cancel keyboard input | `material-symbol` | `24x24` | Keyboard function key |
| `scan` | Search/discovery in progress | `material-symbol` | `32x32` | Wi-Fi and receiver setup |
| `manual-entry` | Enter details manually | `material-symbol` | `24x24` | Wi-Fi and receiver setup action buttons |
| `retry` | Retry failed action | `material-symbol` | `24x24` | Setup action buttons and failure states |
| `success` | Completed successfully | `material-symbol` | `32x32` or `48x48` | Setup result states |
| `warning` | Recoverable warning | `material-symbol` | `32x32` or `48x48` | Setup result states |
| `failure` | Failed action | `material-symbol` | `32x32` or `48x48` | Setup result states |
| `wifi` | Wi-Fi state | `material-symbol` | `32x32` | Wi-Fi setup/status |
| `receiver` | Receiver state | `custom` or `material-symbol` | `32x32` | Receiver setup/status |

## Brand Approval Notes

The inventory must include this note for both `audio-dolby` and `audio-dts`:

```text
Project owner supplied this SVG source candidate and approved use for this project on 2026-06-04.
```
