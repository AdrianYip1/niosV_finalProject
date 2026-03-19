from PIL import Image
import os

IMAGE_WIDTH  = 320
IMAGE_HEIGHT = 240
TILE_SIZE    = 16
TRANSPARENT_KEY   = (255, 0, 255)
WHITE_THRESHOLD   = 240
OUTPUT_C_FILE     = "backdrop1.c"
OUTPUT_H_FILE     = "backdrop1.h"

# How many tiles fit in each dimension
TILES_X = IMAGE_WIDTH  // TILE_SIZE   # 20
TILES_Y = IMAGE_HEIGHT // TILE_SIZE   # 15

PINK_TRANSPARENT = 0xF81F

def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def get_tile_pixels(img, img_w, img_h, tile_col, tile_row):
    """Extract one 16x16 tile's worth of 565 values, padding with pink if OOB."""
    vals = []
    ox = tile_col * TILE_SIZE
    oy = tile_row * TILE_SIZE
    for y in range(TILE_SIZE):
        for x in range(TILE_SIZE):
            px, py = ox + x, oy + y
            if px >= img_w or py >= img_h:
                vals.append(PINK_TRANSPARENT)
                continue
            r, g, b = img.getpixel((px, py))
            if (r, g, b) == TRANSPARENT_KEY or \
               (r > WHITE_THRESHOLD and g > WHITE_THRESHOLD and b > WHITE_THRESHOLD):
                vals.append(PINK_TRANSPARENT)
            else:
                vals.append(rgb_to_565(r, g, b))
    return vals

def write_tile_array(out, var_name, vals):
    """Write a single tile's flat array."""
    out.write(f"const unsigned short {var_name}[BACKDROP1_TILE_SIZE * BACKDROP1_TILE_SIZE] = {{\n")
    hex_vals = [f"0x{v:04X}" for v in vals]
    for i in range(0, len(hex_vals), 16):
        chunk = hex_vals[i:i+16]
        end = "," if i + 16 < len(hex_vals) else ""
        out.write(f"    {', '.join(chunk)}{end}\n")
    out.write("};\n\n")

if __name__ == "__main__":
    name = "backdrop1.png"
    assert os.path.exists(name), f"{name} not found"

    img = Image.open(name).convert("RGB")
    img_w, img_h = img.size

    total_tiles = TILES_X * TILES_Y  # 300

    with open(OUTPUT_C_FILE, "w") as out:
        out.write(f'#include "{OUTPUT_H_FILE}"\n\n')

        # Emit one array per tile, named backdrop1_r{row}_c{col}
        for tr in range(TILES_Y):
            for tc in range(TILES_X):
                var = f"backdrop1_r{tr}_c{tc}"
                vals = get_tile_pixels(img, img_w, img_h, tc, tr)
                write_tile_array(out, var, vals)

        # Emit the lookup table: backdrop1_tiles[TILES_Y][TILES_X]
        out.write(
            f"const unsigned short* const backdrop1_tiles"
            f"[BACKDROP1_TILES_Y][BACKDROP1_TILES_X] = {{\n"
        )
        for tr in range(TILES_Y):
            row_vars = ", ".join(f"backdrop1_r{tr}_c{tc}" for tc in range(TILES_X))
            end = "," if tr + 1 < TILES_Y else ""
            out.write(f"    {{ {row_vars} }}{end}\n")
        out.write("};\n")

    with open(OUTPUT_H_FILE, "w") as h:
        h.write("#pragma once\n\n")
        h.write(f"#define BACKDROP1_TILE_SIZE {TILE_SIZE}\n")
        h.write(f"#define BACKDROP1_TILES_X   {TILES_X}\n")
        h.write(f"#define BACKDROP1_TILES_Y   {TILES_Y}\n\n")
        for tr in range(TILES_Y):
            for tc in range(TILES_X):
                h.write(
                    f"extern const unsigned short "
                    f"backdrop1_r{tr}_c{tc}"
                    f"[BACKDROP1_TILE_SIZE * BACKDROP1_TILE_SIZE];\n"
                )
        h.write(
            f"\nextern const unsigned short* const "
            f"backdrop1_tiles[BACKDROP1_TILES_Y][BACKDROP1_TILES_X];\n"
        )

    print(f"Done! {TILES_X}x{TILES_Y} = {total_tiles} tiles -> {OUTPUT_C_FILE}, {OUTPUT_H_FILE}")