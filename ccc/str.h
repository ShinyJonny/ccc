#pragma once


#include "meta.h"
#include "primitive.h"
#include "array.h"
#include "option.h"
#include "cmp.h"


/// Gets the underlying bytes of `s`, safely cast to `u8`.
CCC_INLINE_ALWAYS
Slice_u8 str_as_bytes(str const self)
{
    return (Slice_u8){
        .dat = (u8 ref)self.dat,
        .len = self.len,
    };
}


/// Gets the underlying mutable bytes of `s`, safely cast to `u8`.
CCC_INLINE_ALWAYS
SliceMut_u8 str_as_bytes_mut(str_mut const self)
{
    return (SliceMut_u8){
        .dat = (u8 ref_mut)self.dat,
        .len = self.len,
    };
}


/// Constructs a `str` from a slice of `u8`.
CCC_INLINE_ALWAYS
str str_from_bytes(Slice_u8 const bytes)
{
    return (str){
        .dat = (char ref)bytes.dat,
        .len = bytes.len,
    };
}


/// Constructs a `str` from a mutable slice of `u8`.
CCC_INLINE_ALWAYS
str_mut str_from_bytes_mut(SliceMut_u8 const bytes)
{
    return (str_mut){
        .dat = (char ref_mut)bytes.dat,
        .len = bytes.len,
    };
}


typedef struct {
    str _0;
    str _1;
} Pair_str;


/// Splits a string at the specified index.
CCC_INLINE_ALWAYS
Pair_str str_split_at(str const self, usize const idx)
{
    CCC_ASSERT_MSG(idx <= self.len, "index out of bounds");

    return (Pair_str){
        ._0 = {
            .dat = self.dat,
            .len = idx,
        },
        ._1 = {
            .dat = self.dat + idx,
            .len = self.len - idx,
        },
    };
}


typedef struct {
    str_mut _0;
    str_mut _1;
} Pair_str_mut;


/// Splits a string at the specified index.
CCC_INLINE_ALWAYS
Pair_str_mut str_split_mut_at(str_mut const self, usize const idx)
{
    CCC_ASSERT_MSG(idx <= self.len, "index out of bounds");

    return (Pair_str_mut){
        ._0 = {
            .dat = self.dat,
            .len = idx,
        },
        ._1 = {
            .dat = self.dat + idx,
            .len = self.len - idx,
        },
    };
}


/// Finds the first occurence of `c` and returns its index.
inline
Option_usize str_find_char(str const self, char const c)
{
    for (usize i = 0; i < self.len; i++) {
        if (self.dat[i] == c) {
            return (Option_usize) CCC_SOME(i);
        }
    }

    return (Option_usize) CCC_NONE;
}


/// Finds the first `char` that satisfies the `predicate` and returns its index.
inline
Option_usize str_find(str const self, bool (* const predicate)(char, usize))
{
    for (usize i = 0; i < self.len; i++) {
        bool const found = predicate(self.dat[i], i);
        if (found) {
            return (Option_usize) CCC_SOME(i);
        }
    }

    return (Option_usize) CCC_NONE;
}


/// Returns a sub-str of `self`, spanning the range [`start`, `end`).
CCC_INLINE_ALWAYS
str str_slice(str const self, usize const start, usize const end)
{
    usize const checked_end = usize_max(&start, &end);

    CCC_ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (str){
        .dat = self.dat + start,
        .len = checked_end - start,
    };
}


/// Returns a mutable sub-str of `self`, spanning the range [`start`, `end`).
CCC_INLINE_ALWAYS
str_mut str_slice_mut(str_mut const self, usize const start, usize const end)
{
    usize const checked_end = usize_max(&start, &end);

    CCC_ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (str_mut){
        .dat = self.dat + start,
        .len = checked_end - start,
    };
}


/// Returns a sub-str of `self`, spanning the range [`start`, `end`].
CCC_INLINE_ALWAYS
str str_slice_incl(str const self, usize const start, usize const end)
{
    usize const checked_end = usize_max(&start, &end);

    CCC_ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (str){
        .dat = self.dat + start,
        .len = checked_end - start + 1,
    };
}


/// Returns a mutable sub-str of `self`, spanning the range [`start`, `end`].
CCC_INLINE_ALWAYS
str_mut str_slice_incl_mut(
    str_mut const self,
    usize const start,
    usize const end
)
{
    usize const checked_end = usize_max(&start, &end);

    CCC_ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (str_mut){
        .dat = self.dat + start,
        .len = checked_end - start + 1,
    };
}


#ifdef CCC_IMPLEMENTATION
Slice_u8 str_as_bytes(str const self);
SliceMut_u8 str_as_bytes_mut(str_mut const self);
str str_from_bytes(Slice_u8 const bytes);
str_mut str_from_bytes_mut(SliceMut_u8 const bytes);
Pair_str str_split_at(str const self, usize const idx);
Pair_str_mut str_split_mut_at(str_mut const self, usize const idx);
Option_usize str_find_char(str const self, char const c);
Option_usize str_find(str const self, bool (* const predicate)(char, usize));
str str_slice(str const self, usize const start, usize const end);
str_mut str_slice_mut(str_mut const self, usize const start, usize const end);
str str_slice_incl(str const self, usize const start, usize const end);
str_mut str_slice_incl_mut(
    str_mut const self,
    usize const start,
    usize const end
);
#endif
