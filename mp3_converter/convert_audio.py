import os
import sys
import subprocess
import wave

def convert_mp3_to_c_array(mp3_path, output_c_path, output_h_path, array_name="audio_data"):
    print(f"Loading {mp3_path}...")
    
    # Use macOS built-in afconvert to convert mp3 to wav
    wav_path = "temp_conversion.wav"
    print("Converting MP3 to WAV using macOS afconvert...")
    
    # -f WAVE : format is WAV
    # -d UI8@8000 : Unsigned 8-bit integers at 8000Hz
    # -c 1 : 1 channel (Mono)
    cmd = ["afconvert", "-f", "WAVE", "-d", "UI8@48000", "-c", "1", mp3_path, wav_path]
    try:
        subprocess.run(cmd, check=True)
    except Exception as e:
        print(f"Failed to run afconvert. Error: {e}")
        sys.exit(1)

    print("Reading converted WAV file...")
    try:
        with wave.open(wav_path, "rb") as wf:
            framerate = wf.getframerate()
            nframes = wf.getnframes()
            raw_data = wf.readframes(nframes)
    except Exception as e:
        print(f"Failed to read WAV file: {e}")
        sys.exit(1)
    finally:
        # Cleanup
        if os.path.exists(wav_path):
            os.remove(wav_path)

    length = len(raw_data)
    
    # Generate Header (.h)
    h_content = f"""#ifndef {array_name.upper()}_H
#define {array_name.upper()}_H

#include <stdint.h>

extern const uint8_t {array_name}[{length}];
extern const unsigned int {array_name}_length;

#endif // {array_name.upper()}_H
"""
    with open(output_h_path, "w") as f:
        f.write(h_content)

    # Generate Source (.c)
    print(f"Writing C array ({length} bytes)... this may take a moment.")
    with open(output_c_path, "w") as f:
        f.write(f'#include "{os.path.basename(output_h_path)}"\n\n')
        f.write(f'const unsigned int {array_name}_length = {length};\n\n')
        f.write(f'const uint8_t {array_name}[{length}] = {{\n')
        
        # Write bytes as hex
        line = "    "
        for i, byte in enumerate(raw_data):
            line += f"0x{byte:02x}, "
            if (i + 1) % 16 == 0:
                f.write(line + "\n")
                line = "    "
        if line != "    ":
            f.write(line + "\n")
            
        f.write("};\n")

    print(f"Success! Saved to {output_c_path} and {output_h_path}")
    print(f"Total audio length: {length / framerate:.2f} seconds.")

if __name__ == "__main__":
    mp3_file = "opening.mp3"
    c_file = "../software/se/opening_audio.c"
    h_file = "../software/se/opening_audio.h"
    
    if not os.path.exists(mp3_file):
        print(f"File not found: {mp3_file}")
        sys.exit(1)
        
    convert_mp3_to_c_array(mp3_file, c_file, h_file, "opening_audio")
