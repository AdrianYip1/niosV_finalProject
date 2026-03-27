from __future__ import annotations

from pathlib import Path

from PIL import Image


SRC_PNG = Path("ImageConvertHelpers/cancelSprite/cancelSprite.png")

OUT_DIR = Path("software/graphics/sprites/menu")
OUT_C = OUT_DIR / "cancelSprite.c"
OUT_H = OUT_DIR / "cancelSprite.h"

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def main() -> None:
    if not SRC_PNG.exists():
        raise RuntimeError(f"Missing input: {SRC_PNG}")

    im = Image.open(SRC_PNG).convert("RGBA")
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

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    OUT_H.write_text(
        "\n".join(
            [
                "#pragma once",
                "",
                f"#define CANCEL_SPRITE_WIDTH  {w}",
                f"#define CANCEL_SPRITE_HEIGHT {h}",
                "",
                "extern const unsigned short cancelSprite[CANCEL_SPRITE_WIDTH * CANCEL_SPRITE_HEIGHT];",
                "",
            ]
        ),
        encoding="utf-8",
    )

    lines: list[str] = [f'#include "{OUT_H.name}"', ""]
    lines.append("const unsigned short cancelSprite[CANCEL_SPRITE_WIDTH * CANCEL_SPRITE_HEIGHT] = {")
    for k in range(0, len(vals), 16):
        chunk = vals[k : k + 16]
        end = "," if k + 16 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)
    lines.append("};")
    lines.append("")

    OUT_C.write_text("\n".join(lines), encoding="utf-8")
    print(f"Wrote {OUT_C} and {OUT_H} ({w}x{h})")


if __name__ == "__main__":
    main()

