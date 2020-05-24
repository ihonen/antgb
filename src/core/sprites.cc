#include "sprites.hh"

#include "bitmanip.hh"
#include "memory.hh"
#include <algorithm>

namespace antgb
{

static bool sprite_priority_comp(const Sprite& a, const Sprite& b)
{
    if (a.attribute->x_pos != b.attribute->x_pos)
    {
        return a.attribute->x_pos < b.attribute->x_pos;
    }

    return a.attribute->tile_number <= b.attribute->tile_number;
}

Sprites::Sprites(Memory* memory)
{
    mem = memory;
    sprite_buffer_size = 0;
}

Sprites::~Sprites()
{

}

void Sprites::refresh()
{
    sprite_buffer_size = 0;

    for (size_t i = 0; i < 40; ++i)
    {
        auto sprite_attributes = &sprite_attributes_base()[i];

        /*
        if (sprite_attributes->y_pos != 0)
        {
            assert(!sprite_buffer_size);
            cout << sprite_buffer_size << endl;
            cout << std::hex << (int)*ppureg->ly << endl;
            cout << std::hex << (int)sprite_attributes->y_pos << endl;
            cout << endl;
        }
        */

        if (sprite_attributes->y_pos - 16 <= mem->read(LY_ADDR)
            && sprite_attributes->y_pos - 9 /*- 16 + sprite_height()*/ >= mem->read(LY_ADDR))
        {
            Sprite& sprite = sprite_buffer[sprite_buffer_size];
            assemble_sprite_info(sprite, sprite_attributes);
            ++sprite_buffer_size;
        }
    }

    sort();
}

memaddr_t Sprites::sprite_attributes_address()
{
    return 0xFE00;
}

Sprite::Attribute* Sprites::sprite_attributes_base()
{
    return (Sprite::Attribute*)(mem->get(sprite_attributes_address()));
}

memaddr_t Sprites::sprite_data_address()
{
    return 0x8000;
}

Tile* Sprites::sprite_data_base()
{
    return (Tile*)(mem->get(sprite_data_address()));
}

uint8_t Sprites::sprite_height()
{
    return get_bit(mem->get(LCDC_ADDR), Ppu::ObjSize) ? 16 : 8;
}

bool Sprites::enabled()
{
    return get_bit(mem->get(LCDC_ADDR), Ppu::ObjDisplayEnable);
}

void Sprites::assemble_sprite_info(Sprite& sprite, Sprite::Attribute* attributes)
{
    sprite.attribute = attributes;
    sprite.width = 8;
    sprite.height = sprite_height();
    sprite.data = &sprite_data_base()[sprite.attribute->tile_number];
}

void Sprites::sort()
{
    std::sort(sprite_buffer.begin(),
              sprite_buffer.begin() + sprite_buffer_size,
              sprite_priority_comp);
}

Sprite* Sprites::get_sprite_at_x(size_t display_x)
{
    for (size_t i = 0; i < sprite_buffer_size; ++i)
    {
        Sprite& sprite = sprite_buffer[i];
        if (sprite.includes_column(display_x))
        {
            return &sprite;
        }
    }

    return nullptr;
}

} // namespace antgb
