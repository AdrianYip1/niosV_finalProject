from __future__ import annotations

from pathlib import Path

from PIL import Image


TRANSPARENT_PINK_RGB = (255, 0, 255)
TRANSPARENT_565 = 0xF81F


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def image_to_rgb565_values(src_png: Path, out_w: int, out_h: int) -> list[str]:
    if not src_png.exists():
        raise RuntimeError(f"Missing input: {src_png}")

    im = Image.open(src_png).convert("RGBA")
    if im.size != (out_w, out_h):
        im = im.resize((out_w, out_h), resample=Image.NEAREST)

    px = im.load()
    vals: list[str] = []
    for y in range(out_h):
        for x in range(out_w):
            r, g, b, a = px[x, y]
            if a == 0 or (r, g, b) == TRANSPARENT_PINK_RGB or is_magentaish(r, g, b):
                v = TRANSPARENT_565
            else:
                v = rgb_to_565(r, g, b)
            vals.append(f"0x{v:04X}")
    return vals


def write_sprite(header_path: Path, source_path: Path, width_macro: str, height_macro: str,
                 symbol_name: str, vals: list[str], out_w: int, out_h: int) -> None:
    header_path.write_text(
        "\n".join(
            [
                "#pragma once",
                "",
                f"#define {width_macro} {out_w}",
                f"#define {height_macro} {out_h}",
                "",
                f"extern const unsigned short {symbol_name}[{width_macro} * {height_macro}];",
                "",
            ]
        ),
        encoding="utf-8",
    )

    c_lines: list[str] = [f'#include "{header_path.name}"', ""]
    c_lines.append(f"const unsigned short {symbol_name}[{width_macro} * {height_macro}] = {{")
    for k in range(0, len(vals), 16):
        chunk = vals[k:k + 16]
        end = "," if k + 16 < len(vals) else ""
        c_lines.append("    " + ", ".join(chunk) + end)
    c_lines.append("};")
    c_lines.append("")

    source_path.write_text("\n".join(c_lines), encoding="utf-8")


def main() -> None:
    cynthia_src = Path("ImageConvertHelpers/trainerSprites/cynthiaFront.png")
    cynthia_out_h = 30
    cynthia_out_w = 24
    cynthia_header = Path("software/graphics/sprites/trainerSprites/cynthiaFrontMapSprite.h")
    cynthia_source = Path("software/graphics/sprites/trainerSprites/cynthiaFrontMapSprite.c")
    cynthia_vals = image_to_rgb565_values(cynthia_src, cynthia_out_w, cynthia_out_h)
    write_sprite(
        cynthia_header,
        cynthia_source,
        "CYNTHIA_FRONT_MAP_WIDTH",
        "CYNTHIA_FRONT_MAP_HEIGHT",
        "cynthiaFrontMapSprite",
        cynthia_vals,
        cynthia_out_w,
        cynthia_out_h,
    )

    nurse_src = Path("ImageConvertHelpers/pokemon_center/pokemon_center_nurse.png")
    nurse_out_w = 24
    nurse_out_h = 33
    nurse_header = Path("software/graphics/sprites/pokemonCenter/pokemonCenterNurseSprite.h")
    nurse_source = Path("software/graphics/sprites/pokemonCenter/pokemonCenterNurseSprite.c")
    nurse_vals = image_to_rgb565_values(nurse_src, nurse_out_w, nurse_out_h)
    write_sprite(
        nurse_header,
        nurse_source,
        "POKEMON_CENTER_NURSE_WIDTH",
        "POKEMON_CENTER_NURSE_HEIGHT",
        "pokemonCenterNurseSprite",
        nurse_vals,
        nurse_out_w,
        nurse_out_h,
    )

    print(f"Wrote {cynthia_source} and {cynthia_header} ({cynthia_out_w}x{cynthia_out_h})")
    print(f"Wrote {nurse_source} and {nurse_header} ({nurse_out_w}x{nurse_out_h})")


if __name__ == "__main__":
    main()
