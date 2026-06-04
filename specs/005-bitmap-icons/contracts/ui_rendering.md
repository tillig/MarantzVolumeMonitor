# Contract: UI Icon Rendering

## Home Screen

### Settings Control

- Replace the manually drawn top-right gear with the `settings` bitmap icon.
- Render at `24x24`.
- Use the existing secondary text treatment unless a highlighted/touched state is added later.
- Preserve the existing touch target.

### Audio Family Row

- Replace textual Dolby, DTS, PCM, and Other pills with centered icon assets.
- Each family keeps the current `102x32` bottom-row canvas.
- Active state uses lit green foreground.
- Inactive state uses dark gray foreground.
- Dolby active when receiver mode is classified as Dolby.
- DTS active when receiver mode is classified as DTS.
- PCM active when receiver mode is classified as PCM.
- Other active when receiver mode is valid but not Dolby, DTS, or PCM.
- All families inactive when receiver status is unavailable.

## Settings Screen

- Replace the text arrow in each navigation row with the `chevron-right` bitmap icon.
- The row label text remains unchanged.
- The chevron matches the row text color.
- The row remains directly tappable across the existing touch region.

## Keyboard Screen

- Replace manually drawn caps lock, password-visibility eye, OK, and Cancel glyphs with bitmap icons.
- Replace the text backspace arrow with a bitmap icon.
- Use distinct visibility and visibility-off icons so the keyboard indicates whether input is masked
  or visible.
- Caps lock inactive uses the normal function-key foreground color.
- Caps lock active uses the selected-state foreground treatment and remains visible against the active
  key background.
- OK and Cancel keep their existing semantic button backgrounds unless changed by a later Material
  screen style feature.

## Setup And Status Screens

- Searching/discovery, manual entry, retry, success, warning, failure, Wi-Fi, and receiver states
  should use inventory-backed icon assets where those states appear.
- Icons supplement concise labels and status messages; they do not remove text needed to complete
  setup.

## Rendering Invariants

- Screens call UI-layer icon rendering helpers; screens do not duplicate bitmap blit logic.
- Icons never perform network I/O or storage reads while drawing.
- Missing icon data is treated as a development error and should not silently fall back to text.
