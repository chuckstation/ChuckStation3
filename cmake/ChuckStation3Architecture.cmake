# ChuckStation3Architecture.cmake
#
# Detect the host / target architecture and expose:
#   * HOST_X64   (bool)
#   * HOST_ARM64 (bool)
#   * CHUCKSTATION3_HOST_ARCH (string: "x64" | "arm64" | "unknown")
#
# Mirrors the original detection logic from ChonkyStation3's CMakeLists.txt,
# but factors it out so the Android cross-compile path uses the same code.

set(HOST_X64   FALSE)
set(HOST_ARM64 FALSE)
set(CHUCKSTATION3_HOST_ARCH "unknown")

if(ANDROID)
    # Android NDK sets CMAKE_ANDROID_ARCH_ABI; trust it over CMAKE_SYSTEM_PROCESSOR.
    if(CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a")
        set(HOST_ARM64 TRUE)
        set(CHUCKSTATION3_HOST_ARCH "arm64")
    elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "armeabi-v7a")
        set(CHUCKSTATION3_HOST_ARCH "armv7")
    elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "x86_64")
        set(HOST_X64 TRUE)
        set(CHUCKSTATION3_HOST_ARCH "x64")
    elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "x86")
        set(CHUCKSTATION3_HOST_ARCH "x86")
    endif()
elseif(APPLE AND NOT "${CMAKE_OSX_ARCHITECTURES}" STREQUAL "")
    # Apple universal builds.
    if("x86_64" IN_LIST CMAKE_OSX_ARCHITECTURES)
        set(HOST_X64 TRUE)
    endif()
    if("arm64" IN_LIST CMAKE_OSX_ARCHITECTURES)
        set(HOST_ARM64 TRUE)
    endif()
    if(HOST_X64 AND HOST_ARM64)
        set(CHUCKSTATION3_HOST_ARCH "universal")
    elseif(HOST_X64)
        set(CHUCKSTATION3_HOST_ARCH "x64")
    elseif(HOST_ARM64)
        set(CHUCKSTATION3_HOST_ARCH "arm64")
    endif()
else()
    # Normal detection via CMAKE_SYSTEM_PROCESSOR.
    set(_proc "${CMAKE_SYSTEM_PROCESSOR}")
    if(_proc STREQUAL "AMD64" OR _proc STREQUAL "x86-64" OR _proc STREQUAL "x86_64")
        set(HOST_X64 TRUE)
        set(CHUCKSTATION3_HOST_ARCH "x64")
    elseif(_proc STREQUAL "aarch64" OR _proc STREQUAL "arm64")
        set(HOST_ARM64 TRUE)
        set(CHUCKSTATION3_HOST_ARCH "arm64")
    endif()
endif()

if(HOST_X64)
    message(STATUS "ChuckStation3: detected target architecture: x64")
endif()
if(HOST_ARM64)
    message(STATUS "ChuckStation3: detected target architecture: arm64")
endif()
if(NOT HOST_X64 AND NOT HOST_ARM64)
    message(STATUS "ChuckStation3: unknown target architecture '${CMAKE_SYSTEM_PROCESSOR}'")
endif()
