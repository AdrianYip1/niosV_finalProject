from __future__ import annotations

import argparse
from pathlib import Path

from PIL import Image

TILE_SIZE = 16
CANVAS_W = 320
CANVAS_H = 240
TILES_X = CANVAS_W // TILE_SIZE
TILES_Y = CANVAS_H // TILE_SIZE
TOTAL_TILES = TILES_X * TILES_Y

TRANSPARENT_KEY = (255, 0, 255)
PINK_TRANSPARENT = 0xF81F

DEFAULT_OUT_DIR = Path("software/graphics/backdrops")


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def sanitize_name(name: str) -> str:
    cleaned = []
    for ch in name:
        if ch.isalnum() or ch == "_":
            cleaned.append(ch.lower())
        else:
            cleaned.append("_")
    result = "".join(cleaned).strip("_")
    if not result:
        raise ValueError("Output name cannot be empty")
    return result


def prepare_image(src: Path) -> Image.Image:
    img = Image.open(src).convert("RGBA")
    w, h = img.size

    if (w, h) == (CANVAS_W * 2, CANVAS_H * 2):
        img = img.resize((CANVAS_W, CANVAS_H), resample=Image.NEAREST)
        w, h = img.size

    if w > CANVAS_W or h > CANVAS_H:
        left = max(0, (w - CANVAS_W) // 2)
        top = max(0, (h - CANVAS_H) // 2)
        img = img.crop((left, top, left + CANVAS_W, top + CANVAS_H))
        w, h = img.size

    if (w, h) != (CANVAS_W, CANVAS_H):
        raise ValueError(
            f"{src} must be {CANVAS_W}x{CANVAS_H} (or 2x that), got {w}x{h}"
        )

    return img


def get_tile_values(img: Image.Image, tile_col: int, tile_row: int) -> list[int]:
    vals: list[int] = []
    for ty in range(TILE_SIZE):
        for tx in range(TILE_SIZE):
            px = tile_col * TILE_SIZE + tx
            py = tile_row * TILE_SIZE + ty
            r, g, b, a = img.getpixel((px, py))
            if a == 0 or (r, g, b) == TRANSPARENT_KEY:
                vals.append(PINK_TRANSPARENT)
            else:
                vals.append(rgb_to_565(r, g, b))
    return vals


def convert_image_to_tiles(img: Image.Image) -> list[list[int]]:
    return [
        get_tile_values(img, tile_col, tile_row)
        for tile_row in range(TILES_Y)
        for tile_col in range(TILES_X)
    ]


def format_tile_array(symbol: str, vals: list[int]) -> str:
    lines = [f"const short {symbol}[TILE_SIZE * TILE_SIZE] = {{"]
    for i in range(0, len(vals), TILE_SIZE):
        row = ", ".join(f"0x{v:04X}" for v in vals[i:i + TILE_SIZE])
        suffix = "," if i + TILE_SIZE < len(vals) else ""
        lines.append(f"    {row}{suffix}")
    lines.append("};")
    return "\n".join(lines)


def build_header_text(name: str) -> str:
    count_name = f"{name.upper()}_TILE_COUNT"
    lines = [
        "#pragma once",
        "",
        '#include "../tiles.h"',
        '#include "../map.h"',
        "",
        "#ifndef TILE_SIZE",
        f"#define TILE_SIZE {TILE_SIZE}",
        "#endif",
        "",
        f"#define {count_name} {TOTAL_TILES}",
        "",
    ]

    for i in range(TOTAL_TILES):
        lines.append(f"extern const short {name}_tile_{i}[TILE_SIZE * TILE_SIZE];")

    lines.extend(
        [
            "",
            f"extern const short* const {name}_tiles[{TOTAL_TILES}];",
            f"extern const TileId preset_{name}[MAP_HEIGHT][MAP_WIDTH];",
            "",
        ]
    )
    return "\n".join(lines)


def build_source_text(name: str, enum_base: str, tile_values: list[list[int]], header_name: str) -> str:
    lines = [f'#include "{header_name}"', ""]

    for i, vals in enumerate(tile_values):
        lines.append(format_tile_array(f"{name}_tile_{i}", vals))
        lines.append("")

    lines.append(f"const short* const {name}_tiles[{TOTAL_TILES}] = {{")
    for i in range(TOTAL_TILES):
        suffix = "," if i + 1 < TOTAL_TILES else ""
        lines.append(f"    {name}_tile_{i}{suffix}")
    lines.append("};")
    lines.append("")

    lines.append(f"const TileId preset_{name}[MAP_HEIGHT][MAP_WIDTH] = {{")
    for tile_row in range(TILES_Y):
        ids = ", ".join(
            f"{enum_base} + {tile_row * TILES_X + tile_col}"
            for tile_col in range(TILES_X)
        )
        suffix = "," if tile_row + 1 < TILES_Y else ""
        lines.append(f"    {{ {ids} }}{suffix}")
    lines.append("};")
    lines.append("")

    return "\n".join(lines)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Convert a 320x240 backdrop PNG into 16x16 tile arrays."
    )
    parser.add_argument("src", help="Source PNG file")
    parser.add_argument(
        "--name",
        help="Base output name. Defaults to the source filename stem.",
    )
    parser.add_argument(
        "--enum-base",
        help="TileId enum base, e.g. TILE_BACKDROP1_START. Defaults to TILE_<NAME>_START.",
    )
    parser.add_argument(
        "--out-dir",
        default=str(DEFAULT_OUT_DIR),
        help=f"Output directory. Default: {DEFAULT_OUT_DIR}",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()

    src_path = Path(args.src)
    if not src_path.exists():
        raise SystemExit(f"Missing input image: {src_path}")

    name = sanitize_name(args.name or src_path.stem)
    enum_base = args.enum_base or f"TILE_{name.upper()}_START"
    out_dir = Path(args.out_dir)
    output_h = out_dir / f"{name}_tiles.h"
    output_c = out_dir / f"{name}_tiles.c"

    img = prepare_image(src_path)
    tile_values = convert_image_to_tiles(img)

    out_dir.mkdir(parents=True, exist_ok=True)
    output_h.write_text(build_header_text(name), encoding="ascii")
    output_c.write_text(
        build_source_text(name, enum_base, tile_values, output_h.name),
        encoding="ascii",
    )

    print(f"Done! Generated {output_c}")
    print(f"Done! Generated {output_h}")


if __name__ == "__main__":
    main()
