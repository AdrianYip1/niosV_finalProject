from __future__ import annotations

from pathlib import Path

from PIL import Image

SRC_DIR = Path("ImageConvertHelpers/healingItems")
OUT_DIR = Path("software/graphics/sprites/battleItemsUI")
OUT_C = OUT_DIR / "healingItemIcons.c"
OUT_H = OUT_DIR / "healingItemIcons.h"

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def load_vals(path: Path, expected_size: tuple[int, int] | None) -> tuple[list[str], int, int]:
    im = Image.open(path).convert("RGBA")
    w, h = im.size
    if expected_size is not None and (w, h) != expected_size:
        raise RuntimeError(f"{path.name}: expected {expected_size}, got {(w, h)}")

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


def emit_array(lines: list[str], name: str, vals: list[str]) -> None:
    lines.append(
        f"const unsigned short {name}[HEALING_ITEM_ICON_WIDTH * HEALING_ITEM_ICON_HEIGHT] = {{"
    )
    for k in range(0, len(vals), 12):
        chunk = vals[k : k + 12]
        end = "," if k + 12 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)
    lines.append("};")
    lines.append("")


def main() -> None:
    sprites = [
        ("healingItemIcon_potion", SRC_DIR / "potion.png"),
        ("healingItemIcon_superPotion", SRC_DIR / "superPotion.png"),
        ("healingItemIcon_hyperPotion", SRC_DIR / "hyperPotion.png"),
        ("healingItemIcon_fullRestore", SRC_DIR / "fullRestore.png"),
    ]

    for _, p in sprites:
        if not p.exists():
            raise RuntimeError(f"Missing sprite: {p}")

    vals0, w, h = load_vals(sprites[0][1], None)
    expected = (w, h)

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    OUT_H.write_text(
        "\n".join(
            [
                "#pragma once",
                "",
                f"#define HEALING_ITEM_ICON_WIDTH  {w}",
                f"#define HEALING_ITEM_ICON_HEIGHT {h}",
                "#define HEALING_ITEM_ICON_COUNT  4",
                "",
                "extern const unsigned short healingItemIcon_potion[HEALING_ITEM_ICON_WIDTH * HEALING_ITEM_ICON_HEIGHT];",
                "extern const unsigned short healingItemIcon_superPotion[HEALING_ITEM_ICON_WIDTH * HEALING_ITEM_ICON_HEIGHT];",
                "extern const unsigned short healingItemIcon_hyperPotion[HEALING_ITEM_ICON_WIDTH * HEALING_ITEM_ICON_HEIGHT];",
                "extern const unsigned short healingItemIcon_fullRestore[HEALING_ITEM_ICON_WIDTH * HEALING_ITEM_ICON_HEIGHT];",
                "",
                "extern const unsigned short* const healingItemIcons[HEALING_ITEM_ICON_COUNT];",
                "",
            ]
        ),
        encoding="utf-8",
    )

    lines: list[str] = []
    lines.append(f'#include "{OUT_H.name}"')
    lines.append("")

    emit_array(lines, sprites[0][0], vals0)
    for name, path in sprites[1:]:
        vals, _, _ = load_vals(path, expected)
        emit_array(lines, name, vals)

    lines.append("const unsigned short* const healingItemIcons[HEALING_ITEM_ICON_COUNT] = {")
    for i, (name, _) in enumerate(sprites):
        end = "," if i + 1 < len(sprites) else ""
        lines.append(f"    {name}{end}")
    lines.append("};")
    lines.append("")

    OUT_C.write_text("\n".join(lines), encoding="utf-8")
    print(f"Wrote {OUT_C} and {OUT_H} ({w}x{h})")


if __name__ == "__main__":
    main()

