#pragma once


#include "meta.h"
#include "primitive.h"
#include "format.h"
#include "array.h"
#include "atomic.h"

/// TODO: remove this.
#include <unistd.h>


#ifdef CCC_MODULE_IO


#ifndef CCC_MODULE_ATOMICS
    #error `CCC_MODULE_IO` requires `CCC_MODULE_ATOMICS`
#endif


typedef enum {
    IoError_FlushFailed,
} IoError;


CCC_DEF_RESULT(IoResult, __unit, IoError)


typedef struct {
    Array_u8 buffer;
    usize head;
    u8 fd;
} StdioSinkBuf;


// TODO: move this to a `sync` module.
#define LOCK_UNLOCKED false
#define LOCK_LOCKED   true


typedef struct {
    StdioSinkBuf* buf;
    AtomicBool ref lock;
} StdioSink;

typedef struct {
    StdioSinkBuf ref_mut buf;
    AtomicBool ref lock;
} StdioSinkGuard;

inline
StdioSinkGuard stdio_sink_lock(StdioSink const handle)
{
    CmpXchgResult_bool result;
    while ((result = atomic_bool_compare_exchange_weak(
        handle.lock,
        LOCK_UNLOCKED,
        LOCK_LOCKED,
        MemOrd_Acquire,
        MemOrd_Relaxed
    )).is_err)
    {
        while (atomic_bool_load(handle.lock, MemOrd_Relaxed) == LOCK_LOCKED) {}
    };

    return (StdioSinkGuard) {
        .buf  = handle.buf,
        .lock = handle.lock,
    };
}

CCC_INLINE_ALWAYS
void stdio_sink_guard__Drop__drop(void ref_mut const void_self);

inline
void stdio_sink_guard_release(StdioSinkGuard sink)
{
    stdio_sink_guard__Drop__drop(&sink);
}

inline
IoResult stdio_sink_guard_flush(StdioSinkGuard ref_mut const self)
{
    SliceMut_u8 const used_slice = slice_u8_slice_mut(
        array_u8_as_slice_mut(&self->buf->buffer),
        0,
        self->buf->head
    );

    Slice_u8 remaining = slice_u8_from_mut(used_slice);

    while (remaining.len > 0) {
        isize count = write(self->buf->fd, remaining.dat, remaining.len);

        if (count < 0) {
            return (IoResult) CCC_ERR(IoError_FlushFailed);
        }

        remaining = slice_u8_slice(remaining, count, remaining.len);
    }

    ccc_memset(used_slice.dat, '0', used_slice.len);
    self->buf->head = 0;

    return (IoResult) CCC_OK(CCC_UNIT);
}

CCC_INLINE_ALWAYS
void stdio_sink_guard__Drop__drop(void ref_mut const void_self)
{
    StdioSinkGuard ref_mut const self = (void*)void_self;
    atomic_bool_store(self->lock, LOCK_UNLOCKED, MemOrd_Release);
}

CCC_INLINE_ALWAYS
FmtResult stdio_sink_guard__Formatter__write_str(
    void ref_mut const void_ctx,
    str const s
)
{
    StdioSinkGuard ref_mut const ctx = void_ctx;
    Slice_u8 const bytes = str_as_bytes(s);

    Array_u8 ref buffer = &ctx->buf->buffer;

    if (ctx->buf->head + bytes.len > buffer->len) {
        IoResult ret;
        if ((ret = stdio_sink_guard_flush(ctx)).is_err) {
            return (FmtResult) CCC_ERR((FmtError) {
                .kind = FmtErrorKind_FormatterError,
                .fmt_error = ret.err,
            });
        };
    }

    ccc_copy_nonoverlapping(buffer->dat + ctx->buf->head, bytes.dat, bytes.len);
    ctx->buf->head += bytes.len;

    return (FmtResult) CCC_OK(CCC_UNIT);
}

