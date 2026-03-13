from PIL import Image

TILE_SIZE = 16
TRANSPARENT_KEY = (255, 0, 255) #FF00FF is transparent colour (pink)

def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def convert(name, var_name):
    img = Image.open(name).convert("RGB")
    w, h = img.size
    assert w == TILE_SIZE and h == TILE_SIZE, f"{name} must be 16x16"

    print(f"short {var_name}[TILE_SIZE * TILE_SIZE] = {{")
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
        print(f"    {line}{end}")
    print("};\n")

if __name__ == "__main__":
    convert("player_up_1.png", "player_up_1_sprite")
    convert("player_up_2.png", "player_up_2_sprite")