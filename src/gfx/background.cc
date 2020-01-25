#include "background.hh"

#include "../util/bitmanip.hh"
#include <cassert>

const memaddr_t Background::TILE_DATA_BASE[2] =
{
    Background::TILE_DATA_BLOCK2_BASE,
    Background::TILE_DATA_BLOCK0_BASE
};

const memaddr_t Background::TILE_MAP_BASE[2] =
{
   Background::TILE_MAP0_BASE,
   Background::TILE_MAP1_BASE
};

Background::Background(Type type_, Memory* memory)
{
    mem = memory;
    type = type_;
}

uint8_t Background::get_pixel_at(size_t display_x, size_t display_y)
{
    assert(includes(display_x, display_y));
    return get_pixel(display_x - left(), display_y - top());
}

uint8_t Background::get_pixel(size_t background_x, size_t background_y)
{
    if (type == Type::Background)
    {
        background_x %= BG_HEIGHT_PIXELS;
        background_y %= BG_WIDTH_PIXELS;
    }

    uint8_t tile_x = background_x / Tile::WIDTH;
    uint8_t tile_y = background_y / Tile::HEIGHT;
    uint8_t tile_number = tile_map_base()[tile_y * WIDTH_TILES + tile_x];

    Tile* tile = nullptr;
    switch (tile_data_address())
    {
        case TILE_DATA_BLOCK0_BASE:
            tile = &tile_data_base()[tile_number];
            break;
        case TILE_DATA_BLOCK2_BASE:
            tile = &tile_data_base()[(int8_t)tile_number];
            break;
        default:
            assert(false && "ILLEGAL TILE DATA BASE ADDRESS");
            break;
    }

    return tile->get_pixel(background_x % Tile::WIDTH,
                           background_y % Tile::HEIGHT);
}

memaddr_t Background::tile_map_address()
{
    if (type == Type::Background)
    {
        return TILE_MAP_BASE[get_bit(&mem->hff40_lcdc, Memory::BgTileMapDisplaySelect)];
    }

    return TILE_MAP_BASE[get_bit(&mem->hff40_lcdc, Memory::WindowTileMapDisplaySelect)];
}

uint8_t* Background::tile_map_base()
{
    return mem->get(tile_map_address());
}

memaddr_t Background::tile_data_address()
{
    return TILE_DATA_BASE[get_bit(&mem->hff40_lcdc, Memory::BgAndWindowTileDataSelect)];
}

Tile* Background::tile_data_base()
{
    return (Tile*)mem->get(tile_data_address());
}

bool Background::includes(size_t display_x, size_t display_y)
{
    return left() <= display_x && right() >= display_x
            && top() <= display_y && bottom() >= display_y;
}

size_t Background::top()
{
    if (type == Type::Background)
    {
        return mem->hff42_scy;
    }

    return mem->hff4a_wy;
}

size_t Background::bottom()
{
    if (type == Type::Background)
    {
        return (mem->hff42_scy + 144 - 1) % BG_HEIGHT_PIXELS;
    }

    return 144;
}

size_t Background::left()
{
    if (type == Type::Background)
    {
        return mem->hff43_scx;
    }

    return mem->hff4b_wx;
}

size_t Background::right()
{
    if (type == Type::Background)
    {
        return (mem->hff43_scx + 160 - 1) % BG_WIDTH_PIXELS;
    }

    return 160;
}

bool Background::is_enabled()
{
    if (type == Type::Background)
    {
        return get_bit(&mem->hff40_lcdc, Memory::BgAndWindowDisplayEnable);
    }

    return get_bit(&mem->hff40_lcdc, Memory::BgAndWindowDisplayEnable)
            && get_bit(&mem->hff40_lcdc, Memory::WindowDisplayEnable);
}
