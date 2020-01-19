#ifndef TILE_HH
#define TILE_HH

#include <cstdint>

struct Tile
{
    Tile();
    uint8_t get_pixel(uint8_t tile_x, uint8_t tile_y);

    static const uint8_t WIDTH = 8;
    static const uint8_t HEIGHT = 8;

    uint8_t data[16];
};

#endif // TILE_HH
