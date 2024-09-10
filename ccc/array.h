#pragma once


#include "meta.h"
#include "intrinsics.h"
#include "assertions.h"
#include "option.h"
#include "primitive.h"
#include "cmp.h"


/// Creates and initialises an array of type `T` and length `length`, returning
/// a slice to the contents of the array. The contents of the array are
/// initialised with the remaining arguments and the rest is zero-initialised.
#define ccc_array(T, length, ...) \
{                                                                              \
    .dat = (T[length]){ __VA_ARGS__ },                                         \
    .len = length,                                                             \
}                                                                              \


#define CCC_DEF_ARRAY_NAMED(type, typename, typename_slice, \
    typename_slice_mut, array_prefix, slice_prefix) \
typedef struct {                                                               \
    type ref_mut dat;                                                          \
    usize len;                                                                 \
} typename;                                                                    \
                                                                               \
typedef struct {                                                               \
    type ref dat;                                                              \
    usize len;                                                                 \
} typename_slice;                                                              \
                                                                               \
typedef struct {                                                               \
    type ref_mut dat;                                                          \
    usize len;                                                                 \
} typename_slice_mut;                                                          \
                                                                               \
CCC_INLINE_ALWAYS                                                              \
typename_slice array_prefix##_as_slice(typename ref const self)                \
{                                                                              \
    return (typename_slice) {                                                  \
        .dat = self->dat,                                                      \
        .len = self->len,                                                      \
    };                                                                         \
}                                                                              \
                                                                               \
CCC_INLINE_ALWAYS                                                              \
typename_slice_mut array_prefix##_as_slice_mut(typename ref_mut const self)    \
{                                                                              \
    return (typename_slice_mut) {                                              \
        .dat = self->dat,                                                      \
        .len = self->len,                                                      \
    };                                                                         \
}                                                                              \
                                                                               \
CCC_INLINE_ALWAYS                                                              \
typename_slice slice_prefix##_from_mut(typename_slice_mut const self)          \
{                                                                              \
    return (typename_slice) {                                                  \
        .dat = self.dat,                                                       \
        .len = self.len,                                                       \
    };                                                                         \
}                                                                              \
                                                                               \
/** Returns a sub-slice of `self`, spanning the range [`start`, `end`). */     \
CCC_INLINE_ALWAYS                                                              \
typename_slice slice_prefix##_slice(                                           \
    typename_slice const self,                                                 \
    usize const start,                                                         \
    usize const end                                                            \
)                                                                              \
{                                                                              \
    usize const checked_end = usize_max(&start, &end);                         \
                                                                               \
    CCC_ASSERT_MSG(checked_end <= self.len, "slice out of bounds");            \
                                                                               \
    return (typename_slice) {                                                  \
        .dat = self.dat + start,                                               \
        .len = checked_end - start,                                            \
    };                                                                         \
}                                                                              \
                                                                               \
/** Returns a mutable sub-slice of `self`, spanning the range */               \
/** [`start`, `end`). */                                                       \
CCC_INLINE_ALWAYS                                                              \
typename_slice_mut slice_prefix##_slice_mut(                                   \
    typename_slice_mut const self,                                             \
    usize const start,                                                         \
    usize const end                                                            \
)                                                                              \
{                                                                              \
    usize const checked_end = usize_max(&start, &end);                         \
                                                                               \
    CCC_ASSERT_MSG(checked_end <= self.len, "slice out of bounds");            \
                                                                               \
    return (typename_slice_mut) {                                              \
        .dat = self.dat + start,                                               \
        .len = checked_end - start,                                            \
    };                                                                         \
}                                                                              \
                                                                               \
/** Returns a sub-slice of `self`, spanning the range [`start`, `end`]. */     \
CCC_INLINE_ALWAYS                                                              \
typename_slice slice_prefix##_slice_incl(                                      \
    typename_slice const self,                                                 \
    usize const start,                                                         \
    usize const end                                                            \
)                                                                              \
{                                                                              \
    usize const checked_end = usize_max(&start, &end);                         \
                                                                               \
    CCC_ASSERT_MSG(checked_end < self.len, "slice out of bounds");             \
                                                                               \
    return (typename_slice) {                                                  \
        .dat = self.dat + start,                                               \
        .len = checked_end - start,                                            \
    };                                                                         \
}                                                                              \
                                                                               \
/** Returns a mutable sub-slice of `self`, spanning the range */               \
/** [`start`, `end`]. */                                                       \
CCC_INLINE_ALWAYS                                                              \
typename_slice_mut slice_prefix##_slice_incl_mut(                              \
    typename_slice_mut const self,                                             \
    usize const start,                                                         \
    usize const end                                                            \
)                                                                              \
{                                                                              \
    usize const checked_end = usize_max(&start, &end);                         \
                                                                               \
    CCC_ASSERT_MSG(checked_end < self.len, "slice out of bounds");             \
                                                                               \
    return (typename_slice_mut) {                                              \
        .dat = self.dat + start,                                               \
        .len = checked_end - start,                                            \
    };                                                                         \
}                                                                              \

#define CCC_DEF_ARRAY(type, impl_name) \
    CCC_DEF_ARRAY_NAMED(type, Array_##type, Slice_##type, SliceMut_##type, \
        array_##impl_name, slice_##impl_name)


#define CCC_DECL_ARRAY_NAMED(type, typename, typename_slice, \
    typename_slice_mut, array_prefix, slice_prefix) \
