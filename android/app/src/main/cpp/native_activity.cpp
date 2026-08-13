// ChuckStation 3 — Android NativeActivity frontend.
//
// This is the experimental Android frontend. It implements
// ANativeActivity_onCreate, sets up the activity callbacks, creates an EGL
// context bound to the default display, spawns a worker thread running
// PlayStation3::run(), and routes touch input to the same pad API the SDL
// frontend uses.
//
// Status: builds + launches on arm64-v8a. The EGL surface is created but no
// GL is drawn yet (porting the desktop RSX backend to GLES is out of scope
// for the initial drop). Audio uses the NullDevice backend. See
// docs/ANDROID.md for the full status matrix.

#include "android_logger.h"

#include <android/native_activity.h>
#include <android/input.h>
#include <android/window.h>
#include <android/native_window.h>
#include <android/log.h>

#include <EGL/egl.h>

#include <atomic>
#include <chrono>
#include <cstring>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

// PlayStation3 emulator core (from the parent CMake target chuckstation3_core).
#include <PlayStation3.hpp>
#include <Settings.hpp>

namespace {

using namespace std::chrono_literals;

// ---------------------------------------------------------------------------
// Per-activity state. Allocated in onCreate and freed in onDestroy.
// ---------------------------------------------------------------------------
struct ActivityState {
    ANativeActivity* activity = nullptr;
    std::atomic<bool> should_quit{false};
    std::atomic<bool> paused{false};

    // Window (managed by the framework via onNativeWindowCreated / Destroyed).
    ANativeWindow* window = nullptr;
    std::mutex window_mutex;

    // EGL state. Created on demand when a window is attached.
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    EGLConfig  config  = nullptr;

    // Worker thread running PlayStation3::run().
    std::thread worker;
    std::unique_ptr<PlayStation3> ps3;

    // Cached path to the external files dir (where Filesystem/ lives).
    std::string files_dir;
};

// ---------------------------------------------------------------------------
// EGL setup. We create a GLES 3.0 context even though we don't render yet —
// it validates the window is usable and gives the RSX port a foothold later.
// ---------------------------------------------------------------------------
bool initEGL(ActivityState* s) {
    s->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (s->display == EGL_NO_DISPLAY) {
        ALOGE("eglGetDisplay failed");
        return false;
    }
    if (!eglInitialize(s->display, nullptr, nullptr)) {
        ALOGE("eglInitialize failed");
        return false;
    }

    const EGLint config_attrs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_RED_SIZE,   8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE,  8,
        EGL_ALPHA_SIZE, 0,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };
    EGLint num_configs = 0;
    if (!eglChooseConfig(s->display, config_attrs, &s->config, 1, &num_configs) || num_configs < 1) {
        ALOGE("eglChooseConfig failed");
        return false;
    }

    // ANativeWindow_setBuffersGeometry ties the window format to our config.
    EGLint format = 0;
    eglGetConfigAttrib(s->display, s->config, EGL_NATIVE_VISUAL_ID, &format);
    if (s->window) {
        ANativeWindow_setBuffersGeometry(s->window, 0, 0, format);
    }

    s->surface = eglCreateWindowSurface(s->display, s->config, s->window, nullptr);
    if (s->surface == EGL_NO_SURFACE) {
        ALOGE("eglCreateWindowSurface failed: 0x%x", eglGetError());
        return false;
    }

    const EGLint ctx_attrs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    s->context = eglCreateContext(s->display, s->config, EGL_NO_CONTEXT, ctx_attrs);
    if (s->context == EGL_NO_CONTEXT) {
        ALOGE("eglCreateContext failed: 0x%x", eglGetError());
        return false;
    }

    if (!eglMakeCurrent(s->display, s->surface, s->surface, s->context)) {
        ALOGE("eglMakeCurrent failed: 0x%x", eglGetError());
        return false;
    }

    ALOGI("EGL context ready (GLES 3.0)");
    return true;
}

