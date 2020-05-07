#pragma once

#include "sprite.hh"
#include "../core/memory.hh"
#include <array>

class Sprites
{
public:
    Sprites(Memory* memory);
    virtual ~Sprites();
    void set_memory(uint8_t* memory);
    void refresh();
    memaddr_t sprite_attributes_address();
    Sprite::Attribute* sprite_attributes_base();
    memaddr_t sprite_data_address();
    Tile* sprite_data_base();
    uint8_t sprite_height();
    bool enabled();
    void assemble_sprite_info(Sprite& sprite, Sprite::Attribute* attributes);
    void sort();
    Sprite* get_sprite_at_x(size_t display_x);

    array<Sprite, 10> sprite_buffer;
    size_t sprite_buffer_size;

    Memory* mem;
};
