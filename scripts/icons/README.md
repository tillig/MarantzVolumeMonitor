# Icon Generation

Run from the repository root:

```bash
python3 scripts/icons/generate-icons.py
```

The script requires `rsvg-convert` on the development workstation. It renders SVG sources from `assets/icons/source/`, writes preview SVGs to `assets/icons/generated/`, and updates firmware bitmap masks in `src/ui/assets/`.

Run the generator twice before finishing icon changes; the second run should produce no diff.