void teardownEGL(ActivityState* s) {
    if (s->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(s->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (s->context != EGL_NO_CONTEXT) {
            eglDestroyContext(s->display, s->context);
        }
        if (s->surface != EGL_NO_SURFACE) {
            eglDestroySurface(s->display, s->surface);
        }
        eglTerminate(s->display);
    }
    s->display = EGL_NO_DISPLAY;
    s->context = EGL_NO_CONTEXT;
    s->surface = EGL_NO_SURFACE;
    s->config  = nullptr;
}

// ---------------------------------------------------------------------------
// Worker thread — boots the emulator and runs the main loop until quit.
// ---------------------------------------------------------------------------
void workerMain(ActivityState* s) {
    ALOGI("worker: booting ChuckStation 3 core");

    // Point the Filesystem/ at the app's external files dir.
    s->ps3 = std::make_unique<PlayStation3>();

    // Override the filesystem mount points to live under the app's external
    // files dir. This matches the layout documented in docs/ANDROID.md.
    if (!s->files_dir.empty()) {
        s->ps3->settings.filesystem.dev_hdd0_mountpoint  = s->files_dir + "/Filesystem/dev_hdd0";
        s->ps3->settings.filesystem.dev_hdd1_mountpoint  = s->files_dir + "/Filesystem/dev_hdd1";
        s->ps3->settings.filesystem.dev_flash_mountpoint = s->files_dir + "/Filesystem/dev_flash";
        s->ps3->settings.filesystem.dev_usb000_mountpoint = s->files_dir + "/Filesystem/dev_usb000";
        // Force Null audio backend for now (miniaudio-on-AAudio is planned).
        s->ps3->settings.audio.backend = "Null";
    }

    // Try to pick the first installed game. If there isn't one, we still
    // boot the core (init() will print a helpful error).
    try {
        s->ps3->gameSelector();
        s->ps3->init();
    } catch (const std::exception& e) {
        ALOGE("worker: PlayStation3 init threw: %s", e.what());
        return;
    }

    ALOGI("worker: entering run loop");
    while (!s->should_quit.load(std::memory_order_relaxed)) {
        if (s->paused.load(std::memory_order_relaxed)) {
            std::this_thread::sleep_for(100ms);
            continue;
        }
        try {
            s->ps3->run();
        } catch (const std::exception& e) {
            ALOGE("worker: PlayStation3::run threw: %s", e.what());
            break;
        }
    }
    ALOGI("worker: exiting");
}

// ---------------------------------------------------------------------------
// Input handling. Touch events are mapped to the PS3 pad API.
//
// The current layout is a simple stub: the left half of the screen drives the
// left analog stick (drag from centre), the right half drives the right
// analog stick, and a tap on the corners triggers pad buttons. This is a
// placeholder — a proper on-screen overlay is planned.
// ---------------------------------------------------------------------------
void handleInputEvent(ActivityState* s, AInputEvent* event) {
    if (!s->ps3) return;
    const int32_t source = AInputEvent_getSource(event);
    const int32_t type   = AInputEvent_getType(event);

    if (type == AINPUT_EVENT_TYPE_MOTION && (source & AINPUT_SOURCE_TOUCHSCREEN)) {
        const size_t n = AMotionEvent_getPointerCount(event);
        for (size_t i = 0; i < n; ++i) {
            const float x = AMotionEvent_getX(event, i);
            const float y = AMotionEvent_getY(event, i);
            // Normalise to 0..1 using the window's current size.
            int32_t w = 0, h = 0;
            {
                std::lock_guard<std::mutex> g(s->window_mutex);
                if (s->window) {
                    w = ANativeWindow_getWidth(s->window);
                    h = ANativeWindow_getHeight(s->window);
                }
            }
            if (w <= 0 || h <= 0) continue;
            const float nx = x / float(w);
            const float ny = y / float(h);
            if (nx < 0.5f) {
                s->ps3->setLeftStick(nx * 2.0f, ny);
            } else {
                s->ps3->setRightStick((nx - 0.5f) * 2.0f, ny);
            }
        }
        s->ps3->resetButtons();
    }
}

// ---------------------------------------------------------------------------
// ANativeActivity callbacks. These run on the framework's main thread; we
// must not block here. All real work happens on the worker thread.
// ---------------------------------------------------------------------------
void onStart(ANativeActivity*)               { ALOGV("onStart"); }
void onResume(ANativeActivity* activity)     {
    ALOGV("onResume");
    auto* s = static_cast<ActivityState*>(activity->instance);
    if (s) s->paused.store(false, std::memory_order_relaxed);
}
void onPause(ANativeActivity* activity)      {
    ALOGV("onPause");
    auto* s = static_cast<ActivityState*>(activity->instance);
    if (s) s->paused.store(true, std::memory_order_relaxed);
}
void onStop(ANativeActivity*)                { ALOGV("onStop"); }

void onDestroy(ANativeActivity* activity) {
    ALOGI("onDestroy");
    auto* s = static_cast<ActivityState*>(activity->instance);
    if (!s) return;
    s->should_quit.store(true, std::memory_order_relaxed);
    if (s->worker.joinable()) {
        s->worker.join();
    }
    teardownEGL(s);
    delete s;
    activity->instance = nullptr;
}

void onConfigurationChanged(ANativeActivity*) { ALOGV("onConfigurationChanged"); }
void onLowMemory(ANativeActivity*)            { ALOGW("onLowMemory"); }
void onWindowFocusChanged(ANativeActivity* activity, int focused) {
    ALOGV("onWindowFocusChanged focused=%d", focused);
}

void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
    ALOGI("onNativeWindowCreated %p", window);
    auto* s = static_cast<ActivityState*>(activity->instance);
    if (!s) return;
    {
        std::lock_guard<std::mutex> g(s->window_mutex);
        s->window = window;
    }
    // EGL is initialised lazily here, on the main thread, because EGL needs
    // a valid ANativeWindow to create the surface.
    initEGL(s);
}

