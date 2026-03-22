from __future__ import annotations

from pathlib import Path
import re

from PIL import Image


SRC_DIR = Path("ImageConvertHelpers/battleSpritesToUse")
OUT_DIR = Path("software/graphics/sprites/battleIcons/battleHp")

TRANSPARENT_PINK_RGB = (255, 0, 255)
TRANSPARENT_565 = 0xF81F


def is_magentaish(red: int, green: int, blue: int) -> bool:
    return red >= 200 and blue >= 200 and green <= 100


def rgb_to_565(red: int, green: int, blue: int) -> int:
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3)


def camel_to_screaming_snake(name: str) -> str:
    with_underscores = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    with_safe_chars = re.sub(r"[^A-Za-z0-9_]", "_", with_underscores)
    normalized = re.sub(r"_+", "_", with_safe_chars).strip("_")
    return normalized.upper()


def main() -> None:
    png_paths = sorted(SRC_DIR.glob("*.png"), key=lambda path: path.name.lower())
    if not png_paths:
        raise RuntimeError(f"No .png files found in {SRC_DIR}")

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    for png_path in png_paths:
        stem = png_path.stem
        macro_prefix = camel_to_screaming_snake(stem)
        variable_name = re.sub(r"[^A-Za-z0-9_]", "_", stem)
        if not variable_name or not variable_name[0].isalpha():
            variable_name = f"sprite_{variable_name}"

        image = Image.open(png_path).convert("RGBA")
        width, height = image.size
        pixels = image.load()

        rgb565_values: list[int] = []
        for y in range(height):
            for x in range(width):
                red, green, blue, alpha = pixels[x, y]
                if alpha == 0 or (red, green, blue) == TRANSPARENT_PINK_RGB or is_magentaish(red, green, blue):
                    rgb565_values.append(TRANSPARENT_565)
                else:
                    rgb565_values.append(rgb_to_565(red, green, blue))

        out_h = OUT_DIR / f"{stem}.h"
        out_c = OUT_DIR / f"{stem}.c"

        header_lines = [
            "#pragma once",
            "",
            f"#define {macro_prefix}_WIDTH  {width}",
            f"#define {macro_prefix}_HEIGHT {height}",
            "",
            f"extern const unsigned short {variable_name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT];",
            "",
        ]

        c_lines: list[str] = [
            f'#include "{out_h.name}"',
            "",
            f"const unsigned short {variable_name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT] = {{",
        ]
        for k in range(0, len(rgb565_values), 16):
            chunk = rgb565_values[k : k + 16]
            chunk_s = ", ".join(f"0x{value:04X}" for value in chunk)
            end = "," if k + 16 < len(rgb565_values) else ""
            c_lines.append(f"    {chunk_s}{end}")
        c_lines += ["};", ""]

        out_h.write_text("\n".join(header_lines), encoding="utf-8")
        out_c.write_text("\n".join(c_lines), encoding="utf-8")

        print(f"Wrote {out_h} and {out_c} ({width}x{height}).")


if __name__ == "__main__":
    main()

