from __future__ import annotations

from pathlib import Path

from PIL import Image


SRC_GIF = Path("ImageConvertHelpers/spacebar.gif")
OUT_C = Path("ImageConvertHelpers/spacebar_frames.c")
OUT_H = Path("ImageConvertHelpers/spacebar_frames.h")

TRANSPARENT_PINK_RGB = (255, 0, 255)  # #FF00FF
TRANSPARENT_565 = 0xF81F

PREFIX = "spacebar_frame_"
FRAMES_ARRAY_NAME = "spacebarFrames"


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def main() -> None:
    im = Image.open(SRC_GIF)
    w, h = im.size

    # Count frames
    frame_count = 0
    try:
        while True:
            im.seek(frame_count)
            frame_count += 1
    except EOFError:
        pass

    if frame_count <= 0:
        raise RuntimeError("GIF has no frames?")

    # Write header
    OUT_H.write_text(
        "\n".join(
            [
                "#pragma once",
                "",
                f"#define SPACEBAR_WIDTH  {w}",
                f"#define SPACEBAR_HEIGHT {h}",
                f"#define SPACEBAR_FRAME_COUNT {frame_count}",
                "",
                *[
                    f"extern const unsigned short {PREFIX}{i}[SPACEBAR_WIDTH * SPACEBAR_HEIGHT];"
                    for i in range(frame_count)
                ],
                "",
                f"extern const unsigned short* const {FRAMES_ARRAY_NAME}[SPACEBAR_FRAME_COUNT];",
                "",
            ]
        ),
        encoding="utf-8",
    )

    # Write C
    lines: list[str] = []
    lines.append(f'#include "{OUT_H.name}"')
    lines.append("")

    for i in range(frame_count):
        im.seek(i)
        frame = im.convert("RGBA")
        px = frame.load()

        vals: list[str] = []
        for y in range(h):
            for x in range(w):
                r, g, b, a = px[x, y]
                if a == 0 or (r, g, b) == TRANSPARENT_PINK_RGB:
                    v = TRANSPARENT_565
                else:
                    v = rgb_to_565(r, g, b)
                vals.append(f"0x{v:04X}")

        lines.append(f"const unsigned short {PREFIX}{i}[SPACEBAR_WIDTH * SPACEBAR_HEIGHT] = {{")
        for k in range(0, len(vals), 16):
            chunk = vals[k : k + 16]
            end = "," if k + 16 < len(vals) else ""
            lines.append("    " + ", ".join(chunk) + end)
        lines.append("};")
        lines.append("")

    lines.append(f"const unsigned short* const {FRAMES_ARRAY_NAME}[SPACEBAR_FRAME_COUNT] = {{")
    for i in range(frame_count):
        end = "," if i + 1 < frame_count else ""
        lines.append(f"    {PREFIX}{i}{end}")
    lines.append("};")
    lines.append("")

    OUT_C.write_text("\n".join(lines), encoding="utf-8")
    print(f"Wrote {OUT_C} and {OUT_H} ({w}x{h}, frames={frame_count})")


if __name__ == "__main__":
    main()

