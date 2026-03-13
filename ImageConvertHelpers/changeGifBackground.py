from PIL import Image

OLD_BG = (0, 0, 0)       # current background (RGB)
NEW_BG = (255, 0, 255)   # new background (transparent)

im = Image.open("anim.gif")
frames = []

frame_idx = 0
while True:
    try:
        im.seek(frame_idx)
    except EOFError:
        break

    frame = im.convert("RGB")
    pixels = frame.load()
    w, h = frame.size
    for y in range(h):
        for x in range(w):
            if pixels[x, y] == OLD_BG:
                pixels[x, y] = NEW_BG
    frames.append(frame)
    frame_idx += 1

frames[0].save(
    "anim_bg_changed.gif",
    save_all=True,
    append_images=frames[1:],
    loop=0,
    duration=im.info.get("duration", 100)
)