# Marantz Volume Monitor - Agent Instructions

## Core Mandates

- **Constitution Compliance**: All development must adhere to the [Project Constitution](.specify/memory/constitution.md).
- **Technology Stack**: ESP32 (Elegoo DevKit V1), PlatformIO, TFT_eSPI, LittleFS.
- **Architectural Pattern**: Layered decoupling (UI, Network, Storage).
- **Act Like a Contributor**: Follow the rules for documentation updates and validation as laid out in `CONTRIBUTING.md`.
- **Documentation Maintenance**: Any changes to functionality or tooling must be reflected in `README.md` or `CONTRIBUTING.md`.

## Documentation Reference

- **Durable Reference Docs**: Hardware, UI, and architecture references are in `docs/`.
- **Feature Specifications**: Spec Kit feature artifacts are in `specs/`.
- **UI Design**: See `docs/ui-reference.md`.
- **Hardware/Wiring**: See `docs/hardware.md`.

## Markdown Instructions

Apply these rules when writing or reviewing `.md` files in this repository. Use CommonMark and GitHub Flavored Markdown behavior as the baseline.

### Markdown Structure

- Use heading levels hierarchically. Start at `#` only when the document title is written in the file; otherwise start at `##`.
- Use ATX headings with `#` and a following space.
- Do not skip heading levels.
- Do not use bold text as a visual substitute for headings.
- Avoid creating a sub-heading level that contains only one heading.
- Avoid "overview" sections; put that content directly under the title when possible.
- Use `-` for bullet lists and `1.` for ordered lists.
- Do not create single-item lists; convert them to normal prose.
- Capitalize the first word of every list item.
- Use fenced code blocks with triple backticks and always include a language identifier.
- Use Markdown tables for tabular data with a header row and matching delimiter row.
- Keep each paragraph on a single line. Do not soft-wrap prose.
- Avoid raw HTML in Markdown unless there is no good Markdown alternative.

### Links and Images

- Use descriptive link text. Do not use generic text like "here," "this," or "read more."
- Do not leave bare URLs in prose when a descriptive link would be clearer.
- Make sure identical link text does not point to different destinations in the same document.
- Use proper image syntax and provide meaningful alt text unless the image is purely decorative.
- Do not use filenames or placeholders like "image" or "screenshot" as alt text.
- For complex visuals, summarize the important information in surrounding text and consider a longer description.

### Plain Language and Accessibility

- Prefer short sentences, common words, and active voice.
- Break up dense paragraphs into smaller sections or lists when that improves scanning.
- When describing UI navigation, write the user actions in plain sequential language first. Visual cues may follow as supplemental context, but not as the only instruction.
- Format code identifiers, settings names, and UI labels as code, such as `Settings`.
- Use Title Case for headings.
- Capitalize "ID" in prose.
- Spell out numbers below 10 unless they are part of code, data, or another literal value.
- Do not replace "and" with `&` except in established names or abbreviations.
- Use emoji sparingly and never as the only carrier of meaning.
- Do not use emoji or special characters as list markers.

### Review Guidance

When reviewing Markdown, check for:

- Missing or weak alt text.
- Non-descriptive link text.
- Skipped heading levels or broken heading hierarchy.
- Dense or jargon-heavy language that should be simplified.
- Lists that should be prose, or prose that should be a list.
- Multiple consecutive emoji or emoji used in place of text.
- Invalid Markdown structures that would fail `markdownlint`.

When you flag an issue, explain the accessibility or readability impact and keep the recommendation specific and actionable.

<!-- SPECKIT START -->
For additional context about technologies to be used, project structure, shell commands, and other important information, read the current plan: [specs/009-receiver-off-hardware/plan.md](specs/009-receiver-off-hardware/plan.md)
<!-- SPECKIT END -->
