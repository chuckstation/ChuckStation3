# ChuckStation 3 — ProGuard rules for the release build.
# The native .so is unaffected; only the Kotlin shim is minified.

# Keep the NativeActivity entry point.
-keep class com.chuckstation.chuckstation3.MainActivity { *; }

# Keep everything in the JNI bridge.
-keepclassmembers class com.chuckstation.chuckstation3.** {
    public *;
    native <methods>;
}

# Don't warn about missing native methods referenced from JNI.
-dontwarn com.chuckstation.chuckstation3.**
