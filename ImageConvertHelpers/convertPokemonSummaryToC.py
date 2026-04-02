from __future__ import annotations

from pathlib import Path
import re

from PIL import Image


SRC_DIR = Path("ImageConvertHelpers/pokemonSummary")

OUT_DIR = Path("software/graphics/sprites/pokemonSummary")
OUT_C = OUT_DIR / "pokemonSummarySprites.c"
OUT_H = OUT_DIR / "pokemonSummarySprites.h"

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F


def normalize_stem(stem: str) -> str:
    # e.g. "summary1(1)" or "effect (2)" -> "summary1" / "effect"
    s = re.sub(r"\s*\(\d+\)\s*$", "", stem).strip()
    return s


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def camelize(s: str) -> str:
    parts = re.split(r"[^A-Za-z0-9]+", s)
    parts = [p for p in parts if p]
    if not parts:
        raise ValueError("empty name")
    return "".join(p[:1].upper() + p[1:] for p in parts)


def camel_to_screaming_snake(name: str) -> str:
    s1 = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    s1 = re.sub(r"[^A-Za-z0-9_]", "_", s1)
    s1 = re.sub(r"_+", "_", s1).strip("_")
    return s1.upper()


def load_vals(path: Path) -> tuple[list[str], int, int]:
    im = Image.open(path).convert("RGBA")
    w, h = im.size
    px = im.load()

    vals: list[str] = []
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0 or (r, g, b) == TRANSPARENT_PINK_RGB or is_magentaish(r, g, b):
                v = TRANSPARENT_565
            else:
                v = rgb_to_565(r, g, b)
            vals.append(f"0x{v:04X}")
    return vals, w, h


def emit_array(lines: list[str], name: str, width_macro: str, height_macro: str, vals: list[str]) -> None:
    lines.append(f"const unsigned short {name}[{width_macro} * {height_macro}] = {{")
    for k in range(0, len(vals), 16):
        chunk = vals[k : k + 16]
        end = "," if k + 16 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)
    lines.append("};")
    lines.append("")


def main() -> None:
    if not SRC_DIR.exists():
        raise RuntimeError(f"Missing source dir: {SRC_DIR}")

    pngs = sorted(SRC_DIR.glob("*.png"), key=lambda p: p.name.lower())
    if not pngs:
        raise RuntimeError(f"No .png files found in {SRC_DIR}")

    sprites: list[tuple[str, Path]] = []
    used: dict[str, str] = {}
    for p in pngs:
        stem = normalize_stem(p.stem)
        prev = used.get(stem)
        if prev is not None:
            raise RuntimeError(f"Duplicate sprite name after normalization: {stem} from {prev} and {p.name}")
        used[stem] = p.name

        base = camelize(stem)
        symbol = f"pokemonSummary{base}Sprite"
        sprites.append((symbol, p))

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    # Header
    h_lines: list[str] = ["#pragma once", ""]
    h_lines.append(f"#define POKEMON_SUMMARY_SPRITE_COUNT  {len(sprites)}")
    h_lines.append("")

    dims: list[tuple[str, int, int, str, str]] = []
    for symbol, path in sprites:
        _, w, h = load_vals(path)
        macro_base = camel_to_screaming_snake(symbol[: -len("Sprite")] if symbol.endswith("Sprite") else symbol)
        w_macro = f"{macro_base}_WIDTH"
        h_macro = f"{macro_base}_HEIGHT"
        dims.append((symbol, w, h, w_macro, h_macro))
        h_lines.extend(
            [
                f"#define {w_macro}  {w}",
                f"#define {h_macro} {h}",
                f"extern const unsigned short {symbol}[{w_macro} * {h_macro}];",
                "",
            ]
        )

    OUT_H.write_text("\n".join(h_lines), encoding="utf-8")

    # Source
    c_lines: list[str] = [f'#include "{OUT_H.name}"', ""]
    for symbol, path in sprites:
        vals, w, h = load_vals(path)
        macro_base = camel_to_screaming_snake(symbol[: -len("Sprite")] if symbol.endswith("Sprite") else symbol)
        w_macro = f"{macro_base}_WIDTH"
        h_macro = f"{macro_base}_HEIGHT"
        if not any(d[0] == symbol and d[1] == w and d[2] == h for d in dims):
            raise RuntimeError(f"Dimension mismatch for {symbol}")
        emit_array(c_lines, symbol, w_macro, h_macro, vals)

    OUT_C.write_text("\n".join(c_lines), encoding="utf-8")

    print(f"Wrote {OUT_C} and {OUT_H} with {len(sprites)} sprites")


if __name__ == "__main__":
    main()

