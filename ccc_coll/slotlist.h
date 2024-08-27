#pragma once

#include <ccc/bit.h>
#include <ccc/result.h>
#include <ccc/intrinsics.h>
#include <ccc/primitive.h>
#include <ccc/option.h>
#include <ccc/format.h>


typedef struct { usize idx; } SlotListId;

DEF_OPTION(SlotListId)

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

DEF_ARRAY(SlotListLinks, slot_list_links)


typedef struct {
    SlotListId          head;
    SlotListId          tail;
    BitField            usage_map;
    Array_SlotListLinks links;
} SlotList;

INLINE_ALWAYS
SlotList slot_list_create(BitField usage_map, Array_SlotListLinks const links)
{
    ASSERT_MSG(
        bit_field_has_capacity_for(&usage_map, links.len),
        "invalid length of `BifField`"
    );

    bit_field_clear_all(&usage_map);

    return (SlotList){
        .head      = SL_ID_INVALID,
        .tail      = SL_ID_INVALID,
        .usage_map = usage_map,
        .links     = links,
    };
}

inline
Option_SlotListId slot_list_next(SlotList ref const self, SlotListId const id)
{
    ASSERT_MSG(
        bit_field_can_index(&self->usage_map, id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(&self->usage_map, id.idx), "invalid ID");

    Slice_SlotListLinks const links
        = array_slot_list_links_as_ref(&self->links);

    SlotListId const next_id = links.dat[id.idx].next;
    if (!sl_id_is_valid(next_id)) {
        return (Option_SlotListId) NONE;
    }

    return (Option_SlotListId) SOME(next_id);
}

inline
Option_SlotListId slot_list_prev(SlotList ref const self, SlotListId const id)
{
    ASSERT_MSG(
        bit_field_can_index(&self->usage_map, id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(&self->usage_map, id.idx), "invalid ID");

    Slice_SlotListLinks const links
        = array_slot_list_links_as_ref(&self->links);

    SlotListId const prev_id = links.dat[id.idx].prev;
    if (!sl_id_is_valid(prev_id)) {
        return (Option_SlotListId) NONE;
    }

    return (Option_SlotListId) SOME(prev_id);
}

inline
Option_SlotListId _slot_list_allocate(SlotList ref_mut const self)
{
    SlotListId new_id = SL_ID_INVALID;
    for (usize i = 0; i < self->links.len; i++) {
        if (!bit_field_get(&self->usage_map, i)) {
            new_id = (SlotListId){ .idx = i };
            break;
        }
    }

    if (!sl_id_is_valid(new_id)) {
        return (Option_SlotListId) NONE;
    }

    bit_field_set(&self->usage_map, new_id.idx);

    return (Option_SlotListId) SOME(new_id);
}

inline
SlotListIdResult slot_list_append(SlotList ref_mut const self)
{
    Option_SlotListId const opt_new_id = _slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) ERR(SlotListCapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;

    SliceMut_SlotListLinks const links
        = array_slot_list_links_as_mut(&self->links);

    links.dat[new_id.idx] = (SlotListLinks){
        .prev = self->tail,
        .next = SL_ID_INVALID,
    };

    if (sl_id_is_valid(self->tail)) {
        links.dat[self->tail.idx].next = new_id;
    } else {
        self->head = new_id;
    }

    self->tail = new_id;

    return (SlotListIdResult) OK(new_id);
}

inline
SlotListIdResult slot_list_prepend(SlotList ref_mut const self)
{
    Option_SlotListId const opt_new_id = _slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) ERR(SlotListCapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;

    SliceMut_SlotListLinks const links
        = array_slot_list_links_as_mut(&self->links);

    links.dat[new_id.idx] = (SlotListLinks){
        .prev = SL_ID_INVALID,
        .next = self->head,
    };

    if (sl_id_is_valid(self->head)) {
        links.dat[self->head.idx].prev = new_id;
    } else {
        self->tail = new_id;
    }

    self->head = new_id;

    return (SlotListIdResult) OK(new_id);
}

inline
SlotListIdResult slot_list_insert_after(
    SlotList ref_mut const self,
    SlotListId const prev_id
)
{
    ASSERT_MSG(
        bit_field_can_index(&self->usage_map, prev_id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(&self->usage_map, prev_id.idx), "invalid ID");

    SliceMut_SlotListLinks const links
        = array_slot_list_links_as_mut(&self->links);

    Option_SlotListId const opt_new_id = _slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) ERR(SlotListCapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;
    SlotListId const new_next_id = links.dat[prev_id.idx].next;

    links.dat[new_id.idx] = (SlotListLinks){
        .prev = prev_id,
        .next = new_next_id,
    };

    links.dat[prev_id.idx].next = new_id;

    if (self->tail.idx == prev_id.idx) {
        self->tail = new_id;
    }

    return (SlotListIdResult) OK(new_id);
}

inline
SlotListIdResult slot_list_insert_before(
    SlotList ref_mut const self,
    SlotListId const next_id
)
{
    ASSERT_MSG(
        bit_field_can_index(&self->usage_map, next_id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(&self->usage_map, next_id.idx), "invalid ID");

    SliceMut_SlotListLinks const links
        = array_slot_list_links_as_mut(&self->links);

    Option_SlotListId const opt_new_id = _slot_list_allocate(self);
    if (!opt_new_id.is_some) {
        return (SlotListIdResult) ERR(SlotListCapacityExceeded);
    }

    SlotListId const new_id = opt_new_id.val;
    SlotListId const new_prev = links.dat[next_id.idx].prev;

    links.dat[new_id.idx] = (SlotListLinks){
        .prev = new_prev,
        .next = next_id,
    };

    links.dat[next_id.idx].prev = new_id;

    if (self->head.idx == next_id.idx) {
        self->head = new_id;
    }

    return (SlotListIdResult) OK(new_id);
}

inline
void slot_list_remove(SlotList ref_mut const self, SlotListId const id)
{
    ASSERT_MSG(
        bit_field_can_index(&self->usage_map, id.idx),
        "ID out of bounds"
    );
    ASSERT_MSG(bit_field_get(&self->usage_map, id.idx), "invalid ID");

    bit_field_clear(&self->usage_map, id.idx);

    SliceMut_SlotListLinks const links
        = array_slot_list_links_as_mut(&self->links);

    SlotListLinks const slot = links.dat[id.idx];

    if (self->head.idx == id.idx) {
        self->head = slot.next;
    }

    if (self->tail.idx == id.idx) {
        self->tail = slot.prev;
    }

    if (sl_id_is_valid(slot.next)) {
        links.dat[slot.next.idx].prev = slot.prev;
    }

    if (sl_id_is_valid(slot.prev)) {
        links.dat[slot.prev.idx].next = slot.next;
    }
}

inline
Option_SlotListId slot_list_pop_head(SlotList ref_mut const self)
{
    if (!sl_id_is_valid(self->head)) {
        return (Option_SlotListId) NONE;
    }

    SlotListId const id = self->head;
    slot_list_remove(self, id);

    return (Option_SlotListId) SOME(id);
}

inline
Option_SlotListId slot_list_pop_tail(SlotList ref_mut const self)
{
    if (!sl_id_is_valid(self->tail)) {
        return (Option_SlotListId) NONE;
    }

    SlotListId const id = self->tail;
    slot_list_remove(self, id);

    return (Option_SlotListId) SOME(id);
}

INLINE_ALWAYS
FmtResult slot_list_fmt_dbg(
    DynRefMut_Formatter const f,
    SlotList ref const list
)
{
    Slice_SlotListLinks const links
        = array_slot_list_links_as_ref(&list->links);

    FmtResult res;
    if ((res = fmt_write_char(f, '[')).is_err) { return res; }

    if (sl_id_is_valid(list->head)) {
        if ((res = usize_fmt(f, list->head.idx)).is_err) { return res; }

        SlotListId next = links.dat[list->head.idx].next;
        while (sl_id_is_valid(next)) {
            if ((res = fmt_write_str(f, cstr(" -> "))).is_err) { return res; }
            if ((res = usize_fmt(f, next.idx)).is_err)         { return res; }

            next = links.dat[next.idx].next;
        }
    }

    return fmt_write_char(f, ']');
}

#undef SL_ID_INVALID
#undef sl_id_is_valid


#ifdef CCC_COLL_IMPLEMENTATION
SlotList slot_list_create(BitField usage_map, Array_SlotListLinks links);
Option_SlotListId slot_list_next(SlotList ref self, SlotListId id);
Option_SlotListId slot_list_prev(SlotList ref self, SlotListId id);
Option_SlotListId _slot_list_allocate(SlotList ref_mut self);
SlotListIdResult slot_list_append(SlotList ref_mut self);
SlotListIdResult slot_list_prepend(SlotList ref_mut self);
FmtResult slot_list_fmt_dbg(DynRefMut_Formatter f, SlotList ref list);
#endif
