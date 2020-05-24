#pragma once

#include "tile.hh"
#include "types.hh"
#include <array>
#include <iostream>
#include <vector>

namespace antgb
{

using namespace std;

class Background
{
public:

    enum class Type {Background, Window};

    static const size_t BG_HEIGHT_PIXELS = 256;
    static const size_t BG_WIDTH_PIXELS = 256;
    static const size_t HEIGHT_TILES = 32;
    static const size_t WIDTH_TILES = 32;

    static const memaddr_t TILE_DATA_BLOCK0_BASE = 0x8000;
    static const memaddr_t TILE_DATA_BLOCK2_BASE = 0x9000;
    static const memaddr_t TILE_DATA_BASE[2];

    static const memaddr_t TILE_MAP0_BASE = 0x9800;
    static const memaddr_t TILE_MAP1_BASE = 0x9C00;
    static const memaddr_t TILE_MAP_BASE[2];

    Background(Type type, Memory* memory);
    void set_memory(uint8_t* memory);
    uint8_t get_pixel_at(size_t display_x, size_t display_y);
    uint8_t get_pixel(size_t background_x, size_t background_y);
    memaddr_t tile_map_address();
    uint8_t* tile_map_base();
    memaddr_t tile_data_address();
    Tile* tile_data_base();
    bool includes(size_t display_x, size_t display_y);
    size_t top();
    size_t bottom();
    size_t left();
    size_t right();
    bool is_enabled();

    Memory* mem;
    Type type;
};

} // namespace antgb
