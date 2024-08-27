#pragma once

#include "primitive.h"
#include "ref.h"
#include "meta.h"
#include "array.h"
#include "option.h"
#include "intrinsics.h"
#include "cmp.h"


/// Bounded string reference.
typedef struct {
    char ref dat;
    usize len;
} str;


CCC_DEF_ARRAY(str, str)
CCC_DEF_OPTION(str)


/// Mutable bounded string reference.
typedef struct {
    char ref_mut dat;
    usize len;
} str_mut;


CCC_DEF_ARRAY(str_mut, str_mut)
CCC_DEF_OPTION(str_mut)


/// Helper for printing `str` and `str_mut`.
///
/// # Example
///
/// ```
/// str const s = make_str();
/// printf(STRF, PSTR(s));
/// ```
#define PSTR(s) (int)(s).len, (s).dat
/// Format literal for printing `str` and `str_mut`.
///
/// # Example
///
/// ```
/// str const s = make_str();
/// printf("This is a `str`: \"" STRF "\".\n", PSTR(s));
/// ```
#define STRF "%.*s"


/// Converts a C string to `str`.
///
/// C string literals can be easily converted using this helper.
INLINE_ALWAYS
str cstr(char ref const c_str)
{
    return (str){
        .dat = c_str,
        .len = ccc_c_str_len(c_str),
    };
}


/// Downcasts `str_mut` to `str`.
///
/// NOTE: this cannot be done the other way around.
INLINE_ALWAYS
str str_from_mut(str_mut const s)
{
    return (str){
        .dat = s.dat,
        .len = s.len,
    };
}


/// Gets the underlying bytes of `s`, safely cast to `u8`.
INLINE_ALWAYS
Slice_u8 str_as_bytes(str const self)
{
    return (Slice_u8){
        .dat = (u8 ref)self.dat,
        .len = self.len,
    };
}


/// Gets the underlying mutable bytes of `s`, safely cast to `u8`.
INLINE_ALWAYS
SliceMut_u8 str_as_bytes_mut(str_mut const self)
{
    return (SliceMut_u8){
        .dat = (u8 ref_mut)self.dat,
        .len = self.len,
    };
}


/// Constructs a `str` from a slice of `u8`.
INLINE_ALWAYS
str str_from_bytes(Slice_u8 const bytes)
{
    return (str){
        .dat = (char ref)bytes.dat,
        .len = bytes.len,
    };
}


/// Constructs a `str` from a mutable slice of `u8`.
INLINE_ALWAYS
str_mut str_from_bytes_mut(SliceMut_u8 const bytes)
{
    return (str_mut){
        .dat = (char ref_mut)bytes.dat,
        .len = bytes.len,
    };
}


/// Checks if strings `a` and `b` are equal.
INLINE_ALWAYS
bool str_eq(str const a, str const b)
{
    if (a.len != b.len) {
        return false;
    }

    Slice_u8 const a_bytes = str_as_bytes(a);
    Slice_u8 const b_bytes = str_as_bytes(b);

    return ccc_mem_eq(a_bytes.dat, b_bytes.dat, a_bytes.len);
}


typedef struct {
    str _0;
    str _1;
} StrSplit;


/// Splits a string at the specified index.
INLINE_ALWAYS
StrSplit str_split_at(str const self, usize const idx)
{
    ASSERT_MSG(idx <= self.len, "index out of bounds");

    return (StrSplit){
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
} StrSplitMut;


INLINE_ALWAYS
StrSplit str_split_from_mut(StrSplitMut const self)
{
    return (StrSplit){
        ._0 = str_from_mut(self._0),
        ._1 = str_from_mut(self._1),
    };
}


/// Splits a string at the specified index.
INLINE_ALWAYS
StrSplitMut str_split_mut_at(str_mut const self, usize const idx)
{
    ASSERT_MSG(idx <= self.len, "index out of bounds");

    return (StrSplitMut){
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
            return (Option_usize) SOME(i);
        }
    }

    return (Option_usize) NONE;
}


/// Finds the first `char` that satisfies the `predicate` and returns its index.
inline
Option_usize str_find(str const self, bool (* const predicate)(char, usize))
{
    for (usize i = 0; i < self.len; i++) {
        bool const found = predicate(self.dat[i], i);
        if (found) {
            return (Option_usize) SOME(i);
        }
    }

    return (Option_usize) NONE;
}


/// Returns a sub-str of `self`, spanning the range [`start`, `end`).
INLINE_ALWAYS
str str_slice(str const self, usize const start, usize const end)
{
    usize const checked_end = usize_max(start, end);

    ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (str){
        .dat = self.dat + start,
        .len = checked_end - start,
    };
}


/// Returns a mutable sub-str of `self`, spanning the range [`start`, `end`).
INLINE_ALWAYS
str_mut str_slice_mut(str_mut const self, usize const start, usize const end)
{
    usize const checked_end = usize_max(start, end);

    ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (str_mut){
        .dat = self.dat + start,
        .len = checked_end - start,
    };
}


/// Returns a sub-str of `self`, spanning the range [`start`, `end`].
INLINE_ALWAYS
str str_slice_incl(str const self, usize const start, usize const end)
{
    usize const checked_end = usize_max(start, end);

    ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (str){
        .dat = self.dat + start,
        .len = checked_end - start + 1,
    };
}


/// Returns a mutable sub-str of `self`, spanning the range [`start`, `end`].
INLINE_ALWAYS
str_mut str_slice_incl_mut(
    str_mut const self,
    usize const start,
    usize const end
)
{
    usize const checked_end = usize_max(start, end);

    ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (str_mut){
        .dat = self.dat + start,
        .len = checked_end - start + 1,
    };
}


#ifdef CCC_IMPLEMENTATION
str cstr(char ref c_str);
str str_from_mut(str_mut s);
Slice_u8 str_as_bytes(str s);
SliceMut_u8 str_as_bytes_mut(str_mut s);
str str_from_bytes(Slice_u8 bytes);
str_mut str_from_bytes_mut(SliceMut_u8 bytes);
bool str_eq(str a, str b);
StrSplit str_split_from_mut(StrSplitMut split);
StrSplit str_split_at(str s, usize idx);
StrSplitMut str_split_mut_at(str_mut s, usize idx);
str str_slice_incl(str self, usize start, usize end);
str_mut str_slice_incl_mut(str_mut self, usize start, usize end);
#endif
