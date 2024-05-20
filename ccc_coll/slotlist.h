#pragma once

#include <ccc/bit.h>
#include <ccc/result.h>
#include <ccc/intrinsics.h>
#include <ccc/primitive.h>
#include <ccc/option.h>
#include <ccc/format.h>


typedef struct { usize idx; } SlotListId;

DEF_OPTION(SlotListId);

#define SL_ID_INVALID      ((SlotListId){ .idx = USIZE_MAX })
#define sl_id_is_valid(id) ((id).idx != SL_ID_INVALID.idx)


typedef enum {
    SlotListCapacityExceeded,
} SlotListError;


DEF_RESULT(SlotListIdResult, SlotListId, SlotListError);


typedef struct {
    SlotListId prev;
    SlotListId next;
} SlotListLinks;

DEF_SLICE(SlotListLinks);


typedef struct {
    SlotListId             head;
    SlotListId             tail;
    BitField               usage_map;
    SliceMut_SlotListLinks links;
} SlotList;

INLINE_ALWAYS
SlotList slot_list_create(
    BitField const usage_map,
    SliceMut_SlotListLinks const links
)
{
    ASSERT_MSG(
        bit_field_has_capacity_for(usage_map, links.len),
        "invalid length of `BifField`"
    );

    bit_field_clear_all(usage_map);

    return (SlotList){
        .head      = SL_ID_INVALID,
        .tail      = SL_ID_INVALID,
        .usage_map = usage_map,
        .links     = links,
    };
}

