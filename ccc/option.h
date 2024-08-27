#pragma once

#include "primitive.h"
#include "meta.h"


#define DEF_OPTION_NAMED(type, name) \
typedef struct NODISCARD {           \
    bool is_some;                    \
    type val;                        \
} name;
#define DEF_OPTION(type) DEF_OPTION_NAMED(type, Option_##type)


DEF_OPTION(u8)
DEF_OPTION(i8)
DEF_OPTION(u16)
DEF_OPTION(i16)
DEF_OPTION(u32)
DEF_OPTION(i32)
DEF_OPTION(u64)
DEF_OPTION(i64)

DEF_OPTION(usize)
DEF_OPTION(isize)
DEF_OPTION(uptr)
DEF_OPTION(iptr)

DEF_OPTION(f32)
DEF_OPTION(f64)

DEF_OPTION(bool)
DEF_OPTION_NAMED(__unit, Option_)


/// Wraps the supplied *some* value in an option.
#define SOME(...) {       \
    .is_some = true,      \
    .val = (__VA_ARGS__), \
}


/// Returns the *none* variant of an option.
#define NONE { .is_some = false, }


#ifdef CCC_ENABLE_GNU_FEATURES
    /// Unwraps the `some` value or propagates the `none` variant.
    #define try_opt(expr, RET_T) ({  \
        __auto_type in_var = (expr); \
        if (!in_var.is_some) {       \
            return (RET_T) NONE;     \
        }                            \
        in_var.val;                  \
    })

    /// Unwraps the `some` value or returns `ret`.
    #define unwrap_opt_or_ret(expr, ret) ({ \
        __auto_type in_var = (expr);        \
        if (!in_var.is_some) {              \
            return (ret);                   \
        }                                   \
        in_var.val;                         \
    })
#endif
