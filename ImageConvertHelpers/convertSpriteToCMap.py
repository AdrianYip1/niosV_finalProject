from PIL import Image
import os

IMAGE_WIDTH = 320
IMAGE_HEIGHT = 149
TRANSPARENT_KEY = (255, 0, 255)  # FF00FF pink
WHITE_THRESHOLD = 240  # treat near-white as transparent too
OUTPUT_C_FILE = "backdrop1.c"
OUTPUT_H_FILE = "backdrop1.h"
SPRITE_VAR_NAME = "backdrop1"

def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def convert(name, var_name, out_file):
    img = Image.open(name).convert("RGB")
    w, h = img.size
    assert w == IMAGE_WIDTH and h == IMAGE_HEIGHT, f"{name} must be {IMAGE_WIDTH}x{IMAGE_HEIGHT}"

    out_file.write(f"const unsigned short {var_name}[BACKDROP1_WIDTH * BACKDROP1_HEIGHT] = {{\n")
    vals = []
    for y in range(IMAGE_HEIGHT):
        for x in range(IMAGE_WIDTH):
            r, g, b = img.getpixel((x, y))
            if (r, g, b) == TRANSPARENT_KEY or (r > WHITE_THRESHOLD and g > WHITE_THRESHOLD and b > WHITE_THRESHOLD):
                val = 0xF81F  # PINK_TRANSPARENT
            else:
                val = rgb_to_565(r, g, b)
            vals.append(f"0x{val:04X}")

    for i in range(0, len(vals), 16):
        line = ", ".join(vals[i:i+16])
        end = "," if i + 16 < len(vals) else ""
        out_file.write(f"    {line}{end}\n")
    out_file.write("};\n\n")

def write_header(header_name):
    with open(header_name, "w") as h:
        h.write("#pragma once\n\n")
        h.write(f"#define BACKDROP1_WIDTH  {IMAGE_WIDTH}\n")
        h.write(f"#define BACKDROP1_HEIGHT {IMAGE_HEIGHT}\n\n")
        h.write(f"extern const unsigned short backdrop1[BACKDROP1_WIDTH * BACKDROP1_HEIGHT];\n")

if __name__ == "__main__":
    name = "backdrop1.png"
    assert os.path.exists(name), f"{name} not found"

    with open(OUTPUT_C_FILE, "w") as out:
        out.write(f'#include "{OUTPUT_H_FILE}"\n\n')
        convert(name, SPRITE_VAR_NAME, out)

    write_header(OUTPUT_H_FILE)
    print(f"Done! Generated {OUTPUT_C_FILE} and {OUTPUT_H_FILE}")