inline
Option_SlotListId slot_list_next(
    SlotList const* const self,
    SlotListId const id
)
{
    ASSERT_MSG(
        bit_field_can_index(self->usage_map, id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(self->usage_map, id.idx), "invalid ID");

    SlotListId const next_id = self->links.ptr[id.idx].next;
    if (!sl_id_is_valid(next_id)) {
        return (Option_SlotListId) none;
    }

    return (Option_SlotListId) wrap_some(next_id);
}

inline
Option_SlotListId slot_list_prev(
    SlotList const* const self,
    SlotListId const id
)
{
    ASSERT_MSG(
        bit_field_can_index(self->usage_map, id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(self->usage_map, id.idx), "invalid ID");

    SlotListId const prev_id = self->links.ptr[id.idx].prev;
    if (!sl_id_is_valid(prev_id)) {
        return (Option_SlotListId) none;
    }

    return (Option_SlotListId) wrap_some(prev_id);
}

inline
Option_SlotListId _slot_list_allocate(SlotList* const self)
{
    SlotListId new_id = SL_ID_INVALID;
    for (usize i = 0; i < self->links.len; i++) {
        if (!bit_field_get(self->usage_map, i)) {
            new_id = (SlotListId){ .idx = i };
            break;
        }
    }

    if (!sl_id_is_valid(new_id)) {
        return (Option_SlotListId) none;
    }

    bit_field_set(self->usage_map, new_id.idx);

    return (Option_SlotListId) wrap_some(new_id);
}

inline
SlotListIdResult slot_list_append(SlotList* const self)
{
    Option_SlotListId const opt_new_id = _slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) wrap_err(SlotListCapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;

    self->links.ptr[new_id.idx] = (SlotListLinks){
        .prev = self->tail,
        .next = SL_ID_INVALID,
    };

    if (sl_id_is_valid(self->tail)) {
        self->links.ptr[self->tail.idx].next = new_id;
    } else {
        self->head = new_id;
    }

    self->tail = new_id;

    return (SlotListIdResult) wrap_ok(new_id);
}

inline
SlotListIdResult slot_list_prepend(SlotList* const self)
{
    Option_SlotListId const opt_new_id = _slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) wrap_err(SlotListCapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;

    self->links.ptr[new_id.idx] = (SlotListLinks){
        .prev = SL_ID_INVALID,
        .next = self->head,
    };

    if (sl_id_is_valid(self->head)) {
        self->links.ptr[self->head.idx].prev = new_id;
    } else {
        self->tail = new_id;
    }

    self->head = new_id;

    return (SlotListIdResult) wrap_ok(new_id);
}

inline
SlotListIdResult slot_list_insert_after(
    SlotList* const self,
    SlotListId const prev_id
)
{
    ASSERT_MSG(
        bit_field_can_index(self->usage_map, prev_id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(self->usage_map, prev_id.idx), "invalid ID");

    SlotListLinks* const prev_slot = &self->links.ptr[prev_id.idx];

    Option_SlotListId const opt_new_id = _slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) wrap_err(SlotListCapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;

    self->links.ptr[new_id.idx] = (SlotListLinks){
        .prev = prev_id,
        .next = prev_slot->next,
    };

    prev_slot->next = new_id;

    if (self->tail.idx == prev_id.idx) {
        self->tail = new_id;
    }

    return (SlotListIdResult) wrap_ok(new_id);
}

inline
SlotListIdResult slot_list_insert_before(
    SlotList* const self,
    SlotListId const next_id
)
{
    ASSERT_MSG(
        bit_field_can_index(self->usage_map, next_id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(self->usage_map, next_id.idx), "invalid ID");

    SlotListLinks* const next_slot = &self->links.ptr[next_id.idx];

    Option_SlotListId const opt_new_id = _slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) wrap_err(SlotListCapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;

    self->links.ptr[new_id.idx] = (SlotListLinks){
        .prev = next_slot->prev,
        .next = next_id,
    };

    next_slot->prev = new_id;

    if (self->head.idx == next_id.idx) {
        self->head = new_id;
    }

    return (SlotListIdResult) wrap_ok(new_id);
}

inline
void slot_list_remove(
    SlotList* const self,
    SlotListId const id
)
{
    ASSERT_MSG(
        bit_field_can_index(self->usage_map, id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(self->usage_map, id.idx), "invalid ID");

    bit_field_clear(self->usage_map, id.idx);
    SlotListLinks* const slot  = &self->links.ptr[id.idx];

    if (self->head.idx == id.idx) {
        self->head = slot->next;
    }

    if (self->tail.idx == id.idx) {
        self->tail = slot->prev;
    }

    if (sl_id_is_valid(slot->next)) {
        self->links.ptr[slot->next.idx].prev = slot->prev;
    }

    if (sl_id_is_valid(slot->prev)) {
        self->links.ptr[slot->prev.idx].next = slot->next;
    }
}

inline
Option_SlotListId slot_list_pop_head(SlotList* const self)
{
    if (!sl_id_is_valid(self->head)) {
        return (Option_SlotListId) none;
    }

    SlotListId const id = self->head;
    slot_list_remove(self, id);

    return (Option_SlotListId) wrap_some(id);
}

inline
Option_SlotListId slot_list_pop_tail(SlotList* const self)
{
    if (!sl_id_is_valid(self->tail)) {
        return (Option_SlotListId) none;
    }

    SlotListId const id = self->tail;
    slot_list_remove(self, id);

    return (Option_SlotListId) wrap_some(id);
}

INLINE_ALWAYS
FmtResult slot_list_fmt_dbg(
    DynRefMut_Formatter const f,
    SlotList const* const list
)
{
    FmtResult res;
    if ((res = fmt_write_char(f, '[')).is_err) { return res; }

    if (sl_id_is_valid(list->head)) {
        if ((res = usize_fmt(f, list->head.idx)).is_err) { return res; }

        SlotListId next = list->links.ptr[list->head.idx].next;
        while (sl_id_is_valid(next)) {
            if ((res = fmt_write_str(f, cstr(" -> "))).is_err) { return res; }
            if ((res = usize_fmt(f, next.idx)).is_err)         { return res; }

            next = list->links.ptr[next.idx].next;
        }
    }

    return fmt_write_char(f, ']');
}

#undef SL_ID_INVALID
#undef sl_id_is_valid
