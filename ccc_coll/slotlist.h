#pragma once


#include <ccc/bit.h>
#include <ccc/result.h>
#include <ccc/intrinsics.h>
#include <ccc/primitive.h>
#include <ccc/option.h>
#include <ccc/format.h>


typedef struct { usize idx; } SlotListId;

CCC_DEF_OPTION(SlotListId)

#define _CCC_SL_ID_INVALID      ((SlotListId){ .idx = CCC_USIZE_MAX })
#define _ccc_sl_id_is_valid(id) ((id).idx != _CCC_SL_ID_INVALID.idx)


typedef enum {
    SlotListError_CapacityExceeded,
} SlotListError;


CCC_DEF_RESULT(SlotListIdResult, SlotListId, SlotListError)


typedef struct {
    SlotListId prev;
    SlotListId next;
} SlotListLinks;

CCC_DEF_ARRAY(SlotListLinks, slot_list_links)


typedef struct {
    SlotListId          head;
    SlotListId          tail;
    BitField            usage_map;
    Array_SlotListLinks links;
} SlotList;

CCC_INLINE_ALWAYS
SlotList slot_list_create(BitField usage_map, Array_SlotListLinks const links)
{
    CCC_ASSERT_MSG(
        bit_field_has_capacity_for(&usage_map, links.len),
        "invalid length of `BifField`"
    );

    bit_field_clear_all(&usage_map);

    return (SlotList){
        .head      = _CCC_SL_ID_INVALID,
        .tail      = _CCC_SL_ID_INVALID,
        .usage_map = usage_map,
        .links     = links,
    };
}

inline
Option_SlotListId slot_list_next(SlotList ref const self, SlotListId const id)
{
    CCC_ASSERT_MSG(
        bit_field_can_index(&self->usage_map, id.idx),
        "ID out of bounds"
    );
    CCC_ASSERT_MSG(bit_field_get(&self->usage_map, id.idx), "invalid ID");

    SlotListId const next_id = self->links.dat[id.idx].next;
    if (!_ccc_sl_id_is_valid(next_id)) {
        return (Option_SlotListId) CCC_NONE;
    }

    return (Option_SlotListId) CCC_SOME(next_id);
}

inline
Option_SlotListId slot_list_prev(SlotList ref const self, SlotListId const id)
{
    CCC_ASSERT_MSG(
        bit_field_can_index(&self->usage_map, id.idx),
        "ID out of bounds"
    );
    CCC_ASSERT_MSG(bit_field_get(&self->usage_map, id.idx), "invalid ID");

    SlotListId const prev_id = self->links.dat[id.idx].prev;
    if (!_ccc_sl_id_is_valid(prev_id)) {
        return (Option_SlotListId) CCC_NONE;
    }

    return (Option_SlotListId) CCC_SOME(prev_id);
}

inline
Option_SlotListId _ccc_slot_list_allocate(SlotList ref_mut const self)
{
    SlotListId new_id = _CCC_SL_ID_INVALID;
    for (usize i = 0; i < self->links.len; i++) {
        if (!bit_field_get(&self->usage_map, i)) {
            new_id = (SlotListId){ .idx = i };
            break;
        }
    }

    if (!_ccc_sl_id_is_valid(new_id)) {
        return (Option_SlotListId) CCC_NONE;
    }

    bit_field_set(&self->usage_map, new_id.idx);

    return (Option_SlotListId) CCC_SOME(new_id);
}

inline
SlotListIdResult slot_list_append(SlotList ref_mut const self)
{
    Option_SlotListId const opt_new_id = _ccc_slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) CCC_ERR(SlotListError_CapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;

    self->links.dat[new_id.idx] = (SlotListLinks) {
        .prev = self->tail,
        .next = _CCC_SL_ID_INVALID,
    };

    if (_ccc_sl_id_is_valid(self->tail)) {
        self->links.dat[self->tail.idx].next = new_id;
    } else {
        self->head = new_id;
    }

    self->tail = new_id;

    return (SlotListIdResult) CCC_OK(new_id);
}

inline
SlotListIdResult slot_list_prepend(SlotList ref_mut const self)
{
    Option_SlotListId const opt_new_id = _ccc_slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) CCC_ERR(SlotListError_CapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;

    self->links.dat[new_id.idx] = (SlotListLinks) {
        .prev = _CCC_SL_ID_INVALID,
        .next = self->head,
    };

    if (_ccc_sl_id_is_valid(self->head)) {
        self->links.dat[self->head.idx].prev = new_id;
    } else {
        self->tail = new_id;
    }

    self->head = new_id;

    return (SlotListIdResult) CCC_OK(new_id);
}

