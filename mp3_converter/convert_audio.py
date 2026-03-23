import os, subprocess, wave, struct

# Settings
SRC, NAME = "opening.mp3", "opening_audio"
OUT_C, OUT_H = f"../software/se/{NAME}.c", f"../software/se/{NAME}.h"

# Convert MP3 to 8kHz Mono WAV (mac-only)
subprocess.run(["afconvert", "-f", "WAVE", "-d", "LEI16@8000", "-c", "1", SRC, "tmp.wav"], check=True)

with wave.open("tmp.wav", "rb") as f:
    samples = struct.unpack(f"<{f.getnframes()}h", f.readframes(f.getnframes()))
os.remove("tmp.wav")

# Write .h
with open(OUT_H, "w") as f:
    f.write(f"#pragma once\n#include <stdint.h>\nextern const int16_t {NAME}[];\nextern const int {NAME}_len;\n")

# Write .c
with open(OUT_C, "w") as f:
    f.write(f'#include "{os.path.basename(OUT_H)}"\nconst int {NAME}_len = {len(samples)};\n')
    f.write(f"const int16_t {NAME}[] = {{\n")
    for i in range(0, len(samples), 12):
        f.write("    " + ", ".join(map(str, samples[i:i+12])) + ",\n")
    f.write("};\n")
