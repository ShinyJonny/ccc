#pragma once


#include "meta.h"
#include "primitive.h"
#include "array.h"


/// Creates a bit field with the length enough to hold `count` bits.
#define ccc_bit_field(count, ...) \
((BitField){                                                                   \
    .bytes = array(u8, ((count) + 7) >> 3, __VA_ARGS__),                       \
})                                                                             \


typedef struct {
    Array_u8 bytes;
} BitField;


inline
bool bit_field_can_index(BitField ref const self, usize const bit)
{
    return (bit >> 3) < self->bytes.len;
}


inline
bool bit_field_has_capacity_for(BitField ref const self, usize const bits)
{
    return ((bits + 7) >> 3) <= self->bytes.len;
}


inline
bool bit_field_get_composite(
    BitField ref const self,
    usize const byte,
    usize const bit
)
{
    CCC_ASSERT_MSG(byte < self->bytes.len, "index out of bounds");

    return ((self->bytes.dat[byte] >> bit) & (u8)0x01) == 1;
}


CCC_INLINE_ALWAYS
bool bit_field_get(BitField ref const self, usize const pos)
{
    usize const byte = pos >> 3;
    usize const bit  = pos - (byte << 3);

    return bit_field_get_composite(self, byte, bit);
}


inline
void bit_field_set_composite(
    BitField ref_mut const self,
    usize const byte,
    usize const bit
)
{
    CCC_ASSERT_MSG(byte < self->bytes.len, "index out of bounds");

    self->bytes.dat[byte] |= ((u8)0x01 << bit);
}


CCC_INLINE_ALWAYS
void bit_field_set(BitField ref_mut const self, usize const pos)
{
    usize const byte = pos >> 3;
    usize const bit  = pos - (byte << 3);

    bit_field_set_composite(self, byte, bit);
}


inline
void bit_field_clear_composite(
    BitField ref_mut const self,
    usize const byte,
    usize const bit
)
{
    CCC_ASSERT_MSG(byte < self->bytes.len, "index out of bounds");

    self->bytes.dat[byte] &= ~(0x01 << bit);
}


CCC_INLINE_ALWAYS
void bit_field_clear(BitField ref_mut const self, usize const pos)
{
    usize const byte = pos >> 3;
    usize const bit  = pos - (byte << 3);

    bit_field_clear_composite(self, byte, bit);
}


inline
void bit_field_clear_all(BitField ref_mut const self)
{
    slice_u8_fill(array_u8_as_slice_mut(&self->bytes), 0x00);
}


inline
void bit_field_set_all(BitField ref_mut const self)
{
    slice_u8_fill(array_u8_as_slice_mut(&self->bytes), 0xff);
}


#ifdef CCC_IMPLEMENTATION
bool bit_field_can_index(BitField ref const self, usize const bit);
bool bit_field_has_capacity_for(BitField ref const self, usize const bits);
bool bit_field_get_composite(
    BitField ref const self,
    usize const byte,
    usize const bit
);
bool bit_field_get(BitField ref const self, usize const pos);
void bit_field_set_composite(
    BitField ref_mut const self,
    usize const byte,
    usize const bit
);
void bit_field_set(BitField ref_mut const self, usize const pos);
void bit_field_clear_composite(
    BitField ref_mut const self,
    usize const byte,
    usize const bit
);
void bit_field_clear(BitField ref_mut const self, usize const pos);
void bit_field_clear_all(BitField ref_mut const self);
void bit_field_set_all(BitField ref_mut const self);
#endif
