# Contract: Current Settings Screen

## Purpose

Define the read-only `Current Settings` overview screen, its required content, and its refresh behavior.

## Entry And Exit

- The screen opens only from the `Current Settings` entry in `Settings`.
- The screen is read-only and must not present save, edit, rescan, or setup-launch actions.
- The screen provides one bottom `OK` action that returns directly to `Settings`.

## Screen Content

### Wi-Fi Section

- Show the saved Wi-Fi SSID when one exists.
- Show `setup required` when no Wi-Fi SSID is saved.
- Show the current monitor IP address when Wi-Fi is connected.
- Show current Wi-Fi signal strength using the established Wi-Fi strength graphic.
- Show numeric RSSI when available.
- If live Wi-Fi details are unavailable, label them as unavailable instead of leaving blank space.

### Receiver Section

- Show the saved receiver IP address when one exists.
- Show `setup required` when no receiver IP is saved.
- Show receiver name or type when the network layer can resolve it.
- Show receiver power as `On`, `Off`, `Unavailable`, or `Not configured` using terminology consistent with the rest of the spec.
- Preserve the distinction between confirmed powered off and generally unavailable receiver state.

## Refresh Behavior

- Load current Wi-Fi and receiver status when the screen opens.
- Continue refreshing live status while the screen remains visible.
- Use the existing 1-second polling rhythm as the refresh cadence.
- Reflect live state changes within 3 seconds.
- Cache receiver identity lookup for the current screen session once it has been resolved or confirmed unavailable.

## Presentation Rules

- Use a read-only section layout rather than tappable settings-row styling for each field.
- Keep labels and values readable on the 480x320 dark high-contrast display.
- Do not allow labels, values, the Wi-Fi graphic, or the `OK` action to overlap or clip.
- Clearly differentiate `setup required`, `unavailable`, and current confirmed values.
