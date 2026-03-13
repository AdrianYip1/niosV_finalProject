from PIL import Image
import os

TILE_SIZE = 48  # change this to the size of the sprite whenever you switch
# sprites will be 48x48 to preserve detail and tiles are 16x16
TRANSPARENT_KEY = (255, 0, 255)  # FF00FF is transparent colour (pink)
OUTPUT_C_FILE = "charizard_frames.c"
OUTPUT_H_FILE = "charizard_frames.h"
SPRITE_PREFIX = "charizard_frame_"


def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def convert(name, var_name, out_file):
    img = Image.open(name).convert("RGB")
    w, h = img.size
    assert w == TILE_SIZE and h == TILE_SIZE, f"{name} must be {TILE_SIZE}x{TILE_SIZE}"

    out_file.write(f"const unsigned short {var_name}[TILE_SIZE * TILE_SIZE] = {{\n")
    vals = []
    for y in range(TILE_SIZE):
        for x in range(TILE_SIZE):
            r, g, b = img.getpixel((x, y))
            if (r, g, b) == TRANSPARENT_KEY:
                val = 0xF81F  # PINK_TRANSPARENT
            else:
                val = rgb_to_565(r, g, b)
            vals.append(f"0x{val:04X}")

    for i in range(0, len(vals), 16):
        line = ", ".join(vals[i:i+16])
        end = "," if i + 16 < len(vals) else ""
        out_file.write(f"    {line}{end}\n")
    out_file.write("};\n\n")


def write_header(header_name, frame_count): #automatically writes the header file for the sprite
    with open(header_name, "w") as h:
        h.write("#pragma once\n\n")
        h.write(f"#define TILE_SIZE {TILE_SIZE}\n")
        h.write(f"#define CHARIZARD_FRAME_COUNT {frame_count}\n\n")

        for i in range(frame_count):
            h.write(
                f"extern const unsigned short charizard_frame_{i}[TILE_SIZE * TILE_SIZE];\n"
            )

        h.write(
            "\nextern const unsigned short* const charizardFrames[CHARIZARD_FRAME_COUNT];\n"
        )


if __name__ == "__main__":  # loop through every single frame_#.png file, deletes the pngs and makes the h adn c files
    frame_count = 0
    with open(OUTPUT_C_FILE, "w") as out:
        i = 0
        while True:
            name = f"frame_{i}.png"
            if not os.path.exists(name):
                break
            convert(name, f"{SPRITE_PREFIX}{i}", out)
            os.remove(name)
            frame_count += 1
            i += 1

        if frame_count > 0:
            out.write(
                f"const unsigned short* const charizardFrames[CHARIZARD_FRAME_COUNT] = {{\n"
            )
            for i in range(frame_count):
                end = "," if i + 1 < frame_count else ""
                out.write(f"    {SPRITE_PREFIX}{i}{end}\n")
            out.write("};\n")

    write_header(OUTPUT_H_FILE, frame_count)