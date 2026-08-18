# ChuckStation3 — a PlayStation 3 emulator.
# Forked from ChonkyStation3 (https://github.com/liuk7071/ChonkyStation3).
#
# Top-level CMake orchestration. Platform frontends are added conditionally
# based on the target platform and the options passed in.

cmake_minimum_required(VERSION 3.20)

# Project metadata. Keep the CMake project name without spaces so it forms a
# valid target namespace prefix.
project(ChuckStation3
    VERSION 0.1.0
    DESCRIPTION "A PlayStation 3 emulator (ChonkyStation3 fork)"
    HOMEPAGE_URL "https://github.com/ChcukStation/ChuckStation3"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Release if the user didn't pick one.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Use LTO in Release builds where the toolchain supports it.
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

# Module path for our own cmake/ helpers.
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Position-independent code is required for the Android shared library and
# is generally useful everywhere else too.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Output binaries to a flat bin/ directory so the SDL frontend can find
# adjacent assets, and the Android packaging step knows where to look.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# SDL_main is handled manually by the frontends; suppress the platform shim.
add_compile_definitions(SDL_MAIN_HANDLED)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
include(ChuckStation3Options)

# ---------------------------------------------------------------------------
# Third-party dependencies
# ---------------------------------------------------------------------------
include(ChuckStation3ThirdParty)

# ---------------------------------------------------------------------------
# Architecture detection (defines CHUCKSTATION3_X64_HOST / _ARM64_HOST)
# ---------------------------------------------------------------------------
include(ChuckStation3Architecture)

# ---------------------------------------------------------------------------
# Emulator core (shared across every frontend)
# ---------------------------------------------------------------------------
add_library(chuckstation3_core STATIC)
add_library(ChuckStation3::Core ALIAS chuckstation3_core)

# Walk the src/ tree and collect sources + headers. Headers are listed so
# they show up in IDEs, but the public API surface lives under include/.
file(GLOB_RECURSE CHUCKSTATION3_CORE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)
file(GLOB_RECURSE CHUCKSTATION3_CORE_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# The Qt frontend files live under src/Frontend/ and are only compiled when
# ENABLE_QT_BUILD is ON; the SDL frontend main lives in src/ChuckStation3.cpp.
# When building for Android, we exclude the SDL/Qt main entirely and provide
# our own NativeActivity entry point in android/app/src/main/cpp/.
if(ANDROID)
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/ChuckStation3\\.cpp$")
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/Frontend/.*")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/.*")
elseif(ENABLE_QT_BUILD)
    # The Qt build's main() lives in src/Frontend/MainWindow.cpp indirectly,
    # but actually the entry main() is in src/ChuckStation3.cpp guarded by
    # CHUCKSTATION3_QT_BUILD. We keep ChuckStation3.cpp in for the Qt path too.
    # No exclusion needed.
else()
    # SDL desktop build. Frontend/Qt-only files are filtered out.
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/Frontend/MainWindow\\.cpp$")
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/Frontend/SettingsWidget\\.cpp$")
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/Frontend/ThreadDebuggerWidget\\.cpp$")
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/Frontend/PPUDebuggerWidget\\.cpp$")
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/Frontend/MemoryWatchpointDialog\\.cpp$")
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/Frontend/AboutWindow\\.cpp$")
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/Frontend/PKGInstallerOverlay\\.cpp$")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/MainWindow\\.hpp$")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/SettingsWidget\\.hpp$")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/ThreadDebuggerWidget\\.hpp$")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/PPUDebuggerWidget\\.hpp$")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/MemoryWatchpointDialog\\.hpp$")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/AboutWindow\\.hpp$")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/PKGInstallerOverlay\\.hpp$")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/DisabledWidgetOverlay\\.hpp$")
endif()

# GameWindow is part of the SDL frontend (uses SDL directly); exclude it from
# Android (where we use NativeActivity + EGL) but include it for SDL/Qt.
if(ANDROID)
    list(FILTER CHUCKSTATION3_CORE_SOURCES EXCLUDE REGEX
        "/src/Frontend/GameWindow\\.cpp$")
    list(FILTER CHUCKSTATION3_CORE_HEADERS EXCLUDE REGEX
        "/src/Frontend/GameWindow\\.hpp$")
endif()

target_sources(chuckstation3_core PRIVATE
    ${CHUCKSTATION3_CORE_SOURCES}
    ${CHUCKSTATION3_CORE_HEADERS}
)

