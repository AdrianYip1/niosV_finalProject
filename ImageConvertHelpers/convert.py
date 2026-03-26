from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import re

from PIL import Image


SRC_DIR = Path("C:\ecefinal\niosV_finalProject\ImageConvertHelpers\cynthia192x192\cynthiaFront.png")
OUT_DIR = Path("software/graphics/sprites/cynthiaFront")

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F

EXPECTED_W = 192
EXPECTED_H = 192


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def to_c_ident(stem: str) -> str:
    # Keep existing repo style (charizardBack, charmeleonFront, ...)
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


def load_rgba_96x96(path: Path) -> Image.Image:
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
class Pair:
    base: str  # e.g. "charizard"
    back: Path
    front: Path


def discover_pairs() -> list[Pair]:
    if not SRC_DIR.exists():
        raise RuntimeError(f"Missing source dir: {SRC_DIR}")

    backs: dict[str, Path] = {}
    fronts: dict[str, Path] = {}

    for p in sorted(SRC_DIR.iterdir(), key=lambda x: x.name.lower()):
        if not p.is_file():
            continue
        if p.suffix.lower() not in {".png", ".gif"}:
            continue

        stem = p.stem
        if stem.lower().endswith("back"):
            base = stem[: -len("back")]
            backs[base.lower()] = p
        elif stem.lower().endswith("front"):
            base = stem[: -len("front")]
            fronts[base.lower()] = p

    bases = sorted(set(backs.keys()) & set(fronts.keys()))
    if not bases:
        raise RuntimeError(f"No <name>Back/<name>Front pairs found in {SRC_DIR}")

    pairs: list[Pair] = []
    for base_lower in bases:
        pairs.append(Pair(base=base_lower, back=backs[base_lower], front=fronts[base_lower]))
    return pairs


def write_pair(pair: Pair) -> tuple[Path, Path]:
    base = to_c_ident(pair.base)

    # Keep the existing repo naming convention: charizardSprite.c/h, charizardBack, CHARIZARD_BACK_WIDTH, ...
    h_path = OUT_DIR / f"{base}Sprite.h"
    c_path = OUT_DIR / f"{base}Sprite.c"

    back_name = f"{base}Back"
    front_name = f"{base}Front"

    back_macro = camel_to_screaming_snake(back_name)
    front_macro = camel_to_screaming_snake(front_name)

    back_img = load_rgba_96x96(pair.back)
    front_img = load_rgba_96x96(pair.front)

    back_vals = sprite_to_565_vals(back_img)
    front_vals = sprite_to_565_vals(front_img)

    # Header
    h_lines: list[str] = [
        "#pragma once",
        "",
        f"#define {back_macro}_WIDTH  {EXPECTED_W}",
        f"#define {back_macro}_HEIGHT {EXPECTED_H}",
        f"extern const unsigned short {back_name}[{back_macro}_WIDTH * {back_macro}_HEIGHT];",
        "",
        f"#define {front_macro}_WIDTH  {EXPECTED_W}",
        f"#define {front_macro}_HEIGHT {EXPECTED_H}",
        f"extern const unsigned short {front_name}[{front_macro}_WIDTH * {front_macro}_HEIGHT];",
        "",
    ]

    # Source
    c_lines: list[str] = [f'#include "{h_path.name}"', ""]

    def emit_array(name: str, macro: str, vals: list[int]) -> None:
        c_lines.append(f"const unsigned short {name}[{macro}_WIDTH * {macro}_HEIGHT] = {{")
        for k in range(0, len(vals), 16):
            chunk = vals[k : k + 16]
            chunk_s = ", ".join(f"0x{v:04X}" for v in chunk)
            end = "," if k + 16 < len(vals) else ""
            c_lines.append(f"    {chunk_s}{end}")
        c_lines.append("};")
        c_lines.append("")

    emit_array(back_name, back_macro, back_vals)
    emit_array(front_name, front_macro, front_vals)

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    h_path.write_text("\n".join(h_lines), encoding="utf-8")
    c_path.write_text("\n".join(c_lines), encoding="utf-8")

    return c_path, h_path


def main() -> None:
    pairs = discover_pairs()
    written: list[tuple[Path, Path]] = []
    for pair in pairs:
        written.append(write_pair(pair))

    for c_path, h_path in written:
        print(f"Wrote {c_path} and {h_path}")


if __name__ == "__main__":
    main()
