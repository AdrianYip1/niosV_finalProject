from __future__ import annotations

import argparse
import re
from pathlib import Path

from PIL import Image


TRANSPARENT_PINK_RGB = (255, 0, 255)
TRANSPARENT_565 = 0xF81F


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def to_c_ident(name: str) -> str:
    ident = re.sub(r"[^A-Za-z0-9_]", "_", name)
    ident = re.sub(r"_+", "_", ident).strip("_")
    if not ident:
        raise ValueError("identifier is empty")
    if ident[0].isdigit():
        ident = f"sprite_{ident}"
    return ident


def to_macro_base(name: str) -> str:
    snake = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    snake = re.sub(r"[^A-Za-z0-9_]", "_", snake)
    snake = re.sub(r"_+", "_", snake).strip("_")
    return snake.upper()


def convert_image(src_png: Path) -> tuple[list[str], int, int]:
    image = Image.open(src_png).convert("RGBA")
    width, height = image.size
    pixels = image.load()

    vals: list[str] = []
    for y in range(height):
        for x in range(width):
            r, g, b, a = pixels[x, y]
            if a == 0 or (r, g, b) == TRANSPARENT_PINK_RGB or is_magentaish(r, g, b):
                value = TRANSPARENT_565
            else:
                value = rgb_to_565(r, g, b)
            vals.append(f"0x{value:04X}")

    return vals, width, height


def write_header(dst_h: Path, symbol: str, macro_base: str, width: int, height: int) -> None:
    dst_h.write_text(
        "\n".join(
            [
                "#pragma once",
                "",
                f"#define {macro_base}_WIDTH  {width}",
                f"#define {macro_base}_HEIGHT {height}",
                "",
                f"extern const unsigned short {symbol}[{macro_base}_WIDTH * {macro_base}_HEIGHT];",
                "",
            ]
        ),
        encoding="utf-8",
    )


def write_source(dst_c: Path, header_name: str, symbol: str, macro_base: str, vals: list[str]) -> None:
    lines = [f'#include "{header_name}"', ""]
    lines.append(f"const unsigned short {symbol}[{macro_base}_WIDTH * {macro_base}_HEIGHT] = {{")
    for i in range(0, len(vals), 16):
        chunk = vals[i : i + 16]
        end = "," if i + 16 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)
    lines.append("};")
    lines.append("")
    dst_c.write_text("\n".join(lines), encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser(description="Convert a PNG sprite into project-style .c/.h files.")
    parser.add_argument("src_png", type=Path, help="Input PNG path")
    parser.add_argument("out_dir", type=Path, help="Output directory")
    parser.add_argument("symbol", help="C symbol name, e.g. mySprite")
    parser.add_argument(
        "--macro-base",
        dest="macro_base",
        help="Optional macro base, e.g. MY_SPRITE. Defaults to symbol-derived name.",
    )
    parser.add_argument(
        "--stem",
        dest="stem",
        help="Optional output filename stem. Defaults to the symbol name.",
    )
    args = parser.parse_args()

    if not args.src_png.exists():
        raise FileNotFoundError(f"missing input PNG: {args.src_png}")

    symbol = to_c_ident(args.symbol)
    macro_base = args.macro_base.strip().upper() if args.macro_base else to_macro_base(symbol)
    stem = args.stem.strip() if args.stem else symbol

    vals, width, height = convert_image(args.src_png)

    args.out_dir.mkdir(parents=True, exist_ok=True)
    dst_h = args.out_dir / f"{stem}.h"
    dst_c = args.out_dir / f"{stem}.c"

    write_header(dst_h, symbol, macro_base, width, height)
    write_source(dst_c, dst_h.name, symbol, macro_base, vals)

    print(f"Wrote {dst_c} and {dst_h} ({width}x{height})")


if __name__ == "__main__":
    main()
