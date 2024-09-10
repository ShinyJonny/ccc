#pragma once

#include "ccc/format.h"
#include "ccc/prelude.h"


typedef struct {
    Array_u8 buf;
    usize len;
} BufFormatter;


typedef enum {
    BufFmtError_CapacityExceeded,
} BufFmtError;


INLINE_ALWAYS
void buf_fmt_drop(void ref_mut self) { (void)self; }


INLINE_ALWAYS
FmtResult buf_fmt_write_str(void ref_mut const void_ctx, str const s)
{
    Slice_u8 const bytes = str_as_bytes(s);
    BufFormatter ref_mut const ctx = void_ctx;

    if (ctx->len + bytes.len > ctx->buf.len) {
        return (FmtResult) ERR((FmtError){
            .kind      = FmtErrorKind_FormatterError,
            .fmt_error = BufFmtError_CapacityExceeded,
        });
    }

    ccc_copy_nonoverlapping(ctx->buf.dat + ctx->len, bytes.dat, bytes.len);
    ctx->len += bytes.len;

    return (FmtResult) OK(UNIT);
}


INLINE_ALWAYS
FmtResult buf_fmt_write_char(void ref_mut const void_ctx, char const c)
{
    BufFormatter ref_mut ctx = void_ctx;

    if (ctx->len >= ctx->buf.len) {
        return (FmtResult) ERR((FmtError){
            .kind      = FmtErrorKind_FormatterError,
            .fmt_error = BufFmtError_CapacityExceeded,
        });
    }
    ctx->buf.dat[ctx->len] = c;
    ctx->len += 1;

    return (FmtResult) OK(UNIT);
}


static
Formatter const BUF_FORMATTER__Formatter = {
    .hdr.size   = sizeof(BufFormatter),
    .hdr.dtor   = buf_fmt_drop,
    .write_str  = buf_fmt_write_str,
    .write_char = buf_fmt_write_char,
};


static INLINE_ALWAYS
DynRefMut_Formatter buf_fmt_as_dyn_mut_formatter(
    BufFormatter ref_mut const self
)
{
    return (DynRefMut_Formatter){
        .dst    = self,
        .vtable = &BUF_FORMATTER__Formatter,
    };
}


INLINE_ALWAYS
BufFormatter buf_fmt_create(Array_u8 const buf)
{
    return (BufFormatter){
        .buf = buf,
        .len = 0,
    };
}


INLINE_ALWAYS
str buf_fmt_get_str(BufFormatter ref const self)
{
    Slice_u8 const slice = array_u8_as_slice(&self->buf);
    return str_from_bytes(slice_u8_slice(slice, 0, self->len));
}
