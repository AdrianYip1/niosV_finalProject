from __future__ import annotations

from pathlib import Path
import re

from PIL import Image


SRC_DIR = Path("ImageConvertHelpers/newPokemartIdea")

OUT_DIR = Path("software/graphics/sprites/newPokemartIdea")
OUT_C = OUT_DIR / "newPokemartIdeaSprites.c"
OUT_H = OUT_DIR / "newPokemartIdeaSprites.h"

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F

# Explicit order so any future additions don't reshuffle IDs unexpectedly.
ORDER = [
    "pokeBuilding",
    "pokeBuildingInterior",
]


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def camel_to_screaming_snake(name: str) -> str:
    s1 = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    s1 = re.sub(r"[^A-Za-z0-9_]", "_", s1)
    s1 = re.sub(r"_+", "_", s1).strip("_")
    return s1.upper()


def load_vals(path: Path) -> tuple[list[str], int, int]:
    im = Image.open(path).convert("RGBA")
    w, h = im.size
    px = im.load()

    vals: list[str] = []
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0 or (r, g, b) == TRANSPARENT_PINK_RGB:
                v = TRANSPARENT_565
            else:
                v = rgb_to_565(r, g, b)
            vals.append(f"0x{v:04X}")
    return vals, w, h


def emit_array(lines: list[str], name: str, width_macro: str, height_macro: str, vals: list[str]) -> None:
    lines.append(f"const unsigned short {name}[{width_macro} * {height_macro}] = {{")
    for k in range(0, len(vals), 16):
        chunk = vals[k : k + 16]
        end = "," if k + 16 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)
    lines.append("};")
    lines.append("")


def main() -> None:
    if not SRC_DIR.exists():
        raise RuntimeError(f"Missing source dir: {SRC_DIR}")

    pngs = {p.stem: p for p in SRC_DIR.glob("*.png")}
    if not pngs:
        raise RuntimeError(f"No .png files found in {SRC_DIR}")

    ordered_stems: list[str] = []
    for stem in ORDER:
        if stem not in pngs:
            raise RuntimeError(f"Missing expected image: {SRC_DIR / (stem + '.png')}")
        ordered_stems.append(stem)

    extras = sorted([s for s in pngs.keys() if s not in set(ordered_stems)], key=str.lower)
    ordered_stems.extend(extras)

    sprites: list[tuple[str, str, Path]] = []
    for stem in ordered_stems:
        name = f"{stem}Sprite"
        macro_base = camel_to_screaming_snake(stem)
        sprites.append((name, macro_base, pngs[stem]))

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    # Header
    h_lines: list[str] = ["#pragma once", "", "#include <stdint.h>", ""]
    h_lines.append(f"#define NEW_POKEMART_IDEA_SPRITE_COUNT  {len(sprites)}")
    h_lines.append("")
    h_lines.append("typedef enum {")
    for i, (_, macro_base, _) in enumerate(sprites):
        h_lines.append(f"    NEW_POKEMART_IDEA_SPRITE_{macro_base} = {i},")
    h_lines.append("} NewPokemartIdeaSpriteId;")
    h_lines.append("")

    sprite_dims: list[tuple[int, int]] = []
    for name, macro_base, path in sprites:
        _, w, h = load_vals(path)
        sprite_dims.append((w, h))
        w_macro = f"NEW_POKEMART_IDEA_{macro_base}_WIDTH"
        h_macro = f"NEW_POKEMART_IDEA_{macro_base}_HEIGHT"
        h_lines.extend(
            [
                f"#define {w_macro}  {w}",
                f"#define {h_macro} {h}",
                f"extern const unsigned short {name}[{w_macro} * {h_macro}];",
                "",
            ]
        )

    h_lines.extend(
        [
            "extern const unsigned short* const newPokemartIdeaSprites[NEW_POKEMART_IDEA_SPRITE_COUNT];",
            "extern const uint16_t newPokemartIdeaSpritesWidths[NEW_POKEMART_IDEA_SPRITE_COUNT];",
            "extern const uint16_t newPokemartIdeaSpritesHeights[NEW_POKEMART_IDEA_SPRITE_COUNT];",
            "",
        ]
    )

    OUT_H.write_text("\n".join(h_lines), encoding="utf-8")

    # Source
    c_lines: list[str] = [f'#include "{OUT_H.name}"', ""]
    for (name, macro_base, path), (w, h) in zip(sprites, sprite_dims):
        vals, w2, h2 = load_vals(path)
        if (w2, h2) != (w, h):
            raise RuntimeError(f"{path.name}: size changed unexpectedly ({w}x{h} -> {w2}x{h2})")
        emit_array(
            c_lines,
            name,
            f"NEW_POKEMART_IDEA_{macro_base}_WIDTH",
            f"NEW_POKEMART_IDEA_{macro_base}_HEIGHT",
            vals,
        )

    c_lines.append("const unsigned short* const newPokemartIdeaSprites[NEW_POKEMART_IDEA_SPRITE_COUNT] = {")
    for i, (name, _, _) in enumerate(sprites):
        end = "," if i + 1 < len(sprites) else ""
        c_lines.append(f"    {name}{end}")
    c_lines.append("};")
    c_lines.append("")

    c_lines.append("const uint16_t newPokemartIdeaSpritesWidths[NEW_POKEMART_IDEA_SPRITE_COUNT] = {")
    for i, (w, _) in enumerate(sprite_dims):
        end = "," if i + 1 < len(sprite_dims) else ""
        c_lines.append(f"    {w}{end}")
    c_lines.append("};")
    c_lines.append("")

    c_lines.append("const uint16_t newPokemartIdeaSpritesHeights[NEW_POKEMART_IDEA_SPRITE_COUNT] = {")
    for i, (_, h) in enumerate(sprite_dims):
        end = "," if i + 1 < len(sprite_dims) else ""
        c_lines.append(f"    {h}{end}")
    c_lines.append("};")
    c_lines.append("")

    OUT_C.write_text("\n".join(c_lines), encoding="utf-8")
    print(f"Wrote {OUT_C} and {OUT_H}")


if __name__ == "__main__":
    main()

