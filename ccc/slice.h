#pragma once

#include "intrinsics.h"
#include "option.h"
#include "primitive.h"


/// Creates and initialises an array of type `T` and length `length`, returning
/// a slice to the contents of the array. The contents of the array are
/// initialised with the remaining arguments and the rest is zero-initialised.
#define array(T, length, ...)          \
{                                      \
    .ptr = (T[length]){ __VA_ARGS__ }, \
    .len = length,                     \
}


#ifdef CCC_RESTRICT_REFERENCES
    #define DEF_SLICE_NAMED(type, typename, typename_mut) \
    typedef struct {                                      \
        type const* restrict ptr;                         \
        usize len;                                        \
    } typename;                                           \
                                                          \
    typedef struct {                                      \
        type* restrict ptr;                               \
        usize len;                                        \
    } typename_mut
#else
    #define DEF_SLICE_NAMED(type, typename)               \
    typedef struct {                                      \
        type const* ptr;                                  \
        usize len;                                        \
    } typename;                                           \
                                                          \
    typedef struct {                                      \
        type* ptr;                                        \
        usize len;                                        \
    } typename_mut
#endif


#define DEF_SLICE(type) DEF_SLICE_NAMED(type, Slice_##type, SliceMut_##type)


DEF_SLICE(u8);
DEF_SLICE(i8);
DEF_SLICE(u16);
DEF_SLICE(i16);
DEF_SLICE(u32);
DEF_SLICE(i32);
DEF_SLICE(u64);
DEF_SLICE(i64);

DEF_SLICE(usize);
DEF_SLICE(isize);

DEF_SLICE(uptr);
DEF_SLICE(iptr);

DEF_SLICE(bool);
DEF_SLICE_NAMED(__unit, Slice_, SliceMut_);


INLINE_ALWAYS
Slice_u8 bytes_from_mut(SliceMut_u8 const self)
{
    return (Slice_u8){
        .ptr = self.ptr,
        .len = self.len,
    };
}


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
            .ptr = self.ptr,
            .len = idx,
        },
        ._1 = (Slice_u8){
            .ptr = self.ptr + idx,
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
            .ptr = self.ptr,
            .len = idx,
        },
        ._1 = (SliceMut_u8){
            .ptr = self.ptr + idx,
            .len = self.len - idx,
        },
    };
}


INLINE_ALWAYS
void bytes_fill(SliceMut_u8 const self, u8 const val)
{
    ccc_memset(self.ptr, val, self.len);
}


INLINE_ALWAYS
bool bytes_eq(Slice_u8 const a, Slice_u8 const b)
{
    return ccc_mem_eq(a.ptr, b.ptr, a.len);
}


inline
Option_usize bytes_find_u8(Slice_u8 const self, u8 const b)
{
    for (usize i = 0; i < self.len; i++) {
        if (self.ptr[i] == b) {
            return (Option_usize) wrap_some(i);
        }
    }

    return (Option_usize) none;
}


inline
Option_usize bytes_find(
    Slice_u8 const self,
    bool (* const predicate)(u8 val, usize index)
)
{
    for (usize i = 0; i < self.len; i++) {
        bool const result = predicate(self.ptr[i], i);
        if (result) {
            return (Option_usize) wrap_some(i);
        }
    }

    return (Option_usize) none;
}


/// Returns a sub-slice of `self`, spanning the range [`start`, `end`).
INLINE_ALWAYS
Slice_u8 bytes_slice(Slice_u8 const self, usize const start, usize const end)
{
    usize const checked_end = end >= start ? end : start;

    ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (Slice_u8){
        .ptr = self.ptr + start,
        .len = checked_end - start,
    };
}


/// Returns a mutable sub-slice of `self`, spanning the range [`start`, `end`).
INLINE_ALWAYS
SliceMut_u8 bytes_slice_mut(
    SliceMut_u8 const self,
    usize const start,
    usize const end
)
{
    usize const checked_end = end >= start ? end : start;

    ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (SliceMut_u8){
        .ptr = self.ptr + start,
        .len = checked_end - start,
    };
}


/// Returns a sub-slice of `self`, spanning the range [`start`, `end`].
INLINE_ALWAYS
Slice_u8 bytes_slice_incl(
    Slice_u8 const self,
    usize const start,
    usize const end
)
{
    usize const checked_end = end >= start ? end : start;

    ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (Slice_u8){
        .ptr = self.ptr + start,
        .len = checked_end - start + 1,
    };
}


/// Returns a mutable sub-slice of `self`, spanning the range [`start`, `end`].
INLINE_ALWAYS
SliceMut_u8 bytes_slice_incl_mut(
    SliceMut_u8 const self,
    usize const start,
    usize const end
)
{
    usize const checked_end = end >= start ? end : start;

    ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (SliceMut_u8){
        .ptr = self.ptr + start,
        .len = checked_end - start + 1,
    };
}
