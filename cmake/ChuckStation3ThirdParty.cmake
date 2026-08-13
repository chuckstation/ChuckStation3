# ChuckStation3ThirdParty.cmake
#
# Wires up the vendored third-party dependencies under third_party/.
# Submodules: ELFIO, xxHash, toml11, SDL, capstone, miniaudio, plusaes.
# Pure-vendored: Dolphin (BitField.hpp), Panda3DS (logger.hpp), lodepng,
# OpenGL (glad).
#
# On Android, SDL2 and miniaudio are pulled in by the Gradle build via
# prefab/NDK, so we skip them here.

# ---------------------------------------------------------------------------
# capstone — disassembler used by the PPU/SPU debuggers
# ---------------------------------------------------------------------------
set(CAPSTONE_ARCHITECTURE_DEFAULT OFF)
set(CAPSTONE_PPC_SUPPORT ON)
if(APPLE)
    set(CAPSTONE_BUILD_MACOS_THIN ON)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/capstone/CMakeLists.txt")
    add_subdirectory(third_party/capstone)
endif()

# ---------------------------------------------------------------------------
# OpenGL function loader (glad)
# ---------------------------------------------------------------------------
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/OpenGL/CMakeLists.txt")
    add_subdirectory(third_party/OpenGL)
endif()

# ---------------------------------------------------------------------------
# SDL2 — only on desktop. On Android, the Gradle build supplies SDL2 via
# the prefab package and links it into the .so directly.
# ---------------------------------------------------------------------------
if(NOT ANDROID)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL/CMakeLists.txt")
        add_subdirectory(third_party/SDL)
    else()
        message(WARNING
            "ChuckStation3: third_party/SDL is empty. "
            "Run `git submodule update --init --recursive third_party/SDL`.")
    endif()
endif()

# ---------------------------------------------------------------------------
# xxHash
# ---------------------------------------------------------------------------
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/xxHash/cmake_unofficial/CMakeLists.txt")
    add_subdirectory(third_party/xxHash/cmake_unofficial EXCLUDE_FROM_ALL)
endif()

# ---------------------------------------------------------------------------
# toml11 — settings file parser
# ---------------------------------------------------------------------------
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/toml11/CMakeLists.txt")
    add_subdirectory(third_party/toml11)
endif()

# ---------------------------------------------------------------------------
# miniaudio — only on desktop. On Android we use OpenSL ES via SDL_audio
# or Oboe (planned); the desktop single-file build is skipped here.
# ---------------------------------------------------------------------------
if(NOT ANDROID)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/miniaudio/CMakeLists.txt")
        add_subdirectory(third_party/miniaudio)
    endif()
endif()
