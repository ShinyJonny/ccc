#pragma once

#include "meta.h"
#include "primitive.h"


#define def_numeric_eq_min_max(type)        \
INLINE_ALWAYS                               \
bool type##_eq(type const a, type const b)  \
{                                           \
    return a == b;                          \
}                                           \
                                            \
INLINE_ALWAYS                               \
type type##_min(type const a, type const b) \
{                                           \
    return a <= b                           \
        ? a                                 \
        : b;                                \
}                                           \
                                            \
INLINE_ALWAYS                               \
type type##_max(type const a, type const b) \
{                                           \
    return a <= b                           \
        ? b                                 \
        : a;                                \
}

def_numeric_eq_min_max(u8)
def_numeric_eq_min_max(i8)
def_numeric_eq_min_max(u16)
def_numeric_eq_min_max(i16)
def_numeric_eq_min_max(u32)
def_numeric_eq_min_max(i32)
def_numeric_eq_min_max(u64)
def_numeric_eq_min_max(i64)
def_numeric_eq_min_max(usize)
def_numeric_eq_min_max(isize)
def_numeric_eq_min_max(uptr)
def_numeric_eq_min_max(iptr)

// TODO: f32
// TODO: f64

def_numeric_eq_min_max(bool)

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

#undef def_numeric_eq_min_max
