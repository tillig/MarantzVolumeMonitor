# Tasks: Background Receiver Polling

**Input**: Design documents from `specs/014-background-receiver-polling/`

**Prerequisites**: plan.md, spec.md

## Phase 1: Implementation

- [x] T001 Add `ReceiverMonitor` with on-demand polling, stale-result expiry, and a result sequence number
- [x] T002 Read cached status in `HomeScreen` and `CurrentSettingsScreen` instead of fetching
- [x] T003 Add the `Connecting to receiver` Home state with Settings access
- [x] T004 Update `docs/architecture.md` and `docs/ui-reference.md`

## Phase 2: Verification

- [ ] V001 With the receiver unreachable, tap Settings on `Receiver unavailable`; Settings opens on the first tap
- [ ] V002 Boot with a reachable receiver; Home briefly shows `Connecting to receiver`, then live status
- [ ] V003 Change the volume quickly; the number and arc follow without a visible lag
- [ ] V004 Unplug the receiver's network cable while Home is live; Home shows `Receiver unavailable` within a few seconds
- [ ] V005 Spend more than a few seconds in Settings, then return Home; Home shows `Connecting to receiver` briefly, then live status
