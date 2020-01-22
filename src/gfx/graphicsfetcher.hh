#ifndef GRAPHICSFETCHER_HH
#define GRAPHICSFETCHER_HH

#include "background.hh"
#include "../mach/mmu.hh"
#include "../mach/ppuregisters.hh"
#include "tile.hh"
#include "sprite.hh"

class GraphicsFetcher
{
public:
    GraphicsFetcher(MMU& mmu, PPUReg& reg);

    void fetch_all();
    void fetch_sprites();
    void fetch_background();
    void fetch_window();

    memaddr_t TILE_DATA_ADDRESS = 0x8000;
    memaddr_t SPRITE_ATTRIBUTE_TABLE_ADDRESS = 0xFE00;
    memaddr_t SPRITE_ATTRIBUTE_TABLE_SIZE_B = 160;

    MMU& mmu;
    PPUReg& reg;
    vector<Sprite> sprite_buffer;
    Background background_buffer;
    Background window_buffer;
    Background::TileMap bg_tilemap;
    Background::TileMap window_tilemap;
};

#endif // GRAPHICSFETCHER_HH
