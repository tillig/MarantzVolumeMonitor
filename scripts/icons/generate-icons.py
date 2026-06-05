#!/usr/bin/env python3
"""Generate firmware bitmap masks and previews from icon SVG sources."""

from __future__ import annotations

import os
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SOURCE_DIR = ROOT / "assets" / "icons" / "source"
GENERATED_DIR = ROOT / "assets" / "icons" / "generated"
FIRMWARE_DIR = ROOT / "src" / "ui" / "assets"


@dataclass(frozen=True)
class IconSpec:
    asset_id: str
    symbol: str
    width: int
    height: int
    svg: str | None
    mask_mode: str = "dark"


def material_svg(path: str) -> str:
    return (
        '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" '
        'viewBox="0 0 24 24"><path fill="#000" d="'
        + path
        + '"/></svg>\n'
    )


def simple_svg(width: int, height: int, body: str, viewbox: str | None = None) -> str:
    vb = viewbox or f"0 0 {width} {height}"
    return (
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
        f'viewBox="{vb}"><g fill="none" stroke="#000" stroke-width="3" '
        f'stroke-linecap="round" stroke-linejoin="round">{body}</g></svg>\n'
    )


ICON_SPECS: list[IconSpec] = [
    IconSpec("audio-dolby", "AUDIO_DOLBY", 102, 32, None),
    IconSpec("audio-dts", "AUDIO_DTS", 102, 32, None, "light"),
    IconSpec(
        "audio-pcm",
        "AUDIO_PCM",
        102,
        32,
        simple_svg(
            102,
            32,
            '<path d="M8 22H20V10H32V22H44V8H56V22H68V14H80V22H94"/>'
            '<path d="M8 26H94"/>',
        ),
    ),
    IconSpec(
        "audio-other",
        "AUDIO_OTHER",
        102,
        32,
        simple_svg(
            102,
            32,
            '<path d="M8 18C16 8 24 28 32 18S48 8 56 18 72 28 80 18"/>'
            '<path d="M88 12C88 7 96 7 96 12C96 16 92 16 92 20"/>'
            '<path d="M92 26H92.1"/>',
        ),
    ),
    IconSpec(
        "settings",
        "SETTINGS",
        24,
        24,
        material_svg(
            "M19.43 12.98c.04-.32.07-.65.07-.98s-.02-.66-.07-.98l2.11-1.65"
            "c.19-.15.24-.42.12-.64l-2-3.46c-.12-.22-.37-.31-.6-.22l-2.49 1"
            "c-.52-.4-1.08-.73-1.69-.98L14.5 2.42C14.47 2.18 14.25 2 14 2h-4"
            "c-.25 0-.46.18-.5.42L9.12 5.07c-.61.25-1.17.59-1.69.98l-2.49-1"
            "c-.23-.08-.48 0-.6.22l-2 3.46c-.13.22-.07.49.12.64l2.11 1.65"
            "c-.04.32-.08.65-.08.98s.03.66.08.98l-2.11 1.65c-.19.15-.24.42-.12.64"
            "l2 3.46c.12.22.37.31.6.22l2.49-1c.52.4 1.08.73 1.69.98l.38 2.65"
            "c.04.24.25.42.5.42h4c.25 0 .47-.18.5-.42l.38-2.65c.61-.25 1.17-.59"
            "1.69-.98l2.49 1c.23.08.48 0 .6-.22l2-3.46c.12-.22.07-.49-.12-.64"
            "l-2.11-1.65zM12 15.5A3.5 3.5 0 1 1 12 8a3.5 3.5 0 0 1 0 7.5z"
        ),
    ),
    IconSpec("chevron-right", "CHEVRON_RIGHT", 24, 24, material_svg("M10 6 8.59 7.41 13.17 12l-4.58 4.59L10 18l6-6z")),
    IconSpec("keyboard-caps-lock", "KEYBOARD_CAPS_LOCK", 24, 24, material_svg("M12 8 6 14l1.41 1.41L12 10.83l4.59 4.58L18 14zm-7 10h14v2H5z")),
    IconSpec(
        "keyboard-visibility",
        "KEYBOARD_VISIBILITY",
        24,
        24,
        material_svg("M12 4.5C7 4.5 2.73 7.61 1 12c1.73 4.39 6 7.5 11 7.5s9.27-3.11 11-7.5c-1.73-4.39-6-7.5-11-7.5zm0 12.5a5 5 0 1 1 0-10 5 5 0 0 1 0 10zm0-8a3 3 0 1 0 0 6 3 3 0 0 0 0-6z"),
    ),
    IconSpec(
        "keyboard-visibility-off",
        "KEYBOARD_VISIBILITY_OFF",
        24,
        24,
        material_svg("M12 6.5c3.79 0 7.17 2.13 8.82 5.5-.7 1.43-1.79 2.61-3.07 3.49l1.42 1.42C21.01 15.64 22.46 13.94 23 12c-1.73-4.39-6-7.5-11-7.5-1.4 0-2.74.25-3.98.7l1.65 1.65c.74-.23 1.53-.35 2.33-.35zM2.56 3 1.29 4.27l2.56 2.56A11.86 11.86 0 0 0 1 12c1.73 4.39 6 7.5 11 7.5 1.55 0 3.03-.3 4.38-.84L19.73 22 21 20.73 2.56 3zm6.57 6.57 1.56 1.56A2.98 2.98 0 0 0 12 15a2.98 2.98 0 0 0 1.87-.65l1.56 1.56A4.95 4.95 0 0 1 12 17a5 5 0 0 1-5-5c0-1.27.47-2.43 1.24-3.31l.89.88zm2.74-.53 3.15 3.15.02-.16A3 3 0 0 0 12 9c-.04 0-.09 0-.13.04z"),
    ),
    IconSpec(
        "keyboard-backspace",
        "KEYBOARD_BACKSPACE",
        24,
        24,
        material_svg("M22 3H7c-.69 0-1.23.35-1.59.88L0 12l5.41 8.12c.36.53.9.88 1.59.88h15c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2zm-3 12.59L17.59 17 14 13.41 10.41 17 9 15.59 12.59 12 9 8.41 10.41 7 14 10.59 17.59 7 19 8.41 15.41 12z"),
    ),
    IconSpec("keyboard-ok", "KEYBOARD_OK", 24, 24, material_svg("M9 16.17 4.83 12l-1.42 1.41L9 19 21 7l-1.41-1.41z")),
    IconSpec("keyboard-cancel", "KEYBOARD_CANCEL", 24, 24, material_svg("M18.3 5.71 12 12l6.3 6.29-1.41 1.41L10.59 13.41 4.29 19.71 2.88 18.3 9.17 12 2.88 5.71 4.29 4.29l6.3 6.3 6.29-6.3z")),
    IconSpec("scan", "SCAN", 32, 32, material_svg("M9.5 3A6.5 6.5 0 0 1 16 9.5c0 1.61-.59 3.09-1.57 4.23l.27.27h.8l5 4.99L19.99 20l-4.99-5v-.8l-.27-.27A6.47 6.47 0 0 1 9.5 16 6.5 6.5 0 1 1 9.5 3m0 2A4.5 4.5 0 1 0 9.5 14 4.5 4.5 0 0 0 9.5 5z")),
    IconSpec("manual-entry", "MANUAL_ENTRY", 24, 24, material_svg("M3 17.25V21h3.75L17.81 9.94l-3.75-3.75L3 17.25zm17.71-10.04a1 1 0 0 0 0-1.41l-2.51-2.51a1 1 0 0 0-1.41 0l-1.96 1.96 3.75 3.75 2.13-1.79z")),
    IconSpec("retry", "RETRY", 24, 24, material_svg("M17.65 6.35A7.95 7.95 0 0 0 12 4a8 8 0 1 0 7.75 10h-2.1A6 6 0 1 1 12 6c1.66 0 3.14.69 4.22 1.78L13 11h8V3z")),
    IconSpec("success", "SUCCESS", 48, 48, material_svg("M12 2a10 10 0 1 0 0 20 10 10 0 0 0 0-20zm-2 15-5-5 1.41-1.41L10 14.17l7.59-7.59L19 8z")),
    IconSpec("warning", "WARNING", 48, 48, material_svg("M1 21h22L12 2 1 21zm12-3h-2v-2h2zm0-4h-2v-4h2z")),
    IconSpec("failure", "FAILURE", 48, 48, material_svg("M12 2a10 10 0 1 0 0 20 10 10 0 0 0 0-20zm5 13.59L15.59 17 12 13.41 8.41 17 7 15.59 10.59 12 7 8.41 8.41 7 12 10.59 15.59 7 17 8.41 13.41 12z")),
    IconSpec("wifi", "WIFI", 32, 32, material_svg("M1 9 3 11c4.97-4.97 13.03-4.97 18 0l2-2C16.93 2.93 7.08 2.93 1 9zm8 8 3 3 3-3c-1.65-1.66-4.34-1.66-6 0zm-4-4 2 2c2.76-2.76 7.24-2.76 10 0l2-2c-3.86-3.86-10.14-3.86-14 0z")),
    IconSpec(
        "touch-calibration",
        "TOUCH_CALIBRATION",
        24,
        24,
        simple_svg(
            24,
            24,
            '<circle cx="12" cy="12" r="5"/>'
            '<path d="M12 2V6"/>'
            '<path d="M12 18V22"/>'
            '<path d="M2 12H6"/>'
            '<path d="M18 12H22"/>',
        ),
    ),
    IconSpec("receiver", "RECEIVER", 32, 32, simple_svg(32, 32, '<rect x="5" y="8" width="22" height="14" rx="2"/><path d="M9 25H23"/><path d="M10 14H16"/><circle cx="22" cy="15" r="2"/>', "0 0 32 32")),
]


