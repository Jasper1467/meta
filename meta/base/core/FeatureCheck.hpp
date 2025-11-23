#pragma once

// ------------------- USER OVERRIDE -------------------
// If the user defines META_DISABLE_FEATURE_CHECK before including any meta headers,
// this file will not enforce feature checks.
#ifndef META_DISABLE_FEATURE_CHECK

// ------------------- CONSTEXPR SUPPORT -------------------
#if !defined(__cpp_consteval)
#error "Your compiler does not support 'consteval'. C++20 or newer is required."
#endif

#if !defined(__cpp_constexpr)
#error "Your compiler does not support 'constexpr'. C++11 or newer is required."
#endif

// ------------------- INLINE / FORCE_INLINE SUPPORT -------------------
#ifndef META_FORCE_INLINE
#error "META_FORCE_INLINE is not defined. Ensure Platform.hpp is included before FeatureCheck.hpp."
#endif

META_FORCE_INLINE consteval int meta_feature_check_force_inline(int x)
{
    return x + 1;
}

static_assert(meta_feature_check_force_inline(1) == 2, "META_FORCE_INLINE test failed");

// ------------------- ADDITIONAL COMPILE-TIME TESTS -------------------
// Test that consteval works
consteval int meta_feature_check_consteval(int x)
{
    return x + 2;
}

// Test constexpr works
constexpr int meta_feature_check_constexpr(int x)
{
    return x + 3;
}

// Compile-time static_assert to verify META_FORCE_INLINE works
static_assert(meta_feature_check_force_inline(1) == 2, "META_FORCE_INLINE test failed");

// Compile-time static_assert to verify consteval works
static_assert(meta_feature_check_consteval(1) == 3, "consteval test failed");

// Compile-time static_assert to verify constexpr works
static_assert(meta_feature_check_constexpr(1) == 4, "constexpr test failed");

#endif // META_DISABLE_FEATURE_CHECK

