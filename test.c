#define CCC_IMPLEMENTATION
#define CCC_COLL_IMPLEMENTATION

#include <ccc/primitive.h>
#include <ccc/str.h>
#include <ccc/format.h>
#include <ccc/result.h>
#include <ccc_coll/slotlist.h>
#include <stdio.h>
#include <ccc/bit.h>
#include <ccc/cmp.h>

#include "buffmt.h"


CCC_DEF_RESULT(MainResult, __unit, FmtError)


MainResult run(SlotList ref const list)
{
    BufFormatter buf_fmt = buf_fmt_create((Array_u8)array(u8, 1024, 0));
    DynRefMut_Formatter const fmt = buf_fmt_as_dyn_mut_formatter(&buf_fmt);

    StrSplit const split = str_split_at(cstr("Harro, everynyan!"), 5);
    FmtResult result;
    if ((result = str_fmt(fmt, cstr("StrSplit (\""))).is_err) {
        return (MainResult) ERR(result.err);
    }
    if ((result = str_fmt(fmt, split._0)).is_err) {
        return (MainResult) ERR(result.err);
    }
    if ((result = str_fmt(fmt, cstr("\", \""))).is_err) {
        return (MainResult) ERR(result.err);
    }
    if ((result = str_fmt(fmt, split._1)).is_err) {
        return (MainResult) ERR(result.err);
    }
    if ((result = str_fmt(fmt, cstr("\")"))).is_err) {
        return (MainResult) ERR(result.err);
    }

    if ((result = str_fmt(fmt, cstr("\n"))).is_err) {
        return (MainResult) ERR(result.err);
    }
    if ((result = slot_list_fmt_dbg(fmt, list)).is_err) {
        return (MainResult) ERR(result.err);
    }

    str const final = buf_fmt_get_str(&buf_fmt);
    printf(STRF "\n", PSTR(final));

    return (MainResult) OK(unit);
}


typedef struct {
    usize magic;
    str text;
} Node;

CCC_DEF_ARRAY(Node, node)


int main(void)
{
    Array_Node const list_data = array(Node, 1024, 0);
    SlotList list = slot_list_create(
        bit_field(1024, 0),
        (Array_SlotListLinks) array(SlotListLinks, 1024, 0)
    );

    SlotListIdResult const first = slot_list_append(&list);
    ASSERT(!first.is_err);
    list_data.ptr[first.ok.idx] = (Node){ .magic = 3, .text = cstr("three") };
    ASSERT(list.tail.idx == first.ok.idx);
    ASSERT(!slot_list_next(&list, first.ok).is_some);
    SlotListIdResult const second = slot_list_append(&list);
    ASSERT(!second.is_err);
    list_data.ptr[second.ok.idx] = (Node){ .magic = 6, .text = cstr("six") };
    ASSERT(slot_list_next(&list, first.ok).is_some);
    ASSERT(slot_list_next(&list, first.ok).val.idx == second.ok.idx);
    ASSERT(list.tail.idx == second.ok.idx);
    SlotListIdResult const third = slot_list_prepend(&list);
    ASSERT(!third.is_err);
    list_data.ptr[third.ok.idx] = (Node){ .magic = 9, .text = cstr("nine") };

    MainResult const result = run(&list);
    if (result.is_err) {
        Array_u8 const buffer = array(u8, 4096, 0);
        BufFormatter buf_fmt = buf_fmt_create(buffer);
        DynRefMut_Formatter const fmt = buf_fmt_as_dyn_mut_formatter(&buf_fmt);

        (void) fmt_write_str(fmt, cstr("Error: "));
        (void) fmt_error_fmt_dbg(fmt, &result.err);
        str const err_msg = buf_fmt_get_str(&buf_fmt);
        fprintf(stderr, STRF "\n", PSTR(err_msg));

        return 1;
    }
}
