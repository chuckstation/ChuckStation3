# Android port (experimental)

This document describes the ChuckStation 3 Android port. **It is experimental.**
The app builds and installs, but it does not yet run commercial games.

## Status

| Area              | Status      | Notes                                                        |
| ----------------- | ----------- | ------------------------------------------------------------ |
| Core cross-compile | Works       | `chuckstation3_core` builds cleanly for `arm64-v8a` with NDK r26+. |
| NativeActivity     | Works       | Boots, creates an EGL surface, runs the PPU main loop.       |
| RSX rendering      | Stub        | EGL surface is created; GL shader path is stubbed for now.   |
| Touch input        | Stub        | Wired to `pressButton` / `setLeftStick` / `setRightStick`.   |
| Audio              | Stub        | Uses `NullDevice`. miniaudio-on-AAudio is planned.           |
| Settings UI        | None        | Settings file is loaded from the app's external files dir.   |
| Game loader        | Works       | Picks a game from the external files dir on startup.         |

The goal of this fork's Android work is **not** to produce a usable player
frontend in the short term. It is to validate that the core emulator
architecture can be packaged as a static library that links cleanly into a
NativeActivity app, with the heavy lifting (PPU/SPU/RSX) running unchanged
from the desktop path.

## Layout

```
android/
├── app/
│   ├── build.gradle.kts              App module: NDK build + packaging
│   └── src/main/
│       ├── AndroidManifest.xml
│       ├── java/com/chuckstation/chuckstation3/
│       │   └── MainActivity.kt       Kotlin shim that loads the .so
│       ├── res/                       Icons, strings, layouts
│       └── cpp/
│           ├── CMakeLists.txt         NDK-side CMake (links chuckstation3_core)
│           ├── native_activity.cpp    NativeActivity entry + EGL + input loop
│           └── android_logger.h       ALOGV/ALOGI/... macros
├── build.gradle.kts                  Root Gradle config
├── settings.gradle.kts
└── gradle.properties
```

## Prerequisites

- Android Studio Ladybug or newer (or just the command-line tools)
- Android NDK (latest stable — the build picks whatever the
  `android-actions/setup-android` action ships at runtime in CI, and
  whatever your SDK manager installs locally)
- CMake 3.22+ (bundled with the Android SDK)
- minSdkVersion 24, targetSdkVersion 34

## Building

From the repo root:

```bash
cd android
./gradlew :app:assembleDebug
```

The APK lands in:

```
android/app/build/outputs/apk/debug/app-debug.apk
```

Install on a connected device:

```bash
adb install -r android/app/build/outputs/apk/debug/app-debug.apk
```

The app shows up in the launcher as **ChuckStation 3**.

## How it works

1. `MainActivity.kt` calls `System.loadLibrary("chuckstation3_android")`,
   which is the shared library built from `app/src/main/cpp/`.
2. The shared library exports `ANativeActivity_onCreate`, which is the
   entry point Android invokes when the activity starts.
3. The native activity creates an EGL context bound to the default display
   and spawns a worker thread running `PlayStation3::run()`.
4. Input events from the touch screen are translated on the UI thread into
   `pressButton` / `setLeftStick` / `setRightStick` calls on the `PlayStation3`
   instance. A small on-screen overlay (planned) will eventually map touch
   regions to pad buttons.
5. On `onDestroy`, the worker thread is signalled to exit and the EGL
   context is torn down.

## Loading games

By default the Android build looks for games under:

```
/sdcard/Android/data/com.chuckstation.chuckstation3/files/Filesystem/dev_hdd0/game/
```

This mirrors the desktop `Filesystem/` layout. The same dev_flash setup that
works on desktop also works on Android — you can `adb push` the entire
`Filesystem/` directory from a working desktop install.

## What's NOT working (and why)

- **RSX rendering.** The desktop RSX backend uses desktop OpenGL 4.1 core.
  Android only exposes OpenGL ES 3.x. Porting the RSX GL backend to GLES is
  a non-trivial task that is intentionally out of scope for the initial
  experimental drop. For now, the EGL surface is created but nothing is
  drawn to it.
- **Audio.** The desktop `miniaudio` backend has an AAudio path, but it
  hasn't been wired into the Android CMake yet. The `NullDevice` is used
  as a placeholder.
- **Performance.** The desktop PPU interpreter is single-threaded. On a
  phone, this means ~0.5 fps for even the simplest games. A JIT or a
  multi-threaded interpreter would be needed for usable performance.

## Reporting issues

If you find an Android-specific build or packaging bug, file it at
<https://github.com/ChcukStation/ChuckStation3/issues> with the **android**
label. Emulation correctness bugs should go upstream to
<https://github.com/liuk7071/ChonkyStation3/issues> first.
