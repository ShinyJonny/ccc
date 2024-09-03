#pragma once


#include "meta.h"
#include "primitive.h"


#define _ccc_def_numeric_eq_min_max(type) \
INLINE_ALWAYS                                                                  \
bool type##_eq(type const a, type const b)                                     \
{                                                                              \
    return a == b;                                                             \
}                                                                              \
                                                                               \
INLINE_ALWAYS                                                                  \
type type##_min(type const a, type const b)                                    \
{                                                                              \
    return a <= b                                                              \
        ? a                                                                    \
        : b;                                                                   \
}                                                                              \
                                                                               \
INLINE_ALWAYS                                                                  \
type type##_max(type const a, type const b)                                    \
{                                                                              \
    return a <= b                                                              \
        ? b                                                                    \
        : a;                                                                   \
}                                                                              \


#define _ccc_decl_numeric_eq_min_max(type) \
bool type##_eq(type const a, type const b);                                    \
type type##_min(type const a, type const b);                                   \
type type##_max(type const a, type const b);                                   \


_ccc_def_numeric_eq_min_max(u8)
_ccc_def_numeric_eq_min_max(i8)
_ccc_def_numeric_eq_min_max(u16)
_ccc_def_numeric_eq_min_max(i16)
_ccc_def_numeric_eq_min_max(u32)
_ccc_def_numeric_eq_min_max(i32)
_ccc_def_numeric_eq_min_max(u64)
_ccc_def_numeric_eq_min_max(i64)
_ccc_def_numeric_eq_min_max(usize)
_ccc_def_numeric_eq_min_max(isize)
_ccc_def_numeric_eq_min_max(uptr)
_ccc_def_numeric_eq_min_max(iptr)

// TODO: f32
// TODO: f64

_ccc_def_numeric_eq_min_max(bool)


INLINE_ALWAYS
bool __unit_eq(__unit const a, __unit const b)
{
    (void)a; (void)b;
    return true;
}

INLINE_ALWAYS
__unit __unit_min(__unit const a, __unit const b)
{
    (void)a; (void)b;
    return unit;
}

INLINE_ALWAYS
__unit __unit_max(__unit const a, __unit const b)
{
    (void)a; (void)b;
    return unit;
}


#undef _ccc_def_numeric_eq_min_max


#ifdef CCC_IMPLEMENTATION
_ccc_decl_numeric_eq_min_max(u8)
_ccc_decl_numeric_eq_min_max(i8)
_ccc_decl_numeric_eq_min_max(u16)
_ccc_decl_numeric_eq_min_max(i16)
_ccc_decl_numeric_eq_min_max(u32)
_ccc_decl_numeric_eq_min_max(i32)
_ccc_decl_numeric_eq_min_max(u64)
_ccc_decl_numeric_eq_min_max(i64)
_ccc_decl_numeric_eq_min_max(usize)
_ccc_decl_numeric_eq_min_max(isize)
_ccc_decl_numeric_eq_min_max(uptr)
_ccc_decl_numeric_eq_min_max(iptr)

// TODO: f32
// TODO: f64

_ccc_decl_numeric_eq_min_max(bool)

bool __unit_eq(__unit const a, __unit const b);
__unit __unit_min(__unit const a, __unit const b);
__unit __unit_max(__unit const a, __unit const b);
#endif


#undef _ccc_decl_numeric_eq_min_max
