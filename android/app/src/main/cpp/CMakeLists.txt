# ChuckStation 3 — Android NDK side.
#
# This is added as a subdirectory by the top-level CMakeLists.txt when ANDROID
# is true. It builds the NativeActivity shared library that Gradle packages
# into the APK as libchuckstation3_android.so.
#
# The chuckstation3_core static library (and all of its dependencies) are
# already defined by the parent scope; we just link against them.

add_library(chuckstation3_android SHARED
    native_activity.cpp
)

target_link_libraries(chuckstation3_android PRIVATE
    ChuckStation3::Core
    android           # ANativeActivity, AInputQueue, AWindow
    log               # __android_log_print
    EGL               # eglCreateContext, eglMakeCurrent, ...
    GLESv3            # OpenGL ES 3.x (used implicitly by glad on Android)
)

# Strip on release builds.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(chuckstation3_android PRIVATE -O3)
endif()

# Android-specific logging macros.
target_include_directories(chuckstation3_android PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_target_properties(chuckstation3_android PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)
