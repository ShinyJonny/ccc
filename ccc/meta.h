#pragma once


// Meta utils.


#define _CCC_STRINGIFY(...) #__VA_ARGS__


// Modules.


#ifndef CCC_NO_DEFAULT
    #define CCC_MODULE_ATOMICS
    #define CCC_MODULE_IO
#endif


// Feature groups.


#ifdef CCC_DEBUG
    #define CCC_DEBUG_ASSERTIONS
#endif


// Reserved keywords.


#define unsafe    _Pragma("ccc unsafe")
#define priv      _Pragma("ccc priv")
#define prot      _Pragma("ccc prot")
#define life(...) _Pragma(_CCC_STRINGIFY(ccc lifetime(__VA_ARGS__)))
#define COPY      _Pragma("ccc set_trait(COPY)")


// Compiler integration.


#ifdef __GNUC__
    #define INLINE_ALWAYS __attribute__((always_inline)) inline
#else
    #define INLINE_ALWAYS inline
#endif


#if __STDC_VERSION__ >= 202000L
    #define NODISCARD [[nodiscard]]
#elif defined(__clang__) // NOTE: gcc doesn't like `warn_unused_result` on types.
    #define NODISCARD __attribute__((warn_unused_result))
#else
    #define NODISCARD _Pragma("ccc nodiscard")
#endif


#if __STDC_VERSION__ >= 202000L
    #define NORETURN [[noreturn]]
#else
    #define NORETURN _Noreturn
#endif


#if __STDC_VERSION__ >= 202000L
    #define STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#else
    #define STATIC_ASSERT(expr, msg) _Static_assert(expr, msg)
#endif


// TODO: check if these really are just compiler hints.
#include <stddef.h>
#if __STDC_VERSION__ >= 202000L
    #define HINT_UNREACHABLE() unreachable()
#elif defined(__GNUC__)
    #define HINT_UNREACHABLE() __builtin_unreachable()
#else
    #define HINT_UNREACHABLE()
#endif
