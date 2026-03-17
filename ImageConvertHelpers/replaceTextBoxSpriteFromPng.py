from __future__ import annotations

from pathlib import Path

from PIL import Image


SRC_PNG = Path("ImageConvertHelpers/newTextBox.png")
DST_C = Path("software/graphics/textbox/textBoxSprite.c")

TEXTBOX_W = 320
TEXTBOX_H = 80

TRANSPARENT_PINK_RGB = (255, 0, 255)  # FF00FF
TRANSPARENT_565 = 0xF81F


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def main() -> None:
    im = Image.open(SRC_PNG).convert("RGBA")
    assert im.size == (TEXTBOX_W, TEXTBOX_H), f"expected {(TEXTBOX_W, TEXTBOX_H)}, got {im.size}"

    px = im.load()
    vals: list[str] = []

    for y in range(TEXTBOX_H):
        for x in range(TEXTBOX_W):
            r, g, b, _a = px[x, y]
            if (r, g, b) == TRANSPARENT_PINK_RGB:
                v = TRANSPARENT_565
            else:
                v = rgb_to_565(r, g, b)
            vals.append(f"0x{v:04X}")

    lines: list[str] = []
    lines.append("// Auto-generated from ImageConvertHelpers/newTextBox.png")
    lines.append('#include "textBoxSprite.h"')
    lines.append("")
    lines.append("const unsigned short textBoxSprite[TEXT_BOX_WIDTH * TEXT_BOX_HEIGHT] = {")

    for i in range(0, len(vals), 16):
        chunk = vals[i : i + 16]
        end = "," if i + 16 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)

    lines.append("};")
    lines.append("")

    DST_C.write_text("\n".join(lines), encoding="utf-8")
    print(f"Wrote {DST_C} ({TEXTBOX_W}x{TEXTBOX_H}) pixels={len(vals)}")


if __name__ == "__main__":
    main()

