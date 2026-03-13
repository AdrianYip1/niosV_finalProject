from PIL import Image

PINK = (255, 0, 255, 255)  # transparent colour (pink)

im = Image.open("in.gif")
frames = []
frame = 0

while True:
    try:
        im.seek(frame)
    except EOFError:
        break

    f = im.convert("RGBA")
    pixels = f.load()
    w, h = f.size

    for y in range(h):
        for x in range(w):
            r, g, b, a = pixels[x, y]
            if a == 0:           # transparent pixel
                pixels[x, y] = PINK


    frames.append(f)
    frame += 1

# save back to a new GIF
frames[0].save(
    "out_pink_bg.gif",
    save_all=True,
    append_images=frames[1:],
    loop=0,
    duration=im.info.get("duration", 100),
    transparency=None,
    disposal=2,
)