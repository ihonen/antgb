#ifndef BITMANIP_HH
#define BITMANIP_HH

#include <cstdint>

void set_bit(uint8_t* byte, uint8_t bit_index);

void clear_bit(uint8_t* byte, uint8_t bit_index);

void toggle_bit(uint8_t* byte, uint8_t bit_index);

uint8_t get_bit(uint8_t* byte, uint8_t bit_index);

#endif // BITMANIP_HH
