#pragma once

#include "macros.hh"
#include <cstdint>

namespace antgb
{

ANTGB_FORCEINLINE void set_bit(uint8_t* byte, uint8_t bit_index)
{
    *byte |= 0x01 << bit_index;
}

ANTGB_FORCEINLINE void clear_bit(uint8_t* byte, uint8_t bit_index)
{
    *byte &= ~(0x01 << bit_index);
}

ANTGB_FORCEINLINE void toggle_bit(uint8_t* byte, uint8_t bit_index)
{
    *byte ^= 0x01 << bit_index;
}

ANTGB_FORCEINLINE uint8_t get_bit(uint8_t* byte, uint8_t bit_index)
{
    return (*byte & (0x01 << bit_index)) >> bit_index;
}

} // namespace antgb
