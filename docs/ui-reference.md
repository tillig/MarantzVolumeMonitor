# UI Reference

## Principles

The main UI is optimized for a 15-foot viewing distance. It uses a high-contrast dark palette, large status text, and restrained animation so the device behaves like a passive appliance.

- Dark mode only.
- No idle animation.
- Volume changes may animate briefly.
- Setup and settings screens should stay touch-first and readable on the 4" display.

## Home Screen

```text
+------------------------------------------------------------+
|                                                    (gear)  |
|                           /  \                             |
|                          | 50 |                            |
|                           \__/                             |
|                                                            |
|      Blu-ray                           Dolby TrueHD        |
|                                                            |
|   [ Dolby ]      [  DTS  ]      [  PCM  ]      [ Other ]   |
|                                                            |
+------------------------------------------------------------+
```

### Volume Number And Gauge

- Center point: `(240, 140)`
- Volume number: white, Font 8 with Font 7 fallback for wider values, centered near `Y=139`
- Live volume always shows one decimal place on the normalized home scale of `0.0` to `100.0` (`receiver dB + 80`).
- Volume caption: dimmed gray, Font 2, centered near `Y=72`
- Gauge arc: outer radius 125 px, inner radius 109 px, 240-degree span
- Gauge colors: green/yellow/red fill over `#303030` background
- These home fonts are intentionally larger than setup-screen roles so the main status remains readable from 15 feet.
- Powered-off and unavailable receiver states do not reuse the live volume region or show stale last-known values as if they were current.

### Settings Entry

- A small bitmap settings icon in the top-right corner opens Settings from the normal Home Screen.
- The same settings icon remains available from powered-off and receiver-unavailable home states so receiver setup can be changed without rebooting.
- Confirmed powered-off receiver state first shows `Receiver off`, then the Home Screen blanks after 3 seconds so no idle status remains visible in the room.
- With the receiver-off backlight circuit documented in `docs/hardware.md` wired and validated, the same 3-second transition also turns off the TFT `LED/BL` backlight. TFT logic, touch, receiver polling, and Settings recovery remain active.
- While the Home Screen is blank because receiver-off was confirmed, touch wake remains active and the first tap wakes only. A separate visible tap on the settings icon is still required to open Settings.
- A non-Settings tap on visible `Receiver off` restarts the 3-second timer and keeps the message visible briefly.
- Settings exposes `Current Settings`, Wi-Fi setup, receiver setup, and touch calibration.
- Flows launched from Settings should return to Settings when complete; the Settings OK button returns to the Home Screen.
- Returning Home while the receiver is still confirmed off shows `Receiver off` again and restarts the 3-second blanking timer. Settings and settings-launched setup flows stay visible while active.
- Setup/boot states expose a bottom-right `Calibrate` maintenance action using the shared bottom action button size, position, and icon-with-text treatment.

### Source And Mode Text

- Typography: white, Font 4
- Vertical position: centered near `Y=255`
- Source: left-aligned near `X=28`
- Mode: right-aligned with the text edge near `X=452`
- Show raw receiver values without labels such as `SRC:`

### Audio Family Icons

- Families: Dolby, DTS, PCM, Other
- Layout: bottom row near `Y=278`
- Canvas: 102x32 px per family
- Active state: lit green icon
- Inactive state: dark gray icon matching the gauge background treatment
- Dolby and DTS use approved brand logo source assets.
- PCM uses a custom pulse-code step waveform icon.
- Other uses a custom uncategorized-audio waveform icon.
- If receiver status is unavailable, all family icons are inactive.

## Icon Assets

The durable icon inventory is [`assets/icons/inventory.md`](../assets/icons/inventory.md). It records each icon's source, usage note, target size, generated firmware symbol, screen usage, and runtime colors.

Source artwork is stored in `assets/icons/source/`. Generated previews are stored in `assets/icons/generated/`. Firmware bitmap masks are generated into `src/ui/assets/`.

Use Material-compatible geometry and visual weight for non-brand icons. Icons supplement setup text; do not remove labels needed for setup comprehension.

Run icon generation from the repository root after changing source artwork:

```bash
python3 scripts/icons/generate-icons.py
```

The firmware build uses checked-in generated masks and does not decode SVG or PNG files at runtime.

## Settings Screen

- Settings navigation rows use the same rounded selection-row treatment as setup selection lists.
- Row labels remain visible, supporting text clarifies the destination, and touch targets remain at least 40 px high.
- Settings order is `Current Settings`, `Wi-Fi Setup`, `Receiver Setup`, then `Touch Calibration`. Calibration is intentionally last because it is a maintenance/debug flow rather than a normal setup path.
- `Current Settings` opens a read-only overview screen for saved and live monitor state; it does not launch setup or editing directly.
- If all settings destinations do not fit cleanly on one screen with the standard row treatment, Settings uses labeled `PREV` and `NEXT` pagination controls above the bottom `OK` action rather than switching to icon-only navigation.
- Touch Calibration uses the shared `touch-calibration` crosshair icon in both the Settings row and the boot/setup action button.

