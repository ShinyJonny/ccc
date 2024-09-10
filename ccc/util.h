#pragma once


#include "meta.h"
#include "primitive.h"
#include "intrinsics.h"


#define CCC_UNSAFE_STATIC_STR(s) ((str) { \
    .dat = s,                                                                  \
    .len = sizeof s,                                                           \
})                                                                             \


/// Helper for printing `str` and `str_mut`.
///
/// # Example
///
/// ```
/// str const s = make_str();
/// printf(CCC_STRF, CCC_PSTR(s));
/// ```
#define CCC_PSTR(s) (int)(s).len, (s).dat
/// Format literal for printing `str` and `str_mut`.
///
/// # Example
///
/// ```
/// str const s = make_str();
/// printf("This is a `str`: \"" CCC_STRF "\".\n", CCC_PSTR(s));
/// ```
#define CCC_STRF "%.*s"


/// Converts a C string to `str`.
CCC_INLINE_ALWAYS
str str_from_cstring(char ref const c_str)
{
    return (str){
        .dat = c_str,
        .len = ccc_c_str_len(c_str),
    };
}


/// Downcasts `str_mut` to `str`.
///
/// NOTE: this cannot be done the other way around.
CCC_INLINE_ALWAYS
str str_from_mut(str_mut const s)
{
    return (str){
        .dat = s.dat,
        .len = s.len,
    };
}


CCC_INLINE_ALWAYS
str _ccc_str_as_str(str const s)
{
    return s;
}


// TODO: wide strings, etc.
/// Converts *all* kinds of strings to safe bounded strings.
///
/// Also works on bounded strings, in which case they will be cast to their
/// immutable versions.
#define CCC_STR(s) _Generic((s), \
    char*: str_from_cstring,                                                   \
    str: _ccc_str_as_str,                                                      \
    str_mut: str_from_mut                                                      \
)(s)                                                                           \


#ifdef CCC_IMPLEMENTATION
str str_from_cstring(char ref const c_str);
str str_from_mut(str_mut const s);
str _ccc_str_as_str(str const s);
#endif
