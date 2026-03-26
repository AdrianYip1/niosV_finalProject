from PIL import Image

TILE_SIZE = 16
CANVAS_W = 320
CANVAS_H = 240
TILES_X = CANVAS_W // TILE_SIZE
TILES_Y = CANVAS_H // TILE_SIZE
TOTAL_TILES = TILES_X * TILES_Y

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

def validate_image_size(img_w, img_h):
    assert img_w <= CANVAS_W and img_h <= CANVAS_H, (
        f"Input image must be at most {CANVAS_W}x{CANVAS_H}, got {img_w}x{img_h}"
    )

def convert_image_to_tiles(img):
    img_w, img_h = img.size
    return [
        get_tile_pixels(img, tile_col, tile_row)
        for tile_row in range(TILES_Y)
        for tile_col in range(TILES_X)
    ]

if __name__ == "__main__":
    img = Image.open("backdrop1.png").convert("RGB")
    img_w, img_h = img.size
    if (img_w, img_h) != (CANVAS_W, CANVAS_H):
        raise ValueError(f"{image_path} must be {CANVAS_W}x{CANVAS_H}, got {img_w}x{img_h}")

    tile_values = convert_image_to_tiles(img)
    OUT_DIR.mkdir(parents=True, exist_ok=True)

    OUTPUT_C_FILE.parent.mkdir(parents=True, exist_ok=True)

    with open(OUTPUT_C_FILE, "w", encoding="utf-8") as out:
        out.write(f'#include "{OUTPUT_H_FILE.name}"\n\n')

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
            out.write(f"    {name}_tile_{i}{end}\n")
        out.write("};\n\n")

        out.write(f"const TileId preset_{name}[MAP_HEIGHT][MAP_WIDTH] = {{\n")
        out.write(f"const TileId preset_{name}[MAP_HEIGHT][MAP_WIDTH] = {{\n")
        for tile_row in range(TILES_Y):
            ids = ", ".join(f"{enum_base} + {tile_row * TILES_X + tile_col}"
            ids = ", ".join(f"{TILE_ENUM_START} + {tile_row * TILES_X + tile_col}"
                            for tile_col in range(TILES_X))
            end = "," if tile_row + 1 < TILES_Y else ""
            out.write(f"    {{ {ids} }}{end}\n")
        out.write("};\n")

    with open(output_h_file, "w", encoding="ascii") as h:
    with open(OUTPUT_H_FILE, "w", encoding="utf-8") as h:
        h.write("#pragma once\n\n")
        h.write('#include "../tiles.h"\n')
        h.write('#include "../map.h"\n\n')
        h.write(f"#define {count_name} {TOTAL_TILES}\n\n")
        h.write('#include "../tiles.h"\n')
        h.write('#include "../map.h"\n\n')
        h.write("#ifndef TILE_SIZE\n")
        h.write(f"#define TILE_SIZE {TILE_SIZE}\n")
        h.write("#endif\n\n")
        h.write(f"#define BACKDROP1_TILE_COUNT {TOTAL_TILES}\n\n")
        for i in range(TOTAL_TILES):
            h.write(f"extern const short {name}_tile_{i}[TILE_SIZE * TILE_SIZE];\n")
        h.write(f"\nextern const short* const {name}_tiles[{TOTAL_TILES}];\n")
        h.write(f"extern const TileId preset_{name}[MAP_HEIGHT][MAP_WIDTH];\n")
            h.write(f"extern const short {name}_tile_{i}[TILE_SIZE * TILE_SIZE];\n")
        h.write(f"\nextern const short* const {name}_tiles[{TOTAL_TILES}];\n")
        h.write(f"extern const TileId preset_{name}[MAP_HEIGHT][MAP_WIDTH];\n")

    print(f"Done! Generated {output_c_file} and {output_h_file}")


if __name__ == "__main__":
    main()

    print(f"Done! Generated {OUTPUT_C_FILE} and {OUTPUT_H_FILE}")
