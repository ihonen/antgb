#include "sprite.hh"

Sprite::Sprite(uint8_t width_,
               uint8_t height_,
               Attribute* attribute_,
               Tile* data_)
{
    assert(width == 8);
    assert(height == 8 || height == 16);
    width = width_;
    height = height_;
    attribute = attribute_;
    data = data_;
}

uint8_t Sprite::top()
{
    return attribute->y_pos - 16;
}

uint8_t Sprite::bottom()
{
    return top() + height - 1;
}

uint8_t Sprite::left()
{
    return attribute->x_pos - 8;
}

uint8_t Sprite::right()
{
    return top() + width - 1;
}

bool Sprite::includes_row(uint8_t display_row)
{
    return display_row >= top() && display_row <= bottom();
}

bool Sprite::includes_column(uint8_t display_column)
{
    return display_column >= left() && display_column <= right();
}

bool Sprite::is_offscreen()
{
    return left() >= 160 && top() >= 144;
}

uint8_t Sprite::get_pixel_at(uint8_t display_x, uint8_t display_y)
{
    if (!includes_row(display_y)
        || !includes_column(display_x)
        || is_offscreen())
    {
        return 0x00;
    }

    return get_pixel(display_x - left(), display_y - top());
}

uint8_t Sprite::get_pixel(uint8_t sprite_x, uint8_t sprite_y)
{
    if (sprite_x >= width || sprite_y >= height)
    {
        return 0x00;
    }

    if (is_x_mirrored())
    {
        sprite_x = (width - 1) - sprite_x;
    }

    if (is_y_mirrored())
    {
        sprite_y = (height - 1) - sprite_y;
    }

    uint8_t  tile_y = sprite_y;
    Tile* pointed_tile = data;

    if (sprite_y >= 8)
    {
        tile_y -= 8;
        ++pointed_tile;
    }

    return pointed_tile->get_pixel(sprite_x, tile_y);
}

bool Sprite::is_above_background()
{
    return attribute->flags.obj_to_bg_priority == 0;
}

bool Sprite::is_x_mirrored()
{
    return attribute->flags.x_flip != 0;
}

bool Sprite::is_y_mirrored()
{
    return attribute->flags.y_flip != 0;
}

uint8_t Sprite::palette_number()
{
    return attribute->flags.palette_number;
}
