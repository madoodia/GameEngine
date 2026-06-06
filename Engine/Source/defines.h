// (C) 2026 madoodia.com
// ---------------------
#pragma once

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef char b8;
typedef int b32;

// Static assertions to ensure type sizes are consistent across platforms
#if defined(__clang) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// Verify that our type definitions have the expected sizes
STATIC_ASSERT(sizeof(u8) == 1, "u8 must be 1 byte");
STATIC_ASSERT(sizeof(u16) == 2, "u16 must be 2 bytes");
STATIC_ASSERT(sizeof(u32) == 4, "u32 must be 4 bytes");
STATIC_ASSERT(sizeof(u64) == 8, "u64 must be 8 bytes");
STATIC_ASSERT(sizeof(i8) == 1, "i8 must be 1 byte");
STATIC_ASSERT(sizeof(i16) == 2, "i16 must be 2 bytes");
STATIC_ASSERT(sizeof(i32) == 4, "i32 must be 4 bytes");
STATIC_ASSERT(sizeof(i64) == 8, "i64 must be 8 bytes");
STATIC_ASSERT(sizeof(f32) == 4, "f32 must be 4 bytes");
STATIC_ASSERT(sizeof(f64) == 8, "f64 must be 8 bytes");
STATIC_ASSERT(sizeof(b8) == 1, "b8 must be 1 byte");
STATIC_ASSERT(sizeof(b32) == 4, "b32 must be 4 bytes");

// Define boolean values for clarity
#define TRUE 1
#define FALSE 0

// Platform Detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define GEPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "We just support 64-bit Windows. Please switch to a 64-bit build target."
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
#define GEPLATFORM_LINUX 1
#if defined(__ANDROID__)
#define GEPLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
#define GEPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#define GEPLATFORM_POSIX 1
#elif defined(__APPLE__)
#define GEPLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define GEPLATFORM_IOS 1
#define GEPLATFORM_MOBILE 1
#elif TARGET_OS_IPHONE
#define GEPLATFORM_IOS 1
#elif TARGET_OS_MAC
// Other types of Apple platforms can be added here if needed
#else
#error "Unknown Apple platform!"
#endif
#error "Unknown platform!"
#endif

// GameEngine Export Macro
#ifdef GEEXPORT
#if _MSC_VER
#define GEAPI __declspec(dllexport)
#else
#define GEAPI __attribute__((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define GEAPI __declspec(dllimport)
#else
#define GEAPI
#endif
#endif
