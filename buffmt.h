#pragma once

#include "ccc/format.h"
#include "ccc/meta.h"


typedef struct {
    Array_u8 buf;
    usize len;
} BufFormatter;


typedef enum {
    CapacityExceeded,
} BufFormatterError;


INLINE_ALWAYS
void buf_fmt_drop(void ref_mut self) { (void)self; }


INLINE_ALWAYS
FmtResult buf_fmt_write_str(void ref_mut const void_ctx, str const s)
{
    Slice_u8 const bytes = str_as_bytes(s);
    BufFormatter ref_mut const ctx = void_ctx;

    SliceMut_u8 const buf = array_u8_as_mut(&ctx->buf);

    if (ctx->len + bytes.len > buf.len) {
        return (FmtResult) ERR((FmtError){
            .kind      = FmtFormatterError,
            .fmt_error = CapacityExceeded,
        });
    }

    ccc_copy_nonoverlapping(buf.dat + ctx->len, bytes.dat, bytes.len);
    ctx->len += bytes.len;

    return (FmtResult) OK(unit);
}


INLINE_ALWAYS
FmtResult buf_fmt_write_char(void ref_mut const void_ctx, char const c)
{
    BufFormatter ref_mut ctx = void_ctx;

    SliceMut_u8 const buf = array_u8_as_mut(&ctx->buf);

    if (ctx->len >= buf.len) {
        return (FmtResult) ERR((FmtError){
            .kind      = FmtFormatterError,
            .fmt_error = CapacityExceeded,
        });
    }
    buf.dat[ctx->len] = c;
    ctx->len += 1;

    return (FmtResult) OK(unit);
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
    return (str){
        .dat = str_from_bytes(array_u8_as_ref(&self->buf)).dat,
        .len = self->len,
    };
}
