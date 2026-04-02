from __future__ import annotations

from pathlib import Path
import re

from PIL import Image


SRC_DIR = Path("ImageConvertHelpers/mainMenuUi")
LEVEL_UP_STAT_SCREEN_PNG = Path("ImageConvertHelpers/levelUpStatScreen/statScreen.png")

OUT_DIR = Path("software/graphics/sprites/mainMenuUi")
OUT_C = OUT_DIR / "mainMenuUiSprites.c"
OUT_H = OUT_DIR / "mainMenuUiSprites.h"

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F

# Prefix C identifiers to avoid symbol collisions with other sprite packs.
NAME_PREFIX = "mainMenuUi"


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def camel_to_screaming_snake(name: str) -> str:
    s1 = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    s1 = re.sub(r"[^A-Za-z0-9_]", "_", s1)
    s1 = re.sub(r"_+", "_", s1).strip("_")
    return s1.upper()


def normalize_stem(stem: str) -> str:
    # e.g. "menuBorder(1)" or "menuBorder (1)" -> "menuBorder"
    s = re.sub(r"\s*\(\d+\)\s*$", "", stem).strip()
    s = re.sub(r"\s+", " ", s)
    return s


def to_lower_camel(name: str) -> str:
    parts = [p for p in re.split(r"[^A-Za-z0-9]+", name) if p]
    if not parts:
        raise ValueError(f"cannot build identifier from: {name!r}")
    first = parts[0]
    out = [first[:1].lower() + first[1:]]
    for p in parts[1:]:
        out.append(p[:1].upper() + p[1:])
    ident = "".join(out)
    if ident and ident[0].isdigit():
        ident = "ui" + ident
    return ident


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

    if not LEVEL_UP_STAT_SCREEN_PNG.exists():
        raise RuntimeError(f"Missing required PNG: {LEVEL_UP_STAT_SCREEN_PNG}")

    pngs = sorted(SRC_DIR.glob("*.png"), key=lambda p: p.name.lower())
    pngs.append(LEVEL_UP_STAT_SCREEN_PNG)
    if not pngs:
        raise RuntimeError(f"No .png files found in {SRC_DIR}")

    sprites: list[tuple[str, str, Path]] = []
    used: dict[str, str] = {}
    for p in pngs:
        stem = normalize_stem(p.stem)
        base = to_lower_camel(stem)
        if base in used:
            raise RuntimeError(f"Duplicate sprite name after normalization: {base} from {used[base]} and {p.name}")
        used[base] = p.name
        name = f"{NAME_PREFIX}{base[:1].upper()}{base[1:]}Sprite"
        macro_base = camel_to_screaming_snake(base)
        sprites.append((name, macro_base, p))

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    # Header
    h_lines: list[str] = ["#pragma once", "", "#include <stdint.h>", ""]
    h_lines.append(f"#define MAIN_MENU_UI_SPRITE_COUNT  {len(sprites)}")
    h_lines.append("")
    h_lines.append("typedef enum {")
    for i, (_, macro_base, _) in enumerate(sprites):
        h_lines.append(f"    MAIN_MENU_UI_SPRITE_{macro_base} = {i},")
    h_lines.append("} MainMenuUiSpriteId;")
    h_lines.append("")

    sprite_dims: list[tuple[int, int]] = []
    for name, macro_base, path in sprites:
        _, w, h = load_vals(path)
        sprite_dims.append((w, h))
        w_macro = f"MAIN_MENU_UI_{macro_base}_WIDTH"
        h_macro = f"MAIN_MENU_UI_{macro_base}_HEIGHT"
        h_lines.extend(
            [
                f"#define {w_macro}  {w}",
                f"#define {h_macro} {h}",
                f"extern const unsigned short {name}[{w_macro} * {h_macro}];",
                "",
            ]
        )

    h_lines.extend(
        [
            "extern const unsigned short* const mainMenuUiSprites[MAIN_MENU_UI_SPRITE_COUNT];",
            "extern const uint16_t mainMenuUiSpritesWidths[MAIN_MENU_UI_SPRITE_COUNT];",
            "extern const uint16_t mainMenuUiSpritesHeights[MAIN_MENU_UI_SPRITE_COUNT];",
            "",
        ]
    )

    OUT_H.write_text("\n".join(h_lines), encoding="utf-8")

    # Source
    c_lines: list[str] = [f'#include "{OUT_H.name}"', ""]
    for (name, macro_base, path), (w, h) in zip(sprites, sprite_dims):
        vals, w2, h2 = load_vals(path)
        if (w2, h2) != (w, h):
            raise RuntimeError(f"{path.name}: size changed unexpectedly ({w}x{h} -> {w2}x{h2})")
        emit_array(c_lines, name, f"MAIN_MENU_UI_{macro_base}_WIDTH", f"MAIN_MENU_UI_{macro_base}_HEIGHT", vals)

    c_lines.append("const unsigned short* const mainMenuUiSprites[MAIN_MENU_UI_SPRITE_COUNT] = {")
    for i, (name, _, _) in enumerate(sprites):
        end = "," if i + 1 < len(sprites) else ""
        c_lines.append(f"    {name}{end}")
    c_lines.append("};")
    c_lines.append("")

    c_lines.append("const uint16_t mainMenuUiSpritesWidths[MAIN_MENU_UI_SPRITE_COUNT] = {")
    for i, (w, _) in enumerate(sprite_dims):
        end = "," if i + 1 < len(sprite_dims) else ""
        c_lines.append(f"    {w}{end}")
    c_lines.append("};")
    c_lines.append("")

    c_lines.append("const uint16_t mainMenuUiSpritesHeights[MAIN_MENU_UI_SPRITE_COUNT] = {")
    for i, (_, h) in enumerate(sprite_dims):
        end = "," if i + 1 < len(sprite_dims) else ""
        c_lines.append(f"    {h}{end}")
    c_lines.append("};")
    c_lines.append("")

    OUT_C.write_text("\n".join(c_lines), encoding="utf-8")
    print(f"Wrote {OUT_C} and {OUT_H}")


if __name__ == "__main__":
    main()

