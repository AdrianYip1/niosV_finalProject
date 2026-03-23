import os, sys, subprocess, wave, struct

if len(sys.argv) < 2:
    print(f"Usage: python {sys.argv[0]} <audio.mp3>")
    sys.exit(1)

SRC = sys.argv[1]
NAME = os.path.splitext(os.path.basename(SRC))[0] + "_audio"
OUT_C, OUT_H = f"../software/se/{NAME}.c", f"../software/se/{NAME}.h"

# only for mac. convert mp3 into tmp.wav
subprocess.run(["afconvert", "-f", "WAVE", "-d", "LEI16@8000", "-c", "1", SRC, "tmp.wav"], check=True)

with wave.open("tmp.wav", "rb") as f:
    samples = struct.unpack(f"<{f.getnframes()}h", f.readframes(f.getnframes()))
os.remove("tmp.wav")

# build header
with open(OUT_H, "w") as f:
    f.write(f"#pragma once\n#include <stdint.h>\nextern const int16_t {NAME}[];\nextern const int {NAME}_len;\n")

# build c file
with open(OUT_C, "w") as f:
    f.write(f'#include "{os.path.basename(OUT_H)}"\nconst int {NAME}_len = {len(samples)};\n')
    f.write(f"const int16_t {NAME}[] = {{\n")
    for i in range(0, len(samples), 12):
        f.write("    " + ", ".join(map(str, samples[i:i+12])) + ",\n")
    f.write("};\n")

print("finish")
