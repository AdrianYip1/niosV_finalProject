from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import re

from PIL import Image


SPRITES_DIR = Path("ImageConvertHelpers/spritesToUse")

BATTLE_OUT_DIR = Path("software/graphics/sprites/pokemon")
BOX_OUT_DIR = Path("software/graphics/sprites/boxSprites")

TRANSPARENT_565 = 0xF81F  # #FF00FF in RGB565
POKEMON_BATTLE_SIZE = 96


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def camel_to_screaming_snake(name: str) -> str:
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


def sprite_to_565_lines(var_name: str, macro_prefix: str, img: Image.Image) -> tuple[list[str], list[str]]:
    w, h = img.size
    px = img.load()
    vals: list[int] = []
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0 or is_magentaish(r, g, b):
                v = TRANSPARENT_565
            else:
                v = rgb_to_565(r, g, b)
            vals.append(v)

    h_lines = [
        f"#define {macro_prefix}_WIDTH  {w}",
        f"#define {macro_prefix}_HEIGHT {h}",
        f"extern const unsigned short {var_name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT];",
        "",
    ]

    c_lines: list[str] = []
    c_lines.append(f"const unsigned short {var_name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT] = {{")
    for k in range(0, len(vals), 16):
        chunk = vals[k : k + 16]
        chunk_s = ", ".join(f"0x{v:04X}" for v in chunk)
        end = "," if k + 16 < len(vals) else ""
        c_lines.append(f"    {chunk_s}{end}")
    c_lines.append("};")
    c_lines.append("")

    return h_lines, c_lines


@dataclass(frozen=True)
class BattlePair:
    pokemon: str
    back_path: Path
    front_path: Path


@dataclass(frozen=True)
class BoxSprite:
    pokemon: str
    path: Path


def discover_inputs() -> tuple[list[BattlePair], list[BoxSprite]]:
    # Battle GIFs
    front = {p.stem[:-5].lower(): p for p in SPRITES_DIR.glob("*Front.gif")}
    back = {p.stem[:-4].lower(): p for p in SPRITES_DIR.glob("*Back.gif")}
    png = {p.stem.lower(): p for p in SPRITES_DIR.glob("*.png")}

    pairs: list[BattlePair] = []
    for pokemon in sorted(set(front.keys()) & set(back.keys())):
        pairs.append(BattlePair(pokemon=pokemon, back_path=back[pokemon], front_path=front[pokemon]))

    # If a Front GIF exists but no Back GIF, fall back to `<name>.png` as the back sprite.
    # (This matches your current asset set where Charizard has only `charizardFront.gif`.)
    for pokemon in sorted(set(front.keys()) - set(back.keys())):
        if pokemon in png:
            pairs.append(BattlePair(pokemon=pokemon, back_path=png[pokemon], front_path=front[pokemon]))

    # Box PNGs (68x56 in your current assets)
    box_sprites: list[BoxSprite] = []
    for p in sorted(SPRITES_DIR.glob("*.png"), key=lambda p: p.name.lower()):
        box_sprites.append(BoxSprite(pokemon=p.stem.lower(), path=p))

    if not pairs and not box_sprites:
        raise RuntimeError(f"No inputs found in {SPRITES_DIR}")
    return pairs, box_sprites


def write_battle_pair(pair: BattlePair) -> None:
    pokemon_camel = pair.pokemon  # already lower; keep consistent with existing variable naming
    file_base = f"{pokemon_camel}Sprite"
    out_h = BATTLE_OUT_DIR / f"{file_base}.h"
    out_c = BATTLE_OUT_DIR / f"{file_base}.c"

    back_img = pad_to_96x96(first_frame_rgba(pair.back_path))
    front_img = pad_to_96x96(first_frame_rgba(pair.front_path))

    h_lines: list[str] = ["#pragma once", ""]
    c_lines: list[str] = [f'#include "{out_h.name}"', ""]

    back_var = f"{pokemon_camel}Back"
    front_var = f"{pokemon_camel}Front"
    back_macro = camel_to_screaming_snake(back_var)
    front_macro = camel_to_screaming_snake(front_var)

    h_part, c_part = sprite_to_565_lines(back_var, back_macro, back_img)
    h_lines += h_part
    c_lines += c_part

    h_part, c_part = sprite_to_565_lines(front_var, front_macro, front_img)
    h_lines += h_part
    c_lines += c_part

    out_h.write_text("\n".join(h_lines), encoding="utf-8")
    out_c.write_text("\n".join(c_lines), encoding="utf-8")


def write_box_sprite(box: BoxSprite) -> None:
    pokemon_camel = box.pokemon
    file_base = f"{pokemon_camel}BoxSprite"
    out_h = BOX_OUT_DIR / f"{file_base}.h"
    out_c = BOX_OUT_DIR / f"{file_base}.c"

    img = first_frame_rgba(box.path)

    h_lines: list[str] = ["#pragma once", ""]
    c_lines: list[str] = [f'#include "{out_h.name}"', ""]

    var = f"{pokemon_camel}Box"
    macro = camel_to_screaming_snake(var)
    h_part, c_part = sprite_to_565_lines(var, macro, img)
    h_lines += h_part
    c_lines += c_part

    out_h.write_text("\n".join(h_lines), encoding="utf-8")
    out_c.write_text("\n".join(c_lines), encoding="utf-8")


def main() -> None:
    pairs, boxes = discover_inputs()
    BATTLE_OUT_DIR.mkdir(parents=True, exist_ok=True)
    BOX_OUT_DIR.mkdir(parents=True, exist_ok=True)

    for pair in pairs:
        write_battle_pair(pair)

    for box in boxes:
        write_box_sprite(box)

    print(f"Wrote {len(pairs)} battle sprite pairs into {BATTLE_OUT_DIR}.")
    print(f"Wrote {len(boxes)} box sprites into {BOX_OUT_DIR}.")


if __name__ == "__main__":
    main()
