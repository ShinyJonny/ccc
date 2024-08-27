#pragma once

#include "ccc/primitive.h"


#ifdef CCC_RESTRICT_REFERENCES
    #define ref const* restrict
    #define ref_mut * restrict
#else
    #define ref const*
    #define ref_mut *
#endif

typedef struct {
    usize const size;
    void (* const dtor)(void ref_mut self);
} VTableHdr;

typedef struct {
    void ref_mut dst;
    struct { VTableHdr hdr; } const* restrict vtable;
} DropRefMut;

#define CCC_DEF_DYN_REF_NAMED(iface_type, refname) \
typedef struct {                       \
    void ref dst;                      \
    iface_type const* restrict vtable; \
} refname

#define CCC_DEF_DYN_REF(iface_type) \
    CCC_DEF_DYN_REF_NAMED(iface_type, DynRef_##iface_type)

#define CCC_DEF_DYN_REF_MUT_NAMED(iface_type, refname) \
typedef struct {                       \
    void ref_mut dst;                  \
    iface_type const* restrict vtable; \
} refname

#define CCC_DEF_DYN_REF_MUT(iface_type) \
    CCC_DEF_DYN_REF_MUT_NAMED(iface_type, DynRefMut_##iface_type)

#ifdef CCC_IMPLEMENTATION
#endif
