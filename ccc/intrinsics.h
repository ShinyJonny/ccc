#pragma once


#include "meta.h"
#include "primitive.h"


#ifdef CCC_USE_LIBC_INTRINSICS


#include <string.h>


/// Computes the length of a c-string.
CCC_INLINE_ALWAYS
usize ccc_c_str_len(char const* const s)
{
    return strlen(s);
}


/// Compares the first `len` bytes pointed to by `a` and `b`, and returns if
/// they are the same.
CCC_INLINE_ALWAYS
bool ccc_mem_eq(u8 const* const a, u8 const* const b, usize const len) {
    return memcmp(a, b, len) == 0;
}


/// Copies the first `len` bytes from `src` to `dest` (non-overlapping).
///
/// `src` must not overlap with `dest`.
CCC_INLINE_ALWAYS
void ccc_copy_nonoverlapping(
    u8* CCC_RESTRICT const dest,
    u8 const* CCC_RESTRICT const src,
    usize const len
)
{
    memcpy(dest, src, len);
}


/// Copies `byte` to the first `len` bytes pointed to by `dest`.
CCC_INLINE_ALWAYS
void ccc_memset(
    u8* CCC_RESTRICT const dest,
    u8 const byte,
    usize const len
)
{
    memset(dest, byte, len);
}


#else


/// Computes the length of a c-string.
INLINE_ALWAYS
usize ccc_c_str_len(char const* const s)
{
    usize len = 0;
    for(; s[len]; len++) {}

    return len;
}


/// Compares the first `len` bytes pointed to by `a` and `b`, and returns if
/// they are the same.
INLINE_ALWAYS
bool ccc_mem_eq(
    u8 const* const a,
    u8 const* const b,
    usize const max
)
{
    for (usize i = 0; i < max; i++) {
        u8 const left  = a[i];
        u8 const right = b[i];

        if (left != right) {
            return false;
        }
    }

    return true;
}


/// Copies the first `len` bytes from `src` to `dest` (non-overlapping).
///
/// `src` must not overlap with `dest`.
INLINE_ALWAYS
void ccc_copy_nonoverlapping(
    u8* CCC_RESTRICT const dest,
    u8 const* CCC_RESTRICT const src,
    usize const len
)
{
    for (usize i = 0; i < len; i++) {
        dest[i] = src[i];
    }
}


/// Copies `byte` to the first `len` bytes pointed to by `dest`.
INLINE_ALWAYS
void ccc_memset(
    u8* CCC_RESTRICT const dest,
    u8 const byte,
    usize const len
)
{
    for (usize i = 0; i < len; i++) {
        dest[i] = byte;
    }
}


#endif


#ifdef CCC_IMPLEMENTATION
usize ccc_c_str_len(char const* const s);
bool ccc_mem_eq(u8 const* const a, u8 const* const b, usize const len);
void ccc_copy_nonoverlapping(
    u8* CCC_RESTRICT const dest,
    u8 const* const src,
    usize const len
);
void ccc_memset(
    u8* CCC_RESTRICT const dest,
    u8 const byte,
    usize const len
);
#endif
