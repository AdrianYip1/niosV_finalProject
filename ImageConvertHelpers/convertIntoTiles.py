from PIL import Image
import os

TILE_SIZE = 16
CANVAS_W = 320
CANVAS_H = 240
TILES_X = CANVAS_W // TILE_SIZE   # 20
TILES_Y = CANVAS_H // TILE_SIZE   # 15
TOTAL_TILES = TILES_X * TILES_Y   # 300

TRANSPARENT_KEY = (255, 0, 255)
PINK_TRANSPARENT = 0xF81F

OUTPUT_C_FILE = "backdrop1_tiles.c"
OUTPUT_H_FILE = "backdrop1_tiles.h"
PREFIX = "backdrop1"
ENUM_PREFIX = "TILE_BACKDROP1_"

def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def get_tile_pixels(img, img_w, img_h, tile_col, tile_row):
    """Extract one 16x16 tile, padding with pink-transparent if out of image bounds."""
    vals = []
    for ty in range(TILE_SIZE):
        for tx in range(TILE_SIZE):
            px = tile_col * TILE_SIZE + tx
            py = tile_row * TILE_SIZE + ty
            if px >= img_w or py >= img_h:
                val = PINK_TRANSPARENT
            else:
                r, g, b = img.getpixel((px, py))
                if (r, g, b) == TRANSPARENT_KEY:
                    val = PINK_TRANSPARENT
                else:
                    val = rgb_to_565(r, g, b)
            vals.append(f"0x{val:04X}")
    return vals

if __name__ == "__main__":
    img = Image.open("backdrop1.png").convert("RGB")
    img_w, img_h = img.size
    print(f"Image size: {img_w}x{img_h}, canvas: {CANVAS_W}x{CANVAS_H}, tiles: {TILES_X}x{TILES_Y} = {TOTAL_TILES}")

    with open(OUTPUT_C_FILE, "w") as out:
        out.write(f'#include "{OUTPUT_H_FILE}"\n\n')
        out.write(f"#define TILE_SIZE {TILE_SIZE}\n\n")

        for tile_row in range(TILES_Y):
            for tile_col in range(TILES_X):
                idx = tile_row * TILES_X + tile_col
                var_name = f"{PREFIX}_tile_{idx}"
                vals = get_tile_pixels(img, img_w, img_h, tile_col, tile_row)

                out.write(f"const short {var_name}[TILE_SIZE * TILE_SIZE] = {{\n")
                for i in range(0, len(vals), 16):
                    line = ", ".join(vals[i:i+16])
                    end = "," if i + 16 < len(vals) else ""
                    out.write(f"    {line}{end}\n")
                out.write("};\n\n")

        # Pointer table in tile order
        out.write(f"const short* const backdrop1_tiles[{TOTAL_TILES}] = {{\n")
        for i in range(TOTAL_TILES):
            end = "," if i + 1 < TOTAL_TILES else ""
            out.write(f"    {PREFIX}_tile_{i}{end}\n")
        out.write("};\n\n")

        # Map preset: 15 rows of 20 TileIds
        out.write(f"const TileId preset_backdrop1[MAP_HEIGHT][MAP_WIDTH] = {{\n")
        for tile_row in range(TILES_Y):
            ids = ", ".join(f"{ENUM_PREFIX}{tile_row * TILES_X + tile_col:03d}"
                            for tile_col in range(TILES_X))
            end = "," if tile_row + 1 < TILES_Y else ""
            out.write(f"    {{ {ids} }}{end}\n")
        out.write("};\n")

    with open(OUTPUT_H_FILE, "w") as h:
        h.write("#pragma once\n\n")
        h.write('#include "tiles.h"\n')
        h.write('#include "map.h"\n\n')
        h.write(f"#define BACKDROP1_TILE_COUNT {TOTAL_TILES}\n\n")
        h.write(f"// Add these to your TileId enum, starting after your last existing entry:\n")
        h.write(f"// {ENUM_PREFIX}000 ... {ENUM_PREFIX}{TOTAL_TILES-1:03d}\n\n")
        for i in range(TOTAL_TILES):
            h.write(f"extern const short {PREFIX}_tile_{i}[TILE_SIZE * TILE_SIZE];\n")
        h.write(f"\nextern const short* const backdrop1_tiles[{TOTAL_TILES}];\n")
        h.write(f"extern const TileId preset_backdrop1[MAP_HEIGHT][MAP_WIDTH];\n")

    print(f"Done! Generated {OUTPUT_C_FILE} and {OUTPUT_H_FILE}")
    print(f"\nSteps to integrate:")
    print(f"1. Add to TileId enum:  {ENUM_PREFIX}000 = <next_value>, ..., {ENUM_PREFIX}{TOTAL_TILES-1:03d}")
    print(f"   Also update TILE_COUNT to include these {TOTAL_TILES} new tiles.")
    print(f"2. In init_predefined_graphics(), add:")
    print(f"   for (int i = 0; i < {TOTAL_TILES}; i++)")
    print(f"       memcpy(tiles[{ENUM_PREFIX}000 + i], backdrop1_tiles[i], sizeof(short) * TILE_SIZE * TILE_SIZE);")
    print(f"3. Add MAP_PRESET_BACKDROP1 to MapPresetId enum and preset_ptrs[].")