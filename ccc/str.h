#pragma once

#include "primitive.h"
#include "meta.h"
#include "slice.h"
#include "option.h"
#include "intrinsics.h"


/// Bounded string reference.
typedef struct {
#ifdef CCC_RESTRICT_REFERENCES
    char const* restrict ptr;
#else
    char const* ptr;
#endif
    usize len;
} str;


DEF_SLICE(str);
DEF_OPTION(str);


/// Mutable bounded string reference.
typedef struct {
#ifdef CCC_RESTRICT_REFERENCES
    char* restrict ptr;
#else
    char* ptr;
#endif
    usize len;
} str_mut;


DEF_SLICE(str_mut);
DEF_OPTION(str_mut);


/// Helper for printing `str` and `str_mut`.
///
/// # Example
///
/// ```
/// str const s = make_str();
/// printf(STRF, PSTR(s));
/// ```
#define PSTR(s) (int)(s).len, (s).ptr
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
str cstr(char const* const c_str)
{
    return (str){
        .ptr = c_str,
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
        .ptr = s.ptr,
        .len = s.len,
    };
}


/// Gets the underlying bytes of `s`, safely cast to `u8`.
INLINE_ALWAYS
Slice_u8 str_as_bytes(str const self)
{
    return (Slice_u8){
        .ptr = (u8 const*)self.ptr,
        .len = self.len,
    };
}


/// Gets the underlying mutable bytes of `s`, safely cast to `u8`.
INLINE_ALWAYS
SliceMut_u8 str_as_bytes_mut(str_mut const self)
{
    return (SliceMut_u8){
        .ptr = (u8*)self.ptr,
        .len = self.len,
    };
}


/// Constructs a `str` from a slice of `u8`.
INLINE_ALWAYS
str str_from_bytes(Slice_u8 const bytes)
{
    return (str){
        .ptr = (char const*)bytes.ptr,
        .len = bytes.len,
    };
}


/// Constructs a `str` from a mutable slice of `u8`.
INLINE_ALWAYS
str_mut str_from_bytes_mut(SliceMut_u8 const bytes)
{
    return (str_mut){
        .ptr = (char*)bytes.ptr,
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

    return ccc_mem_eq(a_bytes.ptr, b_bytes.ptr, a_bytes.len);
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
        ._0 = (str){
            .ptr = self.ptr,
            .len = idx,
        },
        ._1 = (str){
            .ptr = self.ptr + idx,
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
        ._0 = (str_mut){
            .ptr = self.ptr,
            .len = idx,
        },
        ._1 = (str_mut){
            .ptr = self.ptr + idx,
            .len = self.len - idx,
        },
    };
}


/// Finds the first occurence of `c` and returns its index.
inline
Option_usize str_find_char(str const self, char const c)
{
    for (usize i = 0; i < self.len; i++) {
        if (self.ptr[i] == c) {
            return (Option_usize) wrap_some(i);
        }
    }

    return (Option_usize) none;
}


/// Finds the first `char` that satisfies the `predicate` and returns its index.
inline
Option_usize str_find(str const self, bool (* const predicate)(char, usize))
{
    for (usize i = 0; i < self.len; i++) {
        bool const result = predicate(self.ptr[i], i);
        if (result) {
            return (Option_usize) wrap_some(i);
        }
    }

    return (Option_usize) none;
}


/// Returns a sub-str of `self`, spanning the range [`start`, `end`).
INLINE_ALWAYS
str str_slice(str const self, usize const start, usize const end)
{
    usize const checked_end = end >= start ? end : start;

    ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (str){
        .ptr = self.ptr + start,
        .len = checked_end - start,
    };
}


/// Returns a mutable sub-str of `self`, spanning the range [`start`, `end`).
INLINE_ALWAYS
str_mut str_slice_mut(str_mut const self, usize const start, usize const end)
{
    usize const checked_end = end >= start ? end : start;

    ASSERT_MSG(checked_end <= self.len, "slice out of bounds");

    return (str_mut){
        .ptr = self.ptr + start,
        .len = checked_end - start,
    };
}


/// Returns a sub-str of `self`, spanning the range [`start`, `end`].
INLINE_ALWAYS
str str_slice_incl(str const self, usize const start, usize const end)
{
    usize const checked_end = end >= start ? end : start;

    ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (str){
        .ptr = self.ptr + start,
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
    usize const checked_end = end >= start ? end : start;

    ASSERT_MSG(checked_end < self.len, "slice out of bounds");

    return (str_mut){
        .ptr = self.ptr + start,
        .len = checked_end - start + 1,
    };
}
