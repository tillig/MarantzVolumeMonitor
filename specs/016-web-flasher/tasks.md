# Tasks: Web Flasher

**Input**: Design documents from `specs/016-web-flasher/`

**Prerequisites**: plan.md, spec.md

## Phase 1: Implementation

- [x] T001 Add `scripts/version.py` and register it in `platformio.ini`
- [x] T002 Add `release.yml` and `pages.yml` workflows
- [x] T003 Add `web-flasher/index.html` and `firmware-manifest.json`
- [x] T004 Document installing in `README.md` and releasing in `CONTRIBUTING.md`

## Phase 2: Verification

- [ ] V001 Enable GitHub Pages with the GitHub Actions source and allow `v*` tags to deploy to `github-pages`
- [ ] V002 Push a `v*` tag; confirm the release has `firmware.factory.bin` and the flasher page loads it
- [ ] V003 Flash a configured monitor without erasing; confirm settings survive and the diagnostics page reports the tag