## Current Settings Screen

- `Current Settings` is a read-only screen with separate `Wi-Fi` and `Receiver` sections plus one bottom `OK` action that returns to `Settings`.
- The screen shows each section as a compact one-line or two-line summary instead of stacked label/value rows so saved and live status remain readable on the 480x320 display.
- The screen shows saved Wi-Fi SSID, current monitor IP address, Wi-Fi signal strength, saved receiver IP address, receiver power state, and receiver name or type when the network layer can resolve it.
- Wi-Fi signal uses the same 1-to-3 bar graphic language as the Wi-Fi selection screen and shows numeric RSSI when available.
- Missing saved values show `Unconfigured`. Saved Wi-Fi without a live link shows `Disconnected`. A saved receiver without resolved UPnP identity falls back to `Configured receiver`. Live values that cannot be read show `Unavailable`. Powered-off receiver state remains distinct from general receiver unavailability by showing `Off` instead of `Unavailable`.
- The screen refreshes current Wi-Fi and receiver status while it remains visible, using the same restrained polling rhythm as the rest of the appliance UI.
- Live refreshes redraw only the card body content so the screen does not visibly flash during normal status updates.

## Keyboard Icons

- Caps lock, password visibility, backspace, OK, and Cancel use bitmap icons.
- Caps lock inactive uses the normal function-key foreground color.
- Caps lock active uses selected-state foreground treatment.
- Password visibility swaps between visibility and visibility-off icons as masking changes.
- OK and Cancel retain their semantic button backgrounds.

## Color Palette

| Purpose          | Color     |
| ---------------- | --------- |
| Background       | `#101010` |
| Text primary     | `#FFFFFF` |
| Text secondary   | `#A0A0A0` |
| Text dimmed      | `#606060` |
| Accent/fill      | `#3DAEFF` |
| Icon active      | `#35D06E` |
| Gauge background | `#303030` |

## Touch Targets

Touch hitboxes should be sized for direct finger input, with setup keyboards and keypads using large targets appropriate for the 480x320 display. Do not compensate for alignment problems with screen-specific hitbox offsets. Touch coordinates are globally calibrated in `TouchManager` with the 9-point affine transform described in [`docs/hardware.md`](./hardware.md).

## Animation

- Live volume motion uses one shared standard-motion treatment for both the arc and the number.
- Target duration should stay brief but visible on the TFT, roughly `420-820 ms` depending on change size.
- Motion should start promptly and decelerate smoothly into the final value without bounce.
- Rapid retargeting should continue from the in-flight value instead of snapping back to an older start point.
- Per-frame volume animation should redraw only changed gauge shapes and changed numeric glyphs. Do not push the whole gauge/value region per frame; full-screen redraws are reserved for coarse state changes such as switching between live, powered-off, unavailable, and setup-required layouts.
- Screen transitions may be used for settings/setup navigation when they do not distract from the passive display behavior.

## Material Design UI Catalog

This catalog covers the on-device 480x320 touchscreen UI in `src/ui/Screens`. It does not define mobile, desktop, or web UI. Material Design conventions are adapted to the appliance: dark mode, high contrast, 15-foot readability, stable layout, and touch setup clarity take priority over generic Material defaults.

### Priority Order

1. Preserve passive appliance readability and the home screen's volume-first hierarchy.
2. Keep setup flows touch-first with direct, visible recovery actions.
3. Use shared Material-compatible controls and states from [`src/ui/MaterialStyle.h`](../src/ui/MaterialStyle.h).
4. Reuse existing bitmap icons before adding new firmware assets.
5. Document any remaining screen-specific exception here before relying on it.

### Typography Roles

The firmware uses TFT_eSPI built-in bitmap fonts by number rather than desktop or mobile font families such as Roboto, Arial, Verdana, or Times New Roman. This is intentional: built-in TFT fonts are deterministic, fast, memory-light, and already readable on the 480x320 embedded display. Material Design's typography guidance is applied through consistent roles, hierarchy, spacing, and state treatment rather than exact Roboto typeface matching. Do not add generated smooth-font assets unless the memory, build, and readability tradeoffs are evaluated as a separate feature.

