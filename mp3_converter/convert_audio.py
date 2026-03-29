import os, sys, subprocess, wave, struct, shutil

if len(sys.argv) < 2:
    print(f"Usage: python {sys.argv[0]} <audio.mp3>")
    sys.exit(1)

SRC = sys.argv[1]
NAME = os.path.splitext(os.path.basename(SRC))[0] + "_audio"
OUT_C, OUT_H = f"../software/se/{NAME}.c", f"../software/se/{NAME}.h"
LENGTH_NAME = "opening_audio_length" if NAME == "opening_audio" else f"{NAME}_len"

if shutil.which("ffmpeg"):
    subprocess.run(
        [
            "ffmpeg",
            "-y",
            "-i",
            SRC,
            "-ac",
            "1",
            "-ar",
            "8000",
            "-f",
            "wav",
            "tmp.wav",
        ],
        check=True,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
else:
    subprocess.run(["afconvert", "-f", "WAVE", "-d", "LEI16", "-c", "1", "-r", "8000", SRC, "tmp.wav"], check=True)

with wave.open("tmp.wav", "rb") as f:
    samples = struct.unpack(f"<{f.getnframes()}h", f.readframes(f.getnframes()))
os.remove("tmp.wav")

# build header
with open(OUT_H, "w") as f:
    f.write(
        f"#pragma once\n#include <stdint.h>\n"
        f"extern const int16_t {NAME}[];\n"
        f"extern const int {LENGTH_NAME};\n"
    )

# build c file
with open(OUT_C, "w") as f:
    f.write(f'#include "{os.path.basename(OUT_H)}"\nconst int {LENGTH_NAME} = {len(samples)};\n')
    f.write(f"const int16_t {NAME}[] = {{\n")
    for i in range(0, len(samples), 12):
        f.write("    " + ", ".join(map(str, samples[i:i+12])) + ",\n")
    f.write("};\n")

print("finish")
