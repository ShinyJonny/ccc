#pragma once

#include "meta.h"
#include "primitive.h"
#include "assertions.h"
#include "result.h"
#include <stdatomic.h>

#ifdef CCC_MODULE_ATOMICS

typedef enum {
    MemOrd_Relaxed = memory_order_relaxed,
    // NOTE: `memory_order_consume` not supported.
    MemOrd_Acquire = memory_order_acquire,
    MemOrd_Release = memory_order_release,
    MemOrd_AcqRel  = memory_order_acq_rel,
    MemOrd_SeqCst  = memory_order_seq_cst,
} MemOrd;

INLINE_ALWAYS
memory_order _ccc_mem_ord_to_memory_order(MemOrd const self)
{
    switch (self) {
    case MemOrd_Relaxed:
        return memory_order_relaxed;
    case MemOrd_Acquire:
        return memory_order_acquire;
    case MemOrd_Release:
        return memory_order_release;
    case MemOrd_AcqRel:
        return memory_order_acq_rel;
    case MemOrd_SeqCst:
        return memory_order_seq_cst;
    }

    DEBUG_UNREACHABLE();
}

INLINE_ALWAYS
bool mem_ord_is_valid_load(MemOrd const ord)
{
    switch (ord) {
    case MemOrd_Release:
    case MemOrd_AcqRel:
        return false;
    default:
        return true;
    }
}

INLINE_ALWAYS
bool mem_ord_is_valid_store(MemOrd const ord)
{
    switch (ord) {
    case MemOrd_Acquire:
    case MemOrd_AcqRel:
        return false;
    default:
        return true;
    }
}

#define _ccc_def_atomic(atomic_typename, type, typename, impl_prefix) \
typedef struct {                                                               \
    type volatile inner;                                                       \
} atomic_typename;                                                             \
                                                                               \
CCC_DEF_RESULT(CmpXchgResult_##typename, type, type)                           \
                                                                               \
inline                                                                         \
CmpXchgResult_##typename impl_prefix##_compare_exchange_weak(                  \
    atomic_typename ref const self,                                            \
    type expected,                                                             \
    type const new,                                                            \
    MemOrd const success,                                                      \
    MemOrd const failure                                                       \
)                                                                              \
{                                                                              \
    if (!mem_ord_is_valid_load(failure)) {                                     \
        /* TODO: format the ordering. */                                       \
        PANIC_MSG("invalid memory ordering for the failure case");             \
    }                                                                          \
                                                                               \
    /* SAFETY: we can cast an immutable ref to `inner` to a mutable pointer, */\
    /* as it is volatile and touched only by atomic operations. */             \
    type _Atomic volatile* const atomic_ptr = (void*)&self->inner;             \
    type const exchanged = atomic_compare_exchange_weak_explicit(              \
        atomic_ptr,                                                            \
        &expected,                                                             \
        new,                                                                   \
        _ccc_mem_ord_to_memory_order(success),                                 \
        _ccc_mem_ord_to_memory_order(failure)                                  \
    );                                                                         \
                                                                               \
    if (exchanged) {                                                           \
        return (CmpXchgResult_##typename) OK(expected);                        \
    } else {                                                                   \
        return (CmpXchgResult_##typename) ERR(expected);                       \
    }                                                                          \
}                                                                              \
                                                                               \
inline                                                                         \
type impl_prefix##_load(atomic_typename ref const self, MemOrd const ordering) \
{                                                                              \
    if (!mem_ord_is_valid_load(ordering)) {                                    \
        /* TODO: format the ordering. */                                       \
        PANIC_MSG("invalid memory ordering for a load");                       \
    }                                                                          \
                                                                               \
    /* SAFETY: we can cast an immutable ref to `inner` to a mutable pointer, */\
    /* as it is volatile and touched only by atomic operations. */             \
    type _Atomic volatile* const atomic_ptr = (void*)&self->inner;             \
    return atomic_load_explicit(                                               \
        atomic_ptr,                                                            \
        _ccc_mem_ord_to_memory_order(ordering)                                 \
    );                                                                         \
}                                                                              \
                                                                               \
inline                                                                         \
void impl_prefix##_store(                                                      \
    atomic_typename ref const self,                                            \
    type const val,                                                            \
    MemOrd const ordering                                                      \
)                                                                              \
{                                                                              \
    if (!mem_ord_is_valid_store(ordering)) {                                   \
        /* TODO: format the ordering. */                                       \
        PANIC_MSG("invalid memory ordering for a store");                      \
    }                                                                          \
                                                                               \
    /* SAFETY: we can cast an immutable ref to `inner` to a mutable pointer, */\
    /* as it is volatile and touched only by atomic operations. */             \
    type _Atomic volatile* const atomic_ptr = (void*)&self->inner;             \
    atomic_store_explicit(                                                     \
        atomic_ptr,                                                            \
        val,                                                                   \
        _ccc_mem_ord_to_memory_order(ordering)                                 \
    );                                                                         \
}

_ccc_def_atomic(AtomicBool, bool, bool, atomic_bool)

// Const versions.
#define ATOMIC_BOOL_CREATE_TRUE()    ((AtomicBool){ .inner = true, })
#define ATOMIC_BOOL_CREATE_FALSE()   ((AtomicBool){ .inner = false, })
#define ATOMIC_BOOL_CREATE_DEFAULT() ATOMIC_BOOL_CREATE_FALSE()

INLINE_ALWAYS
AtomicBool atomic_bool_create(bool const inner)
{
    ASSERT_MSG(inner == false || inner == true, "invalid boolean value");
    return (AtomicBool){ .inner = inner };
}

INLINE_ALWAYS
AtomicBool atomic_bool_create_default(void)
{
    return ATOMIC_BOOL_CREATE_DEFAULT();
}

_ccc_def_atomic(AtomicUsize, usize, usize, atomic_usize)

#define ATOMIC_USIZE_CREATE(val) ((AtomicUsize){ .inner = (val) })
#define ATOMIC_USIZE_CREATE_DEFAULT(val) ATOMIC_USIZE_CREATE(0u)

_ccc_def_atomic(AtomicU32, u32, u32, atomic_u32)

#define ATOMIC_U32_CREATE(val) ((AtomicU32){ .inner = (val) })
#define ATOMIC_U32_CREATE_DEFAULT(val) ATOMIC_U32_CREATE(0u)

#undef _ccc_def_atomic

#ifdef CCC_IMPLEMENTATION
#endif

#endif