CCC_INLINE_ALWAYS
FmtResult stdio_sink_guard__Formatter__write_char(
    void ref_mut const void_ctx,
    char const c
)
{
    StdioSinkGuard ref_mut ctx = void_ctx;

    Array_u8 ref const buffer = &ctx->buf->buffer;

    if (ctx->buf->head >= buffer->len) {
        IoResult ret;
        if ((ret = stdio_sink_guard_flush(ctx)).is_err) {
            return (FmtResult) CCC_ERR((FmtError) {
                .kind = FmtErrorKind_FormatterError,
                .fmt_error = ret.err,
            });
        };
    }
    buffer->dat[ctx->buf->head] = c;
    ctx->buf->head += 1;

    return (FmtResult) CCC_OK(CCC_UNIT);
}

static
Formatter const STDIO_SINK_GUARD__Formatter = {
    .hdr.size   = sizeof(StdioSinkGuard),
    .hdr.dtor   = stdio_sink_guard__Drop__drop,
    .write_str  = stdio_sink_guard__Formatter__write_str,
    .write_char = stdio_sink_guard__Formatter__write_char,
};

static CCC_INLINE_ALWAYS
DynRefMut_Formatter stdio_sink_guard_as_dyn_mut_formatter(
    StdioSinkGuard ref_mut const self
)
{
    return (DynRefMut_Formatter) {
        .dst    = self,
        .vtable = &STDIO_SINK_GUARD__Formatter,
    };
}


extern StdioSink io_stderr(void);
extern StdioSink io_stdout(void);


#ifdef CCC_IMPLEMENTATION
#include "util.h"
#include "assertions.h"

// TODO: FIXME: currently, it is implemented as a spinlock.
// TODO: FIXME: it is not re-entrant from the same thread.
AtomicBool _CCC_STDERR_LOCK = ATOMIC_BOOL_CREATE_FALSE();
AtomicBool _CCC_STDOUT_LOCK = ATOMIC_BOOL_CREATE_FALSE();

#define CCC_IO_BUF_SIZE 4096
StdioSinkBuf _CCC_STDERR = {
    .buffer = ccc_array(u8, CCC_IO_BUF_SIZE, 0),
    .head   = 0,
    .fd     = STDERR_FILENO,
};
StdioSinkBuf _CCC_STDOUT = {
    .buffer = ccc_array(u8, CCC_IO_BUF_SIZE, 0),
    .head   = 0,
    .fd     = STDOUT_FILENO,
};
#undef CCC_IO_BUF_SIZE


/// Gets the handle to the *standard error* sink and initialises it (if not
/// initialised already).
StdioSink io_stderr(void)
{
    // NOTE: init here if needed.

    return (StdioSink) {
        .buf = &_CCC_STDERR,
        .lock = &_CCC_STDERR_LOCK,
    };
}


/// Gets the handle to the *standard output* sink and initialises it (if not
/// initialised already).
StdioSink io_stdout(void)
{
    // NOTE: init here if needed.

    return (StdioSink) {
        .buf = &_CCC_STDOUT,
        .lock = &_CCC_STDOUT_LOCK,
    };
}


CCC_NORETURN
void ccc_panic_handler(PanicInfo ref const info)
{
    StdioSinkGuard std_err = stdio_sink_lock(io_stderr());

    DynRefMut_Formatter f = stdio_sink_guard_as_dyn_mut_formatter(&std_err);
    (void) fmt_write_str(f, CCC_STR("PANIC: "));
    (void) panic_info_fmt(f, info);
    (void) str_fmt(f, CCC_STR("\n"));
    (void) stdio_sink_guard_flush(&std_err);

    stdio_sink_guard_release(std_err);

    _exit(1);
}


StdioSinkGuard stdio_sink_lock(StdioSink const handle);
void stdio_sink_guard_release(StdioSinkGuard sink);
IoResult stdio_sink_guard_flush(StdioSinkGuard ref_mut const self);
void stdio_sink_guard__Drop__drop(void ref_mut const void_self);
FmtResult stdio_sink_guard__Formatter__write_str(
    void ref_mut const void_ctx,
    str const s
);
FmtResult stdio_sink_guard__Formatter__write_char(
    void ref_mut const void_ctx,
    char const c
);
DynRefMut_Formatter stdio_sink_guard_as_dyn_mut_formatter(
    StdioSinkGuard ref_mut const self
);
#endif

#endif
