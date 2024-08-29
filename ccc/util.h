#pragma once

#include "meta.h"
#include "primitive.h"
#include "intrinsics.h"

#define _CCC_CSTR(s) ((str) { \
    .dat = s,                                                                  \
    .len = sizeof s,                                                           \
})


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


#ifdef CCC_IMPLEMENTATION
str cstr(char ref c_str);
#endif
