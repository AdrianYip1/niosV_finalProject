from PIL import Image

TILE_SIZE = 320
TRANSPARENT_KEY = (255, 0, 255)  # FF00FF pink
OUTPUT_C_FILE = "mcIdle_frames.c"
VAR_NAME = "mcIdle"

def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def convert(name, var_name, out_file):
    img = Image.open(name).convert("RGB")
    w, h = img.size
    assert w == TILE_SIZE and h == TILE_SIZE, f"{name} must be {TILE_SIZE}x{TILE_SIZE}"

    out_file.write(f"#define TILE_SIZE {TILE_SIZE}\n\n")
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
    out_file.write("};\n")

if __name__ == "__main__":
    import sys
    if len(sys.argv) >= 4:

        name, var_name, c_file = sys.argv[1], sys.argv[2], sys.argv[3]
        h_file = sys.argv[4] if len(sys.argv) > 4 else c_file.replace(".c", ".h")
        tile_define = sys.argv[5] if len(sys.argv) > 5 else "TILE_SIZE"
        with open(c_file, "w") as out:
            out.write(f'#include "{h_file}"\n\n')
            out.write(f"#define TILE_SIZE {tile_define}\n\n")
            convert(name, var_name, out)
        print(f"Wrote {c_file}")
    else:
        with open(OUTPUT_C_FILE, "w") as out:
            out.write(f"#define TILE_SIZE {TILE_SIZE}\n\n")
            convert("zardTalk.png", VAR_NAME, out)
        print(f"Wrote {OUTPUT_C_FILE}")
