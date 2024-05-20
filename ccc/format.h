#pragma once

#include "meta.h"
#include "str.h"
#include "result.h"
#include "ref.h"


typedef enum {
    FmtFormatterError,
    FmtInvalidFormat,
    FmtUnknownPlaceholder,
} FmtErrorKind;

typedef struct {
    FmtErrorKind kind;
    union {
        u32 fmt_error;
        str placeholder;
    };
} FmtError;


DEF_RESULT(FmtResult, __unit, FmtError);


// TODO: update the documentation.

/// Formatter interface.
///
/// Types can implement this interface in order to serve as formatters. A
/// pointer to the implementation of this interface is passed as part of
/// `RefFormatter` to every function working with formatters.
typedef struct {
    FmtResult (* const write_str)(RefMut ctx, str s);
    FmtResult (* const write_char)(RefMut ctx, char c);
} Formatter;


/// Fat pointer to a structure implementing `Formatter`.
///
/// Contains pointers to the vtable and to the data of the implementor itself.
///
/// NOTE: when used in static contexts, the dynamic call will be completely
/// compiled away, resulting in a statically dispatched, zero-cost abstraction.
/// Also note that some compilers (GCC) will require `INLINE_ALWAYS` on all the
/// method implementations for the optimisation to work.
DEF_DYN_REF(Formatter);
DEF_DYN_REF_MUT(Formatter);


/// Writes a str into a formatter.
INLINE_ALWAYS
FmtResult fmt_write_str(DynRefMut_Formatter const f, str const s)
{
    return f.iface->write_str((RefMut)as_ref(f), s);
}


/// Writes a `char` into the formatter.
INLINE_ALWAYS
FmtResult fmt_write_char(DynRefMut_Formatter const f, char const c)
{
    return f.iface->write_char((RefMut)as_ref(f), c);
}

// NOTE: this will never be inline due to the use of va args.
FmtResult fmt_write_fmt(
    DynRefMut_Formatter const f,
    char const* const restrict format,
    ...
);


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
        .ptr = buf + __fmt_buf_cap - len,
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
        return fmt_write_str(f, cstr("true"));
    }

    return fmt_write_str(f, cstr("false"));
}


INLINE_ALWAYS
FmtResult __unit_fmt_dbg(DynRefMut_Formatter const f, __unit const _)
{
    (void)_;
    return fmt_write_str(f, cstr("()"));
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

    return (FmtResult) wrap_ok(unit);
}


INLINE_ALWAYS
FmtResult str_split_fmt_dbg(DynRefMut_Formatter const f, StrSplit const split)
{
    FmtResult res;
    if ((res = fmt_write_char(f, '(')).is_err)       { return res; }
    if ((res = str_fmt_dbg(f, split._0)).is_err)     { return res; }
    if ((res = fmt_write_str(f, cstr(", "))).is_err) { return res; }
    if ((res = str_fmt_dbg(f, split._1)).is_err)     { return res; }
    return fmt_write_char(f, ')');
}


// ** Format Types


INLINE_ALWAYS
FmtResult fmt_error_fmt_dbg(
    DynRefMut_Formatter const f,
    FmtError const* const e
)
{
    switch (e->kind) {
    case FmtFormatterError:
    {
        FmtResult res;
        if ((res = fmt_write_str(f,
             cstr("FmtError::FmtFormatterError("))).is_err) { return res; }
        if ((res = u32_fmt(f, e->fmt_error)).is_err) { return res; }
        return fmt_write_char(f, ')');
    }
    case FmtInvalidFormat:
        return fmt_write_str(f, cstr("FmtError::FmtInvalidFormat"));
    case FmtUnknownPlaceholder:
    {
        FmtResult res;
        if ((res = fmt_write_str(f,
             cstr("FmtError::FmtUnknownPlaceholder("))).is_err) { return res; }
        if ((res = str_fmt_dbg(f, e->placeholder)).is_err) { return res; }
        return fmt_write_char(f, ')');
    }
    }

    DEBUG_UNREACHABLE();
}
