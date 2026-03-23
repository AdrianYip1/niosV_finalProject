import os
import sys
import subprocess
import wave
import struct

def convert_mp3_to_c_array(mp3_path, output_c_path, output_h_path, array_name="audio_data"):
    print(f"Loading {mp3_path}...")
    
    # Use macOS built-in afconvert to convert mp3 to wav
    wav_path = "temp_conversion.wav"
    print("Converting MP3 to WAV using macOS afconvert...")
    
    # -f WAVE : format is WAV
    # -d LEI16@8000 : Little-Endian Signed 16-bit integers at 8000Hz
    # -c 1 : 1 channel (Mono)
    cmd = ["afconvert", "-f", "WAVE", "-d", "LEI16@8000", "-c", "1", mp3_path, wav_path]
    try:
        subprocess.run(cmd, check=True)
    except Exception as e:
        print(f"Failed to run afconvert. Error: {e}")
        sys.exit(1)

    print("Reading converted WAV file...")
    samples = []
    try:
        with wave.open(wav_path, "rb") as wf:
            framerate = wf.getframerate()
            nframes = wf.getnframes()
            sampwidth = wf.getsampwidth()
            print(f"Sampling Rate: {framerate}Hz, Sample Width: {sampwidth} bytes, Frames: {nframes}")
            
            raw_data = wf.readframes(nframes)
            # Unpack 16-bit signed integers (h is short, 2 bytes)
            num_samples = len(raw_data) // 2
            samples = struct.unpack(f"<{num_samples}h", raw_data)
            
    except Exception as e:
        print(f"Failed to read WAV file: {e}")
        sys.exit(1)
    finally:
        # Cleanup
        if os.path.exists(wav_path):
            os.remove(wav_path)

    length = len(samples)
    
    # Generate Header (.h)
    h_content = f"""#ifndef {array_name.upper()}_H
#define {array_name.upper()}_H

#include <stdint.h>

extern const int16_t {array_name}[{length}];
extern const unsigned int {array_name}_length;

#endif // {array_name.upper()}_H
"""
    with open(output_h_path, "w") as f:
        f.write(h_content)

    # Generate Source (.c)
    print(f"Writing C array ({length} samples)... this may take a moment.")
    with open(output_c_path, "w") as f:
        f.write(f'#include "{os.path.basename(output_h_path)}"\n\n')
        f.write(f'const unsigned int {array_name}_length = {length};\n\n')
        f.write(f'const int16_t {array_name}[{length}] = {{\n')
        
        # Write samples as decimal integers
        line = "    "
        for i, sample in enumerate(samples):
            line += f"{sample}, "
            if (i + 1) % 12 == 0:
                f.write(line + "\n")
                line = "    "
        if line != "    ":
            f.write(line + "\n")
            
        f.write("};\n")

    print(f"Success! Saved to {output_c_path} and {output_h_path}")
    print(f"Total audio length: {length / framerate:.2f} seconds.")

if __name__ == "__main__":
    # Ensure terminal cwd is where the script is or relative paths work
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Default paths based on project structure
    mp3_file = os.path.join(script_dir, "opening.mp3")
    c_out = os.path.join(script_dir, "../software/se/opening_audio.c")
    h_out = os.path.join(script_dir, "../software/se/opening_audio.h")
    
    if not os.path.exists(mp3_file):
        print(f"File not found: {mp3_file}")
        sys.exit(1)
        
    convert_mp3_to_c_array(mp3_file, c_out, h_out, "opening_audio")
