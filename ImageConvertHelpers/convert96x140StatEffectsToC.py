from __future__ import annotations

from pathlib import Path

from PIL import Image


SRC_DIR = Path("ImageConvertHelpers/battleItems126x57")
OUT_DIR = Path("software/graphics/sprites/battleItemsUI")

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F

EXPECTED_W = 126
EXPECTED_H = 57


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def load_rgba(path: Path) -> Image.Image:
    im = Image.open(path)
    try:
        im.seek(0)
    except EOFError:
        pass
    rgba = im.convert("RGBA")
    w, h = rgba.size
    if (w, h) != (EXPECTED_W, EXPECTED_H):
        raise ValueError(f"{path} must be {EXPECTED_W}x{EXPECTED_H}, got {w}x{h}")
    return rgba


def image_to_565_vals(img: Image.Image, *, transparent_border: bool) -> list[int]:
    w, h = img.size
    px = img.load()

    bg_palette: set[tuple[int, int, int]] = set()
    if transparent_border:
        # Some PNGs come with a solid/gradient background instead of alpha.
        # Optionally treat common border colours as transparent "background".
        border: list[tuple[int, int, int, int]] = []
        for x in range(w):
            border.append(px[x, 0])
            border.append(px[x, h - 1])
        for y in range(h):
            border.append(px[0, y])
            border.append(px[w - 1, y])

        bg_palette = {
            (r, g, b)
            for (r, g, b, a), _ in Counter(border).most_common(8)
            if a != 0
        }

    vals: list[int] = []
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0 or (r, g, b) == TRANSPARENT_PINK_RGB or (transparent_border and (r, g, b) in bg_palette):
                vals.append(TRANSPARENT_565)
            else:
                vals.append(rgb_to_565(r, g, b))
    return vals


def write_sprite(name: str, macro_prefix: str, png_path: Path, *, transparent_border: bool) -> None:
    img = load_rgba(png_path)
    vals = image_to_565_vals(img, transparent_border=transparent_border)

    h_path = OUT_DIR / f"{name}.h"
    c_path = OUT_DIR / f"{name}.c"

    h_text = "\n".join(
        [
            "#pragma once",
            "",
            f"#define {macro_prefix}_WIDTH  {EXPECTED_W}",
            f"#define {macro_prefix}_HEIGHT {EXPECTED_H}",
            "",
            f"extern const unsigned short {name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT];",
            "",
        ]
    )

    c_lines: list[str] = [f'#include "{h_path.name}"', ""]
    c_lines.append(f"const unsigned short {name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT] = {{")
    for k in range(0, len(vals), 16):
        chunk = vals[k : k + 16]
        chunk_s = ", ".join(f"0x{v:04X}" for v in chunk)
        end = "," if k + 16 < len(vals) else ""
        c_lines.append(f"    {chunk_s}{end}")
    c_lines.append("};")
    c_lines.append("")

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    h_path.write_text(h_text, encoding="utf-8")
    c_path.write_text("\n".join(c_lines), encoding="utf-8")

    print(f"Wrote {c_path} and {h_path}")


def main() -> None:
    if not SRC_DIR.exists():
        raise RuntimeError(f"Missing source dir: {SRC_DIR}")

    # Only treat #FF00FF as transparent for these assets.
    write_sprite("hp", "BATTLE_ITEM_HP", SRC_DIR / "hp.png", transparent_border=False)
    write_sprite("pokeballs", "POKEBALLS", SRC_DIR / "pokeballs.png", transparent_border=False)


if __name__ == "__main__":
    main()
