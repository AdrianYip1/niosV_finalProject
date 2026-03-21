from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from PIL import Image


TRANSPARENT_565 = 0xF81F  # project transparent key (pink)

# Fight icon sizing rules:
# - run/bag are cropped to non-pink bbox then padded (top-left) to a shared size
# - fight is vertically stretched (by duplicating interior rows) to be ~2x the small icon height + a little extra padding
FIGHT_EXTRA_PAD_Y = 8
FIGHT_MAX_HEIGHT = 90


def is_magentaish(r: int, g: int, b: int, a: int) -> bool:
    if a == 0:
        return True
    # Exact #FF00FF and also near-magenta from conversion artifacts
    return r >= 200 and b >= 200 and g <= 100


def rgb_to_565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


@dataclass(frozen=True)
class Sprite:
    name: str
    var_name: str
    img_path: Path
    group: str


def find_non_magenta_bbox(im: Image.Image) -> tuple[int, int, int, int]:
    rgba = im.convert("RGBA")
    w, h = rgba.size
    px = rgba.load()

    left = w
    top = h
    right = -1
    bottom = -1
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if is_magentaish(r, g, b, a):
                continue
            if x < left:
                left = x
            if y < top:
                top = y
            if x > right:
                right = x
            if y > bottom:
                bottom = y

    if right < left or bottom < top:
        return (0, 0, 1, 1)
    return (left, top, right + 1, bottom + 1)


def crop_and_pad_top_left(im: Image.Image, target_w: int, target_h: int) -> Image.Image:
    bbox = find_non_magenta_bbox(im)
    cropped = im.convert("RGBA").crop(bbox)

    out = Image.new("RGBA", (target_w, target_h), (255, 0, 255, 255))
    out.paste(cropped, (0, 0))
    return out


def find_text_rows(im: Image.Image) -> tuple[int, int] | None:
    """
    Heuristic: rows containing bright/near-white pixels (the label text).
    Returns (top, bottom) inclusive, or None if not found.
    """
    rgba = im.convert("RGBA")
    w, h = rgba.size
    px = rgba.load()

    bright_counts: list[int] = [0] * h
    for y in range(h):
        bright = 0
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0:
                continue
            if is_magentaish(r, g, b, a):
                continue
            if r >= 230 and g >= 230 and b >= 230:
                bright += 1
        bright_counts[y] = bright

    peak = max(bright_counts) if bright_counts else 0
    if peak < 3:
        return None

    y_peak = max(range(h), key=lambda y: bright_counts[y])
    # Expand around the peak to cover the text band.
    threshold = max(2, int(peak * 0.30))
    top = y_peak
    bottom = y_peak
    while top > 0 and bright_counts[top - 1] >= threshold:
        top -= 1
    while bottom < h - 1 and bright_counts[bottom + 1] >= threshold:
        bottom += 1
    return (top, bottom)


