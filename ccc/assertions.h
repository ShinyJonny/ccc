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


CCC_NORETURN extern
void ccc_panic_handler(PanicInfo ref msg);


#define CCC_PANIC() ccc_panic_handler(&(PanicInfo) { \
    .reason = CCC_STR("explicit panic!"),                                      \
})                                                                             \


#define CCC_PANIC_MSG(message) ccc_panic_handler(&(PanicInfo) { \
    .reason = CCC_STR("explicit panic"),                                       \
    .msg    = CCC_SOME(CCC_STR(message)),                                      \
})                                                                             \


#define CCC_ASSERT(expression) { \
    if (!(expression)) {                                                       \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = CCC_STR("assertion failed"),                            \
            .context = CCC_SOME(CCC_STR("`" _CCC_STRINGIFY(expression) "`")),  \
        });                                                                    \
    }                                                                          \
}                                                                              \


#define CCC_ASSERT_MSG(expression, message) { \
    if (!(expression)) {                                                       \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = CCC_STR("assertion failed"),                            \
            .context = CCC_SOME(CCC_STR("`" _CCC_STRINGIFY(expression) "`")),  \
            .msg     = CCC_SOME(CCC_STR(message)),                             \
        });                                                                    \
    }                                                                          \
}                                                                              \


#define CCC_ASSERT_EQ(lhs, rhs) { \
    if ((lhs) != (rhs)) {                                                      \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = CCC_STR("equality assertionn failed"),                  \
            .context = CCC_SOME(CCC_STR(                                       \
                "`"                                                            \
                _CCC_STRINGIFY(lhs)                                            \
                "` != `"                                                       \
                _CCC_STRINGIFY(rhs)                                            \
                "`"                                                            \
            )),                                                                \
        });                                                                    \
    }                                                                          \
}                                                                              \


#define CCC_ASSERT_EQ_MSG(lhs, rhs, message) { \
    if ((lhs) != (rhs)) {                                                      \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = CCC_STR("equality assertionn failed"),                  \
            .context = CCC_SOME(CCC_STR(                                       \
                "`"                                                            \
                _CCC_STRINGIFY(lhs)                                            \
                "` != `"                                                       \
                _CCC_STRINGIFY(rhs)                                            \
                "`"                                                            \
            )),                                                                \
            .msg  = CCC_SOME(CCC_STR(message)),                                \
        });                                                                    \
    }                                                                          \
}                                                                              \


#define CCC_ASSERT_NE(lhs, rhs) { \
    if ((lhs) == (rhs)) {                                                      \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = CCC_STR("inequality assertionn failed"),                \
            .context = CCC_SOME(CCC_STR(                                       \
                "`"                                                            \
                _CCC_STRINGIFY(lhs)                                            \
                "` == `"                                                       \
                _CCC_STRINGIFY(rhs)                                            \
                "`"                                                            \
            )),                                                                \
        });                                                                    \
    }                                                                          \
}                                                                              \


#define CCC_ASSERT_NE_MSG(lhs, rhs, message) { \
    if ((lhs) == (rhs)) {                                                      \
        ccc_panic_handler(&(PanicInfo) {                                       \
            .reason  = CCC_STR("inequality assertionn failed"),                \
            .context = CCC_SOME(CCC_STR(                                       \
                "`"                                                            \
                _CCC_STRINGIFY(lhs)                                            \
                "` == `"                                                       \
                _CCC_STRINGIFY(rhs)                                            \
                "`"                                                            \
            )),                                                                \
            .msg  = CCC_SOME(CCC_STR(message)),                                \
        });                                                                    \
    }                                                                          \
}                                                                              \


#define CCC_TODO() ccc_panic_handler(&(PanicInfo) { \
    .reason = CCC_STR("todo!"),                                                \
})                                                                             \


#define CCC_TODO_MSG(message) ccc_panic_handler(&(PanicInfo) { \
    .reason = CCC_STR("todo"),                                                 \
    .msg    = CCC_SOME(CCC_STR(message)),                                      \
})                                                                             \


#define CCC_UNREACHABLE() ccc_panic_handler(&(PanicInfo) { \
    .reason = CCC_STR("unreachable!"),                                         \
})                                                                             \


#define CCC_UNREACHABLE_MSG(message) ccc_panic_handler(&(PanicInfo) { \
    .reason = CCC_STR("unreachable"),                                          \
    .msg    = CCC_SOME(CCC_STR(message)),                                      \
})                                                                             \


#ifdef CCC_DEBUG_ASSERTIONS
    #define CCC_DEBUG_ASSERT(expression)               CCC_ASSERT(expression)
    #define CCC_DEBUG_ASSERT_MSG(expr, message)        CCC_ASSERT_MSG(expr, message)
    #define CCC_DEBUG_ASSERT_EQ(lhs, rhs)              CCC_ASSERT_EQ(lhs, rhs)
    #define CCC_DEBUG_ASSERT_EQ_MSG(lhs, rhs, message) CCC_ASSERT_EQ_MSG(lhs, rhs, message)
    #define CCC_DEBUG_ASSERT_NE(lhs, rhs)              CCC_ASSERT_NE(lhs, rhs)
    #define CCC_DEBUG_ASSERT_NE_MSG(lhs, rhs, message) CCC_ASSERT_NE_MSG(lhs, rhs, message)
    #define CCC_DEBUG_UNREACHABLE()                    CCC_UNREACHABLE()
    #define CCC_DEBUG_UNREACHABLE_MSG(message)         CCC_UNREACHABLE_MSG(message)
#else
    #define CCC_DEBUG_ASSERT(expr)
    #define CCC_DEBUG_ASSERT_MSG(expr, message)
    #define CCC_DEBUG_ASSERT_EQ(lhs, rhs)
    #define CCC_DEBUG_ASSERT_EQ_MSG(lhs, rhs, message)
    #define CCC_DEBUG_ASSERT_NE(lhs, rhs)
    #define CCC_DEBUG_ASSERT_NE_MSG(lhs, rhs, message)
    #define CCC_DEBUG_UNREACHABLE()            CCC_HINT_UNREACHABLE()
    #define CCC_DEBUG_UNREACHABLE_MSG(message) CCC_HINT_UNREACHABLE()
#endif
