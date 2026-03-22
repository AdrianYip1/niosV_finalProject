from __future__ import annotations

from collections import deque
from pathlib import Path

import numpy as np
from PIL import Image


SPRITES_DIR = Path("ImageConvertHelpers/spritesToUse")
OUT_DIR = Path("software/graphics/sprites/statEffects")

TRANSPARENT_565 = 0xF81F
MAX_WIDTH = 96  # limit width to match battle sprite max width
BG_FLOOD_TOL = 50  # neighbour-to-neighbour RGB L1 tolerance for background flood fill


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def flood_background_mask(rgb: np.ndarray, tol: int) -> np.ndarray:
    # rgb: HxWx3 uint8
    image_height, image_width, _ = rgb.shape
    rgb_int16 = rgb.astype(np.int16, copy=False)
    visited = np.zeros((image_height, image_width), dtype=np.uint8)
    queue: deque[tuple[int, int]] = deque()

    for x in range(image_width):
        queue.append((0, x))
        queue.append((image_height - 1, x))
    for y in range(image_height):
        queue.append((y, 0))
        queue.append((y, image_width - 1))

    while queue:
        y, x = queue.popleft()
        if visited[y, x]:
            continue
        visited[y, x] = 1
        current_rgb = rgb_int16[y, x]

        for ny, nx in ((y - 1, x), (y + 1, x), (y, x - 1), (y, x + 1)):
            if 0 <= ny < image_height and 0 <= nx < image_width and not visited[ny, nx]:
                neighbour_delta = np.abs(rgb_int16[ny, nx] - current_rgb).sum()
                if neighbour_delta <= tol:
                    queue.append((ny, nx))

    return visited.astype(bool)


def write_sheet(stem: str) -> None:
    source_path = SPRITES_DIR / f"{stem}.png"
    image_rgba = Image.open(source_path).convert("RGBA")
    image_rgb = np.array(image_rgba)[:, :, :3]

    background_mask = flood_background_mask(image_rgb, BG_FLOOD_TOL)

    image_height, image_width = background_mask.shape
    output_width = min(image_width, MAX_WIDTH)
    x_offset = max(0, (image_width - output_width) // 2)

    # Crop to 96-wide (center crop) but keep full height so you can "scroll" the window.
    background_mask = background_mask[:, x_offset : x_offset + output_width]
    image_rgb = image_rgb[:, x_offset : x_offset + output_width, :]

    rgb565_values: list[int] = []
    for y in range(image_height):
        for x in range(output_width):
            if background_mask[y, x]:
                rgb565_values.append(TRANSPARENT_565)
            else:
                red, green, blue = (int(channel) for channel in image_rgb[y, x])
                rgb565_values.append(rgb_to_565(red, green, blue))

    macro = stem.upper()
    var = stem  # `statUp`, `statDown`

    out_h = OUT_DIR / f"{stem}.h"
    out_c = OUT_DIR / f"{stem}.c"

    h_lines = [
        "#pragma once",
        "",
        f"#define {macro}_WIDTH  {output_width}",
        f"#define {macro}_HEIGHT {image_height}",
        "",
        f"extern const unsigned short {var}[{macro}_WIDTH * {macro}_HEIGHT];",
        "",
    ]

    c_lines: list[str] = [
        f'#include "{out_h.name}"',
        "",
        f"const unsigned short {var}[{macro}_WIDTH * {macro}_HEIGHT] = {{",
    ]

    for k in range(0, len(rgb565_values), 16):
        chunk = rgb565_values[k : k + 16]
        chunk_s = ", ".join(f"0x{value:04X}" for value in chunk)
        end = "," if k + 16 < len(rgb565_values) else ""
        c_lines.append(f"    {chunk_s}{end}")
    c_lines += ["};", ""]

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    out_h.write_text("\n".join(h_lines), encoding="utf-8")
    out_c.write_text("\n".join(c_lines), encoding="utf-8")

    print(f"Wrote {out_h} and {out_c} (src {image_width}x{image_height} -> {output_width}x{image_height}).")


def main() -> None:
    write_sheet("statUp")
    write_sheet("statDown")


if __name__ == "__main__":
    main()
