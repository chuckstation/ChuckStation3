// ChuckStation 3 — Android logging macros.
//
// Thin wrappers around __android_log_print that mirror the ALOG* macros from
// liblog. Kept in a separate header so the rest of the native code can use
// ALOGI(...) / ALOGE(...) without thinking about <android/log.h>.

#pragma once

#include <android/log.h>

namespace ChuckStation3::Android {

constexpr const char* kLogTag = "ChuckStation3";

}  // namespace ChuckStation3::Android

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, ChuckStation3::Android::kLogTag, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG,   ChuckStation3::Android::kLogTag, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO,    ChuckStation3::Android::kLogTag, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN,    ChuckStation3::Android::kLogTag, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR,   ChuckStation3::Android::kLogTag, __VA_ARGS__)
#define ALOGF(...) __android_log_print(ANDROID_LOG_FATAL,   ChuckStation3::Android::kLogTag, __VA_ARGS__)