typename_slice array_prefix##_as_slice(typename ref const self);               \
typename_slice_mut array_prefix##_as_slice_mut(typename ref_mut const self);   \
typename_slice slice_prefix##_from_mut(typename_slice_mut const self);         \
typename_slice slice_prefix##_slice(                                           \
    typename_slice const self,                                                 \
    usize const start,                                                         \
    usize const end                                                            \
);                                                                             \
typename_slice_mut slice_prefix##_slice_mut(                                   \
    typename_slice_mut const self,                                             \
    usize const start,                                                         \
    usize const end                                                            \
);                                                                             \
typename_slice slice_prefix##_slice_incl(                                      \
    typename_slice const self,                                                 \
    usize const start,                                                         \
    usize const end                                                            \
);                                                                             \
typename_slice_mut slice_prefix##_slice_incl_mut(                              \
    typename_slice_mut const self,                                             \
    usize const start,                                                         \
    usize const end                                                            \
);                                                                             \

#define CCC_DECL_ARRAY(type, impl_name) \
    CCC_DECL_ARRAY_NAMED(type, Array_##type, Slice_##type, SliceMut_##type, \
        array_##impl_name, slice_##impl_name)


CCC_DEF_ARRAY(u8, u8)
CCC_DEF_ARRAY(i8, i8)
CCC_DEF_ARRAY(u16, u16)
CCC_DEF_ARRAY(i16, i16)
CCC_DEF_ARRAY(u32, u32)
CCC_DEF_ARRAY(i32, i32)
CCC_DEF_ARRAY(u64, u64)
CCC_DEF_ARRAY(i64, i64)

CCC_DEF_ARRAY(usize, usize)
CCC_DEF_ARRAY(isize, isize)
CCC_DEF_ARRAY(uptr, uptr)
CCC_DEF_ARRAY(iptr, iptr)

CCC_DEF_ARRAY(f32, f32)
CCC_DEF_ARRAY(f64, f64)

CCC_DEF_ARRAY(bool, bool)
CCC_DEF_ARRAY(__unit, __unit)

CCC_DEF_ARRAY(str, str)
CCC_DEF_ARRAY(str_mut, str_mut)

inline
Option_usize slice_u8_find(
    Slice_u8 const self,
    u8 const needle
)
{
    for (usize i = 0; i < self.len; i++) {
        if (self.dat[i] == needle) {
            return (Option_usize) CCC_SOME(i);
        }
    }

    return (Option_usize) CCC_NONE;
}

inline
Option_usize slice_u8_find_by(
    Slice_u8 const self,
    bool (* const predicate)(u8 ref item, usize index)
)
{
    for (usize i = 0; i < self.len; i++) {
        if (predicate(&self.dat[i], i)) {
            return (Option_usize) CCC_SOME(i);
        }
    }

    return (Option_usize) CCC_NONE;
}


typedef struct {
    Slice_u8 _0;
    Slice_u8 _1;
} Pair_Slice_u8;

CCC_INLINE_ALWAYS
Pair_Slice_u8 slice_u8_split_at(Slice_u8 const self, usize const idx)
{
    CCC_ASSERT_MSG(idx <= self.len, "index out of bounds");

    return (Pair_Slice_u8) {
        ._0 = (Slice_u8) {
            .dat = self.dat,
            .len = idx,
        },
        ._1 = (Slice_u8) {
            .dat = self.dat + idx,
            .len = self.len - idx,
        },
    };
}


typedef struct {
    SliceMut_u8 _0;
    SliceMut_u8 _1;
} Pair_SliceMut_u8;

CCC_INLINE_ALWAYS
Pair_SliceMut_u8 slice_u8_split_at_mut(SliceMut_u8 const self, usize const idx)
{
    CCC_ASSERT_MSG(idx <= self.len, "index out of bounds");

    return (Pair_SliceMut_u8) {
        ._0 = (SliceMut_u8) {
            .dat = self.dat,
            .len = idx,
        },
        ._1 = (SliceMut_u8) {
            .dat = self.dat + idx,
            .len = self.len - idx,
        },
    };
}


CCC_INLINE_ALWAYS
void slice_u8_fill(SliceMut_u8 const self, u8 const val)
{
    ccc_memset(self.dat, val, self.len);
}


CCC_INLINE_ALWAYS
bool slice_u8_eq(Slice_u8 const a, Slice_u8 const b)
{
    return ccc_mem_eq(a.dat, b.dat, a.len);
}


#ifdef CCC_IMPLEMENTATION
CCC_DECL_ARRAY(u8, u8)
CCC_DECL_ARRAY(i8, i8)
CCC_DECL_ARRAY(u16, u16)
CCC_DECL_ARRAY(i16, i16)
CCC_DECL_ARRAY(u32, u32)
CCC_DECL_ARRAY(i32, i32)
CCC_DECL_ARRAY(u64, u64)
CCC_DECL_ARRAY(i64, i64)

CCC_DECL_ARRAY(usize, usize)
CCC_DECL_ARRAY(isize, isize)
CCC_DECL_ARRAY(uptr, uptr)
CCC_DECL_ARRAY(iptr, iptr)

CCC_DECL_ARRAY(f32, f32)
CCC_DECL_ARRAY(f64, f64)

CCC_DECL_ARRAY(bool, bool)
CCC_DECL_ARRAY(__unit, __unit)

CCC_DECL_ARRAY(str, str)
CCC_DECL_ARRAY(str_mut, str_mut)


Pair_Slice_u8 slice_u8_split_at(Slice_u8 const self, usize const idx);
Pair_SliceMut_u8 slice_u8_split_at_mut(SliceMut_u8 const self, usize const idx);
void slice_u8_fill(SliceMut_u8 const self, u8 const val);
bool slice_u8_eq(Slice_u8 const a, Slice_u8 const b);
#endif
