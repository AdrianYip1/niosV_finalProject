from pathlib import Path
import sys

from PIL import Image

SCRIPT_DIR = Path(__file__).resolve().parent
REPO_ROOT = SCRIPT_DIR.parent

TILE_SIZE = 16
CANVAS_W = 320
CANVAS_H = 240
TILES_X = CANVAS_W // TILE_SIZE
TILES_Y = CANVAS_H // TILE_SIZE
TOTAL_TILES = TILES_X * TILES_Y
SRC_DIR = SCRIPT_DIR
OUT_DIR = REPO_ROOT / "software/graphics/backdrops"

TRANSPARENT_KEY = (255, 0, 255)
PINK_TRANSPARENT = 0xF81F


def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def get_tile_pixels(img, tile_col, tile_row):
    vals = []
    for ty in range(TILE_SIZE):
        for tx in range(TILE_SIZE):
            px = tile_col * TILE_SIZE + tx
            py = tile_row * TILE_SIZE + ty
            r, g, b = img.getpixel((px, py))
            if (r, g, b) == TRANSPARENT_KEY:
                val = PINK_TRANSPARENT
            else:
                val = rgb_to_565(r, g, b)
            vals.append(f"0x{val:04X}")
    return vals


def convert_image_to_tiles(img):
    return [
        get_tile_pixels(img, tile_col, tile_row)
        for tile_row in range(TILES_Y)
        for tile_col in range(TILES_X)
    ]


def main():
    if len(sys.argv) != 2:
        raise SystemExit("usage: python3 convertIntoTiles.py <name>")

    name = sys.argv[1]
    image_path = SRC_DIR / f"{name}.png"
    output_c_file = OUT_DIR / f"{name}_tiles.c"
    output_h_file = OUT_DIR / f"{name}_tiles.h"
    count_name = f"{name.upper()}_TILE_COUNT"
    enum_base = f"TILE_{name.upper()}_START"

    img = Image.open(image_path).convert("RGB")
    img_w, img_h = img.size
    if (img_w, img_h) != (CANVAS_W, CANVAS_H):
        raise ValueError(f"{image_path} must be {CANVAS_W}x{CANVAS_H}, got {img_w}x{img_h}")

    tile_values = convert_image_to_tiles(img)
    OUT_DIR.mkdir(parents=True, exist_ok=True)

    with open(output_c_file, "w", encoding="ascii") as out:
        out.write(f'#include "{output_h_file.name}"\n\n')

        for tile_row in range(TILES_Y):
            for tile_col in range(TILES_X):
                idx = tile_row * TILES_X + tile_col
                var_name = f"{name}_tile_{idx}"
                vals = tile_values[idx]

                out.write(f"const short {var_name}[TILE_SIZE * TILE_SIZE] = {{\n")
                for i in range(0, len(vals), 16):
                    line = ", ".join(vals[i:i + 16])
                    end = "," if i + 16 < len(vals) else ""
                    out.write(f"    {line}{end}\n")
                out.write("};\n\n")

        out.write(f"const short* const {name}_tiles[{TOTAL_TILES}] = {{\n")
        for i in range(TOTAL_TILES):
            end = "," if i + 1 < TOTAL_TILES else ""
            out.write(f"    {name}_tile_{i}{end}\n")
        out.write("};\n\n")

        out.write(f"const TileId preset_{name}[MAP_HEIGHT][MAP_WIDTH] = {{\n")
        for tile_row in range(TILES_Y):
            ids = ", ".join(f"{enum_base} + {tile_row * TILES_X + tile_col}"
                            for tile_col in range(TILES_X))
            end = "," if tile_row + 1 < TILES_Y else ""
            out.write(f"    {{ {ids} }}{end}\n")
        out.write("};\n")

    with open(output_h_file, "w", encoding="ascii") as h:
        h.write("#pragma once\n\n")
        h.write('#include "../tiles.h"\n')
        h.write('#include "../map.h"\n\n')
        h.write(f"#define {count_name} {TOTAL_TILES}\n\n")
        for i in range(TOTAL_TILES):
            h.write(f"extern const short {name}_tile_{i}[TILE_SIZE * TILE_SIZE];\n")
        h.write(f"\nextern const short* const {name}_tiles[{TOTAL_TILES}];\n")
        h.write(f"extern const TileId preset_{name}[MAP_HEIGHT][MAP_WIDTH];\n")

    print(f"Done! Generated {output_c_file} and {output_h_file}")


if __name__ == "__main__":
    main()
