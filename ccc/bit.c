#include "bit.h"

bool bit_field_can_index(BitField const self, usize const bit);
bool bit_field_has_capacity_for(BitField const self, usize const bits);
bool bit_field_get_composite(
    BitField const self,
    usize const byte,
    usize const bit
);
bool bit_field_get(BitField const self, usize const pos);
void bit_field_set_composite(
    BitField const self,
    usize const byte,
    usize const bit
);
void bit_field_set(BitField const self, usize const pos);
void bit_field_clear_composite(
    BitField const self,
    usize const byte,
    usize const bit
);
void bit_field_clear(BitField const self, usize const pos);
void bit_field_clear_all(BitField const self);
void bit_field_set_all(BitField const self);
