# Contributing to Marantz Volume Monitor v2

Welcome! We appreciate your interest in contributing to the Marantz Volume Monitor modernization project. Whether you are a human developer or an AI assistant, this guide will help you understand the project's standards and workflows.

## Development Environment

- **Framework:** Arduino
- **Platform:** Espressif 32 (Elegoo ESP32 DevKit V1)
- **Tooling:** PlatformIO, [Spec Kit](https://github.com/tillig/speckit)
- **Key Libraries:**
  - `TFT_eSPI`: High-performance graphics library.
  - `XPT2046_Touchscreen`: Touch controller driver.
  - `ArduinoJson`: Configuration serialization.
  - `LittleFS`: Filesystem for persistent storage.

## Project Principles

1. **Clean UI/UX:** Adhere to `docs/ui-reference.md` for visual changes. Maintain the dark-mode aesthetic and high-contrast labels.
2. **Non-Blocking Logic:** Ensure the UI remains responsive. Avoid using `delay()` in the `loop()` or screen updates. Use timer-based polling for network requests.
3. **Surgical Changes:** When fixing bugs or adding features, modify only the relevant managers or screens.
4. **Documentation First:** Feature work should be driven through Spec Kit artifacts in `specs/`. Durable behavior, hardware, tooling, or architecture changes must update the appropriate top-level documentation.

## Quality Standards

- **Validation:** All changes must pass `pre-commit` validation. Run `pre-commit run --all-files` before committing.
- **Linting:** Markdown must pass `markdownlint` as configured in `.markdownlint.json`.
- **Firmware Build:** Run `platformio run` before finishing firmware changes.

## Build And Validation

Use these commands from the repository root:

```bash
platformio run
pre-commit run --all-files
```

If you change icon source artwork in `assets/icons/source/`, regenerate firmware icon masks before
building:

```bash
python3 scripts/icons/generate-icons.py
```

Run the generator twice when reviewing icon changes; the second run should produce no additional
diff.

To flash a connected ESP32, use PlatformIO's upload action or:

```bash
platformio run --target upload
```

## How to Contribute

1. **Report Issues:** Use the GitHub issue tracker for bugs or feature requests.
2. **Submit PRs:**
   - Follow the existing code style (CamelCase for classes, camelCase for methods/variables).
   - Use the Spec Kit branch convention in `.specify/branch-convention.yml`. In this repository,
     `/speckit.specify` should create branches in the `feature/NNN-short-name` form.
   - Ensure the code compiles and follows the `platformio.ini` configuration.
   - Update tests or documentation if applicable.

## Documentation Responsibilities

- Update `README.md` for user-facing setup, usage, hardware summary, and project consumption changes.
- Update this file for build, validation, contribution, or workflow changes.
- Update `docs/hardware.md` for wiring, power, touch calibration, or physical assembly changes.
- Update `docs/ui-reference.md` for durable visual language, layout, or touch-target guidance.
- Update `docs/architecture.md` for durable layer, ownership, or source-layout changes.
- Update `assets/icons/inventory.md` when adding, replacing, or reusing icon assets.
- Keep feature-specific requirements, contracts, quickstarts, and implementation tasks in `specs/NNN-*`.

## AI Contributor Guidelines

If you are an AI assistant (like Gemini CLI) working on this codebase:

- **Context Awareness:** Always read `AGENTS.md`, the active Spec Kit feature under `specs/`, and relevant durable references in `docs/` before making changes.
- **Spec Kit:** Use Spec Kit commands for planning and task management.
- **Branching:** Prefer `/speckit.specify` so the git hook can generate a compliant feature branch
  from `.specify/branch-convention.yml`.
- **Safety:** Do not hardcode credentials. Use the `ConfigStore` for secrets.
- **Verification:** Always run `pre-commit run --all-files` before finishing a task. Describe how to verify your changes in a real or simulated environment.

## Contact

For questions or suggestions, please open a discussion on GitHub.
