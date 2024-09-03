#pragma once


#include "meta.h"
#include "util.h"
#include "assertions.h"
#include "str.h"
#include "result.h"
#include "ref.h"


typedef enum {
    FmtErrorKind_FormatterError,
    FmtErrorKind_InvalidFormat,
    FmtErrorKind_UnknownPlaceholder,
} FmtErrorKind;


typedef struct {
    FmtErrorKind kind;
    union {
        u32 fmt_error;
        str placeholder;
    };
} FmtError;


CCC_DEF_RESULT(FmtResult, __unit, FmtError)


// TODO: update the documentation.

/// Formatter interface.
///
/// Types can implement this interface in order to serve as formatters. A
/// pointer to the implementation of this interface is passed as part of
/// `RefFormatter` to every function working with formatters.
typedef struct {
    VTableHdr hdr;
    FmtResult (* const write_str)(void ref_mut ctx, str s);
    FmtResult (* const write_char)(void ref_mut, char c);
} Formatter;


/// Fat pointer to a structure implementing `Formatter`.
///
/// Contains pointers to the vtable and to the data of the implementor itself.
///
/// NOTE: when used in static contexts, the dynamic call will be completely
/// compiled away, resulting in a statically dispatched, zero-cost abstraction.
/// Also note that some compilers (GCC) will require `INLINE_ALWAYS` on all the
/// method implementations for the optimisation to work.
CCC_DEF_DYN_REF(Formatter);
CCC_DEF_DYN_REF_MUT(Formatter);


/// Writes a str into a formatter.
INLINE_ALWAYS
FmtResult fmt_write_str(DynRefMut_Formatter const f, str const s)
{
    return f.vtable->write_str(f.dst, s);
}


/// Writes a `char` into the formatter.
INLINE_ALWAYS
FmtResult fmt_write_char(DynRefMut_Formatter const f, char const c)
{
    return f.vtable->write_char(f.dst, c);
}


// Formatting implementations for types.


// ** Primitive types.


INLINE_ALWAYS
FmtResult u64_fmt(DynRefMut_Formatter const f, u64 num)
{
    if (num == 0) {
        return fmt_write_char(f, '0');
    }

    #define __fmt_buf_cap 20
    char buf[__fmt_buf_cap];
    usize len = 0;

    while (num > 0) {
        len += 1;
        const u32 digit = (num % 10) + 0x30;
        buf[__fmt_buf_cap - len] = digit;
        num /= 10;
    }

    str const s = (str){
        .dat = buf + __fmt_buf_cap - len,
        .len = len,
    };

    return fmt_write_str(f, s);
    #undef __fmt_buf_cap
}


INLINE_ALWAYS
FmtResult u32_fmt(DynRefMut_Formatter const f, u32 const num)
{
    return u64_fmt(f, num);
}


INLINE_ALWAYS
FmtResult u16_fmt(DynRefMut_Formatter const f, u16 const num)
{
    return u64_fmt(f, num);
}


INLINE_ALWAYS
FmtResult u8_fmt(DynRefMut_Formatter const f, u8 const num)
{
    return u64_fmt(f, num);
}


INLINE_ALWAYS
FmtResult i64_fmt(DynRefMut_Formatter const f, i64 num)
{
    if (num < 0) {
        FmtResult const res = fmt_write_char(f, '-');
        if (res.is_err) { return res; }

        num *= -1;
    }

    return u64_fmt(f, num);
}


INLINE_ALWAYS
FmtResult i32_fmt(DynRefMut_Formatter const f, i32 const num)
{
    return i64_fmt(f, num);
}


INLINE_ALWAYS
FmtResult i16_fmt(DynRefMut_Formatter const f, i16 const num)
{
    return i64_fmt(f, num);
}


INLINE_ALWAYS
FmtResult i8_fmt(DynRefMut_Formatter const f, i8 const num)
{
    return i64_fmt(f, num);
}


INLINE_ALWAYS
FmtResult usize_fmt(DynRefMut_Formatter const f, usize const num)
{
    // FIXME: better picking of the integer size.
    return u64_fmt(f, num);
}


INLINE_ALWAYS
FmtResult isize_fmt(DynRefMut_Formatter const f, isize const num)
{
    // FIXME: better picking of the integer size.
    return i64_fmt(f, num);
}


INLINE_ALWAYS
FmtResult bool_fmt(DynRefMut_Formatter const f, bool const b)
{
    if (b) {
        return fmt_write_str(f, CCC_STR("true"));
    }

    return fmt_write_str(f, CCC_STR("false"));
}


INLINE_ALWAYS
FmtResult __unit_fmt_dbg(DynRefMut_Formatter const f, __unit const _)
{
    (void)_;
    return fmt_write_str(f, CCC_STR("()"));
}


