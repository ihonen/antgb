#include "renderer.hh"

#include <cstdlib>
#include "tile.hh"

static const uint32_t colors[4] =
{
    /*
    0xFFFFFFFF,
    0xFFAAAAAA,
    0xFF555555,
    0xFF000000
    */
    0xFFE0F8D0,
    0xFF88C070,
    0xFF346856,
    0xFF081820,
};

Renderer::Renderer(uint8_t* memory_, QObject* parent) :
    QObject(parent),
    memory(memory_)
{
    srand(time(NULL));
}

void Renderer::set_memory(uint8_t* memory_)
{
    memory = memory_;
}

void Renderer::render_frame()
{    
    for (size_t y = 0; y < 144; ++y)
    {
        for (size_t x = 0; x < 160; ++x)
        {
            size_t tile_y = (y / 8);
            size_t tile_x = (x / 8);

            /*
            size_t tmp = tile_y;
            tile_y = tile_x;
            tile_x = tmp;
*/
            uint8_t tile_no = memory[0x9800 + tile_y * 32 + tile_x];
            Tile* tile_base = (Tile*)(&memory[0x8000]);
            Tile* tile = &tile_base[tile_no];

            frame_buffer[y][x] = colors[tile->get_pixel(x % 8, y % 8)];

//            frame_buffer[y][x] = colors[rand() % 4];
        }
    }

    emit(frame_ready());
}

uint32_t* Renderer::get_frame_buffer()
{
    return (uint32_t*)frame_buffer;
}
