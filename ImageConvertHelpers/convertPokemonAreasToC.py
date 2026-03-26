from __future__ import annotations

from pathlib import Path

from PIL import Image


SRC_DIR = Path("ImageConvertHelpers/pokemonAreas")
OUT_DIR = Path("software/graphics/sprites/pokemonAreas")

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def load_rgba(path: Path) -> Image.Image:
    im = Image.open(path)
    try:
        im.seek(0)
    except EOFError:
        pass
    return im.convert("RGBA")


def image_to_565_vals(img: Image.Image) -> list[int]:
    w, h = img.size
    px = img.load()
    vals: list[int] = []
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0 or (r, g, b) == TRANSPARENT_PINK_RGB:
                vals.append(TRANSPARENT_565)
            else:
                vals.append(rgb_to_565(r, g, b))
    return vals


def write_sprite(
    *,
    name: str,
    macro_prefix: str,
    png_path: Path,
) -> None:
    img = load_rgba(png_path)
    w, h = img.size
    vals = image_to_565_vals(img)

    h_path = OUT_DIR / f"{name}.h"
    c_path = OUT_DIR / f"{name}.c"

    h_text = "\n".join(
        [
            "#pragma once",
            "",
            f"#define {macro_prefix}_WIDTH  {w}",
            f"#define {macro_prefix}_HEIGHT {h}",
            "",
            f"extern const unsigned short {name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT];",
            "",
        ]
    )

    c_lines: list[str] = [f'#include "{h_path.name}"', ""]
    c_lines.append(f"const unsigned short {name}[{macro_prefix}_WIDTH * {macro_prefix}_HEIGHT] = {{")
    for k in range(0, len(vals), 16):
        chunk = vals[k : k + 16]
        chunk_s = ", ".join(f"0x{v:04X}" for v in chunk)
        end = "," if k + 16 < len(vals) else ""
        c_lines.append(f"    {chunk_s}{end}")
    c_lines.append("};")
    c_lines.append("")

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    h_path.write_text(h_text, encoding="utf-8")
    c_path.write_text("\n".join(c_lines), encoding="utf-8")
    print(f"Wrote {c_path} and {h_path} ({w}x{h})")


def main() -> None:
    if not SRC_DIR.exists():
        raise RuntimeError(f"Missing source dir: {SRC_DIR}")

    write_sprite(
        name="pokemonAreaBack",
        macro_prefix="POKEMON_AREA_BACK",
        png_path=SRC_DIR / "bgback.png",
    )
    write_sprite(
        name="pokemonAreaFront",
        macro_prefix="POKEMON_AREA_FRONT",
        png_path=SRC_DIR / "bgfront.png",
    )


if __name__ == "__main__":
    main()

