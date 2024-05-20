#include "slotlist.h"


SlotList slot_list_create(
    BitField const usage_map,
    SliceMut_SlotListLinks const links
);
Option_SlotListId slot_list_next(
    SlotList const* const self,
    SlotListId const id
);
SlotListIdResult slot_list_append(SlotList* const self);
SlotListIdResult slot_list_prepend(SlotList* const self);
FmtResult slot_list_fmt_dbg(
    DynRefMut_Formatter const f,
    SlotList const* const list
);
Option_SlotListId _slot_list_allocate(SlotList* const self);
