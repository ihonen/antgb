#include "renderer.hh"

#include <cstdlib>
#include "tile.hh"

Renderer::Renderer(PPUReg* ppu_reg, uint8_t* memory_, QObject* parent) :
    QObject(parent),
    memory(memory_),
    ppureg(ppu_reg),
    background(Background(Background::Type::BG, ppureg, memory)),
    window(Background(Background::Type::Window, ppureg, memory))
{

}

void Renderer::set_memory(uint8_t* memory_)
{
    memory = memory_;

    PPUReg* ppu_reg_new = new PPUReg;
    ppu_reg_new->lcdc = &memory[0xFF40];
    ppu_reg_new->scy = &memory[0xFF42];
    ppu_reg_new->scx = &memory[0xFF43];
    ppu_reg_new->wy = &memory[0xFF4A];
    ppu_reg_new->wx = &memory[0xFF4B];
    background.set_ppu_reg(ppu_reg_new);
    background.set_memory(memory);
    window.set_ppu_reg(ppu_reg_new);
    window.set_memory(memory);
}

void Renderer::render_frame()
{    
    for (size_t y = 0; y < 144; ++y)
    {
        for (size_t x = 0; x < 160; ++x)
        {
            frame_buffer[y][x] = background.get_pixel_at(x, y);
        }
    }

    emit(frame_ready());
}

uint32_t* Renderer::get_frame_buffer()
{
    return (uint32_t*)frame_buffer;
}
