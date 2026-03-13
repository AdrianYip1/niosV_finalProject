from PIL import Image

TILE_SIZE = 16

im = Image.open("anim.gif")
frame = 0
while True:
    try:
        im.seek(frame)
    except EOFError:
        break

    frame_img = im.convert("RGB").resize((TILE_SIZE, TILE_SIZE), Image.NEAREST)
    frame_img.save(f"frame_{frame}.png")
    frame += 1