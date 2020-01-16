#include "ppu.hh"

#include <cassert>

PPU::PPU(MMU& mmu_)
{
    mmu = mmu_;
}

vector<vector<uint8_t>> PPU::read_tile(void* address_)
{
    vector<vector<uint8_t>> pixels;
    uint8_t* tile_start = (uint8_t*)address_;

    for (uint8_t row = 0; row < 8; ++row)
    {
        pixels.push_back({});
        const uint8_t& lower_byte = tile_start[row + 1];
        const uint8_t& upper_byte = tile_start[row];

        for (int8_t column = 7; column >= 0; --column)
        {
            // Bit 0 from lower byte, bit 1 from upper byte:
            uint8_t lower_bit = (upper_byte & (0x01 << column))
                                  >> column;
            uint8_t upper_bit = (lower_byte & (0x01 << column))
                                  >> column;
            uint8_t pixel_value = (upper_bit << 1) | lower_bit;
            pixels.at(row).push_back(pixel_value);
        }
    }

    return pixels;
}
