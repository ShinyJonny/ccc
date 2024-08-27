#pragma once

#include "ref.h"
#include "intrinsics.h"
#include "option.h"
#include "primitive.h"
#include "cmp.h"


/// Creates and initialises an array of type `T` and length `length`, returning
/// a slice to the contents of the array. The contents of the array are
/// initialised with the remaining arguments and the rest is zero-initialised.
#define array(T, length, ...)          \
{                                      \
    .ptr = (T[length]){ __VA_ARGS__ }, \
    .len = length,                     \
}


#define DEF_ARRAY_NAMED(type, typename, typename_slice, typename_slice_mut, array_prefix, slice_prefix) \
typedef struct {                                                            \
    type* ptr;                                                              \
    usize len;                                                              \
} typename;                                                                 \
                                                                            \
typedef struct {                                                            \
    type ref dat;                                                           \
    usize len;                                                              \
} typename_slice;                                                           \
                                                                            \
typedef struct {                                                            \
    type ref_mut dat;                                                       \
    usize len;                                                              \
} typename_slice_mut;                                                       \
                                                                            \
INLINE_ALWAYS                                                               \
typename_slice array_prefix##_as_ref(typename ref const self)               \
{                                                                           \
    return (typename_slice){                                                \
        .dat = self->ptr,                                                   \
        .len = self->len,                                                   \
    };                                                                      \
}                                                                           \
                                                                            \
INLINE_ALWAYS                                                               \
typename_slice_mut array_prefix##_as_mut(typename ref_mut const self)       \
{                                                                           \
    return (typename_slice_mut){                                            \
        .dat = self->ptr,                                                   \
        .len = self->len,                                                   \
    };                                                                      \
}                                                                           \
                                                                            \
INLINE_ALWAYS                                                               \
typename_slice slice_prefix##_from_mut(typename_slice_mut const self)       \
{                                                                           \
    return (typename_slice){                                                \
        .dat = self.dat,                                                    \
        .len = self.len,                                                    \
    };                                                                      \
}

#define DEF_ARRAY(type, impl_name) \
    DEF_ARRAY_NAMED(type, Array_##type, Slice_##type, SliceMut_##type, array_##impl_name, slice_##impl_name)


DEF_ARRAY(u8, u8)
DEF_ARRAY(i8, i8)
DEF_ARRAY(u16, u16)
DEF_ARRAY(i16, i16)
DEF_ARRAY(u32, u32)
DEF_ARRAY(i32, i32)
DEF_ARRAY(u64, u64)
DEF_ARRAY(i64, i64)

DEF_ARRAY(usize, usize)
DEF_ARRAY(isize, isize)
DEF_ARRAY(uptr, uptr)
DEF_ARRAY(iptr, iptr)

DEF_ARRAY(f32, f32)
DEF_ARRAY(f64, f64)

DEF_ARRAY(bool, bool)
DEF_ARRAY(__unit, __unit)


typedef struct {
    Slice_u8 _0;
    Slice_u8 _1;
} ByteSplit;


INLINE_ALWAYS
ByteSplit bytes_split_at(Slice_u8 const self, usize const idx)
{
    ASSERT_MSG(idx <= self.len, "index out of bounds");

    return (ByteSplit){
        ._0 = (Slice_u8){
            .dat = self.dat,
            .len = idx,
        },
        ._1 = (Slice_u8){
            .dat = self.dat + idx,
            .len = self.len - idx,
        },
    };
}


typedef struct {
    SliceMut_u8 _0;
    SliceMut_u8 _1;
} ByteSplitMut;


INLINE_ALWAYS
ByteSplitMut bytes_split_at_mut(SliceMut_u8 const self, usize const idx)
{
    ASSERT_MSG(idx <= self.len, "index out of bounds");

    return (ByteSplitMut){
        ._0 = (SliceMut_u8){
            .dat = self.dat,
            .len = idx,
        },
        ._1 = (SliceMut_u8){
            .dat = self.dat + idx,
            .len = self.len - idx,
        },
    };
}


INLINE_ALWAYS
void slice_u8_fill(SliceMut_u8 const self, u8 const val)
{
    ccc_memset(self.dat, val, self.len);
}


INLINE_ALWAYS
bool slice_u8_eq(Slice_u8 const a, Slice_u8 const b)
{
    return ccc_mem_eq(a.dat, b.dat, a.len);
}


inline
Option_usize slice_u8_find_u8(Slice_u8 const self, u8 const b)
{
    for (usize i = 0; i < self.len; i++) {
        if (self.dat[i] == b) {
            return (Option_usize) SOME(i);
        }
    }

    return (Option_usize) NONE;
}


inline
Option_usize slice_u8_find(
    Slice_u8 const self,
    bool (* const predicate)(u8 val, usize index)
)
{
    for (usize i = 0; i < self.len; i++) {
        bool const result = predicate(self.dat[i], i);
        if (result) {
            return (Option_usize) SOME(i);
        }
    }

    return (Option_usize) NONE;
}


/// Returns a sub-slice of `self`, spanning the range [`start`, `end`).
INLINE_ALWAYS
Slice_u8 slice_u8_slice(Slice_u8 const self, usize const start, usize const end)
{
    usize const checked_end = usize_max(start, end);

    ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (Slice_u8){
        .dat = self.dat + start,
        .len = checked_end - start,
    };
}


/// Returns a mutable sub-slice of `self`, spanning the range [`start`, `end`).
INLINE_ALWAYS
SliceMut_u8 slice_u8_slice_mut(
    SliceMut_u8 const self,
    usize const start,
    usize const end
)
{
    usize const checked_end = usize_max(start, end);

    ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (SliceMut_u8){
        .dat = self.dat + start,
        .len = checked_end - start,
    };
}


/// Returns a sub-slice of `self`, spanning the range [`start`, `end`].
INLINE_ALWAYS
Slice_u8 slice_u8_slice_incl(
    Slice_u8 const self,
    usize const start,
    usize const end
)
{
    usize const checked_end = usize_max(start, end);

    ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (Slice_u8){
        .dat = self.dat + start,
        .len = checked_end - start + 1,
    };
}


/// Returns a mutable sub-slice of `self`, spanning the range [`start`, `end`].
INLINE_ALWAYS
SliceMut_u8 slice_u8_slice_incl_mut(
    SliceMut_u8 const self,
    usize const start,
    usize const end
)
{
    usize const checked_end = usize_max(start, end);

    ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (SliceMut_u8){
        .dat = self.dat + start,
        .len = checked_end - start + 1,
    };
}