// ** assertion


INLINE_ALWAYS
FmtResult panic_info_fmt(DynRefMut_Formatter const f, PanicInfo ref const self)
{
    FmtResult res;
    if ((res = fmt_write_str(f, self->reason)).is_err)    { return res; }
    if (self->context.is_some) {
        if ((res = fmt_write_str(f, CCC_STR(": "))   ).is_err)     { return res; }
        if ((res = fmt_write_str(f, self->context.val)).is_err) { return res; }
    }
    if (self->msg.is_some) {
        if ((res = fmt_write_str(f, CCC_STR(": "))   ).is_err) { return res; }
        if ((res = fmt_write_str(f, self->msg.val)).is_err) { return res; }
    }

    return (FmtResult) OK(unit);
}


// ** str TODO


INLINE_ALWAYS
FmtResult str_fmt(DynRefMut_Formatter const f, str const s)
{
    return fmt_write_str(f, s);
}


INLINE_ALWAYS
FmtResult str_fmt_dbg(DynRefMut_Formatter const f, str const s)
{
    FmtResult res;
    if ((res = fmt_write_char(f, '"')).is_err) { return res; }
    if ((res = fmt_write_str(f, s)   ).is_err) { return res; }
    if ((res = fmt_write_char(f, '"')).is_err) { return res; }

    return (FmtResult) OK(unit);
}


INLINE_ALWAYS
FmtResult str_split_fmt_dbg(DynRefMut_Formatter const f, StrSplit const split)
{
    FmtResult res;
    if ((res = fmt_write_char(f, '(')).is_err)       { return res; }
    if ((res = str_fmt_dbg(f, split._0)).is_err)     { return res; }
    if ((res = fmt_write_str(f, CCC_STR(", "))).is_err) { return res; }
    if ((res = str_fmt_dbg(f, split._1)).is_err)     { return res; }
    return fmt_write_char(f, ')');
}


// ** Format Types


INLINE_ALWAYS
FmtResult fmt_error_fmt_dbg(
    DynRefMut_Formatter const f,
    FmtError ref const e
)
{
    switch (e->kind) {
    case FmtErrorKind_FormatterError:
    {
        FmtResult res;
        if ((res = fmt_write_str(f,
             CCC_STR("FmtError::FmtFormatterError("))).is_err) { return res; }
        if ((res = u32_fmt(f, e->fmt_error)).is_err) { return res; }
        return fmt_write_char(f, ')');
    }
    case FmtErrorKind_InvalidFormat:
        return fmt_write_str(f, CCC_STR("FmtError::FmtInvalidFormat"));
    case FmtErrorKind_UnknownPlaceholder:
    {
        FmtResult res;
        if ((res = fmt_write_str(f,
             CCC_STR("FmtError::FmtUnknownPlaceholder("))).is_err)
            { return res; }
        if ((res = str_fmt_dbg(f, e->placeholder)).is_err) { return res; }
        return fmt_write_char(f, ')');
    }
    }

    DEBUG_UNREACHABLE();
}


#ifdef CCC_IMPLEMENTATION
FmtResult fmt_write_str(DynRefMut_Formatter const f, str const s);
FmtResult fmt_write_char(DynRefMut_Formatter const f, char const c);
FmtResult u64_fmt(DynRefMut_Formatter const f, u64 num);
FmtResult u32_fmt(DynRefMut_Formatter const f, u32 const num);
FmtResult u16_fmt(DynRefMut_Formatter const f, u16 const num);
FmtResult u8_fmt(DynRefMut_Formatter const f, u8 const num);
FmtResult i64_fmt(DynRefMut_Formatter const f, i64 num);
FmtResult i32_fmt(DynRefMut_Formatter const f, i32 const num);
FmtResult i16_fmt(DynRefMut_Formatter const f, i16 const num);
FmtResult i8_fmt(DynRefMut_Formatter const f, i8 const num);
FmtResult usize_fmt(DynRefMut_Formatter const f, usize const num);
FmtResult isize_fmt(DynRefMut_Formatter const f, isize const num);
FmtResult bool_fmt(DynRefMut_Formatter const f, bool const b);
FmtResult __unit_fmt_dbg(DynRefMut_Formatter const f, __unit const _);
FmtResult panic_info_fmt(DynRefMut_Formatter const f, PanicInfo ref const self);
FmtResult str_fmt(DynRefMut_Formatter const f, str const s);
FmtResult str_fmt_dbg(DynRefMut_Formatter const f, str const s);
FmtResult str_split_fmt_dbg(DynRefMut_Formatter const f, StrSplit const split);
FmtResult fmt_error_fmt_dbg(
    DynRefMut_Formatter const f,
    FmtError ref const e
);
#endif
