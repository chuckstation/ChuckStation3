// ChuckStation 3 — app module.
//
// Builds the experimental Android frontend as a single-ABI (arm64-v8a)
// APK. The native side is built by CMake via externalNativeBuild and links
// against the chuckstation3_core static library that's defined at the
// repo root.

plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "com.chuckstation.chuckstation3"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.chuckstation.chuckstation3"
        minSdk = 24
        targetSdk = 34
        versionCode = 1
        versionName = "0.1.0"

        // Single ABI for now; multi-ABI would multiply CI time.
        ndk {
            abiFilters += "arm64-v8a"
        }

        // Drive the NDK CMake build. The path is relative to this module.
        externalNativeBuild {
            cmake {
                // Tell Gradle to use the repo root as the CMake source dir,
                // so the top-level CMakeLists.txt is reused.
                path(file("../../CMakeLists.txt"))
                version = "3.22.1"

                // Pass build flags through to CMake.
                arguments += listOf(
                    "-DANDROID_STL=c++_shared",
                    "-DANDROID_PLATFORM=android-24",
                    "-DCHUCKSTATION3_ANDROID=ON",
                    "-DENABLE_USER_BUILD=ON",
                    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
                )

                // C++ flags — the core needs a recent C++ standard; the NDK
                // defaults are sufficient but we set explicit flags anyway.
                cppFlags += listOf(
                    "-std=c++23",
                    "-fno-exceptions",
                    "-fno-rtti",
                    "-fvisibility=hidden",
                    "-fvisibility-inlines-hidden"
                )

                cFlags += listOf(
                    "-fvisibility=hidden"
                )
            }
        }
    }

    buildTypes {
        getByName("debug") {
            isMinifyEnabled = false
            isDebuggable = true
            // Native code is built in Debug by default for debug builds.
            externalNativeBuild {
                cmake {
                    arguments += listOf("-DCMAKE_BUILD_TYPE=Debug")
                }
            }
        }
        getByName("release") {
            isMinifyEnabled = false
            isDebuggable = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            externalNativeBuild {
                cmake {
                    arguments += listOf("-DCMAKE_BUILD_TYPE=Release")
                }
            }
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    kotlinOptions {
        jvmTarget = "17"
    }

    // The NDK build produces libchuckstation3_android.so; let Gradle package
    // it into the APK automatically (default behaviour).
    packaging {
        jniLibs {
            useLegacyPackaging = false
        }
    }

    // Suppress known-native-build warnings.
    androidResources {
        // no-op
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.13.1")
    implementation("androidx.appcompat:appcompat:1.7.0")
}
