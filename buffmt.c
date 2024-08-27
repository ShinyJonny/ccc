#include "buffmt.h"


FmtResult buf_fmt_write_str(void ref_mut void_ctx, str s);
FmtResult buf_fmt_write_char(void ref_mut void_ctx, char c);
BufFormatter buf_fmt_create(Array_u8 buf);
str buf_fmt_get_str(BufFormatter ref self);