# Public include root: the upstream layout uses multiple include roots so
# that headers can be referenced as e.g. <PPU.hpp>, <Memory.hpp>,
# <common.hpp>, <Frontend/GameWindow.hpp>. We mirror that exactly to avoid
# touching every include directive in the source tree.
target_include_directories(chuckstation3_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/Common"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/Loaders"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/Memory"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/PPU"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/SPU"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/RSX"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/OS"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/Scheduler"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/Filesystem"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/Audio"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChuckStation3/Frontend"
)
target_include_directories(chuckstation3_core PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Third-party includes — exposed as PUBLIC so consumer targets can find headers (e.g. BitField.hpp).
target_include_directories(chuckstation3_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/ELFIO"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/Dolphin"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/OpenGL"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/Panda3DS"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/xxHash"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/lodepng"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/toml11"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/miniaudio"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/plusaes/include"
    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(chuckstation3_core PUBLIC
    capstone
    glad
    xxHash::xxhash
    toml11::toml11
)

# SDL2 is required for the desktop frontends (GameWindow, Settings).
# On Android we link SDL2 from the gradle-managed NDK build instead, so the
# top-level CMake doesn't pull it in directly.
if(NOT ANDROID)
    target_link_libraries(chuckstation3_core PUBLIC SDL2-static)
else()
    # NDK 26 libc++ requires linking c++experimental for std::format symbols.
    target_link_libraries(chuckstation3_core PUBLIC c++experimental)
endif()

# miniaudio is a single .c file that needs to be compiled into the core.
target_sources(chuckstation3_core PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/miniaudio/miniaudio.c")

# lodepng is two files; compile them directly into the core.
target_sources(chuckstation3_core PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/lodepng/lodepng.cpp"
)

# Compiler warnings — kept permissive to match upstream's warning set.
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(chuckstation3_core PRIVATE
        -Wno-format-nonliteral
        -Wno-format-security
        -Wno-invalid-offsetof
    )
endif()

# Architecture macros
if(HOST_X64)
    target_compile_definitions(chuckstation3_core PRIVATE CHUCKSTATION3_X64_HOST)
endif()
if(HOST_ARM64)
    target_compile_definitions(chuckstation3_core PRIVATE CHUCKSTATION3_ARM64_HOST)
endif()

# Build-mode macros
if(ENABLE_USER_BUILD)
    target_compile_definitions(chuckstation3_core PUBLIC CHUCKSTATION3_USER_BUILD)
endif()
if(ENABLE_QT_BUILD)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    target_compile_definitions(chuckstation3_core PUBLIC CHUCKSTATION3_QT_BUILD)
    target_link_libraries(chuckstation3_core PUBLIC Qt6::Widgets)
    target_include_directories(chuckstation3_core PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
endif()

set_target_properties(chuckstation3_core PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION ON
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

# Group sources in IDEs by tree.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    ${CHUCKSTATION3_CORE_SOURCES}
    ${CHUCKSTATION3_CORE_HEADERS}
)

# ---------------------------------------------------------------------------
# Frontends
# ---------------------------------------------------------------------------
if(ANDROID)
    # Gradle invokes this top-level CMakeLists.txt with the NDK toolchain.
    # We've already built the chuckstation3_core static library above; now add
    # the NativeActivity shared library that becomes libchuckstation3_android.so.
    add_subdirectory(android/app/src/main/cpp)
elseif(ENABLE_QT_BUILD)
    # Qt6 desktop frontend. The main() in src/ChuckStation3.cpp is guarded by
    # CHUCKSTATION3_QT_BUILD, so we just turn it into the executable.
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    add_executable(ChuckStation3 src/ChuckStation3.cpp)
    target_link_libraries(ChuckStation3 PRIVATE chuckstation3_core Qt6::Widgets)
    target_compile_definitions(ChuckStation3 PRIVATE QT_NO_OPENGL)
    target_include_directories(ChuckStation3 PRIVATE
        ${Qt6Gui_PRIVATE_INCLUDE_DIRS})

    set_target_properties(ChuckStation3 PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
    )
    qt_standard_project_setup()

    # Qt UI files. Keep co-located with src/Frontend/UI/.
    qt_add_ui(ChuckStation3 SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Frontend/UI/mainwindow.ui"
        INCLUDE_PREFIX "Frontend/UI")
    qt_add_ui(ChuckStation3 SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Frontend/UI/settings.ui"
        INCLUDE_PREFIX "Frontend/UI")
    qt_add_ui(ChuckStation3 SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Frontend/UI/thread_debugger.ui"
        INCLUDE_PREFIX "Frontend/UI")
    qt_add_ui(ChuckStation3 SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Frontend/UI/ppu_debugger.ui"
        INCLUDE_PREFIX "Frontend/UI")
    qt_add_ui(ChuckStation3 SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Frontend/UI/about.ui"
        INCLUDE_PREFIX "Frontend/UI")

    qt_add_resources(ChuckStation3 "resources" PREFIX "/"
        FILES resources/icon-small.ico)
    qt_add_resources(ChuckStation3 "resources" PREFIX "/"
        FILES resources/Icons/loading.png)
else()
    # SDL desktop frontend. main() lives in src/ChuckStation3.cpp without the
    # Qt guard.
    add_executable(ChuckStation3 src/ChuckStation3.cpp)
    target_link_libraries(ChuckStation3 PRIVATE chuckstation3_core)

    if(WIN32)
        target_sources(ChuckStation3 PRIVATE resources/icon.rc)
    endif()
endif()

# ---------------------------------------------------------------------------
# Tests (opt-in)
# ---------------------------------------------------------------------------
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Summary
# ---------------------------------------------------------------------------
message(STATUS "")
message(STATUS "ChuckStation3 configuration summary:")
message(STATUS "  Version           : ${PROJECT_VERSION}")
message(STATUS "  Build type        : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Host architecture : ${CHUCKSTATION3_HOST_ARCH}")
message(STATUS "  User build        : ${ENABLE_USER_BUILD}")
message(STATUS "  Qt6 frontend      : ${ENABLE_QT_BUILD}")
message(STATUS "  Android target    : ${ANDROID}")
message(STATUS "  Tests             : ${ENABLE_TESTS}")
message(STATUS "")