| Role                | Font | Color                     | Usage And Alignment                                                                        |
| ------------------- | ---- | ------------------------- | ------------------------------------------------------------------------------------------ |
| Home primary value  | 8    | Text primary              | Large centered volume value only.                                                          |
| Screen title        | 4    | Text primary              | Top title, centered for setup/search/status screens unless a documented exception applies. |
| Section label       | 4    | Text primary or secondary | Prominent setup labels, IP values, and major grouped information.                          |
| Body text           | 2    | Text secondary            | Instructions, explanatory text, and supporting copy, including centered setup subtitles.   |
| Button label        | 2    | Text primary              | Standard action and compact key labels.                                                    |
| List primary text   | 2    | Text primary              | Wi-Fi SSID, receiver name, and navigable row label.                                        |
| List secondary text | 2    | Text dimmed               | Signal strength, IP address, and metadata.                                                 |
| Status message      | 4    | State color               | Success, warning, failure, empty, and loading headline text.                               |
| Compact metadata    | 1    | Text dimmed               | Page counts, row numbers, and small validation messages.                                   |

Long primary values must be truncated to the available width with an ellipsis rather than resizing neighboring controls.

### Spacing And Geometry

| Token                  | Value  | Usage                                                          |
| ---------------------- | ------ | -------------------------------------------------------------- |
| Screen margin          | 20 px  | Normal left/right content inset.                               |
| Standard button height | 40 px  | Standard icon-with-text actions.                               |
| Standard button Y      | 270 px | Bottom action row on non-keyboard setup/settings screens.      |
| Standard button radius | 20 px  | Rounded icon-with-text action buttons.                         |
| Input radius           | 12 px  | Rounded outlined text-entry fields.                            |
| Keyboard button radius | 6 px   | Compact keyboard and keypad controls.                          |
| Selection row height   | 46 px  | Wi-Fi networks, receiver candidates, and future setup choices. |
| Selection row radius   | 8 px   | Selection list rows.                                           |
| Row gap                | 9 px   | Vertical space between repeated rows.                          |
| Button gap             | 12 px  | Horizontal space between standard actions.                     |
| Keyboard key gap       | 6 px   | Keyboard and keypad grid spacing.                              |
| Icon-to-label gap      | 8 px   | Standard button icon and label spacing.                        |
| Minimum touch target   | 40 px  | Smallest direct-touch control target.                          |

### Button Variants

| Variant                | Required Use                                                                                                                       | Treatment                                                                                                 |
| ---------------------- | ---------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| `standard-icon-text`   | OK, Cancel, Save, Retry, Back, Confirm, Manual, Rescan, Discover, Verify, and equivalent standard actions outside compact contexts | Rounded 40 px control, icon left of label, Font 2 label, shared fill/outline/state colors.                |
| `keyboard-icon-only`   | Keyboard OK, Cancel, backspace, visibility, and caps-lock compact controls                                                         | Rounded compact key, centered bitmap icon, aligned to the key's visual center.                            |
| `pagination-text`      | Previous/next page controls in constrained list screens                                                                            | Rounded text button with the same surface, outline, font, and touch target treatment as standard actions. |
| `documented-exception` | Controls that cannot use the standard or keyboard variants                                                                         | Must be listed in Exceptions with reason and verification constraints.                                    |

Equivalent actions must use the same icon, label treatment, height, corner radius, and state color across screens. The keyboard is the approved compact-context exception for icon-only OK and Cancel. Cancel actions use the error state color unless they are inside a compact keyboard key.

### Selection Lists

Wi-Fi network rows and receiver candidate rows use the shared selection row:

- Row rectangle: 46 px high, 8 px radius, panel fill, dim outline.
- Leading visual: receiver icon or Wi-Fi strength icon, centered at the row's vertical midpoint.
- Primary text: Font 2, primary color, single line, truncated within the row.
- Secondary text: Font 2, dimmed color, IP address or other useful metadata.
- Trailing text: compact metadata such as row number.
- Wi-Fi strength uses three signal segments: strong lights all three green, fair lights the lower two yellow and leaves the top dimmed, weak lights only the bottom red and leaves the upper segments dimmed.
- Empty or unavailable lists use a status block plus the same bottom recovery buttons as populated lists.
- Pagination metadata stays above bottom actions and previous/next controls render as rounded text buttons instead of raw links.

### Searching Screens

Wi-Fi scanning and receiver discovery use one searching pattern:

- Page title at the top center using the screen-title role and matching the Settings menu label: `Wi-Fi Setup` or `Receiver Setup`.
- A centered instruction line below the title using the body role.
- Scan icon centered in the upper content area.
- Restrained progress bar below the headline.
- Progress movement starts immediately for explicit searching screens.
- Progress movement updates only the progress bar region; full-screen redraws are reserved for state changes such as search complete or no results.
- Manual, rescan, and cancel actions remain in the bottom action row when the flow supports them.

