#pragma once


#include "meta.h"


#define RESTRICT                 CCC_RESTRICT
#define INLINE_ALWAYS            CCC_INLINE_ALWAYS
#define NODISCARD                CCC_NODISCARD
#define NORETURN                 CCC_NORETURN
#define STATIC_ASSERT(expr, msg) CCC_STATIC_ASSERT(expr, msg)
#define HINT_UNREACHABLE()       CCC_HINT_UNREACHABLE()

#define UNIT CCC_UNIT

#define PSTR(s) CCC_PSTR(s)
#define STRF    CCC_STRF
#define cs(s)   CCC_STR(s)

#define PANIC()                          CCC_PANIC()
#define PANIC_MSG(message)               CCC_PANIC_MSG(message)
#define ASSERT(expression)               CCC_ASSERT(expression)
#define ASSERT_MSG(expression, message)  CCC_ASSERT_MSG(expression, message)
#define ASSERT_EQ(lhs, rhs)              CCC_ASSERT_EQ(lhs, rhs)
#define ASSERT_EQ_MSG(lhs, rhs, message) CCC_ASSERT_EQ_MSG(lhs, rhs, message)
#define ASSERT_NE(lhs, rhs)              CCC_ASSERT_NE(lhs, rhs)
#define ASSERT_NE_MSG(lhs, rhs, message) CCC_ASSERT_NE_MSG(lhs, rhs, message)
#define TODO()                           CCC_TODO()
#define TODO_MSG(message)                CCC_TODO_MSG(message)
#define UNREACHABLE()                    CCC_UNREACHABLE()
#define UNREACHABLE_MSG(message)         CCC_UNREACHABLE_MSG(message)
#define DEBUG_ASSERT(expr)               CCC_DEBUG_ASSERT(expr)
#define DEBUG_ASSERT_MSG(expr, message)  CCC_DEBUG_ASSERT_MSG(expr, message)
#define DEBUG_ASSERT_EQ(lhs, rhs)        CCC_DEBUG_ASSERT_EQ(lhs, rhs)
#define DEBUG_ASSERT_EQ_MSG(lhs, rhs, message) \
    CCC_DEBUG_ASSERT_EQ_MSG(lhs, rhs, message)
#define DEBUG_ASSERT_NE(lhs, rhs)        CCC_DEBUG_ASSERT_NE(lhs, rhs)
#define DEBUG_ASSERT_NE_MSG(lhs, rhs, message) \
    CCC_DEBUG_ASSERT_NE_MSG(lhs, rhs, message)
#define DEBUG_UNREACHABLE()              CCC_DEBUG_UNREACHABLE()
#define DEBUG_UNREACHABLE_MSG(message)   CCC_DEBUG_UNREACHABLE_MSG(message)

#define SOME(...) CCC_SOME(__VA_ARGS__)
#define NONE      CCC_NONE
#ifdef CCC_ENABLE_GNU_FEATURES
    #define try_opt(expr, RET_T)         CCC_TRY_OPT(expr, RET_T)
    #define unwrap_opt_or_ret(expr, ret) CCC_UNWRAP_OPT_OR_RET(expr, ret)
#endif

#define ERR(...) CCC_ERR(__VA_ARGS__)
#define OK(...)  CCC_OK(__VA_ARGS__)
#ifdef CCC_ENABLE_GNU_FEATURES
    #define try(expr, RET_T)         CCC_TRY(expr, RET_T)
    #define unwrap_or_ret(expr, ret) CCC_UNWRAP_OR_RET(expr, ret)
#endif

#define array(T, length, ...) ccc_array(T, length, __VA_ARGS__)

#define bit_field(count, ...) ccc_bit_field(count, __VA_ARGS__)