def ensure_directories() -> None:
    for path in (SOURCE_DIR, GENERATED_DIR, FIRMWARE_DIR):
        path.mkdir(parents=True, exist_ok=True)


def ensure_sources() -> None:
    for spec in ICON_SPECS:
        path = SOURCE_DIR / f"{spec.asset_id}.svg"
        if path.exists() or spec.svg is None:
            continue
        path.write_text(spec.svg, encoding="utf-8")


def unfilter_scanline(filter_type: int, scan: bytes, prev: bytes, bpp: int) -> bytes:
    out = bytearray(scan)
    for i, value in enumerate(out):
        left = out[i - bpp] if i >= bpp else 0
        up = prev[i] if prev else 0
        up_left = prev[i - bpp] if prev and i >= bpp else 0
        if filter_type == 1:
            out[i] = (value + left) & 0xFF
        elif filter_type == 2:
            out[i] = (value + up) & 0xFF
        elif filter_type == 3:
            out[i] = (value + ((left + up) // 2)) & 0xFF
        elif filter_type == 4:
            p = left + up - up_left
            pa = abs(p - left)
            pb = abs(p - up)
            pc = abs(p - up_left)
            pred = left if pa <= pb and pa <= pc else up if pb <= pc else up_left
            out[i] = (value + pred) & 0xFF
        elif filter_type != 0:
            raise ValueError(f"Unsupported PNG filter {filter_type}")
    return bytes(out)


def read_png_rgba(path: Path) -> tuple[int, int, list[tuple[int, int, int, int]]]:
    data = path.read_bytes()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError(f"{path} is not a PNG")

    pos = 8
    width = height = color_type = bit_depth = None
    idat = bytearray()
    while pos < len(data):
        length = struct.unpack(">I", data[pos : pos + 4])[0]
        chunk_type = data[pos + 4 : pos + 8]
        payload = data[pos + 8 : pos + 8 + length]
        pos += length + 12
        if chunk_type == b"IHDR":
            width, height, bit_depth, color_type, _, _, _ = struct.unpack(">IIBBBBB", payload)
        elif chunk_type == b"IDAT":
            idat.extend(payload)
        elif chunk_type == b"IEND":
            break

    if width is None or height is None or bit_depth != 8 or color_type not in (2, 6):
        raise ValueError(f"Unsupported PNG format from {path}")

    channels = 4 if color_type == 6 else 3
    raw = zlib.decompress(bytes(idat))
    stride = width * channels
    prev = b"\x00" * stride
    pixels: list[tuple[int, int, int, int]] = []
    offset = 0
    for _ in range(height):
        filter_type = raw[offset]
        offset += 1
        scan = unfilter_scanline(filter_type, raw[offset : offset + stride], prev, channels)
        offset += stride
        prev = scan
        for x in range(0, len(scan), channels):
            r, g, b = scan[x], scan[x + 1], scan[x + 2]
            a = scan[x + 3] if channels == 4 else 255
            pixels.append((r, g, b, a))
    return width, height, pixels


def render_svg_to_mask(source: Path, width: int, height: int, mask_mode: str) -> list[int]:
    converter = shutil.which("rsvg-convert")
    if converter is None:
        raise RuntimeError("rsvg-convert is required to generate icon bitmaps")

    with tempfile.TemporaryDirectory() as tmp:
        png = Path(tmp) / "icon.png"
        subprocess.run(
            [converter, "-a", "-f", "png", "-w", str(width), "-h", str(height), "-o", str(png), str(source)],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        png_width, png_height, pixels = read_png_rgba(png)
    target_pixels = [(0, 0, 0, 0)] * (width * height)
    offset_x = (width - png_width) // 2
    offset_y = (height - png_height) // 2
    for y in range(png_height):
        target_y = y + offset_y
        if target_y < 0 or target_y >= height:
            continue
        for x in range(png_width):
            target_x = x + offset_x
            if target_x < 0 or target_x >= width:
                continue
            target_pixels[target_y * width + target_x] = pixels[y * png_width + x]
    pixels = target_pixels
    if mask_mode == "light":
        return [1 if a > 64 and ((r + g + b) // 3) > 245 else 0 for r, g, b, a in pixels]
    return [1 if a > 64 and ((r + g + b) // 3) < 245 else 0 for r, g, b, a in pixels]


def pack_mask(mask: list[int], width: int, height: int) -> list[int]:
    packed: list[int] = []
    for y in range(height):
        for x0 in range(0, width, 8):
            byte = 0
            for bit in range(8):
                x = x0 + bit
                if x < width and mask[y * width + x]:
                    byte |= 1 << (7 - bit)
            packed.append(byte)
    return packed


def write_preview(spec: IconSpec, mask: list[int]) -> None:
    rects = []
    for y in range(spec.height):
        for x in range(spec.width):
            if mask[y * spec.width + x]:
                rects.append(f'<rect x="{x}" y="{y}" width="1" height="1"/>')
    svg = (
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{spec.width}" height="{spec.height}" '
        f'viewBox="0 0 {spec.width} {spec.height}"><rect width="100%" height="100%" '
        f'fill="#101010"/><g fill="#35D06E">{"".join(rects)}</g></svg>\n'
    )
    (GENERATED_DIR / f"{spec.asset_id}.preview.svg").write_text(svg, encoding="utf-8")


def write_firmware(masks: dict[str, list[int]]) -> None:
    header_lines = [
        "#ifndef ICON_BITMAPS_H",
        "#define ICON_BITMAPS_H",
        "",
        "#include <Arduino.h>",
        "",
        "namespace Icons {",
        "",
        "struct IconBitmap {",
        "    uint16_t width;",
        "    uint16_t height;",
        "    const uint8_t* data;",
        "    uint16_t dataSize;",
        "};",
        "",
    ]
    for spec in ICON_SPECS:
        header_lines.append(f"extern const IconBitmap {spec.symbol};")
    header_lines.extend(["", "} // namespace Icons", "", "#endif", ""])
    (FIRMWARE_DIR / "IconBitmaps.h").write_text("\n".join(header_lines), encoding="utf-8")

    cpp_lines = [
        '#include "IconBitmaps.h"',
        "",
        "namespace Icons {",
        "",
    ]
    for spec in ICON_SPECS:
        data = masks[spec.asset_id]
        cpp_lines.append(f"static const uint8_t {spec.symbol}_DATA[] PROGMEM = {{")
        for i in range(0, len(data), 12):
            chunk = ", ".join(f"0x{b:02X}" for b in data[i : i + 12])
            cpp_lines.append(f"    {chunk},")
        cpp_lines.append("};")
        cpp_lines.append(
            f"const IconBitmap {spec.symbol} = "
            f"{{{spec.width}, {spec.height}, {spec.symbol}_DATA, sizeof({spec.symbol}_DATA)}};"
        )
        cpp_lines.append("")
    cpp_lines.extend(["} // namespace Icons", ""])
    (FIRMWARE_DIR / "IconBitmaps.cpp").write_text("\n".join(cpp_lines), encoding="utf-8")


def main() -> int:
    os.chdir(ROOT)
    ensure_directories()
    ensure_sources()

    masks: dict[str, list[int]] = {}
    for spec in ICON_SPECS:
        source = SOURCE_DIR / f"{spec.asset_id}.svg"
        if not source.exists():
            raise FileNotFoundError(source)
        mask = render_svg_to_mask(source, spec.width, spec.height, spec.mask_mode)
        masks[spec.asset_id] = pack_mask(mask, spec.width, spec.height)
        write_preview(spec, mask)

    write_firmware(masks)
    print(f"Generated {len(ICON_SPECS)} icons")
    return 0


if __name__ == "__main__":
    sys.exit(main())
