#pragma once


#include "meta.h"
#include "primitive.h"


#define _ccc_def_numeric_eq_min_max(type) \
CCC_INLINE_ALWAYS                                                              \
bool type##_eq(type ref const a, type ref const b)                             \
{                                                                              \
    return *a == *b;                                                           \
}                                                                              \
                                                                               \
CCC_INLINE_ALWAYS                                                              \
type type##_min(type ref const a, type ref const b)                            \
{                                                                              \
    return *a <= *b                                                            \
        ? *a                                                                   \
        : *b;                                                                  \
}                                                                              \
                                                                               \
CCC_INLINE_ALWAYS                                                              \
type type##_max(type ref const a, type ref const b)                            \
{                                                                              \
    return *a <= *b                                                            \
        ? *b                                                                   \
        : *a;                                                                  \
}                                                                              \


#define _ccc_decl_numeric_eq_min_max(type) \
bool type##_eq(type ref const a, type ref const b);                            \
type type##_min(type ref const a, type ref const b);                           \
type type##_max(type ref const a, type ref const b);                           \


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


CCC_INLINE_ALWAYS
bool __unit_eq(__unit ref const a, __unit ref const b)
{
    (void)a; (void)b;
    return true;
}

CCC_INLINE_ALWAYS
__unit __unit_min(__unit ref const a, __unit ref const b)
{
    (void)a; (void)b;
    return CCC_UNIT;
}

CCC_INLINE_ALWAYS
__unit __unit_max(__unit ref const a, __unit ref const b)
{
    (void)a; (void)b;
    return CCC_UNIT;
}


/// Checks if `a` and `b` are equal.
CCC_INLINE_ALWAYS
bool str_eq(str ref const a, str ref const b)
{
    if (a->len != b->len) {
        return false;
    }

    usize const len = a->len;
    return ccc_mem_eq((u8 ref)a->dat, (u8 ref)b->dat, len);
}


/// Checks if `a` and `b` are equal.
CCC_INLINE_ALWAYS
bool str_mut_eq(str_mut ref const a, str_mut ref const b)
{
    if (a->len != b->len) {
        return false;
    }

    usize const len = a->len;
    return ccc_mem_eq((u8 ref)a->dat, (u8 ref)b->dat, len);
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


bool __unit_eq(__unit ref const a, __unit ref const b);
__unit __unit_min(__unit ref const a, __unit ref const b);
__unit __unit_max(__unit ref const a, __unit ref const b);
bool str_eq(str ref const a, str ref const b);
bool str_mut_eq(str_mut ref const a, str_mut ref const b);
#endif


#undef _ccc_decl_numeric_eq_min_max
