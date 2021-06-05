#include "renderer.hh"

#include "addresses.hh"

#include <cstdlib>
#include "memory.hh"
#include "tile.hh"

Renderer::Renderer(Memory* memory) :
    mem(memory),
    background(Background(Background::Type::Background, mem)),
    window(Background(Background::Type::Window, mem)),
    sprites(Sprites(memory))
{
    pixels = {{}};
}

void Renderer::set_frontend(iFrontend* frontend_)
{
    frontend = frontend_;
}

void Renderer::render_frame()
{    
    auto ly_backup = *mem->get(LY_ADDR);

    for (size_t pixel_y = 0; pixel_y < iFrontend::SCREEN_HEIGHT; ++pixel_y)
    {
        *mem->get(LY_ADDR) = static_cast<uint8_t>(pixel_y);
        sprites.refresh();
        for (size_t pixel_x = 0; pixel_x < iFrontend::SCREEN_WIDTH; ++pixel_x)
        {
            if (background.includes(pixel_x, pixel_y))
            {
                pixels.at(pixel_x).at(pixel_y) = background.get_pixel_at(pixel_x, pixel_y);
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

    *mem->get(LY_ADDR) = ly_backup;

    if (frontend)
    {
        frontend->render(pixels);
    }
}
