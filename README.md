## ChuckStation 3

A work-in-progress PlayStation® 3 emulator

ChuckStation 3 is an experimental PS3 emulator based on "ChonkyStation3" (https://github.com/liuk7071/ChonkyStation3).

## The project focuses on:

- A cleaner source structure
- Cross-platform CI
- An experimental Android port
- Learning and experimentation

«⚠️ ChuckStation 3 is not ready for playing commercial games.»

---

## Repository

- Repository: https://github.com/ChuckkStation/ChuckStation3
- Upstream: https://github.com/liuk7071/ChonkyStation3
- License: GNU GPL v3

This project is a downstream fork of ChonkyStation3. The emulator core comes from the original project.

---


## The project supports:

- Windows / Linux / macOS — SDL or Qt6
- Android — Experimental NativeActivity frontend

---

## Building

### Requirements

- C++23 compiler
- GCC 13+, Clang 16+, or MSVC 2022+
- CMake 3.20+
- Ninja

### Desktop
```bash
git clone --recursive https://github.com/ChcukStation/ChuckStation3.git
cd ChuckStation3

cmake --preset default
cmake --build --preset default
```

### For Qt6:
```bash
cmake --preset qt
cmake --build --preset qt
```

### For a debug build:
```bash
cmake --preset debug
cmake --build --preset debug
```
The executable will be placed in:

**build/<preset>/bin/**

---

## Android

NOTE: The Android port is experimental.

### Current status:

- ARM64 ("arm64-v8a") core builds successfully
- NativeActivity frontend works
- EGL rendering is initialized
- Touch controls are connected
- Audio currently uses a null backend
- Commercial games are not supported yet

### Build APK
```bash
cd android
./gradlew :app:assembleRelease
```

APK output:

**android/app/build/outputs/apk/release/app-release-unsigned.apk**

---

## Usage

When ChuckStation 3 starts, it creates:
```
Filesystem/
├── dev_flash/
└── dev_hdd0/
```

These folders contain the virtual PS3 filesystem.

### Game Installation

ChuckStation 3 currently does not install ".pkg" files directly.

### For testing:

1. Install the game using RPCS3.
2. Copy the game folder from RPCS3's "dev_hdd0/game/" into ChuckStation 3.
3. Decrypt "EBOOT.BIN" using RPCS3.
4. Put the decrypted "EBOOT.elf" beside the original "EBOOT.BIN".
5. Decrypt required ".edat" files.

---

### PS3 System Software

PS3 system software is required for games.

Install the PS3 firmware using RPCS3, then copy the required files into ChuckStation 3's "dev_flash".

Currently required PRX files include:

liblv2.prx
libsre.prx
libspurs_jq.prx
libkey2char.prx
libl10n.prx
libfiber.prx
libresc.prx
libsync2.prx

This list may change as development continues.

---


## Acknowledgements

- ChonkyStation3 — Original emulator code
- RPCS3 — PS3 emulation research
- PSL1GHT — Open-source PS3 SDK
- psdevwiki — PS3 technical information
- Panda3DS — Logger helper
- Dolphin — "BitField" utility

---