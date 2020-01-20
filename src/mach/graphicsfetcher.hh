#ifndef GRAPHICSFETCHER_HH
#define GRAPHICSFETCHER_HH

#include "background.hh"
#include "mmu.hh"
#include "ppuregisters.hh"
#include "tile.hh"
#include "sprite.hh"

class GraphicsFetcher
{
public:
    GraphicsFetcher(MMU& mmu, PPUReg& reg);

    void fetch_all();

    vector<Sprite> sprite_buffer;
    Background background_buffer;
    Background window_buffer;

    MMU& mmu;
    PPUReg& reg;

    Background::TileMap bg_tilemap;
    Background::TileMap window_tilemap;
};

#endif // GRAPHICSFETCHER_HH
