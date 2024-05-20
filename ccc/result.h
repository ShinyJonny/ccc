#pragma once

#include "meta.h"


#define DEF_RESULT(name, T, E) \
typedef struct NODISCARD {     \
    bool is_err;               \
    union {                    \
        T ok;                  \
        E err;                 \
    };                         \
} name


#ifdef CCC_ENABLE_GNU_FEATURES
    /// Unwraps the `ok` value or propagates the `err` value if the return
    /// type's `err` variant has the same type.
    #define try(expr, RET_T) ({                  \
        __auto_type const in_var = (expr);       \
        if (in_var.is_err) {                     \
            return (RET_T) wrap_err(in_var.err); \
        }                                        \
        in_var.ok;                               \
    })

    /// Unwraps the `ok` value or returns `ret`.
    #define unwrap_or_ret(expr, ret) ({    \
        __auto_type const in_var = (expr); \
        if (in_var.is_err) {               \
            return (ret);                  \
        }                                  \
        in_var.ok;                         \
    })
#endif


/// Wraps the supplied *error* value in a result.
#define wrap_err(...) {   \
    .is_err = true,       \
    .err = __VA_ARGS__,   \
}


/// Wraps the supplied *ok* value in a result.
#define wrap_ok(...) {   \
    .is_err = false,     \
    .ok = __VA_ARGS__,   \
}
