# Contributing to Marantz Volume Monitor v2

Welcome! We appreciate your interest in contributing to the Marantz Volume Monitor modernization project. Whether you are a human developer or an AI assistant, this guide will help you understand the project's standards and workflows.

## Development Environment

- **Framework:** Arduino
- **Platform:** Espressif 8266 (NodeMCU v2)
- **Tooling:** PlatformIO
- **Key Libraries:**
  - `TFT_eSPI`: High-performance graphics library.
  - `XPT2046_Touchscreen`: Touch controller driver.
  - `ArduinoJson`: Configuration serialization.
  - `LittleFS`: Filesystem for persistent storage.

## Project Principles

1. **Clean UI/UX:** Adhere to the `UIPlan.md` for all visual changes. Maintain the Dark Mode aesthetic and high-contrast labels.
2. **Non-Blocking Logic:** Ensure the UI remains responsive. Avoid using `delay()` in the `loop()` or screen updates. Use timer-based polling for network requests.
3. **Surgical Changes:** When fixing bugs or adding features, modify only the relevant managers or screens.
4. **Documentation First:** Significant architectural changes should be reflected in `UpdatePlan.md` or a new design doc before implementation.

## How to Contribute

1. **Report Issues:** Use the GitHub issue tracker for bugs or feature requests.
2. **Submit PRs:**
   - Follow the existing code style (CamelCase for classes, camelCase for methods/variables).
   - Use the `feature/` branch naming convention.
   - Ensure the code compiles and follows the `platformio.ini` configuration.
   - Update tests or documentation if applicable.

## AI Contributor Guidelines

If you are an AI assistant (like Gemini CLI) working on this codebase:

- **Context Awareness:** Always read `GEMINI.md`, `UpdatePlan.md`, and `UIPlan.md` before making changes.
- **Safety:** Do not hardcode credentials. Use the `ConfigStore` for secrets.
- **Verification:** If possible, describe how to verify your changes in a real or simulated environment.

## Contact

For questions or suggestions, please open a discussion on GitHub.
