#ifndef RENDERER_HH
#define RENDERER_HH

#include "background.hh"
#include "../mach/memory.hh"
#include "sprites.hh"
#include <array>
#include <cstdint>
#include <iostream>
#include <QObject>

class Renderer : public QObject
{
    Q_OBJECT
public:
    Renderer(Memory* mem, QObject* parent = nullptr);
    virtual ~Renderer() override = default;
    void set_memory(uint8_t* mem);
    void render_frame();

    uint32_t* get_frame_buffer();
signals:
    void frame_ready();
private:

    uint16_t TILE_DATA_BASE = 0x8000;

    Memory* mem;
    uint32_t frame_buffer[144][160];

    Background background;
    Background window;
    Sprites sprites;
};

#endif // RENDERER_HH
