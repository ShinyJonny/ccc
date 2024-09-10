#pragma once


#include "meta.h"


#define CCC_DEF_RESULT(name, T, E) \
typedef struct CCC_NODISCARD {                                                 \
    bool is_err;                                                               \
    union {                                                                    \
        T ok;                                                                  \
        E err;                                                                 \
    };                                                                         \
} name;                                                                        \


/// Wraps the supplied *ok* value in a result.
#define CCC_OK(...) { \
    .is_err = false,                                                           \
    .ok = __VA_ARGS__,                                                         \
}                                                                              \


/// Wraps the supplied *error* value in a result.
#define CCC_ERR(...) { \
    .is_err = true,                                                            \
    .err = __VA_ARGS__,                                                        \
}                                                                              \


#ifdef CCC_ENABLE_GNU_FEATURES
    /// Unwraps the `ok` value or propagates the `err` value if the return
    /// type's `err` variant has the same type.
    #define CCC_TRY(expr, RET_T) ({ \
        __auto_type const in_var = (expr);                                     \
        if (in_var.is_err) {                                                   \
            return (RET_T) CCC_ERR(in_var.err);                                \
        }                                                                      \
        in_var.ok;                                                             \
    })                                                                         \

    /// Unwraps the `ok` value or returns `ret`.
    #define CCC_UNWRAP_OR_RET(expr, ret) ({ \
        __auto_type const in_var = (expr);                                     \
        if (in_var.is_err) {                                                   \
            return (ret);                                                      \
        }                                                                      \
        in_var.ok;                                                             \
    })                                                                         \

#endif
