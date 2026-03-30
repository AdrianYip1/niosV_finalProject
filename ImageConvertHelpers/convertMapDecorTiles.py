from pathlib import Path

from PIL import Image


TRANSPARENT_RGB = (255, 0, 255)
TRANSPARENT_565 = 0xF81F
TREE_SRC = Path("ImageConvertHelpers/tree_pink.png")
GRASS_SRC = Path("ImageConvertHelpers/grass_pink.png")
POKEMON_CENTER_SRC = Path("ImageConvertHelpers/buildings/pokemon_center/pokemon_center.png")
POKE_MART_SRC = Path("ImageConvertHelpers/buildings/poke_mart.png")
OUT_C = Path("software/graphics/backdrops/decor_tiles.c")
OUT_H = Path("software/graphics/backdrops/decor_tiles.h")


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def load_rgba(path: Path, expected_w: int, expected_h: int) -> Image.Image:
    img = Image.open(path).convert("RGBA")
    w, h = img.size
    if (w, h) != (expected_w, expected_h):
        raise ValueError(f"{path} must be {expected_w}x{expected_h}, got {w}x{h}")
    return img


def rgba_to_565_values(img: Image.Image) -> list[int]:
    vals: list[int] = []
    px = img.load()
    w, h = img.size
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0 or (r, g, b) == TRANSPARENT_RGB:
                vals.append(TRANSPARENT_565)
            else:
                vals.append(rgb_to_565(r, g, b))
    return vals


def format_array(name: str, vals: list[int]) -> str:
    lines = [f"const short {name}[TILE_SIZE * TILE_SIZE] = {{"]
    for i in range(0, len(vals), 16):
        line = ", ".join(f"0x{v:04X}" for v in vals[i:i + 16])
        end = "," if i + 16 < len(vals) else ""
        lines.append(f"    {line}{end}")
    lines.append("};")
    return "\n".join(lines)


def main() -> None:
    tree = load_rgba(TREE_SRC, 16, 32)
    grass = load_rgba(GRASS_SRC, 16, 16)
    pokemon_center = load_rgba(POKEMON_CENTER_SRC, 48, 48)
    poke_mart = load_rgba(POKE_MART_SRC, 48, 48)

    tree_top_vals = rgba_to_565_values(tree.crop((0, 0, 16, 16)))
    tree_bottom_vals = rgba_to_565_values(tree.crop((0, 16, 16, 32)))
    grass_vals = rgba_to_565_values(grass)
    pokemon_center_top_left_vals = rgba_to_565_values(pokemon_center.crop((0, 0, 16, 16)))
    pokemon_center_top_middle_vals = rgba_to_565_values(pokemon_center.crop((16, 0, 32, 16)))
    pokemon_center_top_right_vals = rgba_to_565_values(pokemon_center.crop((32, 0, 48, 16)))
    pokemon_center_middle_left_vals = rgba_to_565_values(pokemon_center.crop((0, 16, 16, 32)))
    pokemon_center_middle_middle_vals = rgba_to_565_values(pokemon_center.crop((16, 16, 32, 32)))
    pokemon_center_middle_right_vals = rgba_to_565_values(pokemon_center.crop((32, 16, 48, 32)))
    pokemon_center_bottom_left_vals = rgba_to_565_values(pokemon_center.crop((0, 32, 16, 48)))
    pokemon_center_bottom_middle_vals = rgba_to_565_values(pokemon_center.crop((16, 32, 32, 48)))
    pokemon_center_bottom_right_vals = rgba_to_565_values(pokemon_center.crop((32, 32, 48, 48)))
    poke_mart_top_left_vals = rgba_to_565_values(poke_mart.crop((0, 0, 16, 16)))
    poke_mart_top_middle_vals = rgba_to_565_values(poke_mart.crop((16, 0, 32, 16)))
    poke_mart_top_right_vals = rgba_to_565_values(poke_mart.crop((32, 0, 48, 16)))
    poke_mart_middle_left_vals = rgba_to_565_values(poke_mart.crop((0, 16, 16, 32)))
    poke_mart_middle_middle_vals = rgba_to_565_values(poke_mart.crop((16, 16, 32, 32)))
    poke_mart_middle_right_vals = rgba_to_565_values(poke_mart.crop((32, 16, 48, 32)))
    poke_mart_bottom_left_vals = rgba_to_565_values(poke_mart.crop((0, 32, 16, 48)))
    poke_mart_bottom_middle_vals = rgba_to_565_values(poke_mart.crop((16, 32, 32, 48)))
    poke_mart_bottom_right_vals = rgba_to_565_values(poke_mart.crop((32, 32, 48, 48)))

    header = "\n".join([
        "#pragma once",
        "",
        '#include "../graphics.h"',
        "",
        "extern const short tree_top_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short tree_bottom_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short grass_patch_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short pokemon_center_top_left_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short pokemon_center_top_middle_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short pokemon_center_top_right_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short pokemon_center_middle_left_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short pokemon_center_middle_middle_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short pokemon_center_middle_right_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short pokemon_center_bottom_left_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short pokemon_center_bottom_middle_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short pokemon_center_bottom_right_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short poke_mart_top_left_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short poke_mart_top_middle_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short poke_mart_top_right_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short poke_mart_middle_left_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short poke_mart_middle_middle_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short poke_mart_middle_right_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short poke_mart_bottom_left_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short poke_mart_bottom_middle_tile[TILE_SIZE * TILE_SIZE];",
        "extern const short poke_mart_bottom_right_tile[TILE_SIZE * TILE_SIZE];",
        "",
    ])
    OUT_H.write_text(header, encoding="ascii")

    source = "\n\n".join([
        '#include "decor_tiles.h"',
        format_array("tree_top_tile", tree_top_vals),
        format_array("tree_bottom_tile", tree_bottom_vals),
        format_array("grass_patch_tile", grass_vals),
        format_array("pokemon_center_top_left_tile", pokemon_center_top_left_vals),
        format_array("pokemon_center_top_middle_tile", pokemon_center_top_middle_vals),
        format_array("pokemon_center_top_right_tile", pokemon_center_top_right_vals),
        format_array("pokemon_center_middle_left_tile", pokemon_center_middle_left_vals),
        format_array("pokemon_center_middle_middle_tile", pokemon_center_middle_middle_vals),
        format_array("pokemon_center_middle_right_tile", pokemon_center_middle_right_vals),
        format_array("pokemon_center_bottom_left_tile", pokemon_center_bottom_left_vals),
        format_array("pokemon_center_bottom_middle_tile", pokemon_center_bottom_middle_vals),
        format_array("pokemon_center_bottom_right_tile", pokemon_center_bottom_right_vals),
        format_array("poke_mart_top_left_tile", poke_mart_top_left_vals),
        format_array("poke_mart_top_middle_tile", poke_mart_top_middle_vals),
        format_array("poke_mart_top_right_tile", poke_mart_top_right_vals),
        format_array("poke_mart_middle_left_tile", poke_mart_middle_left_vals),
        format_array("poke_mart_middle_middle_tile", poke_mart_middle_middle_vals),
        format_array("poke_mart_middle_right_tile", poke_mart_middle_right_vals),
        format_array("poke_mart_bottom_left_tile", poke_mart_bottom_left_vals),
        format_array("poke_mart_bottom_middle_tile", poke_mart_bottom_middle_vals),
        format_array("poke_mart_bottom_right_tile", poke_mart_bottom_right_vals),
        "",
    ])
    OUT_C.write_text(source, encoding="ascii")

    print(f"Generated {OUT_C}")
    print(f"Generated {OUT_H}")


if __name__ == "__main__":
    main()