inline
SlotListIdResult slot_list_insert_after(
    SlotList ref_mut const self,
    SlotListId const prev_id
)
{
    CCC_ASSERT_MSG(
        bit_field_can_index(&self->usage_map, prev_id.idx),
        "ID out of bounds"
    );
    CCC_ASSERT_MSG(bit_field_get(&self->usage_map, prev_id.idx), "invalid ID");

    Option_SlotListId const opt_new_id = _ccc_slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) CCC_ERR(SlotListError_CapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;
    SlotListId const new_next_id = self->links.dat[prev_id.idx].next;

    self->links.dat[new_id.idx] = (SlotListLinks){
        .prev = prev_id,
        .next = new_next_id,
    };

    self->links.dat[prev_id.idx].next = new_id;

    if (self->tail.idx == prev_id.idx) {
        self->tail = new_id;
    }

    return (SlotListIdResult) CCC_OK(new_id);
}

inline
SlotListIdResult slot_list_insert_before(
    SlotList ref_mut const self,
    SlotListId const next_id
)
{
    CCC_ASSERT_MSG(
        bit_field_can_index(&self->usage_map, next_id.idx),
        "ID out of bounds"
    );
    CCC_ASSERT_MSG(bit_field_get(&self->usage_map, next_id.idx), "invalid ID");

    Option_SlotListId const opt_new_id = _ccc_slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) CCC_ERR(SlotListError_CapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;
    SlotListId const new_prev = self->links.dat[next_id.idx].prev;

    self->links.dat[new_id.idx] = (SlotListLinks){
        .prev = new_prev,
        .next = next_id,
    };

    self->links.dat[next_id.idx].prev = new_id;

    if (self->head.idx == next_id.idx) {
        self->head = new_id;
    }

    return (SlotListIdResult) CCC_OK(new_id);
}

inline
void slot_list_remove(SlotList ref_mut const self, SlotListId const id)
{
    CCC_ASSERT_MSG(
        bit_field_can_index(&self->usage_map, id.idx),
        "ID out of bounds"
    );
    CCC_ASSERT_MSG(bit_field_get(&self->usage_map, id.idx), "invalid ID");

    bit_field_clear(&self->usage_map, id.idx);

    SlotListLinks const slot = self->links.dat[id.idx];

    if (self->head.idx == id.idx) {
        self->head = slot.next;
    }

    if (self->tail.idx == id.idx) {
        self->tail = slot.prev;
    }

    if (_ccc_sl_id_is_valid(slot.next)) {
        self->links.dat[slot.next.idx].prev = slot.prev;
    }

    if (_ccc_sl_id_is_valid(slot.prev)) {
        self->links.dat[slot.prev.idx].next = slot.next;
    }
}

inline
Option_SlotListId slot_list_pop_head(SlotList ref_mut const self)
{
    if (!_ccc_sl_id_is_valid(self->head)) {
        return (Option_SlotListId) CCC_NONE;
    }

    SlotListId const id = self->head;
    slot_list_remove(self, id);

    return (Option_SlotListId) CCC_SOME(id);
}

inline
Option_SlotListId slot_list_pop_tail(SlotList ref_mut const self)
{
    if (!_ccc_sl_id_is_valid(self->tail)) {
        return (Option_SlotListId) CCC_NONE;
    }

    SlotListId const id = self->tail;
    slot_list_remove(self, id);

    return (Option_SlotListId) CCC_SOME(id);
}

CCC_INLINE_ALWAYS
FmtResult slot_list_fmt_dbg(
    DynRefMut_Formatter const f,
    SlotList ref const list
)
{
    FmtResult res;
    if ((res = fmt_write_char(f, '[')).is_err) { return res; }

    if (_ccc_sl_id_is_valid(list->head)) {
        if ((res = usize_fmt(f, list->head.idx)).is_err) { return res; }

        SlotListId next = list->links.dat[list->head.idx].next;
        while (_ccc_sl_id_is_valid(next)) {
            if ((res = fmt_write_str(f, CCC_STR(" -> "))).is_err)
                { return res; }
            if ((res = usize_fmt(f, next.idx)).is_err)         { return res; }

            next = list->links.dat[next.idx].next;
        }
    }

    return fmt_write_char(f, ']');
}


#ifdef CCC_COLL_IMPLEMENTATION
SlotList slot_list_create(BitField usage_map, Array_SlotListLinks const links);
Option_SlotListId slot_list_next(SlotList ref const self, SlotListId const id);
Option_SlotListId slot_list_prev(SlotList ref const self, SlotListId const id);
Option_SlotListId _ccc_slot_list_allocate(SlotList ref_mut const self);
SlotListIdResult slot_list_append(SlotList ref_mut const self);
SlotListIdResult slot_list_prepend(SlotList ref_mut const self);
SlotListIdResult slot_list_insert_after(
    SlotList ref_mut const self,
    SlotListId const prev_id
);
SlotListIdResult slot_list_insert_before(
    SlotList ref_mut const self,
    SlotListId const next_id
);
void slot_list_remove(SlotList ref_mut const self, SlotListId const id);
Option_SlotListId slot_list_pop_head(SlotList ref_mut const self);
Option_SlotListId slot_list_pop_tail(SlotList ref_mut const self);
FmtResult slot_list_fmt_dbg(
    DynRefMut_Formatter const f,
    SlotList ref const list
);
#endif


#undef _CCC_SL_ID_INVALID
#undef _ccc_sl_id_is_valid
