from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import re

from PIL import Image


SPRITES_DIR = Path("ImageConvertHelpers/spritesToUse")
OUT_DIR = Path("software/graphics/sprites/pokemon")
OUT_C = OUT_DIR / "pokemon_sprites.c"
OUT_H = OUT_DIR / "pokemon_sprites.h"

TRANSPARENT_565 = 0xF81F  # RGB565 for #FF00FF
POKEMON_BATTLE_SIZE = 96  # front/back should be 96x96


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def camel_to_screaming_snake(name: str) -> str:
    # charizardFront -> CHARIZARD_FRONT
    s1 = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    s1 = re.sub(r"[^A-Za-z0-9_]", "_", s1)
    s1 = re.sub(r"_+", "_", s1).strip("_")
    return s1.upper()


def first_frame_rgba(path: Path) -> Image.Image:
    im = Image.open(path)
    try:
        im.seek(0)
    except EOFError:
        pass
    return im.convert("RGBA")


def pad_to_96x96(img: Image.Image) -> Image.Image:
    w, h = img.size
    canvas = Image.new("RGBA", (POKEMON_BATTLE_SIZE, POKEMON_BATTLE_SIZE), (255, 0, 255, 0))
    x0 = max(0, (POKEMON_BATTLE_SIZE - w) // 2)
    y0 = max(0, (POKEMON_BATTLE_SIZE - h) // 2)
    canvas.alpha_composite(img, (x0, y0))
    return canvas


@dataclass(frozen=True)
class Sprite:
    name: str
    img: Image.Image


def discover_sprites() -> list[Sprite]:
    sprites: list[Sprite] = []
    for path in sorted(SPRITES_DIR.iterdir(), key=lambda p: p.name.lower()):
        if not path.is_file():
            continue
        if path.suffix.lower() not in {".png", ".gif"}:
            continue

        stem = path.stem

        # Special-case: treat `charizard.png` as the (static) back sprite.
        if stem.lower() == "charizard" and path.suffix.lower() == ".png":
            name = "charizardBack"
        else:
            name = stem

        img = first_frame_rgba(path)

        if name.lower().endswith(("front", "back")):
            img = pad_to_96x96(img)

        sprites.append(Sprite(name=name, img=img))

    if not sprites:
        raise RuntimeError(f"No .png/.gif found in {SPRITES_DIR}")
    return sprites


def sprite_to_565_vals(img: Image.Image) -> list[int]:
    w, h = img.size
    px = img.load()
    vals: list[int] = []
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0 or is_magentaish(r, g, b):
                vals.append(TRANSPARENT_565)
            else:
                vals.append(rgb_to_565(r, g, b))
    return vals


def main() -> None:
    sprites = discover_sprites()
    OUT_DIR.mkdir(parents=True, exist_ok=True)

    h_lines: list[str] = ["#pragma once", ""]
    c_lines: list[str] = ['#include "pokemon_sprites.h"', ""]

    for sprite in sprites:
        w, h = sprite.img.size
        macro = camel_to_screaming_snake(sprite.name)
        h_lines += [
            f"#define {macro}_WIDTH  {w}",
            f"#define {macro}_HEIGHT {h}",
            f"extern const unsigned short {sprite.name}[{macro}_WIDTH * {macro}_HEIGHT];",
            "",
        ]

        vals = sprite_to_565_vals(sprite.img)
        c_lines.append(f"const unsigned short {sprite.name}[{macro}_WIDTH * {macro}_HEIGHT] = {{")
        for k in range(0, len(vals), 16):
            chunk = vals[k : k + 16]
            chunk_s = ", ".join(f"0x{v:04X}" for v in chunk)
            end = "," if k + 16 < len(vals) else ""
            c_lines.append(f"    {chunk_s}{end}")
        c_lines.append("};")
        c_lines.append("")

    OUT_H.write_text("\n".join(h_lines), encoding="utf-8")
    OUT_C.write_text("\n".join(c_lines), encoding="utf-8")

    print(f"Wrote {OUT_H} and {OUT_C} ({len(sprites)} sprites).")


if __name__ == "__main__":
    main()

