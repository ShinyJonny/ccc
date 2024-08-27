#pragma once


#include <assert.h>

#define CCC_STRINGIFY(...) #__VA_ARGS__

// Reserved keywords.
#define unsafe    _Pragma("ccc unsafe")
#define life(...) _Pragma(CCC_STRINGIFY(ccc lifetime(__VA_ARGS__)))
#define COPY      _Pragma("ccc set_trait(COPY)")


#ifdef __GNUC__
    #define INLINE_ALWAYS __attribute__((always_inline)) inline
#else
    #define INLINE_ALWAYS inline
#endif

#if __STDC_VERSION__ >= 202000L
    #define NODISCARD [[nodiscard]]
// NOTE: gcc doesn't like `warn_unused_result` on types.
#elif defined(__clang__)
    #define NODISCARD __attribute__((warn_unused_result))
#else
    #define NODISCARD _Pragma("ccc nodiscard")
#endif

#if __STDC_VERSION__ >= 202000L
    #define STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#else
    #define STATIC_ASSERT(expr, msg) _Static_assert(expr, msg)
#endif

// TODO: check if these really are just compiler hints.
#if __STDC_VERSION__ >= 202000L
    #include <stddef.h>
    #define HINT_UNREACHABLE() unreachable()
#elif defined(__GNUC__)
    #define HINT_UNREACHABLE() __builtin_unreachable()
#else
    #define HINT_UNREACHABLE()
#endif

#define ASSERT_MSG(expr, msg) assert((expr) && msg)
#define ASSERT(expr) assert(expr)
#define UNREACHABLE_MSG(msg) assert(!"unreachable: " msg)
#define UNREACHABLE() assert(!"unreachable!")
#define TODO_MSG(msg) assert(!"todo: " msg)
#define TODO() assert(!"todo!")

#ifdef CCC_DEBUG
    #define CCC_DEBUG_ASSERTIONS
#endif

#ifdef CCC_DEBUG_ASSERTIONS
    #define DEBUG_ASSERT(expr) assert(expr)
    #define DEBUG_ASSERT_MSG(expr, msg) assert((expr) && msg)
    #define DEBUG_UNREACHABLE() UNREACHABLE()
    #define DEBUG_UNREACHABLE_MSG(msg) UNREACHABLE(msg)
#else
    #define DEBUG_ASSERT(expr)
    #define DEBUG_ASSERT_MSG(expr, msg)
    #define DEBUG_UNREACHABLE() HINT_UNREACHABLE()
    #define DEBUG_UNREACHABLE_MSG(msg) HINT_UNREACHABLE()
#endif
