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
    with_underscores = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    with_safe_chars = re.sub(r"[^A-Za-z0-9_]", "_", with_underscores)
    normalized = re.sub(r"_+", "_", with_safe_chars).strip("_")
    return normalized.upper()


def first_frame_rgba(path: Path) -> Image.Image:
    image = Image.open(path)
    try:
        image.seek(0)
    except EOFError:
        pass
    return image.convert("RGBA")


def pad_to_96x96(img: Image.Image) -> Image.Image:
    width, height = img.size
    canvas = Image.new("RGBA", (POKEMON_BATTLE_SIZE, POKEMON_BATTLE_SIZE), (255, 0, 255, 0))
    x_offset = max(0, (POKEMON_BATTLE_SIZE - width) // 2)
    y_offset = max(0, (POKEMON_BATTLE_SIZE - height) // 2)
    canvas.alpha_composite(img, (x_offset, y_offset))
    return canvas


def sprite_to_565_lines(var_name: str, macro_prefix: str, img: Image.Image) -> tuple[list[str], list[str]]:
    width, height = img.size
    pixels = img.load()
    rgb565_values: list[int] = []
    for y in range(height):
        for x in range(width):
            red, green, blue, alpha = pixels[x, y]
            if alpha == 0 or is_magentaish(red, green, blue):
                rgb565 = TRANSPARENT_565
            else:
                rgb565 = rgb_to_565(red, green, blue)
            rgb565_values.append(rgb565)

    h_lines = [
        f"#define {macro_prefix}_WIDTH  {width}",
        f"#define {macro_prefix}_HEIGHT {height}",
        f"extern const unsigned short {var_name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT];",
        "",
    ]

    c_lines: list[str] = []
    c_lines.append(f"const unsigned short {var_name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT] = {{")
    for k in range(0, len(rgb565_values), 16):
        chunk = rgb565_values[k : k + 16]
        chunk_s = ", ".join(f"0x{value:04X}" for value in chunk)
        end = "," if k + 16 < len(rgb565_values) else ""
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
    front_gif_paths_by_name = {path.stem[:-5].lower(): path for path in SPRITES_DIR.glob("*Front.gif")}
    back_gif_paths_by_name = {path.stem[:-4].lower(): path for path in SPRITES_DIR.glob("*Back.gif")}
    png_paths_by_name = {path.stem.lower(): path for path in SPRITES_DIR.glob("*.png")}

    pairs: list[BattlePair] = []
    for pokemon in sorted(set(front_gif_paths_by_name.keys()) & set(back_gif_paths_by_name.keys())):
        pairs.append(
            BattlePair(
                pokemon=pokemon,
                back_path=back_gif_paths_by_name[pokemon],
                front_path=front_gif_paths_by_name[pokemon],
            )
        )

    # If a Front GIF exists but no Back GIF, fall back to `<name>.png` as the back sprite.
    # (This matches your current asset set where Charizard has only `charizardFront.gif`.)
    for pokemon in sorted(set(front_gif_paths_by_name.keys()) - set(back_gif_paths_by_name.keys())):
        if pokemon in png_paths_by_name:
            pairs.append(
                BattlePair(
                    pokemon=pokemon,
                    back_path=png_paths_by_name[pokemon],
                    front_path=front_gif_paths_by_name[pokemon],
                )
            )

    # Box PNGs (68x56 in your current assets)
    box_sprites: list[BoxSprite] = []
    for png_path in sorted(SPRITES_DIR.glob("*.png"), key=lambda path: path.name.lower()):
        if png_path.stem.lower() in {"statup", "statdown"}:
            continue
        box_sprites.append(BoxSprite(pokemon=png_path.stem.lower(), path=png_path))

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
