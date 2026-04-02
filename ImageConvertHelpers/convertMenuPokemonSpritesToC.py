from __future__ import annotations

from pathlib import Path
import re

from PIL import Image


SRC_DIR = Path("ImageConvertHelpers/menuPokemonSprites")

OUT_DIR = Path("software/graphics/sprites/menuPokemon")
OUT_C = OUT_DIR / "menuPokemonSprites.c"
OUT_H = OUT_DIR / "menuPokemonSprites.h"

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F

EXPECTED_W = 40
EXPECTED_H = 30


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def camel_to_screaming_snake(name: str) -> str:
    s1 = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    s1 = re.sub(r"[^A-Za-z0-9_]", "_", s1)
    s1 = re.sub(r"_+", "_", s1).strip("_")
    return s1.upper()


def load_vals(path: Path) -> list[str]:
    im = Image.open(path).convert("RGBA")
    w, h = im.size
    if (w, h) != (EXPECTED_W, EXPECTED_H):
        raise RuntimeError(f"{path.name}: expected {EXPECTED_W}x{EXPECTED_H}, got {w}x{h}")

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
    return vals


def emit_array(lines: list[str], name: str, vals: list[str]) -> None:
    lines.append(f"const unsigned short {name}[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT] = {{")
    for k in range(0, len(vals), 16):
        chunk = vals[k : k + 16]
        end = "," if k + 16 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)
    lines.append("};")
    lines.append("")


def main() -> None:
    sprites = [
        ("menuCharmanderSprite", SRC_DIR / "menuCharmander.png"),
        ("menuCharmeleonSprite", SRC_DIR / "menuCharmeleon.png"),
        ("menuCharizardSprite", SRC_DIR / "menuCharizard.png"),
        ("menuRayquazaSprite", SRC_DIR / "menuRayquaza.png"),
        ("menuGibleSprite", SRC_DIR / "gible.png"),
        ("menuGabiteSprite", SRC_DIR / "gabite.png"),
        ("menuGarchompSprite", SRC_DIR / "menuGarchomp.png"),
        ("menuRioluSprite", SRC_DIR / "riolu.png"),
        ("menuLucarioSprite", SRC_DIR / "menuLucario.png"),
        ("menuFeebasSprite", SRC_DIR / "feebas.png"),
        ("menuMiloticSprite", SRC_DIR / "menuMilotic.png"),
        ("menuBudewSprite", SRC_DIR / "budew.png"),
        ("menuRoseradeSprite", SRC_DIR / "menuRoserade.png"),
        ("menuSpiritombSprite", SRC_DIR / "menuSpiritomb.png"),
        ("menuTogepiSprite", SRC_DIR / "togepi.png"),
        ("menuTogeticSprite", SRC_DIR / "togetic.png"),
        ("menuTogekissSprite", SRC_DIR / "menuTogekiss.png"),
    ]

    for _, p in sprites:
        if not p.exists():
            raise RuntimeError(f"Missing sprite: {p}")

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    h_lines: list[str] = [
        "#pragma once",
        "",
        '#include "../pokemon/pokemonSpriteInit.h"',
        "",
        f"#define MENU_POKEMON_SPRITE_WIDTH  {EXPECTED_W}",
        f"#define MENU_POKEMON_SPRITE_HEIGHT {EXPECTED_H}",
        f"#define MENU_POKEMON_SPRITE_COUNT  {len(sprites)}",
        "",
    ]
    for name, _ in sprites:
        h_lines.append(
            f"extern const unsigned short {name}[MENU_POKEMON_SPRITE_WIDTH * MENU_POKEMON_SPRITE_HEIGHT];"
        )
    h_lines.extend(
        [
            "",
            "// Indexed by POKEMON_ID_* (0 is NULL). Size is (max id + 1).",
            "extern const unsigned short* const menuPokemonSpriteById[POKEMON_ID_MAX + 1];",
            "",
            "static inline const unsigned short* menuPokemonSpriteForId(int pokemon_id) {",
            "    return (pokemon_id >= 0 && pokemon_id <= POKEMON_ID_MAX) ? menuPokemonSpriteById[pokemon_id] : 0;",
            "}",
            "",
        ]
    )

    OUT_H.write_text("\n".join(h_lines), encoding="utf-8")

    lines: list[str] = [f'#include "{OUT_H.name}"', ""]
    for name, path in sprites:
        vals = load_vals(path)
        emit_array(lines, name, vals)

    # Mapping table aligned with POKEMON_ID_* in pokemonSpriteInit.h
    mapping = {
        "POKEMON_ID_CHARMANDER": "menuCharmanderSprite",
        "POKEMON_ID_CHARMELEON": "menuCharmeleonSprite",
        "POKEMON_ID_CHARIZARD": "menuCharizardSprite",
        "POKEMON_ID_RAYQUAZA": "menuRayquazaSprite",
        "POKEMON_ID_GIBLE": "menuGibleSprite",
        "POKEMON_ID_GABITE": "menuGabiteSprite",
        "POKEMON_ID_GARCHOMP": "menuGarchompSprite",
        "POKEMON_ID_RIOLU": "menuRioluSprite",
        "POKEMON_ID_LUCARIO": "menuLucarioSprite",
        "POKEMON_ID_FEEBAS": "menuFeebasSprite",
        "POKEMON_ID_MILOTIC": "menuMiloticSprite",
        "POKEMON_ID_BUDEW": "menuBudewSprite",
        "POKEMON_ID_ROSERADE": "menuRoseradeSprite",
        "POKEMON_ID_SPIRITOMB": "menuSpiritombSprite",
        "POKEMON_ID_TOGEPI": "menuTogepiSprite",
        "POKEMON_ID_TOGETIC": "menuTogeticSprite",
        "POKEMON_ID_TOGEKISS": "menuTogekissSprite",
    }

    lines.append("const unsigned short* const menuPokemonSpriteById[POKEMON_ID_MAX + 1] = {")
    lines.append("    [0] = 0,")
    for k in [
        "POKEMON_ID_CHARMANDER",
        "POKEMON_ID_CHARMELEON",
        "POKEMON_ID_CHARIZARD",
        "POKEMON_ID_RAYQUAZA",
        "POKEMON_ID_GARCHOMP",
        "POKEMON_ID_LUCARIO",
        "POKEMON_ID_MILOTIC",
        "POKEMON_ID_ROSERADE",
        "POKEMON_ID_SPIRITOMB",
        "POKEMON_ID_TOGEKISS",
        "POKEMON_ID_BUDEW",
        "POKEMON_ID_FEEBAS",
        "POKEMON_ID_GIBLE",
        "POKEMON_ID_GABITE",
        "POKEMON_ID_RIOLU",
        "POKEMON_ID_TOGEPI",
        "POKEMON_ID_TOGETIC",
    ]:
        lines.append(f"    [{k}] = {mapping[k]},")
    lines.append("};")
    lines.append("")

    OUT_C.write_text("\n".join(lines), encoding="utf-8")
    print(f"Wrote {OUT_C} and {OUT_H}")


if __name__ == "__main__":
    main()
