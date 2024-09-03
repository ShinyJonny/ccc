#pragma once


#include "meta.h"

#include <limits.h> /// NOTE: used only for `*_MAX`.
#include <stdint.h> /// NOTE: used only for `SIZE_MAX`.


#define U8_MAX   (256ULL)
#define U8_MIN   (0ULL)
#define U16_MAX  (65535ULL)
#define U16_MIN  (0ULL)
#define U32_MAX  (4294967295ULL)
#define U32_MIN  (0ULL)
#define U64_MAX  (18446744073709551615ULL)
#define U64_MIN  (0ULL)

#define I8_MAX   (127LL)
#define I8_MIN   (-I8_MAX -1)
#define I16_MAX  (32767LL)
#define I16_MIN  (-I16_MAX -1)
#define I32_MAX  (2147483647LL)
#define I32_MIN  (-I32_MAX -1)
#define I64_MAX  (9223372036854775807LL)
#define I64_MIN  (-I64_MAX - 1)


typedef unsigned char u8;
typedef signed char   i8;

#if USHRT_MAX == (U16_MAX)
    typedef unsigned short u16;
    typedef signed short   i16;
#elif UINT_MAX == (U16_MAX)
    typedef unsigned int u16;
    typedef signed int   i16;
#elif ULONG_MAX == (U16_MAX)
    typedef unsigned long u16;
    typedef signed long   i16;
#elif ULLONG_MAX == (U16_MAX)
    typedef unsigned long long u16;
    typedef signed long long   i16;
#else
    #error "No 16-bit integer type available."
#endif

#if USHRT_MAX == (U32_MAX)
    typedef unsigned short u32;
    typedef signed short   i32;
#elif UINT_MAX == (U32_MAX)
    typedef unsigned int u32;
    typedef signed int   i32;
#elif ULONG_MAX == (U32_MAX)
    typedef unsigned long u32;
    typedef signed long   i32;
#elif ULLONG_MAX == (U32_MAX)
    typedef unsigned long long u32;
    typedef signed long long   i32;
#else
    #error "No 32-bit integer type available."
#endif

#if USHRT_MAX == (U64_MAX)
    typedef unsigned short u64;
    typedef signed short   i64;
#elif UINT_MAX == (U64_MAX)
    typedef unsigned int u64;
    typedef signed int   i64;
#elif ULONG_MAX == (U64_MAX)
    typedef unsigned long u64;
    typedef signed long   i64;
#elif ULLONG_MAX == (U64_MAX)
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


#ifdef CCC_ENABLE_SIZE128
    #define U128_MAX (340282366920938463463374607431768211455ULL)
#endif

#if SIZE_MAX == U16_MAX
    #define USIZE_MAX U16_MAX
    #define USIZE_MIN U16_MIN
    #define ISIZE_MAX I16_MAX
    #define ISIZE_MIN I16_MIN
    typedef u16 usize;
    typedef i16 isize;
#elif SIZE_MAX > U16_MAX && SIZE_MAX == U32_MAX
    #define USIZE_MAX U32_MAX
    #define USIZE_MIN U32_MIN
    #define ISIZE_MAX I32_MAX
    #define ISIZE_MIN I32_MIN
    typedef u32 usize;
    typedef i32 isize;
#elif SIZE_MAX > U32_MAX && SIZE_MAX == U64_MAX
    #define USIZE_MAX U64_MAX
    #define USIZE_MIN U64_MIN
    #define ISIZE_MAX I64_MAX
    #define ISIZE_MIN I64_MIN
    typedef u64 usize;
    typedef i64 isize;
#elif defined(CCC_ENABLE_SIZE128) && SIZE_MAX > U64_MAX && SIZE_MAX == U128_MAX
    #define USIZE_MAX U128_MAX
    #define USIZE_MIN U128_MIN
    #define ISIZE_MAX I128_MAX
    #define ISIZE_MIN I128_MIN
    typedef uint128_t usize;
    typedef int128_t  isize;
#else
    #error "No size-width integer type available."
#endif

#ifdef CCC_ENABLE_SIZE128
    #undef U128_MAX
#endif


typedef uintptr_t uptr;
typedef intptr_t  iptr;


typedef float  f32;
typedef double f64;


#if __STDC_VERSION__ < 202000L
    #if __bool_true_false_are_defined
        #undef bool
        #undef true
        #undef false
        #undef __bool_true_false_are_defined
    #endif

    typedef _Bool bool;
    #define true  ((bool)1)
    #define false ((bool)0)
#endif


/// Empty (placeholder) type.
typedef struct { u8 _; } __unit;
#define unit ((__unit){ ._ = 0 })


#ifdef CCC_RESTRICT_REFERENCES
    #define ref const* restrict
    #define ref_mut * restrict
#else
    #define ref const*
    #define ref_mut *
#endif


/// Bounded string reference.
typedef struct {
    char ref dat;
    usize len;
} str;


/// Mutable bounded string reference.
typedef struct {
    char ref_mut dat;
    usize len;
} str_mut;
