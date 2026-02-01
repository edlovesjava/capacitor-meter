#!/usr/bin/env python3
"""
Sync PlatformIO source files to Arduino IDE folder.

Run this after making changes to src/ to keep arduino/ in sync.
Usage: python sync_arduino.py
"""

import shutil
from pathlib import Path

# Paths
SRC_DIR = Path(__file__).parent / "src"
ARDUINO_DIR = Path(__file__).parent / "arduino" / "capacitance_meter_button"

# Files to sync (shared classes)
SYNC_FILES = [
    "Display.h",
    "SSD1306Display.h",
    "SSD1306Display.cpp",
    "CapacitanceMeter.h",
    "CapacitanceMeter.cpp",
]

def sync():
    print("Syncing PlatformIO -> Arduino IDE")

    for filename in SYNC_FILES:
        src_file = SRC_DIR / filename
        dst_file = ARDUINO_DIR / filename

        if src_file.exists():
            shutil.copy2(src_file, dst_file)
            print(f"  {filename}")
        else:
            print(f"  {filename} - NOT FOUND (skipped)")

    print("\nDone. Note: main.cpp and .ino are maintained separately.")
    print("If you changed pin definitions or setup/loop, update the .ino manually.")

if __name__ == "__main__":
    sync()
