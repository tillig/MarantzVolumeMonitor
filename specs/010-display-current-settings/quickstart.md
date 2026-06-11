# Quickstart: Display Current Settings

## Scope

Implement a new read-only `Current Settings` screen and the related `Settings` menu update. Do not add new editable settings, new persistent config fields, or icon-only settings navigation.

## References

- Feature spec: `specs/010-display-current-settings/spec.md`
- Plan: `specs/010-display-current-settings/plan.md`
- Contracts:
  - `specs/010-display-current-settings/contracts/current_settings_screen_contract.md`
  - `specs/010-display-current-settings/contracts/settings_menu_navigation_contract.md`
  - `specs/010-display-current-settings/contracts/validation_contract.md`
- Durable UI reference: `docs/ui-reference.md`

## Implementation Outline

1. Add `CurrentSettingsScreen` as a new read-only screen under `src/ui/Screens/`.
2. Update `SettingsScreen` so `Current Settings` is the first destination and the expanded menu uses paginated labeled navigation if needed.
3. Read saved Wi-Fi SSID and receiver IP from `ConfigStore`.
4. Extend `WiFiManager` with the current Wi-Fi signal access needed for the overview screen if no suitable accessor already exists.
5. Reuse or extend a network-layer helper to resolve receiver name or type from the configured receiver IP without adding a new saved config field.
6. Refresh current Wi-Fi and receiver status when `CurrentSettingsScreen` opens and continue refreshing while it remains visible.
7. Present Wi-Fi strength using the existing graphic plus numeric RSSI when available.
8. Keep the screen read-only and return to `Settings` with a single `OK` action.
9. Update `docs/ui-reference.md` and `README.md` with the new settings-menu and current-settings behavior.

## Validation

Run static validation:

```bash
pre-commit run --all-files
git diff --check
/Users/travis.illig/.platformio/penv/bin/platformio run
```

Run hardware verification:

1. Open `Settings` and confirm `Current Settings` appears first.
2. If pagination is present, confirm `PREV` and `NEXT` keep all settings destinations reachable.
3. Open `Current Settings` and confirm the screen is read-only.
4. Confirm `OK` returns to `Settings`.
5. With Wi-Fi configured and connected, confirm SSID, monitor IP, Wi-Fi icon, and numeric RSSI are visible.
6. Disconnect Wi-Fi and confirm saved SSID remains identifiable while live Wi-Fi details show unavailable.
7. With receiver configured and reachable, confirm receiver IP, receiver power, and receiver name or type appear when available.
8. Turn the receiver off and confirm the screen shows powered-off state rather than general unavailability.
9. Make the configured receiver unreachable and confirm the screen shows unavailable state while keeping the saved receiver IP visible.
10. Leave the screen open, change Wi-Fi or receiver state, and confirm the visible values update within 3 seconds.
