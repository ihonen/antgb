#include "background.hh"

#include "../mach/bitmanip.hh"
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

Background::Background(Type type_, PPUReg* ppu_reg, uint8_t* memory)
{
    ppureg = ppu_reg;
    mem = memory;
    type = type_;
}

void Background::set_ppu_reg(PPUReg* ppu_reg)
{
    ppureg = ppu_reg;
}

void Background::set_memory(uint8_t* memory)
{
    mem = memory;
}

uint8_t Background::get_pixel_at(size_t display_x, size_t display_y)
{
    assert(includes(display_x, display_y));
    return get_pixel(display_x - left(), display_y - top());
}

uint8_t Background::get_pixel(size_t background_x, size_t background_y)
{
    if (type == Type::BG)
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
    if (type == Type::BG)
    {
        return TILE_MAP_BASE[get_bit(ppureg->lcdc, PPUReg::BgTileMapDisplaySelect)];
    }

    return TILE_MAP_BASE[get_bit(ppureg->lcdc, PPUReg::WindowTileMapDisplaySelect)];
}

uint8_t* Background::tile_map_base()
{
    return &mem[tile_map_address()];
}

memaddr_t Background::tile_data_address()
{
    return TILE_DATA_BASE[get_bit(ppureg->lcdc, PPUReg::BgAndWindowTileDataSelect)];
}

Tile* Background::tile_data_base()
{
    return (Tile*)&mem[tile_data_address()];
}

bool Background::includes(size_t display_x, size_t display_y)
{
    return left() <= display_x && right() >= display_x
            && top() <= display_y && bottom() >= display_y;
}

size_t Background::top()
{
    if (type == Type::BG)
    {
        return *ppureg->scy;
    }

    return *ppureg->wy;
}

size_t Background::bottom()
{
    if (type == Type::BG)
    {
        return (*ppureg->scy + 144 - 1) % BG_HEIGHT_PIXELS;
    }

    return 144;
}

size_t Background::left()
{
    if (type == Type::BG)
    {
        return *ppureg->scx;
    }

    return *ppureg->wx - 7;
}

size_t Background::right()
{
    if (type == Type::BG)
    {
        return (*ppureg->scx + 160 - 1) % BG_WIDTH_PIXELS;
    }

    return 160;
}

bool Background::is_enabled()
{
    if (type == Type::BG)
    {
        return get_bit(ppureg->lcdc, PPUReg::BgAndWindowDisplayEnable);
    }

    return get_bit(ppureg->lcdc, PPUReg::BgAndWindowDisplayEnable)
            && get_bit(ppureg->lcdc, PPUReg::WindowDisplayEnable);
}
