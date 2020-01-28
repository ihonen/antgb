#include "renderer.hh"

#include <cstdlib>
#include "tile.hh"

Renderer::Renderer(Memory* memory, QObject* parent) :
    QObject(parent),
    mem(memory),
    background(Background(Background::Type::Background, mem)),
    window(Background(Background::Type::Window, mem)),
    sprites(Sprites(memory))
{
    memset(frame_buffer, 0x00, 160 * 144 * 4);
}

void Renderer::render_frame()
{    
    auto ly_backup = mem->hff44_ly;

    for (size_t y = 0; y < 144; ++y)
    {
        mem->hff44_ly = y;
        sprites.refresh();
        for (size_t x = 0; x < 160; ++x)
        {
            if (background.includes(x, y))
            {
                frame_buffer[y][x] = background.get_pixel_at(x, y);
            }
/*
            Sprite* sprite = sprites.get_sprite_at_x(x);
            if (sprite)
            {
                frame_buffer[y][x] = sprite->get_pixel_at(x, y);
            }
*/
        }
    }

    mem->hff44_ly = ly_backup;

    emit(frame_ready());
}

uint32_t* Renderer::get_frame_buffer()
{
    return (uint32_t*)frame_buffer;
}
