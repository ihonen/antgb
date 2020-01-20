#include "graphicsfetcher.hh"


GraphicsFetcher::GraphicsFetcher(MMU& mmu_, PPUReg& reg_) :
    mmu(mmu_),
    reg(reg_),
    background_buffer(Background(&bg_tilemap)),
    window_buffer(Background(&window_tilemap))
{
    background_buffer = Background(&bg_tilemap);
    window_buffer = Background(&window_tilemap);
}

void GraphicsFetcher::fetch_all()
{
    fetch_sprites();
    fetch_background();
    fetch_window();
}

void GraphicsFetcher::fetch_sprites()
{
    sprite_buffer.clear();

    size_t fetched_sprites = 0;
    size_t checked_sprites = 0;
    uint8_t sprite_height = *reg.lcdc & PPUReg::ObjSize ? 16 : 8;

    auto attribute_table_base = (Sprite::Attribute*)&mmu.mem[SPRITE_ATTRIBUTE_TABLE_ADDRESS];
    while (fetched_sprites < 10 && checked_sprites < 40)
    {
        Sprite::Attribute* attributes = &attribute_table_base[checked_sprites];
        uint8_t* tile_data_base = &mmu.mem[TILE_DATA_ADDRESS];
        Tile* tile_data = &((Tile*)tile_data_base)[attributes->tile_number];
        Sprite sprite = Sprite(8, sprite_height, attributes, tile_data);

        if (sprite.includes_row(*reg.ly))
        {
            sprite_buffer.push_back(sprite);
            ++fetched_sprites;
        }

        ++checked_sprites;
    }
}

void GraphicsFetcher::fetch_background()
{
    Tile* tile_data_base = *reg.lcdc & PPUReg::BgAndWindowTileDataSelect ?
                                    (Tile*)&mmu.mem.data[0x8000] : (Tile*)&mmu.mem.data[0x9000];
    uint8_t* tile_map_base = *reg.lcdc & PPUReg::BgTileMapDisplaySelect ?
                &mmu.mem.data[0x9C00] : &mmu.mem.data[0x9800];

    Background::TileMap tile_map;

    for (size_t x = 0; x < 32; ++x)
    {
        for (size_t y = 0; y < 32; ++y)
        {
            uint8_t tile_index = tile_map_base[y * 8 + x];
            Tile* tile = nullptr;
            if (*reg.lcdc & PPUReg::BgAndWindowTileDataSelect)
            {
                tile = &tile_data_base[tile_index];
            }
            else
            {
                tile = &tile_data_base[(int8_t)tile_index];
            }
            tile_map[x][y] = tile;
        }
    }

    background_buffer.set_tile_map(&tile_map);
    background_buffer.set_scroll(*reg.scx, *reg.scy);
}

void GraphicsFetcher::fetch_window()
{

}
