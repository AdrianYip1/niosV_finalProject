from __future__ import annotations

from pathlib import Path

from PIL import Image


SRC_PNG = Path("ImageConvertHelpers/cynthia192x192/cynthiaFront.png")

OUT_DIR = Path("software/graphics/sprites/cynthia192x192")
OUT_C = OUT_DIR / "cynthiaFrontSprite.c"
OUT_H = OUT_DIR / "cynthiaFrontSprite.h"

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F

EXPECTED_W = 192
EXPECTED_H = 192


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def main() -> None:
    if not SRC_PNG.exists():
        raise RuntimeError(f"Missing input: {SRC_PNG}")

    im = Image.open(SRC_PNG).convert("RGBA")
    w, h = im.size
    if (w, h) == (EXPECTED_W * 2, EXPECTED_H * 2):
        im = im.resize((EXPECTED_W, EXPECTED_H), resample=Image.NEAREST)
        w, h = im.size
    if (w, h) != (EXPECTED_W, EXPECTED_H):
        raise RuntimeError(f"{SRC_PNG} must be {EXPECTED_W}x{EXPECTED_H} (or 2x), got {w}x{h}")

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
                f"#define CYNTHIA_FRONT_WIDTH  {w}",
                f"#define CYNTHIA_FRONT_HEIGHT {h}",
                "",
                "extern const unsigned short cynthiaFrontSprite[CYNTHIA_FRONT_WIDTH * CYNTHIA_FRONT_HEIGHT];",
                "",
            ]
        ),
        encoding="utf-8",
    )

    c_lines: list[str] = [f'#include "{OUT_H.name}"', ""]
    c_lines.append(
        "const unsigned short cynthiaFrontSprite[CYNTHIA_FRONT_WIDTH * CYNTHIA_FRONT_HEIGHT] = {"
    )
    for k in range(0, len(vals), 16):
        chunk = vals[k : k + 16]
        end = "," if k + 16 < len(vals) else ""
        c_lines.append("    " + ", ".join(chunk) + end)
    c_lines.append("};")
    c_lines.append("")

    OUT_C.write_text("\n".join(c_lines), encoding="utf-8")
    print(f"Wrote {OUT_C} and {OUT_H} ({w}x{h})")


if __name__ == "__main__":
    main()

