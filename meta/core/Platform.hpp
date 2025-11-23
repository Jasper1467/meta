#pragma once

// ==================== PLATFORM DETECTION ====================
#if defined(_WIN32) || defined(_WIN64)
#define META_PLATFORM_WINDOWS 1
#elif defined(__APPLE__) || defined(__MACH__)
#define META_PLATFORM_MAC 1
#elif defined(__linux__)
#define META_PLATFORM_LINUX 1
#else
#define META_PLATFORM_UNKNOWN 1
#endif

// ==================== COMPILER DETECTION ====================
#if defined(_MSC_VER)
#define META_COMPILER_MSVC 1
#elif defined(__clang__)
#define META_COMPILER_CLANG 1
#elif defined(__GNUC__)
#define META_COMPILER_GCC 1
#else
#define META_COMPILER_UNKNOWN 1
#endif

// ==================== C++ VERSION HELPERS ====================
#if __cplusplus >= 202311L
#define META_HAS_CPP23 1
#elif __cplusplus >= 202002L
#define META_HAS_CPP20 1
#elif __cplusplus >= 201703L
#define META_HAS_CPP17 1
#else
#define META_HAS_CPP14 1
#endif

// ==================== INLINE / FORCE_INLINE ====================
#if defined(META_COMPILER_MSVC)
#define META_FORCE_INLINE __forceinline
#elif defined(META_COMPILER_GCC) || defined(META_COMPILER_CLANG)
#define META_FORCE_INLINE inline __attribute__((always_inline))
#else
#define META_FORCE_INLINE inline
#endif

#define META_INLINE inline

// ==================== NODISCARD ====================
#if defined(META_HAS_CPP17)
#define META_NODISCARD [[nodiscard]]
#else
#define META_NODISCARD
#endif

// ==================== LIKELY / UNLIKELY ====================
#if defined(META_HAS_CPP20)
#define META_LIKELY [[likely]]
#define META_UNLIKELY [[unlikely]]
#else
#if defined(META_COMPILER_GCC) || defined(META_COMPILER_CLANG)
#define META_LIKELY __builtin_expect(!!(x), 1)
#define META_UNLIKELY __builtin_expect(!!(x), 0)
#else
#define META_LIKELY
#define META_UNLIKELY
#endif
#endif

// ==================== ALIGN ====================
#if defined(META_COMPILER_MSVC)
#define META_ALIGN(x) __declspec(align(x))
#elif defined(META_COMPILER_GCC) || defined(META_COMPILER_CLANG)
#define META_ALIGN(x) __attribute__((aligned(x)))
#else
#define META_ALIGN(x)
#endif

// ==================== NO_UNIQUE_ADDRESS ====================
#if __cpp_no_unique_address >= 201907L
#define META_NO_UNIQUE_ADDRESS [[no_unique_address]]
#else
#define META_NO_UNIQUE_ADDRESS
#endif

// ==================== ISATTY / FILE DESCRIPTORS ====================
#if defined(META_PLATFORM_LINUX) || defined(META_PLATFORM_MAC)
#include <unistd.h>
#define META_HAS_ISATTY 1
#elif defined(META_PLATFORM_WINDOWS)
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#define META_HAS_ISATTY 1
#else
#define META_HAS_ISATTY 0
#endif

