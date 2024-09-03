#pragma once


#include "meta.h"
#include "primitive.h"


#define CCC_DEF_OPTION_NAMED(type, name) \
typedef struct NODISCARD {                                                     \
    bool is_some;                                                              \
    type val;                                                                  \
} name;
#define CCC_DEF_OPTION(type) CCC_DEF_OPTION_NAMED(type, Option_##type)


CCC_DEF_OPTION(u8)
CCC_DEF_OPTION(i8)
CCC_DEF_OPTION(u16)
CCC_DEF_OPTION(i16)
CCC_DEF_OPTION(u32)
CCC_DEF_OPTION(i32)
CCC_DEF_OPTION(u64)
CCC_DEF_OPTION(i64)

CCC_DEF_OPTION(usize)
CCC_DEF_OPTION(isize)
CCC_DEF_OPTION(uptr)
CCC_DEF_OPTION(iptr)

CCC_DEF_OPTION(f32)
CCC_DEF_OPTION(f64)

CCC_DEF_OPTION(bool)
CCC_DEF_OPTION_NAMED(__unit, Option_)

CCC_DEF_OPTION(str)
CCC_DEF_OPTION(str_mut)


/// Wraps the supplied *some* value in an option.
#define SOME(...) { \
    .is_some = true,                                                           \
    .val = (__VA_ARGS__),                                                      \
}                                                                              \


/// Returns the *none* variant of an option.
#define NONE { .is_some = false, }


#ifdef CCC_ENABLE_GNU_FEATURES
    /// Unwraps the `some` value or propagates the `none` variant.
    #define try_opt(expr, RET_T) ({ \
        __auto_type in_var = (expr);                                           \
        if (!in_var.is_some) {                                                 \
            return (RET_T) NONE;                                               \
        }                                                                      \
        in_var.val;                                                            \
    })                                                                         \

    /// Unwraps the `some` value or returns `ret`.
    #define unwrap_opt_or_ret(expr, ret) ({ \
        __auto_type in_var = (expr);                                           \
        if (!in_var.is_some) {                                                 \
            return (ret);                                                      \
        }                                                                      \
        in_var.val;                                                            \
    })                                                                         \

#endif
