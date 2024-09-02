#pragma once

#include "meta.h"
#include "primitive.h"
#include "intrinsics.h"


#define _CCC_STR_FROM_CSTRING_LITERAL(s) ((str) { \
    .dat = s,                                                                  \
    .len = sizeof s,                                                           \
})


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
INLINE_ALWAYS
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
INLINE_ALWAYS
str str_from_mut(str_mut const s)
{
    return (str){
        .dat = s.dat,
        .len = s.len,
    };
}


INLINE_ALWAYS
str _ccc_str_from_str(str const s)
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
    str: _ccc_str_from_str,                                                    \
    str_mut: str_from_mut                                                      \
)(s)


/// Short-hand alias to `CCC_STR`.
///
/// Users can undefine this if they want to.
#define cs(s) CCC_STR(s)


#ifdef CCC_IMPLEMENTATION
str str_from_cstring(char ref c_str);
str str_from_mut(str_mut s);
str _ccc_str_from_str(str s);
#endif
