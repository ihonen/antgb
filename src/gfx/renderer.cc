#include "renderer.hh"

#include <cstdlib>

static const uint32_t colors[4] = {0xFF000000, 0xFF555555, 0xFFAAAAAA, 0xFFFFFFFF};

Renderer::Renderer(QObject* parent) :
    QObject(parent)
{
    srand(time(NULL));
}

void Renderer::render_frame()
{
    for (size_t y = 0; y < 144; ++y)
    {
        for (size_t x = 0; x < 160; ++x)
        {
            frame_buffer[y][x] = colors[rand() % 4];
        }
    }

    emit(frame_ready());
}

uint32_t* Renderer::get_frame_buffer()
{
    return (uint32_t*)frame_buffer;
}
