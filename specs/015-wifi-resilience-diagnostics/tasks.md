# Tasks: Wi-Fi Resilience and Diagnostics

**Input**: Design documents from `specs/015-wifi-resilience-diagnostics/`

**Prerequisites**: plan.md, spec.md

## Phase 1: Implementation

- [x] T001 Add `DiagnosticLog` ring buffer and `DeviceInfo` helpers in `src/diagnostics/`
- [x] T002 Add Wi-Fi event logging, modem sleep disable, and retry supervisor to `WiFiManager`
- [x] T003 Report the driver's disconnect reason on `SetupStatusScreen` and restore saved credentials after a failed attempt
- [x] T004 Record receiver poll failures and recoveries in `ReceiverMonitor`
- [x] T005 Log Home Screen state changes, including states hidden by the blank screen
- [x] T006 Add `DiagnosticsServer` with `/`, `/api/status`, and `/api/log`
- [x] T007 Add `Settings` > `Diagnostics` screen
- [x] T008 Update `README.md`, `docs/architecture.md`, and `assets/icons/inventory.md`

## Phase 2: Verification

- [ ] V001 Turn the access point off for two minutes, then on; the monitor reconnects without a touch and the log shows the retries
- [ ] V002 Enter a wrong password; the failure screen shows a reason and code, then the monitor reconnects to the saved network
- [ ] V003 Browse to the monitor's IP address and confirm status and log refresh
- [ ] V004 Open `Settings` > `Diagnostics` and confirm the newest event is first and the web address is shown
- [ ] V005 Confirm the Home Screen stays responsive while a browser polls the diagnostics page
- [ ] V006 Reset Wi-Fi from `Reset To Defaults`, then restart the access point; the monitor stays on `Unconfigured` and does not rejoin
