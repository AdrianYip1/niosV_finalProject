from __future__ import annotations
from pathlib import Path
import argparse
import re
from PIL import Image

DEFAULT_SRC_DIR = Path("ImageConvertHelpers/menu")
OUT_DIR = Path("software/graphics/sprites/menuSprites")

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F


EXPECTED_SIZES: dict[str, tuple[int, int]] = {
    "partyMenu":        (256, 192),
    "pokemonSelected":  (128,  49),
    "pokemonUnselcred": (126,  45),
}

DEFAULT_PNGS = [f"{stem}.png" for stem in EXPECTED_SIZES]


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def stem_to_upper_snake(stem: str) -> str:
    s = re.sub(r'([A-Z])', r'_\1', stem).upper().lstrip('_')
    return s


def convert_png(src_png: Path, out_dir: Path, output_stem: str | None = None) -> None:
    if not src_png.exists():
        print(f"  SKIP: {src_png} not found")
        return

    im = Image.open(src_png).convert("RGBA")
    w, h = im.size

    stem = output_stem or src_png.stem 

    expected = EXPECTED_SIZES.get(stem)
    if expected and (w, h) != expected:
        if (w, h) == (expected[0] * 2, expected[1] * 2):
            im = im.resize(expected, resample=Image.NEAREST)
            w, h = im.size
            print(f"  2x detected — downscaled to {w}x{h}")
        else:
            raise RuntimeError(
                f"{src_png.name}: expected {expected[0]}x{expected[1]} "
                f"(or 2x), got {w}x{h}"
            )

    upper    = stem_to_upper_snake(stem)             # e.g. "PARTY_MENU"
    var_name = stem[0].lower() + stem[1:] + "Sprite" # e.g. "partyMenuSprite"

    out_c = out_dir / f"{stem}Sprite.c"
    out_h = out_dir / f"{stem}Sprite.h"

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

    out_dir.mkdir(parents=True, exist_ok=True)

    # ── Header ───────────────────────────────────────────────────────────────
    out_h.write_text(
        "\n".join([
            "#pragma once",
            "",
            f"#define {upper}_WIDTH  {w}",
            f"#define {upper}_HEIGHT {h}",
            "",
            f"extern const unsigned short {var_name}[{upper}_WIDTH * {upper}_HEIGHT];",
            "",
        ]),
        encoding="utf-8",
    )

    # ── Source ───────────────────────────────────────────────────────────────
    lines: list[str] = [f'#include "{out_h.name}"', ""]
    lines.append(
        f"const unsigned short {var_name}[{upper}_WIDTH * {upper}_HEIGHT] = {{"
    )
    for k in range(0, len(vals), 16):
        chunk = vals[k: k + 16]
        end = "," if k + 16 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)
    lines.append("};")
    lines.append("")
    out_c.write_text("\n".join(lines), encoding="utf-8")

    print(f"  OK  {out_c.name} + {out_h.name}  ({w}x{h}, {len(vals):,} pixels)")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Convert menu PNGs to C RGB565 sprite arrays."
    )
    parser.add_argument(
        "srcs",
        nargs="*",
        default=[str(DEFAULT_SRC_DIR / p) for p in DEFAULT_PNGS],
        help="Input PNG paths. Defaults to all 3 menu PNGs in ImageConvertHelpers/menu/.",
    )
    parser.add_argument(
        "--out-dir",
        default=str(OUT_DIR),
        help=f"Output directory (default: {OUT_DIR})",
    )
    args = parser.parse_args()
    out_dir = Path(args.out_dir)
    print(f"Output → {out_dir}\n")

    for src in args.srcs:
        src_path = Path(src)
        # Strip trailing _1_, _2_, etc. so "pokemonSelected_1_.png" → "pokemonSelected"
        raw_stem   = src_path.stem
        canonical  = re.sub(r'[_\s]+\d+[_\s]*$', '', raw_stem)
        print(f"Converting: {src_path.name}  (→ {canonical}Sprite.*)")
        convert_png(src_path, out_dir, output_stem=canonical)

    print("\nDone.")


if __name__ == "__main__":
    main()