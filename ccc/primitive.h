#pragma once


#include "meta.h"

#include <limits.h> /// NOTE: used only for `*_MAX`.
#include <stdint.h> /// NOTE: used only for `SIZE_MAX`.


#define CCC_U8_MAX   (256ULL)
#define CCC_U8_MIN   (0ULL)
#define CCC_U16_MAX  (65535ULL)
#define CCC_U16_MIN  (0ULL)
#define CCC_U32_MAX  (4294967295ULL)
#define CCC_U32_MIN  (0ULL)
#define CCC_U64_MAX  (18446744073709551615ULL)
#define CCC_U64_MIN  (0ULL)

#define CCC_I8_MAX   (127LL)
#define CCC_I8_MIN   (-CCC_I8_MAX -1ULL)
#define CCC_I16_MAX  (32767LL)
#define CCC_I16_MIN  (-CCC_I16_MAX -1ULL)
#define CCC_I32_MAX  (2147483647LL)
#define CCC_I32_MIN  (-CCC_I32_MAX -1ULL)
#define CCC_I64_MAX  (9223372036854775807LL)
#define CCC_I64_MIN  (-CCC_I64_MAX - 1ULL)


typedef unsigned char u8;
typedef signed char   i8;

#if USHRT_MAX == (CCC_U16_MAX)
    typedef unsigned short u16;
    typedef signed short   i16;
#elif UINT_MAX == (CCC_U16_MAX)
    typedef unsigned int u16;
    typedef signed int   i16;
#elif ULONG_MAX == (CCC_U16_MAX)
    typedef unsigned long u16;
    typedef signed long   i16;
#elif ULLONG_MAX == (CCC_U16_MAX)
    typedef unsigned long long u16;
    typedef signed long long   i16;
#else
    #error "No 16-bit integer type available."
#endif

#if USHRT_MAX == (CCC_U32_MAX)
    typedef unsigned short u32;
    typedef signed short   i32;
#elif UINT_MAX == (CCC_U32_MAX)
    typedef unsigned int u32;
    typedef signed int   i32;
#elif ULONG_MAX == (CCC_U32_MAX)
    typedef unsigned long u32;
    typedef signed long   i32;
#elif ULLONG_MAX == (CCC_U32_MAX)
    typedef unsigned long long u32;
    typedef signed long long   i32;
#else
    #error "No 32-bit integer type available."
#endif

#if USHRT_MAX == (CCC_U64_MAX)
    typedef unsigned short u64;
    typedef signed short   i64;
#elif UINT_MAX == (CCC_U64_MAX)
    typedef unsigned int u64;
    typedef signed int   i64;
#elif ULONG_MAX == (CCC_U64_MAX)
    typedef unsigned long u64;
    typedef signed long   i64;
#elif ULLONG_MAX == (CCC_U64_MAX)
    typedef unsigned long long u64;
    typedef signed long long   i64;
#else
    #error "No 64-bit integer type available."
#endif


CCC_STATIC_ASSERT(sizeof (u8)  == 1, "invalid size of u8");
CCC_STATIC_ASSERT(sizeof (i8)  == 1, "invalid size of i8");
CCC_STATIC_ASSERT(sizeof (u16) == 2, "invalid size of u16");
CCC_STATIC_ASSERT(sizeof (i16) == 2, "invalid size of i16");
CCC_STATIC_ASSERT(sizeof (u32) == 4, "invalid size of u32");
CCC_STATIC_ASSERT(sizeof (i32) == 4, "invalid size of i32");
CCC_STATIC_ASSERT(sizeof (u64) == 8, "invalid size of u64");
CCC_STATIC_ASSERT(sizeof (i64) == 8, "invalid size of i64");


#ifdef CCC_ENABLE_SIZE128
    #define CCC_U128_MAX (340282366920938463463374607431768211455ULL)
#endif

#if SIZE_MAX == CCC_U16_MAX
    #define CCC_USIZE_MAX CCC_U16_MAX
    #define CCC_USIZE_MIN CCC_U16_MIN
    #define CCC_ISIZE_MAX CCC_I16_MAX
    #define CCC_ISIZE_MIN CCC_I16_MIN
    typedef u16 usize;
    typedef i16 isize;
#elif SIZE_MAX > CCC_U16_MAX && SIZE_MAX == CCC_U32_MAX
    #define CCC_USIZE_MAX CCC_U32_MAX
    #define CCC_USIZE_MIN CCC_U32_MIN
    #define CCC_ISIZE_MAX CCC_I32_MAX
    #define CCC_ISIZE_MIN CCC_I32_MIN
    typedef u32 usize;
    typedef i32 isize;
#elif SIZE_MAX > CCC_U32_MAX && SIZE_MAX == CCC_U64_MAX
    #define CCC_USIZE_MAX CCC_U64_MAX
    #define CCC_USIZE_MIN CCC_U64_MIN
    #define CCC_ISIZE_MAX CCC_I64_MAX
    #define CCC_ISIZE_MIN CCC_I64_MIN
    typedef u64 usize;
    typedef i64 isize;
#elif defined(CCC_ENABLE_SIZE128) && SIZE_MAX > CCC_U64_MAX && SIZE_MAX == CCC_U128_MAX
    #define CCC_USIZE_MAX CCC_U128_MAX
    #define CCC_USIZE_MIN CCC_U128_MIN
    #define CCC_ISIZE_MAX CCC_I128_MAX
    #define CCC_ISIZE_MIN CCC_I128_MIN
    typedef uint128_t usize;
    typedef int128_t  isize;
#else
    #error "No size-width integer type available."
#endif

#ifdef CCC_ENABLE_SIZE128
    #undef CCC_U128_MAX
#endif


typedef uintptr_t uptr;
typedef intptr_t  iptr;


typedef float  f32;
typedef double f64;


CCC_STATIC_ASSERT(sizeof (f32)  == 4, "invalid size of f32");
CCC_STATIC_ASSERT(sizeof (f64)  == 8, "invalid size of f64");


#if __STDC_VERSION__ < 202000L
    #if defined(__bool_true_false_are_defined) && __bool_true_false_are_defined
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
#define CCC_UNIT ((__unit){ 0 })


#ifdef CCC_RESTRICT_REFERENCES
    #define ref     const* CCC_RESTRICT
    #define ref_mut *      CCC_RESTRICT
#else
    #define ref     const*
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
