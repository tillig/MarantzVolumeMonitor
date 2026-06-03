# Quickstart: Wi-Fi Setup UI

## Integration Scenario

To test the Wi-Fi Setup flow on a fresh device:

1. **Flash Firmware**: Upload the code using `pio run -t upload`.
2. **Clear Config**: If the device already has Wi-Fi set up, use the "Reset Configuration" option in the Settings menu (or manually delete `config.json` via LittleFS tools).
3. **Boot**: The device should display the **"UNCONFIGURED"** message on the Home Screen.
4. **Trigger Setup**: Tap anywhere on the setup screen except the small `CAL` button in the lower-right corner.
5. **Follow Flow**:
    - Observe the "Scanning..." spinner.
    - Select your network from the list.
    - Enter password using the on-screen keyboard.
    - Verify the "Connecting..." feedback.
6. **Verify**: Upon success, the credentials should be written to `config.json`, and the Home Screen should show Wi-Fi connected status even if receiver setup is still pending.

## Touch Verification

If touch alignment regresses:

1. Boot to the **"UNCONFIGURED"** Home Screen.
2. Tap the small `CAL` button in the lower-right corner to open the calibration capture tool.
3. Tap all 9 crosshairs once and copy the serial output from `CALIBRATION_SESSION_BEGIN` through `CALIBRATION_SUMMARY_END`.
4. Recalculate the affine coefficients in `TouchManager`; do not reintroduce per-screen touch offsets unless hardware data proves the affine model is wrong.

## Mock Data

During development, you can mock the network scan results in `WiFiManager::scanNetworks()` to test the list rendering without actual hardware radio activity.
