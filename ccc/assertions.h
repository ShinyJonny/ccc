#pragma once

#include "meta.h"
#include "primitive.h"
#include "option.h"
#include "util.h"

typedef struct {
    str reason;
    Option_str msg;
    Option_str context;
} PanicInfo;

NORETURN extern
void ccc_panic_handler(PanicInfo ref msg);

#define PANIC() ccc_panic_handler(&(PanicInfo) { \
    .reason = cstr("explicit panic!"),                                         \
})

#define PANIC_MSG(message) ccc_panic_handler(&(PanicInfo) { \
    .reason = cstr("explicit panic"),                                          \
    .msg    = SOME(cstr(message)),                                             \
})

#define ASSERT(expression) { \
    if (!(expression)) {                                                       \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = cstr("assertion failed"),                               \
            .context = SOME(cstr("`" _CCC_STRINGIFY(expression) "`")),         \
        });                                                                    \
    }                                                                          \
}

#define ASSERT_MSG(expression, message) { \
    if (!(expression)) {                                                       \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = cstr("assertion failed"),                               \
            .context = SOME(cstr("`" _CCC_STRINGIFY(expression) "`")),         \
            .msg     = SOME(cstr(message)),                                    \
        });                                                                    \
    }                                                                          \
}

#define ASSERT_EQ(lhs, rhs) { \
    if ((lhs) != (rhs)) {                                                      \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = cstr("equality assertionn failed"),                     \
            .context = SOME(cstr(                                              \
                "`"                                                            \
                _CCC_STRINGIFY(lhs)                                            \
                "` != `"                                                       \
                _CCC_STRINGIFY(rhs)                                            \
                "`"                                                            \
            )),                                                                \
        });                                                                    \
    }                                                                          \
}

#define ASSERT_EQ_MSG(lhs, rhs, message) { \
    if ((lhs) != (rhs)) {                                                      \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = cstr("equality assertionn failed"),                     \
            .context = SOME(cstr(                                              \
                "`"                                                            \
                _CCC_STRINGIFY(lhs)                                            \
                "` != `"                                                       \
                _CCC_STRINGIFY(rhs)                                            \
                "`"                                                            \
            )),                                                                \
            .msg  = SOME(cstr(message)),                                       \
        });                                                                    \
    }                                                                          \
}

#define ASSERT_NE(lhs, rhs) { \
    if ((lhs) == (rhs)) {                                                      \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = cstr("inequality assertionn failed"),                   \
            .context = SOME(cstr(                                              \
                "`"                                                            \
                _CCC_STRINGIFY(lhs)                                            \
                "` == `"                                                       \
                _CCC_STRINGIFY(rhs)                                            \
                "`"                                                            \
            )),                                                                \
        });                                                                    \
    }                                                                          \
}

#define ASSERT_NE_MSG(lhs, rhs, message) { \
    if ((lhs) == (rhs)) {                                                      \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = cstr("inequality assertionn failed"),                   \
            .context = SOME(cstr(                                              \
                "`"                                                            \
                _CCC_STRINGIFY(lhs)                                            \
                "` == `"                                                       \
                _CCC_STRINGIFY(rhs)                                            \
                "`"                                                            \
            )),                                                                \
            .msg  = SOME(cstr(message)),                                       \
        });                                                                    \
    }                                                                          \
}

#define TODO() ccc_panic_handler(&(PanicInfo) { \
    .reason = cstr("todo!"),                                                   \
})

#define TODO_MSG(message) ccc_panic_handler(&(PanicInfo) { \
    .reason = cstr("todo"),                                                    \
    .msg    = SOME(cstr(message)),                                             \
})

#define UNREACHABLE() ccc_panic_handler(&(PanicInfo) { \
    .reason = cstr("unreachable!"),                                            \
})

#define UNREACHABLE_MSG(message) ccc_panic_handler(&(PanicInfo) { \
    .reason = cstr("unreachable"),                                             \
    .msg    = SOME(cstr(message)),                                             \
})

#ifdef CCC_DEBUG_ASSERTIONS
    #define DEBUG_ASSERT(expression)               ASSERT(expression)
    #define DEBUG_ASSERT_MSG(expr, message)        ASSERT_MSG(expr, message)
    #define DEBUG_ASSERT_EQ(lhs, rhs)              ASSERT_EQ(lhs, rhs)
    #define DEBUG_ASSERT_EQ_MSG(lhs, rhs, message) ASSERT_EQ_MSG(lhs, rhs, message)
    #define DEBUG_ASSERT_NE(lhs, rhs)              ASSERT_NE(lhs, rhs)
    #define DEBUG_ASSERT_NE_MSG(lhs, rhs, message) ASSERT_NE_MSG(lhs, rhs, message)
    #define DEBUG_UNREACHABLE()                    UNREACHABLE()
    #define DEBUG_UNREACHABLE_MSG(message)         UNREACHABLE_MSG(message)
#else
    #define DEBUG_ASSERT(expr)
    #define DEBUG_ASSERT_MSG(expr, message)
    #define DEBUG_ASSERT_EQ(lhs, rhs)
    #define DEBUG_ASSERT_EQ_MSG(lhs, rhs, message)
    #define DEBUG_ASSERT_NE(lhs, rhs)
    #define DEBUG_ASSERT_NE_MSG(lhs, rhs, message)
    #define DEBUG_UNREACHABLE()            HINT_UNREACHABLE()
    #define DEBUG_UNREACHABLE_MSG(message) HINT_UNREACHABLE()
#endif
