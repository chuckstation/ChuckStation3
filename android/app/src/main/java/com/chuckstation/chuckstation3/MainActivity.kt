// ChuckStation 3 — Android entry point.
//
// This is a thin Kotlin shim. Its only job is to ensure the native shared
// library is loaded before the NativeActivity machinery kicks in. All real
// work happens in native_activity.cpp via the ANativeActivity callbacks.

package com.chuckstation.chuckstation3

import android.app.NativeActivity
import android.os.Bundle

class MainActivity : NativeActivity() {
    init {
        // Load the .so eagerly so that ANativeActivity_onCreate is available
        // before the NativeActivity framework tries to invoke it.
        try {
            System.loadLibrary("chuckstation3_android")
        } catch (e: UnsatisfiedLinkError) {
            // The NativeActivity framework would throw a more cryptic error
            // later; surface a clearer one now.
            throw RuntimeException(
                "Failed to load libchuckstation3_android.so: ${e.message}", e
            )
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }
}
