from __future__ import annotations

from pathlib import Path
import argparse

from PIL import Image

TILE_SIZE = 16
CANVAS_W = 320
CANVAS_H = 240
TILES_X = CANVAS_W // TILE_SIZE
TILES_Y = CANVAS_H // TILE_SIZE
TOTAL_TILES = TILES_X * TILES_Y

TRANSPARENT_KEY = (255, 0, 255)
PINK_TRANSPARENT = 0xF81F

DEFAULT_SRC = Path("ImageConvertHelpers/background/bg1.png")

OUTPUT_C_FILE = Path("software/graphics/backdrops/backdrop1_tiles.c")
OUTPUT_H_FILE = Path("software/graphics/backdrops/backdrop1_tiles.h")
PREFIX = "backdrop1"
ENUM_PREFIX = "TILE_BACKDROP1_"

def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def get_tile_pixels(img: Image.Image, img_w: int, img_h: int, tile_col: int, tile_row: int):
    vals = []
    for ty in range(TILE_SIZE):
        for tx in range(TILE_SIZE):
            px = tile_col * TILE_SIZE + tx
            py = tile_row * TILE_SIZE + ty
            if px >= img_w or py >= img_h:
                val = PINK_TRANSPARENT
            else:
                r, g, b, a = img.getpixel((px, py))
                if a == 0 or (r, g, b) == TRANSPARENT_KEY:
                    val = PINK_TRANSPARENT
                else:
                    val = rgb_to_565(r, g, b)
            vals.append(f"0x{val:04X}")
    return vals

def prepare_image(src: Path) -> Image.Image:
    im = Image.open(src).convert("RGBA")
    w, h = im.size

    # Support 2x scale sources.
    if (w, h) == (CANVAS_W * 2, CANVAS_H * 2):
        im = im.resize((CANVAS_W, CANVAS_H), resample=Image.NEAREST)
        w, h = im.size

    # If larger than the canvas, crop the center to the visible 320x240 region.
    if w > CANVAS_W or h > CANVAS_H:
        left = max(0, (w - CANVAS_W) // 2)
        top = max(0, (h - CANVAS_H) // 2)
        im = im.crop((left, top, left + CANVAS_W, top + CANVAS_H))
        w, h = im.size

    if w > CANVAS_W or h > CANVAS_H:
        raise ValueError(f"Input image must be <= {CANVAS_W}x{CANVAS_H} after crop/scale, got {w}x{h}")

    return im

def convert_image_to_tiles(img):
    img_w, img_h = img.size
    return [
        get_tile_pixels(img, img_w, img_h, tile_col, tile_row)
        for tile_row in range(TILES_Y)
        for tile_col in range(TILES_X)
    ]

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert a backdrop PNG into 16x16 tile arrays for the 320x240 canvas.")
    parser.add_argument("src", nargs="?", default=str(DEFAULT_SRC), help=f"Source PNG. Default: {DEFAULT_SRC}")
    args = parser.parse_args()

    src_path = Path(args.src)
    if not src_path.exists():
        raise SystemExit(f"Missing input image: {src_path}")

    img = prepare_image(src_path)
    img_w, img_h = img.size
    tile_values = convert_image_to_tiles(img)
    print(f"Image size: {img_w}x{img_h}, canvas: {CANVAS_W}x{CANVAS_H}, tiles: {TILES_X}x{TILES_Y} = {TOTAL_TILES}")

    OUTPUT_C_FILE.parent.mkdir(parents=True, exist_ok=True)

    with open(OUTPUT_C_FILE, "w", encoding="utf-8") as out:
        out.write(f'#include "{OUTPUT_H_FILE.name}"\n\n')
        out.write(f"#define TILE_SIZE {TILE_SIZE}\n\n")

        for tile_row in range(TILES_Y):
            for tile_col in range(TILES_X):
                idx = tile_row * TILES_X + tile_col
                var_name = f"{PREFIX}_tile_{idx}"
                vals = tile_values[idx]

                out.write(f"const short {var_name}[TILE_SIZE * TILE_SIZE] = {{\n")
                for i in range(0, len(vals), 16):
                    line = ", ".join(vals[i:i+16])
                    end = "," if i + 16 < len(vals) else ""
                    out.write(f"    {line}{end}\n")
                out.write("};\n\n")

        out.write(f"const short* const backdrop1_tiles[{TOTAL_TILES}] = {{\n")
        for i in range(TOTAL_TILES):
            end = "," if i + 1 < TOTAL_TILES else ""
            out.write(f"    {PREFIX}_tile_{i}{end}\n")
        out.write("};\n\n")

        out.write(f"const TileId preset_backdrop1[MAP_HEIGHT][MAP_WIDTH] = {{\n")
        for tile_row in range(TILES_Y):
            ids = ", ".join(f"{ENUM_PREFIX}{tile_row * TILES_X + tile_col:03d}"
                            for tile_col in range(TILES_X))
            end = "," if tile_row + 1 < TILES_Y else ""
            out.write(f"    {{ {ids} }}{end}\n")
        out.write("};\n")

    with open(OUTPUT_H_FILE, "w", encoding="utf-8") as h:
        h.write("#pragma once\n\n")
        h.write('#include "tiles.h"\n')
        h.write('#include "map.h"\n\n')
        h.write(f"#define BACKDROP1_TILE_COUNT {TOTAL_TILES}\n\n")
        for i in range(TOTAL_TILES):
            h.write(f"extern const short {PREFIX}_tile_{i}[TILE_SIZE * TILE_SIZE];\n")
        h.write(f"\nextern const short* const backdrop1_tiles[{TOTAL_TILES}];\n")
        h.write(f"extern const TileId preset_backdrop1[MAP_HEIGHT][MAP_WIDTH];\n")

    print(f"Done! Generated {OUTPUT_C_FILE} and {OUTPUT_H_FILE}")
