#include "tile.hh"

#include <iostream>

namespace antgb
{

uint8_t Tile::get_pixel(uint8_t tile_x, uint8_t tile_y)
{
    if (tile_x > 7 || tile_y > 7)
    {
        return 0x00;
    }

    uint8_t* row_low = &data[tile_y * 2];
    uint8_t* row_high = row_low + 1;
    uint8_t low_bit = (*row_low & (0x01 << (7 - tile_x))) >> ((7 - tile_x));
    uint8_t high_bit = (*row_high & (0x01 << (7 - tile_x))) >> ((7 - tile_x));
    return (high_bit << 1) | low_bit;
}

} // namespace antgb
