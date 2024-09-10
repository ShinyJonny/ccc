#pragma once


#include "primitive.h"


typedef struct {
    usize const size;
    void (* const dtor)(void ref_mut self);
} VTableHdr;


typedef struct {
    void ref_mut dst;
    struct { VTableHdr hdr; } const* CCC_RESTRICT vtable;
} DropRefMut;


#define CCC_DEF_DYN_REF_NAMED(iface_type, refname) \
typedef struct {                                                               \
    void ref dst;                                                              \
    iface_type const* CCC_RESTRICT vtable;                                     \
} refname                                                                      \


#define CCC_DEF_DYN_REF(iface_type) \
    CCC_DEF_DYN_REF_NAMED(iface_type, DynRef_##iface_type)


#define CCC_DEF_DYN_REF_MUT_NAMED(iface_type, refname) \
typedef struct {                                                               \
    void ref_mut dst;                                                          \
    iface_type const* CCC_RESTRICT vtable;                                     \
} refname                                                                      \


#define CCC_DEF_DYN_REF_MUT(iface_type) \
    CCC_DEF_DYN_REF_MUT_NAMED(iface_type, DynRefMut_##iface_type)
