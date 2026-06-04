# Research: Receiver Detection and Configuration

## SSDP Receiver Discovery

- **Decision**: Use SSDP/UPnP M-SEARCH over UDP to discover compatible Marantz/Denon receivers on the local network.
- **Rationale**: The project requirements already identify SSDP as the primary discovery method, and Marantz/Denon receivers commonly advertise via UPnP. This keeps setup automatic while staying within the ESP32 networking stack.
- **Alternatives considered**: Manual-only setup (too error-prone for first boot), IP range scanning (slow and noisy on home networks), static default IP assumptions (unreliable).

## Discovery Execution Model

- **Decision**: Implement discovery as a state machine that sends a search request, collects responses over repeated `update()` calls, and times out within 15 seconds.
- **Rationale**: The constitution requires passive appliance stability and responsive UI. A state machine avoids freezing the screen during UDP receive windows.
- **Alternatives considered**: Blocking discovery loop (violates UI responsiveness), background task abstraction (unnecessary complexity for this single flow).

## Receiver Verification

- **Decision**: Treat verification as successful only when the receiver responds to a live receiver-status request.
- **Rationale**: Reachability alone does not prove the target is a compatible receiver. The existing `MarantzClient::getStatus()` behavior already represents the user-visible data needed by the monitor.
- **Alternatives considered**: Ping or TCP reachability (false positives), accepting SSDP responses without verification (stale or wrong devices can be saved), asking the user to confirm without probing (weak acceptance test).

## Manual Entry Format

- **Decision**: Manual receiver configuration accepts IPv4 address only.
- **Rationale**: The project constitution explicitly calls out Receiver IP setup, and IPv4 entry is feasible on the device touchscreen. Hostname and custom port support would add input, validation, and failure cases outside the feature scope.
- **Alternatives considered**: Hostnames (requires name resolution UX and failure states), full URL or endpoint entry (too cumbersome on touchscreen), custom port (not required for standard Marantz status polling).

## Persistence Policy

- **Decision**: Save `receiverIp` only after verification succeeds; retain the previous saved receiver if replacement verification fails.
- **Rationale**: This prevents accidental loss of a working configuration. It also matches the spec requirement that temporary offline status must not clear saved receiver configuration.
- **Alternatives considered**: Save before verification (can persist bad targets), clear on repeated failures (bad behavior when the receiver is powered off), keep multiple candidates (out of scope).

## UI Flow

- **Decision**: Add dedicated receiver setup screens for discovery results, manual IPv4 entry, and verification feedback.
- **Rationale**: The current Wi-Fi flow has specialized list, keyboard, and status screens. Receiver setup has different input and verification rules, so dedicated screens keep the UI and networking responsibilities clear.
- **Alternatives considered**: Reuse Wi-Fi setup screens directly (SSID/password assumptions leak into receiver setup), put receiver setup in `HomeScreen` (violates screen separation and grows the setup state too much).
