from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import re

from PIL import Image


SRC_DIR = Path("ImageConvertHelpers/68x56Sprites")
OUT_DIR = Path("software/graphics/sprites/boxSprites")

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F

EXPECTED_W = 68
EXPECTED_H = 56


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def to_c_ident(stem: str) -> str:
    # Keep existing repo style (charizard, charmeleon, ...)
    s = re.sub(r"[^A-Za-z0-9_]", "_", stem)
    s = re.sub(r"_+", "_", s).strip("_")
    if not s:
        raise ValueError("empty identifier")
    if s[0].isdigit():
        s = f"sprite_{s}"
    return s


def camel_to_screaming_snake(name: str) -> str:
    s1 = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    s1 = re.sub(r"[^A-Za-z0-9_]", "_", s1)
    s1 = re.sub(r"_+", "_", s1).strip("_")
    return s1.upper()


def load_rgba(path: Path) -> Image.Image:
    im = Image.open(path)
    try:
        im.seek(0)
    except EOFError:
        pass
    rgba = im.convert("RGBA")
    w, h = rgba.size
    if (w, h) != (EXPECTED_W, EXPECTED_H):
        raise ValueError(f"{path} must be {EXPECTED_W}x{EXPECTED_H}, got {w}x{h}")
    return rgba


def sprite_to_565_vals(img: Image.Image) -> list[int]:
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


@dataclass(frozen=True)
class Sprite:
    base: str
    path: Path


def discover_sprites() -> list[Sprite]:
    if not SRC_DIR.exists():
        raise RuntimeError(f"Missing source dir: {SRC_DIR}")

    sprites: list[Sprite] = []
    for p in sorted(SRC_DIR.iterdir(), key=lambda x: x.name.lower()):
        if not p.is_file():
            continue
        if p.suffix.lower() not in {".png", ".gif"}:
            continue
        base = to_c_ident(p.stem)
        sprites.append(Sprite(base=base, path=p))

    if not sprites:
        raise RuntimeError(f"No sprites found in {SRC_DIR}")
    return sprites


def write_sprite(sprite: Sprite) -> tuple[Path, Path]:
    base = sprite.base
    array_name = f"{base}Box"

    macro = camel_to_screaming_snake(array_name)  # CHARIZARD_BOX
    width_macro = f"{macro}_WIDTH"
    height_macro = f"{macro}_HEIGHT"

    h_path = OUT_DIR / f"{base}BoxSprite.h"
    c_path = OUT_DIR / f"{base}BoxSprite.c"

    img = load_rgba(sprite.path)
    vals = sprite_to_565_vals(img)

    h_lines: list[str] = [
        "#pragma once",
        "",
        f"#define {width_macro}  {EXPECTED_W}",
        f"#define {height_macro} {EXPECTED_H}",
        f"extern const unsigned short {array_name}[{width_macro} * {height_macro}];",
        "",
    ]

    c_lines: list[str] = [f'#include "{h_path.name}"', ""]
    c_lines.append(f"const unsigned short {array_name}[{width_macro} * {height_macro}] = {{")
    for k in range(0, len(vals), 16):
        chunk = vals[k : k + 16]
        chunk_s = ", ".join(f"0x{v:04X}" for v in chunk)
        end = "," if k + 16 < len(vals) else ""
        c_lines.append(f"    {chunk_s}{end}")
    c_lines.append("};")
    c_lines.append("")

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    h_path.write_text("\n".join(h_lines), encoding="utf-8")
    c_path.write_text("\n".join(c_lines), encoding="utf-8")

    return c_path, h_path


def main() -> None:
    sprites = discover_sprites()
    written: list[tuple[Path, Path]] = []
    for sprite in sprites:
        written.append(write_sprite(sprite))

    for c_path, h_path in written:
        print(f"Wrote {c_path} and {h_path}")


if __name__ == "__main__":
    main()

