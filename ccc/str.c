#include "str.h"

str cstr(char const* const c_str);
str str_from_mut(str_mut const s);
Slice_u8 str_as_bytes(str const s);
SliceMut_u8 str_as_bytes_mut(str_mut const s);
str str_from_bytes(Slice_u8 const bytes);
str_mut str_from_bytes_mut(SliceMut_u8 const bytes);
bool str_eq(str const a, str const b);
StrSplit str_split_from_mut(StrSplitMut const split);
StrSplit str_split_at(str const s, usize const idx);
StrSplitMut str_split_mut_at(str_mut const s, usize const idx);
