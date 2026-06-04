# Contract: Receiver Setup Flow

## Entry Points

- Home Screen must offer receiver setup when Wi-Fi is connected and no receiver is configured.
- Settings Screen must offer receiver setup to replace an existing receiver configuration.
- Receiver setup must not require changing Wi-Fi settings.

## Discovery Screen

### Initial State

- Start SSDP discovery when the screen is opened.
- Show visible progress while discovery is active.
- Keep touch handling responsive during discovery.

### Results State

- Display one row per discovered receiver.
- Each row must show receiver name and IPv4 address.
- Duplicate IPv4 addresses must appear once.
- Provide actions for retry and manual IPv4 entry.

### Empty State

- If no compatible receiver is found within 15 seconds, show a recoverable empty state.
- Empty state must offer retry and manual IPv4 entry.

## Manual IPv4 Entry

- Accept IPv4 address input only.
- Reject empty, malformed, hostname, URL, and custom-port input before verification.
- Keep the user in the manual entry flow when validation fails.
- Provide a cancel/back path to discovery results or Settings/Home depending on entry source.

## Verification

- Verification succeeds only when live receiver status is returned for the selected IPv4 address.
- Verification failure must show a recoverable failure state within 15 seconds.
- A failed replacement attempt must not overwrite an existing saved receiver IP.
- Successful verification must save the receiver IP and return the user to the Home Screen.

## Offline Saved Receiver

- If a saved receiver becomes unreachable later, show offline/disconnected status.
- Do not clear the saved receiver configuration automatically.
- Let the user retry implicitly through normal polling or intentionally open receiver setup to replace it.
