#include "format.h"

#include <stdarg.h>


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
FmtResult __unit_fmt_dbg(DynRefMut_Formatter const f, __unit const /* _ */);
FmtResult str_fmt(DynRefMut_Formatter const f, str const s);
FmtResult str_fmt_dbg(DynRefMut_Formatter const f, str const s);
FmtResult str_split_fmt_dbg(DynRefMut_Formatter const f, StrSplit const split);
FmtResult fmt_error_fmt_dbg(
    DynRefMut_Formatter const f,
    FmtError const* const e
);


FmtResult fmt_write_fmt(
    DynRefMut_Formatter const f,
    char const* const restrict format,
    ...
)
{
    va_list args;
    va_start(args, format);

    str remaining = cstr(format);

    while (true) {
        Option_usize const bracket_pos = str_find_char(remaining, '{');
        if (!bracket_pos.is_some) {
            return fmt_write_str(f, remaining);
        }

        usize const placeholder_pos = bracket_pos.val + 1;
        if (placeholder_pos >= remaining.len) {
            return (FmtResult) wrap_err((FmtError){ .kind = FmtInvalidFormat });
        }

        StrSplit const bracket_split = str_split_at(remaining, placeholder_pos);

        // `{{` escaping brackets.
        if (remaining.ptr[placeholder_pos] == '{') {
            FmtResult const res = fmt_write_str(f, bracket_split._0);
            if (res.is_err) {
                return res;
            }

            remaining
                = str_slice(bracket_split._1, 1, bracket_split._1.len);
            continue;
        }

        Option_usize const closing_pos = str_find_char(bracket_split._1, '}');
        if (!closing_pos.is_some) {
            return (FmtResult) wrap_err((FmtError){ .kind = FmtInvalidFormat });
        }

        StrSplit const end_split
            = str_split_at(bracket_split._1, closing_pos.val);
        str const placeholder = end_split._0;

        FmtResult res;
        if (str_eq(placeholder, cstr("str"))) {
            str const val = va_arg(args, str);
            if ((res = fmt_write_str(f, val)).is_err) {
                return res;
            }
        } else if (str_eq(placeholder, cstr("c"))) {
            char const val = va_arg(args, int);
            if ((res = fmt_write_char(f, val)).is_err) {
                return res;
            }
        } else {
            return (FmtResult) wrap_err((FmtError){
                .kind = FmtUnknownPlaceholder,
                .placeholder = placeholder,
            });
        }

        remaining = str_slice(end_split._1, 1, end_split._1.len);
    }
}
