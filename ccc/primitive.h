#pragma once

#include "meta.h"

#include <limits.h> /// NOTE: used only for `*_MAX`.
#include <stdint.h> /// NOTE: used only for `SIZE_MAX`.


#define U8MAX   (256ULL)
#define U8MIN   (0ULL)
#define U16MAX  (65535ULL)
#define U16MIN  (0ULL)
#define U32MAX  (4294967295ULL)
#define U32MIN  (0ULL)
#define U64MAX  (18446744073709551615ULL)
#define U64MIN  (0ULL)

#define I8MAX   (127LL)
#define I8MIN   (-I8MAX -1)
#define I16MAX  (32767LL)
#define I16MIN  (-I16MAX -1)
#define I32MAX  (2147483647LL)
#define I32MIN  (-I32MAX -1)
#define I64MAX  (9223372036854775807LL)
#define I64MIN  (-I64MAX - 1)


typedef unsigned char u8;
typedef signed char   i8;

#if USHRT_MAX == (U16MAX)
    typedef unsigned short u16;
    typedef signed short   i16;
#elif UINT_MAX == (U16MAX)
    typedef unsigned int u16;
    typedef signed int   i16;
#elif ULONG_MAX == (U16MAX)
    typedef unsigned long u16;
    typedef signed long   i16;
#elif ULLONG_MAX == (U16MAX)
    typedef unsigned long long u16;
    typedef signed long long   i16;
#else
    #error "No 16-bit integer type available."
#endif

#if USHRT_MAX == (U32MAX)
    typedef unsigned short u32;
    typedef signed short   i32;
#elif UINT_MAX == (U32MAX)
    typedef unsigned int u32;
    typedef signed int   i32;
#elif ULONG_MAX == (U32MAX)
    typedef unsigned long u32;
    typedef signed long   i32;
#elif ULLONG_MAX == (U32MAX)
    typedef unsigned long long u32;
    typedef signed long long   i32;
#else
    #error "No 32-bit integer type available."
#endif

#if USHRT_MAX == (U64MAX)
    typedef unsigned short u64;
    typedef signed short   i64;
#elif UINT_MAX == (U64MAX)
    typedef unsigned int u64;
    typedef signed int   i64;
#elif ULONG_MAX == (U64MAX)
    typedef unsigned long u64;
    typedef signed long   i64;
#elif ULLONG_MAX == (U64MAX)
    typedef unsigned long long u64;
    typedef signed long long   i64;
#else
    #error "No 64-bit integer type available."
#endif


STATIC_ASSERT(sizeof (u8)  == 1, "invalid size of u8");
STATIC_ASSERT(sizeof (i8)  == 1, "invalid size of i8");
STATIC_ASSERT(sizeof (u16) == 2, "invalid size of u16");
STATIC_ASSERT(sizeof (i16) == 2, "invalid size of i16");
STATIC_ASSERT(sizeof (u32) == 4, "invalid size of u32");
STATIC_ASSERT(sizeof (i32) == 4, "invalid size of i32");
STATIC_ASSERT(sizeof (u64) == 8, "invalid size of u64");
STATIC_ASSERT(sizeof (i64) == 8, "invalid size of i64");


typedef uintptr_t uptr;
typedef intptr_t  iptr;

#ifdef CCC_ENABLE_SIZE128
    #define U128MAX (340282366920938463463374607431768211455ULL)
#endif

#if SIZE_MAX == U16MAX
    #define USIZE_MAX U16MAX
    #define USIZE_MIN U16MIN
    #define ISIZE_MAX I16MAX
    #define ISIZE_MIN I16MIN
    typedef u16 usize;
    typedef i16 isize;
#elif SIZE_MAX > U16MAX && SIZE_MAX == U32MAX
    #define USIZE_MAX U32MAX
    #define USIZE_MIN U32MIN
    #define ISIZE_MAX I32MAX
    #define ISIZE_MIN I32MIN
    typedef u32 usize;
    typedef i32 isize;
#elif SIZE_MAX > U32MAX && SIZE_MAX == U64MAX
    #define USIZE_MAX U64MAX
    #define USIZE_MIN U64MIN
    #define ISIZE_MAX I64MAX
    #define ISIZE_MIN I64MIN
    typedef u64 usize;
    typedef i64 isize;
#elif defined(CCC_ENABLE_SIZE128) && SIZE_MAX > U64MAX && SIZE_MAX == U128MAX
    #define USIZE_MAX U128MAX
    #define USIZE_MIN U128MIN
    #define ISIZE_MAX I128MAX
    #define ISIZE_MIN I128MIN
    typedef uint128_t usize;
    typedef int128_t  isize;
#else
    #error "No size-width integer type available."
#endif

#ifdef CCC_ENABLE_SIZE128
    #undef U128MAX
#endif


#if __STDC_VERSION__ < 202000L
    #if __bool_true_false_are_defined
        #undef bool
        #undef true
        #undef false
        #undef __bool_true_false_are_defined
    #endif

    typedef _Bool bool;
    #define true  1
    #define false 0
#endif


/// Empty (placeholder) type.
typedef struct { u8 _; } __unit;
#define unit { ._ = 0 }
