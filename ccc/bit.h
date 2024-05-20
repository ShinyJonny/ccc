#pragma once

#include "slice.h"
#include "primitive.h"


/// Creates a bit field with the length enough to hold `count` bits.
#define bit_field(count, ...)                            \
{                                                        \
    .bytes = array(u8, ((count) + 7) >> 3, __VA_ARGS__), \
}


typedef struct {
    SliceMut_u8 bytes;
} BitField;


inline
bool bit_field_can_index(BitField const self, usize const bit)
{
    return (bit >> 3) < self.bytes.len;
}


inline
bool bit_field_has_capacity_for(BitField const self, usize const bits)
{
    return ((bits + 7) >> 3) <= self.bytes.len;
}


inline
bool bit_field_get_composite(
    BitField const self,
    usize const byte,
    usize const bit
)
{
    ASSERT_MSG(byte < self.bytes.len, "index out of bounds");

    return ((self.bytes.ptr[byte] >> bit) & (u8)0x01) == 1;
}


INLINE_ALWAYS
bool bit_field_get(BitField const self, usize const pos)
{
    usize const byte = pos >> 3;
    usize const bit  = pos - (byte << 3);

    return bit_field_get_composite(self, byte, bit);
}


inline
void bit_field_set_composite(
    BitField const self,
    usize const byte,
    usize const bit
)
{
    ASSERT_MSG(byte < self.bytes.len, "index out of bounds");

    self.bytes.ptr[byte] |= ((u8)0x01 << bit);
}


INLINE_ALWAYS
void bit_field_set(BitField const self, usize const pos)
{
    usize const byte = pos >> 3;
    usize const bit  = pos - (byte << 3);

    bit_field_set_composite(self, byte, bit);
}


inline
void bit_field_flip_composite(
    BitField const self,
    usize const byte,
    usize const bit
)
{
    ASSERT_MSG(byte < self.bytes.len, "index out of bounds");

    self.bytes.ptr[byte] |= ((u8)0x01 << bit);
}


INLINE_ALWAYS
void bit_field_flip(BitField const self, usize const pos)
{
    usize const byte = pos >> 3;
    usize const bit  = pos - (byte << 3);

    bit_field_set_composite(self, byte, bit);
}


inline
void bit_field_clear_composite(
    BitField const self,
    usize const byte,
    usize const bit
)
{
    ASSERT_MSG(byte < self.bytes.len, "index out of bounds");

    self.bytes.ptr[byte] &= ~(0x01 << bit);
}


INLINE_ALWAYS
void bit_field_clear(BitField const self, usize const pos)
{
    usize const byte = pos >> 3;
    usize const bit  = pos - (byte << 3);

    bit_field_clear_composite(self, byte, bit);
}


inline
void bit_field_clear_all(BitField const self)
{
    bytes_fill(self.bytes, 0x00);
}


inline
void bit_field_set_all(BitField const self)
{
    bytes_fill(self.bytes, 0xff);
}