Wi-Fi and receiver selection-list screens reuse the same centered setup header pattern:

- `Wi-Fi Setup` + `Select a Wi-Fi network`
- `Receiver Setup` + `Select a discovered receiver`

The title and instruction line stay centered in both scanning and selection states; they must not switch from centered to left-aligned when results appear.

### State Treatments

| State                         | Treatment                                                                         |
| ----------------------------- | --------------------------------------------------------------------------------- |
| Normal                        | Panel fill, dim outline, primary text.                                            |
| Pressed/focused               | Accent fill or outline where implemented by the active control.                   |
| Selected/success              | Green state color plus success icon or supporting text.                           |
| Warning                       | Yellow state color plus warning icon or explanatory text.                         |
| Error                         | Red state color plus failure icon, concise message, and recovery action.          |
| Disabled/inactive/unavailable | Dimmed text and icon treatment; do not imply a successful state.                  |
| Loading                       | Loading message plus restrained progress only when allowed by the operation type. |

Searching screens show progress immediately. Non-searching operations, such as Wi-Fi connection and receiver verification, show movement only after they remain active for at least 500 ms. Progress updates must be driven from screen `update()` methods and must not block touch handling.

### Screen Application Matrix

| Screen                 | Applied Patterns                                                                                            | Notes                                                                                                                                                  |
| ---------------------- | ----------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `HomeScreen`           | Home hierarchy, setup status blocks, settings icon, audio-family state                                      | The normal home layout remains volume-first and does not add idle progress animation.                                                                  |
| `SettingsScreen`       | Screen title, rounded settings rows, standard OK action                                                     | Settings destinations use the same row treatment as setup selection lists.                                                                             |
| `NetworkListScreen`    | Searching pattern, centered setup header, selection rows, pagination metadata, Manual/Rescan/Cancel buttons | Title stays `Wi-Fi Setup` in all states.                                                                                                               |
| `KeyboardScreen`       | Input field, keyboard key grid, compact icon-only function buttons                                          | OK and Cancel share y-coordinate, height, visual center, and icon center.                                                                              |
| `SetupStatusScreen`    | Wi-Fi loading, error status, retry action, 500 ms non-search progress threshold                             | Success still returns through the existing flow rather than adding a confirmation stop.                                                                |
| `ReceiverListScreen`   | Searching pattern, centered setup header, receiver selection rows, Manual/Rescan/Cancel buttons             | Title stays `Receiver Setup` in all states.                                                                                                            |
| `ReceiverIpScreen`     | Manual IP input, keypad, standard Cancel/Verify actions, validation message                                 | Numeric keypad uses compact key geometry; bottom actions use standard icon-with-text buttons.                                                          |
| `ReceiverStatusScreen` | Verification loading, success, failure, Retry/Manual/Discover actions                                       | Title remains Receiver Setup; loading, success, and error use the same status placement. Successful verification stays visible until the user taps OK. |
| `CalibrationScreen`    | Screen title, instruction text, success status, crosshair target                                            | The red crosshair is a documented calibration target exception.                                                                                        |

### Exceptions

| Component                            | Shared Rule Bypassed                                 | Reason                                                                                  | Required Checks                                                                             |
| ------------------------------------ | ---------------------------------------------------- | --------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- |
| Keyboard OK/Cancel/function controls | Standard actions normally use icon-with-text buttons | The keyboard has compact fixed keys and limited horizontal space                        | Icon-only keys must be aligned, centered, 40 px or taller, and documented as keyboard-only. |
| Calibration crosshair                | Standard status and button controls                  | Calibration requires a precise visual target, not a generic touch button                | Crosshair must remain visible and must not receive per-screen touch offsets.                |
| Home settings icon                   | Standard actions normally include text               | The home screen needs a low-noise settings affordance that does not compete with volume | Icon must stay in the top-right hit target and the home hierarchy must remain volume-first. |

### Hardware Visual Verification Checklist

- Confirm the home volume, source, listening mode, and audio-family state are identifiable from 15 feet within 5 seconds.
- Open every current screen and verify standard actions use icon-with-text buttons outside the documented keyboard/home exceptions.
- Verify keyboard OK and Cancel share y-coordinate, height, visual center, and icon center.
- Compare Wi-Fi and receiver searching screens for matching title, message, icon, progress, and action placement.
- Compare Wi-Fi and receiver selection lists for matching row height, typography, spacing, and state treatment.
- Force empty, unavailable, success, warning, and failure states where practical and verify each has a clear message and recovery action when recovery is possible.
- Verify no primary label, long SSID, receiver name, IP address, or button label overlaps or clips into adjacent controls.