def stretch_fight_button_to_height(im: Image.Image, target_h: int) -> Image.Image:
    """
    Stretch the fight button vertically by inserting duplicated rows in the
    interior (above and below the label text), preserving borders and text.
    """
    rgba = im.convert("RGBA")
    w, h = rgba.size
    if target_h <= h:
        return rgba

    delta = target_h - h

    text_band = find_text_rows(rgba)
    if text_band is None:
        # Fallback: treat the center as the "text" band.
        text_top = max(1, h // 2 - 1)
        text_bottom = min(h - 2, h // 2 + 1)
    else:
        text_top, text_bottom = text_band
        text_top = max(1, text_top)
        text_bottom = min(h - 2, text_bottom)
        if text_bottom < text_top:
            text_top = max(1, h // 2 - 1)
            text_bottom = min(h - 2, h // 2 + 1)

    insert_above_count = delta // 2
    insert_below_count = delta - insert_above_count

    px = rgba.load()

    def row_stats(y: int) -> tuple[int, int, int, int]:
        magenta = 0
        bright = 0
        lum_sum = 0
        lum_sq_sum = 0
        non_magenta = 0
        for x in range(w):
            r, g, b, a = px[x, y]
            if a == 0 or is_magentaish(r, g, b, a):
                magenta += 1
                continue
            # Cheap luminance approximation.
            lum = r * 3 + g * 6 + b
            lum_sum += lum
            lum_sq_sum += lum * lum
            non_magenta += 1
            if r >= 230 and g >= 230 and b >= 230:
                bright += 1
        if non_magenta <= 1:
            mean_i = 0
            variance = 1_000_000_000
        else:
            mean = lum_sum / non_magenta
            mean_i = int(mean)
            variance = int((lum_sq_sum / non_magenta) - (mean * mean))
        return magenta, bright, variance, mean_i

    def pick_seam_row(y_start: int, y_end_inclusive: int) -> int:
        min_y = 6 if h > 14 else 2
        max_y = (h - 7) if h > 14 else (h - 3)

        candidates: list[tuple[int, int, int, int, int]] = []
        for y in range(max(min_y, y_start), min(max_y, y_end_inclusive) + 1):
            magenta, bright, variance, mean_i = row_stats(y)
            if y <= 0 or y >= h - 1:
                neighbor_diff = 1_000_000_000
            else:
                _, _, _, mean_prev = row_stats(y - 1)
                _, _, _, mean_next = row_stats(y + 1)
                neighbor_diff = abs(mean_i - mean_prev) + abs(mean_i - mean_next)
            candidates.append((magenta, neighbor_diff, variance, bright, y))
        if not candidates:
            # Broad fallback: any non-border row outside the text band.
            for y in range(min_y, max_y + 1):
                if text_top <= y <= text_bottom:
                    continue
                magenta, bright, variance, mean_i = row_stats(y)
                if y <= 0 or y >= h - 1:
                    neighbor_diff = 1_000_000_000
                else:
                    _, _, _, mean_prev = row_stats(y - 1)
                    _, _, _, mean_next = row_stats(y + 1)
                    neighbor_diff = abs(mean_i - mean_prev) + abs(mean_i - mean_next)
                candidates.append((magenta, neighbor_diff, variance, bright, y))
        if not candidates:
            # Last resort: any non-border row.
            for y in range(min_y, max_y + 1):
                magenta, bright, variance, mean_i = row_stats(y)
                if y <= 0 or y >= h - 1:
                    neighbor_diff = 1_000_000_000
                else:
                    _, _, _, mean_prev = row_stats(y - 1)
                    _, _, _, mean_next = row_stats(y + 1)
                    neighbor_diff = abs(mean_i - mean_prev) + abs(mean_i - mean_next)
                candidates.append((magenta, neighbor_diff, variance, bright, y))
        candidates.sort()
        return candidates[0][4]

    seam_above_y = pick_seam_row(text_top - 10, text_top - 1)
    seam_below_y = pick_seam_row(text_bottom + 1, text_bottom + 10)

    def band_rows_around(y_center: int, radius: int = 1) -> list[Image.Image]:
        rows: list[Image.Image] = []
        for yy in range(max(1, y_center - radius), min(h - 2, y_center + radius) + 1):
            rows.append(rgba.crop((0, yy, w, yy + 1)))
        if not rows:
            rows.append(rgba.crop((0, y_center, w, y_center + 1)))
        return rows

    above_band_rows = band_rows_around(seam_above_y, radius=1)
    below_band_rows = band_rows_around(seam_below_y, radius=1)

    # Split into upper/text/lower sections; insert repeated "seam" rows.
    upper = rgba.crop((0, 0, w, text_top))
    text = rgba.crop((0, text_top, w, text_bottom + 1))
    lower = rgba.crop((0, text_bottom + 1, w, h))

    out = Image.new("RGBA", (w, target_h), (255, 0, 255, 255))
    y = 0
    out.paste(upper, (0, y))
    y += upper.size[1]

    for i in range(insert_above_count):
        out.paste(above_band_rows[i % len(above_band_rows)], (0, y))
        y += 1

    out.paste(text, (0, y))
    y += text.size[1]

    for i in range(insert_below_count):
        out.paste(below_band_rows[i % len(below_band_rows)], (0, y))
        y += 1

    out.paste(lower, (0, y))
    y += lower.size[1]

    # Safety: if rounding/heuristics created off-by-one, pad/truncate.
    if y < target_h:
        pad = Image.new("RGBA", (w, target_h - y), (255, 0, 255, 255))
        out.paste(pad, (0, y))
    elif y > target_h:
        out = out.crop((0, 0, w, target_h))

    return out


def image_to_565_vals(im: Image.Image) -> list[str]:
    rgba = im.convert("RGBA")
    w, h = rgba.size
    px = rgba.load()

    vals: list[str] = []
    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]
            if is_magentaish(r, g, b, a):
                v = TRANSPARENT_565
            else:
                v = rgb_to_565(r, g, b)
            vals.append(f"0x{v:04X}")
    return vals


def emit_array(
    lines: list[str],
    c_type: str,
    var_name: str,
    w_macro: str,
    h_macro: str,
    vals: list[str],
) -> None:
    lines.append(f"const {c_type} {var_name}[{w_macro} * {h_macro}] = {{")
    for i in range(0, len(vals), 16):
        chunk = vals[i : i + 16]
        end = "," if i + 16 < len(vals) else ""
        lines.append("    " + ", ".join(chunk) + end)
    lines.append("};")
    lines.append("")


def main() -> None:
    repo_root = Path(__file__).resolve().parents[1]

    sprites = [
        Sprite(
            name="run",
            var_name="battleIconRun",
            img_path=repo_root / "ImageConvertHelpers" / "60x41Run.png",
            group="small",
        ),
        Sprite(
            name="bag",
            var_name="battleIconBag",
            img_path=repo_root / "ImageConvertHelpers" / "60x41Bag.png",
            group="small",
        ),
        Sprite(
            name="fight",
            var_name="battleIconFight",
            img_path=repo_root / "ImageConvertHelpers" / "110x58Fight.png",
            group="fight",
        ),
    ]

    for s in sprites:
        if not s.img_path.exists():
            raise FileNotFoundError(f"Missing input image: {s.img_path}")

    # Compute cropped sizes per sprite (ignoring magenta)
    cropped_sizes: dict[str, tuple[int, int]] = {}
    for s in sprites:
        im = Image.open(s.img_path)
        l, t, r, b = find_non_magenta_bbox(im)
        cropped_sizes[s.name] = (r - l, b - t)

    # Determine base target sizes per group (max within the group)
    groups: dict[str, list[Sprite]] = {}
    for s in sprites:
        groups.setdefault(s.group, []).append(s)

    target_sizes: dict[str, tuple[int, int]] = {}
    for group_name, group_sprites in groups.items():
        target_w = max(cropped_sizes[s.name][0] for s in group_sprites)
        target_h = max(cropped_sizes[s.name][1] for s in group_sprites)
        target_sizes[group_name] = (target_w, target_h)

    # Special rule: fight height should be ~2x small height (+ padding), but capped.
    small_w, small_h = target_sizes["small"]
    fight_w, fight_h = target_sizes["fight"]
    desired_fight_h = small_h * 2 + FIGHT_EXTRA_PAD_Y
    fight_h = max(fight_h, min(FIGHT_MAX_HEIGHT, desired_fight_h))
    target_sizes["fight"] = (fight_w, fight_h)

    out_dir = repo_root / "software" / "graphics" / "sprites" / "battleIcons"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_h = out_dir / "battle_icons.h"
    out_c = out_dir / "battle_icons.c"

    # Header
    h_lines: list[str] = []
    h_lines.append("#pragma once")
    h_lines.append("")
    h_lines.append("// Auto-generated by ImageConvertHelpers/convertBattleIconsToC.py")
    h_lines.append("")
    h_lines.append(f"#define BATTLE_ICON_SMALL_WIDTH  {target_sizes['small'][0]}")
    h_lines.append(f"#define BATTLE_ICON_SMALL_HEIGHT {target_sizes['small'][1]}")
    h_lines.append(f"#define BATTLE_ICON_FIGHT_WIDTH  {target_sizes['fight'][0]}")
    h_lines.append(f"#define BATTLE_ICON_FIGHT_HEIGHT {target_sizes['fight'][1]}")
    h_lines.append("")
    h_lines.append("extern const unsigned short battleIconRun[BATTLE_ICON_SMALL_WIDTH * BATTLE_ICON_SMALL_HEIGHT];")
    h_lines.append("extern const unsigned short battleIconBag[BATTLE_ICON_SMALL_WIDTH * BATTLE_ICON_SMALL_HEIGHT];")
    h_lines.append("extern const unsigned short battleIconFight[BATTLE_ICON_FIGHT_WIDTH * BATTLE_ICON_FIGHT_HEIGHT];")
    h_lines.append("")

    out_h.write_text("\n".join(h_lines) + "\n", encoding="utf-8")

    # C
    c_lines: list[str] = []
    c_lines.append('#include "battle_icons.h"')
    c_lines.append("")

    for s in sprites:
        im = Image.open(s.img_path)
        target_w, target_h = target_sizes[s.group]
        if s.group == "fight":
            bbox = find_non_magenta_bbox(im)
            cropped = im.convert("RGBA").crop(bbox)
            stretched = stretch_fight_button_to_height(cropped, target_h)
            padded = crop_and_pad_top_left(stretched, target_w, target_h)
        else:
            padded = crop_and_pad_top_left(im, target_w, target_h)
        vals = image_to_565_vals(padded)

        if s.group == "small":
            emit_array(
                c_lines,
                "unsigned short",
                s.var_name,
                "BATTLE_ICON_SMALL_WIDTH",
                "BATTLE_ICON_SMALL_HEIGHT",
                vals,
            )
        else:
            emit_array(
                c_lines,
                "unsigned short",
                s.var_name,
                "BATTLE_ICON_FIGHT_WIDTH",
                "BATTLE_ICON_FIGHT_HEIGHT",
                vals,
            )

    out_c.write_text("\n".join(c_lines), encoding="utf-8")

    print("Cropped (non-pink) sizes:")
    for name, (w, h) in cropped_sizes.items():
        print(f"  {name}: {w}x{h}")
    print("Target sizes (after pad, anchored top-left):")
    for group_name, (w, h) in target_sizes.items():
        print(f"  {group_name}: {w}x{h}")
    print(f"Wrote: {out_h}")
    print(f"Wrote: {out_c}")


if __name__ == "__main__":
    main()
