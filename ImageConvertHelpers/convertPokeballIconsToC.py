from __future__ import annotations

from pathlib import Path

from PIL import Image

SRC_DIR = Path("ImageConvertHelpers/pokeballSprites")
OUT_DIR = Path("software/graphics/sprites/battleItemsUI")
OUT_C = OUT_DIR / "pokeballIcons.c"
OUT_H = OUT_DIR / "pokeballIcons.h"

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


def emit_array(lines: list[str], name: str, vals: list[str], w: int, h: int) -> None:
    lines.append(f"const unsigned short {name}[POKEBALL_ICON_WIDTH * POKEBALL_ICON_HEIGHT] = {{")
    for k in range(0, len(vals), 14):
        chunk = vals[k : k + 14]
        end = "," if k + 14 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)
    lines.append("};")
    lines.append("")


def main() -> None:
    sprites = [
        ("pokeballIcon_poke", SRC_DIR / "pokeball.png"),
        ("pokeballIcon_great", SRC_DIR / "greatball.png"),
        ("pokeballIcon_ultra", SRC_DIR / "ultraball.png"),
        ("pokeballIcon_premier", SRC_DIR / "premierball.png"),
        ("pokeballIcon_master", SRC_DIR / "masterball.png"),
    ]

    for _, p in sprites:
        if not p.exists():
            raise RuntimeError(f"Missing sprite: {p}")

    vals0, w, h = load_vals(sprites[0][1], None)
    expected = (w, h)

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    h_lines: list[str] = [
        "#pragma once",
        "",
        f"#define POKEBALL_ICON_WIDTH  {w}",
        f"#define POKEBALL_ICON_HEIGHT {h}",
        f"#define POKEBALL_ICON_COUNT  {len(sprites)}",
        "",
    ]
    for name, _ in sprites:
        h_lines.append(
            f"extern const unsigned short {name}[POKEBALL_ICON_WIDTH * POKEBALL_ICON_HEIGHT];"
        )
    h_lines.extend(
        [
            "",
            "extern const unsigned short* const pokeballIcons[POKEBALL_ICON_COUNT];",
            "",
        ]
    )
    OUT_H.write_text("\n".join(h_lines), encoding="utf-8")

    lines: list[str] = []
    lines.append(f'#include "{OUT_H.name}"')
    lines.append("")

    emit_array(lines, sprites[0][0], vals0, w, h)
    for name, path in sprites[1:]:
        vals, _, _ = load_vals(path, expected)
        emit_array(lines, name, vals, w, h)

    lines.append("const unsigned short* const pokeballIcons[POKEBALL_ICON_COUNT] = {")
    for i, (name, _) in enumerate(sprites):
        end = "," if i + 1 < len(sprites) else ""
        lines.append(f"    {name}{end}")
    lines.append("};")
    lines.append("")

    OUT_C.write_text("\n".join(lines), encoding="utf-8")
    print(f"Wrote {OUT_C} and {OUT_H} ({w}x{h})")


if __name__ == "__main__":
    main()
