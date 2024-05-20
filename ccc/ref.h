//#undef CCC_RESTRICT_REFERENCES
#ifdef CCC_RESTRICT_REFERENCES
    #define DEF_REF_NAMED(type, refname) \
    typedef struct {                     \
        type const* restrict ptr;        \
    } refname
#else
    #define DEF_REF_NAMED(type, refname) \
    typedef struct {                     \
        type const* ptr;                 \
    } refname
#endif

#define DEF_REF(type) DEF_REF_NAMED(type, Ref_##type)

#ifdef CCC_RESTRICT_REFERENCES
    #define DEF_REF_MUT_NAMED(type, refname) \
    typedef struct {                         \
        type* restrict ptr;                  \
    } refname
#else
    #define DEF_REF_MUT_NAMED(type, refname) \
    typedef struct {                         \
        type* ptr;                           \
    } refname
#endif

#define DEF_REF_MUT(type) DEF_REF_MUT_NAMED(type, RefMut_##type)

// ---

#ifdef CCC_RESTRICT_REFERENCES
    #define DEF_DYN_REF_NAMED(iface_type, refname)       \
    typedef struct {                                     \
        void const* restrict ptr;                        \
        iface_type const* restrict iface;                \
    } refname
#else
    #define DEF_DYN_REF_NAMED(iface_type, name)       \
    typedef struct {                                  \
        void const* ptr;                              \
        iface_type const* restrict iface;             \
    } refname
#endif

#define DEF_DYN_REF(iface_type) DEF_DYN_REF_NAMED(iface_type, DynRef_##iface_type)

#ifdef CCC_RESTRICT_REFERENCES
    #define DEF_DYN_REF_MUT_NAMED(iface_type, refname)       \
    typedef struct {                                         \
        void* restrict ptr;                                  \
        iface_type const* restrict iface;                    \
    } refname
#else
    #define DEF_DYN_REF_MUT_NAMED(iface_type, refname)       \
    typedef struct {                                         \
        void* ptr;                                           \
        iface_type const* restrict iface;                    \
    } refname
#endif

#define DEF_DYN_REF_MUT(iface_type) DEF_DYN_REF_MUT_NAMED(iface_type, DynRefMut_##iface_type)

DEF_REF_NAMED(void, Ref);
DEF_REF_MUT_NAMED(void, RefMut);
DEF_DYN_REF_NAMED(void, DynRef);
DEF_DYN_REF_MUT_NAMED(void, DynRefMut);

#define as_ref(ref) { .ptr = (ref).ptr }
#define as_dyn(ref)       \
{                         \
    .ptr   = (ref).ptr,   \
    .iface = (ref).iface, \
}                         \
