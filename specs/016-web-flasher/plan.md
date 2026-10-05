# Implementation Plan: Web Flasher

**Branch**: `feature/016-web-flasher` | **Date**: 2026-10-02 | **Spec**: [spec.md](spec.md)

## Summary

A version tag builds a release whose factory image the web flasher serves through ESP Web Tools on GitHub Pages, and the firmware reports the tag it was built from.

## Constitution Check

- **IV. Deterministic Dependency Management**: Release builds use the same pinned `platformio.ini` as CI.
- **V. Intuitive On-Device Setup**: Flashing changes nothing about setup; configuration stays on the touchscreen.

## Design Notes

Settings live in the LittleFS partition, past the end of the factory image, so one image and one manifest cover both install and update. ESP Web Tools' erase prompt is the only way to clear settings.
