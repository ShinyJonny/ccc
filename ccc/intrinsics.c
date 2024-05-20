#include "intrinsics.h"

usize ccc_c_str_len(char const* const s);
bool ccc_mem_eq(u8 const* const a, u8 const* const b, usize const len);
void ccc_copy_nonoverlapping(
    u8* restrict const dest,
    u8 const* const src,
    usize const len
);
