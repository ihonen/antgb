#ifndef SPRITE_HH
#define SPRITE_HH

#include "tile.hh"

#include <cassert>
#include <cstdint>
#include <iostream>

using namespace std;

class Sprite
{
public:
    typedef struct __attribute__((packed))
    {
        uint8_t y_pos;
        uint8_t x_pos;
        uint8_t tile_number;
        struct __attribute__((packed))
        {
            uint8_t __unused_in_dmg_1 : 1;
            uint8_t __unused_in_dmg_2: 3;
            uint8_t palette_number : 1;
            uint8_t x_flip : 1;
            uint8_t y_flip : 1;
            uint8_t obj_to_bg_priority : 1;
        } flags;
    } Attribute;

    uint8_t top();
    uint8_t bottom();
    uint8_t left();
    uint8_t right();
    bool includes(uint8_t display_row, uint8_t display_column);
    bool includes_row(uint8_t display_row);
    bool includes_column(uint8_t display_column);
    bool is_offscreen();
    uint8_t get_pixel_at(uint8_t display_x, uint8_t display_y);
    uint8_t get_pixel(uint8_t sprite_x, uint8_t sprite_y);
    bool is_above_background();
    bool is_x_mirrored();
    bool is_y_mirrored();
    uint8_t palette_number();

    uint8_t width;
    uint8_t height;
    Tile* data;
    Attribute* attribute;
};


#endif // SPRITE_HH
