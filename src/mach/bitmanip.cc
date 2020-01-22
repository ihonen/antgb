#include "bitmanip.hh"

void set_bit(uint8_t* byte, uint8_t bit_index)
{
    *byte |= 0x01 << bit_index;
}

void clear_bit(uint8_t* byte, uint8_t bit_index)
{
    *byte &= ~(0x01 << bit_index);
}

void toggle_bit(uint8_t* byte, uint8_t bit_index)
{
    *byte ^= 0x01 << bit_index;
}

uint8_t get_bit(uint8_t* byte, uint8_t bit_index)
{
    return (*byte & (0x01 << bit_index)) >> bit_index;
}
