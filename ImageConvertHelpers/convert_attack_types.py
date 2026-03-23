from __future__ import annotations
from pathlib import Path
import argparse
from PIL import Image

TYPE_NAMES = [
    "bug", "dark", "dragon", "electric", "fairy", "fighting",
    "fire", "flying", "ghost", "grass", "ground", "ice",
    "normal", "poison", "psy", "rock", "steel", "water",
]

DEFAULT_SRC_DIR = Path("124x42")
OUT_DIR  = Path("../software/graphics/sprites/battleIcons/attackTypes")
OUT_H    = OUT_DIR / "attackTypeSprites.h"
OUT_C    = OUT_DIR / "attackTypeSprites.c"

TRANSPARENT_PINK_RGB = (255, 0, 255)
TRANSPARENT_565 = 0xF81F


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def load_sprite(src_png: Path) -> tuple[list[str], int, int]:
    """Load a PNG and return (vals, width, height)."""
    if not src_png.exists():
        raise RuntimeError(f"Missing input: {src_png}")

    im = Image.open(src_png).convert("RGBA")
    w, h = im.size
    px = im.load()

    vals: list[str] = []
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0 or (r, g, b) == TRANSPARENT_PINK_RGB or is_magentaish(r, g, b):
                v = TRANSPARENT_565
            else:
                v = rgb_to_565(r, g, b)
            vals.append(f"0x{v:04X}")

    return vals, w, h


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Convert attack-type PNGs into a single combined C/H sprite file."
    )
    parser.add_argument(
        "src_dir",
        nargs="?",
        default=str(DEFAULT_SRC_DIR),
        help=f"Folder containing the type PNGs (default: {DEFAULT_SRC_DIR})",
    )
    args = parser.parse_args()

    src_dir = Path(args.src_dir)
    if not src_dir.is_dir():
        raise RuntimeError(f"Source directory not found: {src_dir}")

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    # ── Build .h ─────────────────────────────────────────────────────────────
    h_lines: list[str] = [
        "#pragma once",
        "",
        '#include <stdint.h>',
        "",
    ]

    sprite_info: list[tuple[str, int, int]] = []  # (name, w, h)

    for name in TYPE_NAMES:
        src_png = src_dir / f"{name}.png"
        _, w, h = load_sprite(src_png)
        sprite_info.append((name, w, h))

        macro = name.upper()
        var   = f"{name}TypeSprite"
        h_lines += [
            f"#define {macro}_TYPE_WIDTH  {w}",
            f"#define {macro}_TYPE_HEIGHT {h}",
            f"extern const unsigned short {var}[{macro}_TYPE_WIDTH * {macro}_TYPE_HEIGHT];",
            "",
        ]

    OUT_H.write_text("\n".join(h_lines), encoding="utf-8")

    # ── Build .c ─────────────────────────────────────────────────────────────
    c_lines: list[str] = [
        f'#include "attackTypeSprites.h"',
        "",
    ]

    for name in TYPE_NAMES:
        src_png = src_dir / f"{name}.png"
        vals, w, h = load_sprite(src_png)

        macro = name.upper()
        var   = f"{name}TypeSprite"

        c_lines.append(
            f"const unsigned short {var}[{macro}_TYPE_WIDTH * {macro}_TYPE_HEIGHT] = {{"
        )
        for k in range(0, len(vals), 16):
            chunk = vals[k : k + 16]
            end = "," if k + 16 < len(vals) else ""
            c_lines.append("    " + ", ".join(chunk) + end)
        c_lines.append("};")
        c_lines.append("")
        print(f"  converted {name}.png ({w}x{h})")

    OUT_C.write_text("\n".join(c_lines), encoding="utf-8")
    print(f"\nWrote {OUT_H}")
    print(f"Wrote {OUT_C}")


if __name__ == "__main__":
    main()