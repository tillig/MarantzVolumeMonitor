# Implementation Plan: Web Flasher

**Branch**: `feature/016-web-flasher` | **Date**: 2026-10-02 | **Spec**: [spec.md](spec.md)

## Summary

Port the `somfy-matter-remote` release pipeline: `scripts/version.py` stamps the git tag into `FIRMWARE_VERSION`, `release.yml` attaches the pioarduino `firmware.factory.bin` to a release on `v*` tags, and `pages.yml` deploys `web-flasher/` with that image to GitHub Pages using ESP Web Tools.

## Constitution Check

- **IV. Deterministic Dependency Management**: Release builds use the same pinned `platformio.ini` as CI.
- **V. Intuitive On-Device Setup**: Flashing changes nothing about setup; configuration stays on the touchscreen.

## Design Notes

`somfy-matter-remote` needs a second app-only image because its factory image overwrites NVS. Here settings live in the LittleFS partition at `0x210000`, past the end of the factory image, so one manifest covers install and update. ESP Web Tools' erase prompt is the only way to clear settings.