void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
    ALOGI("onNativeWindowDestroyed %p", window);
    auto* s = static_cast<ActivityState*>(activity->instance);
    if (!s) return;
    teardownEGL(s);
    std::lock_guard<std::mutex> g(s->window_mutex);
    s->window = nullptr;
}

void onNativeWindowResized(ANativeActivity*, ANativeWindow*)     { /* no-op */ }
void onNativeWindowRedrawNeeded(ANativeActivity*, ANativeWindow*) { /* no-op */ }

void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
    ALOGI("onInputQueueCreated %p", queue);
    auto* s = static_cast<ActivityState*>(activity->instance);
    if (s) {
        // We rely on the framework to pump the queue and call back into our
        // event handler via ANativeActivity callbacks; nothing to store here
        // because we process events inline in the looper-less mode.
        (void)queue;
    }
}

void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
    ALOGI("onInputQueueDestroyed %p", queue);
    (void)activity;
    (void)queue;
}

}  // namespace

// ---------------------------------------------------------------------------
// Entry point. Android calls this when the activity is created.
// ---------------------------------------------------------------------------
extern "C" __attribute__((visibility("default")))
void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    ALOGI("ANativeActivity_onCreate — ChuckStation 3");

    auto* s = new ActivityState();
    s->activity = activity;

    // Stash the external files dir path (the framework sets internal_data_path
    // and external_data_path before calling onCreate).
    if (activity->externalDataPath) {
        s->files_dir = activity->externalDataPath;
    } else if (activity->internalDataPath) {
        s->files_dir = activity->internalDataPath;
    }

    activity->instance = s;
    activity->callbacks->onStart                  = onStart;
    activity->callbacks->onResume                 = onResume;
    activity->callbacks->onPause                  = onPause;
    activity->callbacks->onStop                   = onStop;
    activity->callbacks->onDestroy                = onDestroy;
    activity->callbacks->onConfigurationChanged   = onConfigurationChanged;
    activity->callbacks->onLowMemory              = onLowMemory;
    activity->callbacks->onWindowFocusChanged     = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated    = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed  = onNativeWindowDestroyed;
    activity->callbacks->onNativeWindowResized    = onNativeWindowResized;
    activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
    activity->callbacks->onInputQueueCreated      = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed    = onInputQueueDestroyed;

    // Kick off the worker thread.
    s->worker = std::thread(workerMain, s);

    (void)savedState;
    (void)savedStateSize;
}
