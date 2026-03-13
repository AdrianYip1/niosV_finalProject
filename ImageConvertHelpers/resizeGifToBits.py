from PIL import Image

TILE_SIZE = 48

im = Image.open("out_pink_bg.gif")
frame = 0
while True:
    try:
        im.seek(frame)
    except EOFError:
        break

    frame_img = im.convert("RGB").resize((TILE_SIZE, TILE_SIZE), Image.NEAREST)
    frame_img.save(f"frame_{frame}.png")
    frame += 1