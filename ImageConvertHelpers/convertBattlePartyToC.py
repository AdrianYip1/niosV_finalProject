from __future__ import annotations

from pathlib import Path

from PIL import Image


SRC_DIR = Path("ImageConvertHelpers/160x91")
SLOT_PNGS = [
    SRC_DIR / "slot1.png",
    SRC_DIR / "slot2.png",
    SRC_DIR / "slot3.png",
    SRC_DIR / "slot4.png",
    SRC_DIR / "slot5.png",
    SRC_DIR / "slot6.png",
]
OUT_DIR = Path("software/graphics/sprites/battleParty")
OUT_C = OUT_DIR / "battlePartySprite.c"
OUT_H = OUT_DIR / "battlePartySprite.h"

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F

EXPECTED_W = 160
EXPECTED_H = 91


def is_magentaish(r: int, g: int, b: int) -> bool:
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def png_to_vals(path: Path) -> tuple[int, int, list[str]]:
    if not path.exists():
        raise RuntimeError(f"Missing input: {path}")

    im = Image.open(path).convert("RGBA")
    w, h = im.size
    if (w, h) == (EXPECTED_W * 2, EXPECTED_H * 2):
        # Some exported assets are 2x scaled. Downsample with nearest-neighbor for pixel art.
        im = im.resize((EXPECTED_W, EXPECTED_H), resample=Image.NEAREST)
        w, h = im.size
    if (w, h) != (EXPECTED_W, EXPECTED_H):
        raise RuntimeError(f"{path} must be {EXPECTED_W}x{EXPECTED_H} (or 2x), got {w}x{h}")

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
    return w, h, vals


def main() -> None:
    slot_data: list[tuple[str, int, int, list[str]]] = []
    for i, path in enumerate(SLOT_PNGS, start=1):
        w, h, vals = png_to_vals(path)
        slot_data.append((f"battlePartySlot{i}Sprite", w, h, vals))

    w = slot_data[0][1]
    h = slot_data[0][2]

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    OUT_H.write_text(
        "\n".join(
            [
                "#pragma once",
                "",
                f"#define BATTLE_PARTY_WIDTH  {w}",
                f"#define BATTLE_PARTY_HEIGHT {h}",
                "",
                "extern const unsigned short battlePartySlot1Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];",
                "extern const unsigned short battlePartySlot2Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];",
                "extern const unsigned short battlePartySlot3Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];",
                "extern const unsigned short battlePartySlot4Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];",
                "extern const unsigned short battlePartySlot5Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];",
                "extern const unsigned short battlePartySlot6Sprite[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT];",
                "",
                "extern const unsigned short* const battlePartySlotSprites[6];",
                "",
                "// Backwards-compatible name (slot1).",
                "#define battlePartySprite battlePartySlot1Sprite",
                "",
            ]
        ),
        encoding="utf-8",
    )

    lines: list[str] = [f'#include "{OUT_H.name}"', ""]
    for name, _w, _h, vals in slot_data:
        lines.append(f"const unsigned short {name}[BATTLE_PARTY_WIDTH * BATTLE_PARTY_HEIGHT] = {{")
        for k in range(0, len(vals), 16):
            chunk = vals[k : k + 16]
            end = "," if k + 16 < len(vals) else ""
            lines.append("    " + ", ".join(chunk) + end)
        lines.append("};")
        lines.append("")

    lines.append("const unsigned short* const battlePartySlotSprites[6] = {")
    for i in range(1, 7):
        comma = "," if i < 6 else ""
        lines.append(f"    battlePartySlot{i}Sprite{comma}")
    lines.append("};")
    lines.append("")

    OUT_C.write_text("\n".join(lines), encoding="utf-8")
    print(f"Wrote {OUT_C} and {OUT_H} ({w}x{h}) with 6 slot variants")


if __name__ == "__main__":
    main()
