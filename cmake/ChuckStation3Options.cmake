# ChuckStation3Options.cmake
#
# Build options that affect every frontend. Defined here so they have a single
# source of truth and can be displayed in `ccmake` / `cmake-gui`.

option(ENABLE_USER_BUILD  "Enable user build (suppresses debug logs, disables some debugging options)" OFF)
option(ENABLE_QT_BUILD    "Enable the Qt6 desktop frontend (requires Qt6::Widgets)"                      OFF)
option(ENABLE_TESTS       "Build the smoke-test suite under tests/"                                       OFF)
option(ENABLE_LINT        "Enable clang-tidy target when clang-tidy is available"                        ON)

# Internal flag set when CMake is invoked by the Android Gradle plugin.
# This is implicit when ANDROID is true, but we expose it as an option so
# users can also drive a cross-compile manually.
if(ANDROID)
    set(CHUCKSTATION3_ANDROID ON)
else()
    set(CHUCKSTATION3_ANDROID OFF)
endif()
