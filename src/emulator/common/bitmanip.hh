#pragma once

#include "macros.hh"
#include <cstdint>

FORCE_INLINE void set_bit(uint8_t* byte, uint8_t bit_index)
{
    *byte |= 0x01 << bit_index;
}

FORCE_INLINE void clear_bit(uint8_t* byte, uint8_t bit_index)
{
    *byte &= ~(0x01 << bit_index);
}

FORCE_INLINE void toggle_bit(uint8_t* byte, uint8_t bit_index)
{
    *byte ^= 0x01 << bit_index;
}

FORCE_INLINE uint8_t get_bit(uint8_t* byte, uint8_t bit_index)
{
    return (*byte & (0x01 << bit_index)) >> bit_index;
}
