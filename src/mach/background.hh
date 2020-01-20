#ifndef BACKGROUND_HH
#define BACKGROUND_HH

#include "tile.hh"
#include <array>
#include <iostream>
#include <vector>

using namespace std;

class Background
{
public:

    static const size_t HEIGHT_PIXELS = 256;
    static const size_t WIDTH_PIXELS = 256;
    static const size_t HEIGHT_TILES = 32;
    static const size_t WIDTH_TILES = 32;

    typedef array<array<Tile*, HEIGHT_TILES>, WIDTH_TILES> TileMap;

    Background(TileMap* tile_map, size_t scroll_x = 0, size_t scroll_y = 0);
    void set_tile_map(TileMap* tile_map);
    void set_scroll(size_t scroll_x, size_t scroll_y);
    size_t get_scroll_x();
    size_t get_scroll_y();
    uint8_t get_pixel_at(size_t display_x, size_t display_y);
    uint8_t get_pixel(size_t background_x, size_t background_y);

    array<array<Tile*, HEIGHT_TILES>, WIDTH_TILES>* tile_map;
    size_t scroll_x;
    size_t scroll_y;
};

#endif // BACKGROUND_HH
