#include "background.hh"

#include <cassert>

Background::Background(Background::TileMap* tile_map_,
                       size_t scroll_x_,
                       size_t scroll_y_)
{
    assert(tile_map_ != nullptr);
    set_scroll(scroll_x_, scroll_y_);
    set_tile_map(tile_map_);
}

void Background::set_tile_map(Background::TileMap* tile_map_)
{
    tile_map = tile_map_;
}

void Background::set_scroll(size_t scroll_x_, size_t scroll_y_)
{
    scroll_x = scroll_x_;
    scroll_y = scroll_y_;
}

size_t Background::get_scroll_x()
{
    return scroll_x;
}

size_t Background::get_scroll_y()
{
    return scroll_y;
}

uint8_t Background::get_pixel_at(size_t display_x, size_t display_y)
{
    return get_pixel(display_x + scroll_x, display_y + scroll_y);
}

uint8_t Background::get_pixel(size_t background_x, size_t background_y)
{
    background_x %= HEIGHT_PIXELS;
    background_y %= WIDTH_PIXELS;

    Tile* tile = (*tile_map)[background_x / Tile::WIDTH][background_y / Tile::HEIGHT];
    uint8_t tile_x = background_x % Tile::WIDTH;
    uint8_t tile_y = background_y % Tile::HEIGHT;

    return tile->get_pixel(tile_x, tile_y);
}

