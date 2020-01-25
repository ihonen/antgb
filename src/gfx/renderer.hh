#ifndef RENDERER_HH
#define RENDERER_HH

#include "background.hh"
#include "../mach/mmu.hh"
#include "sprites.hh"
#include <array>
#include <cstdint>
#include <iostream>
#include <QObject>

using namespace std;

class Renderer : public QObject
{
    Q_OBJECT
public:
    Renderer(MMU* mem, QObject* parent = nullptr);
    virtual ~Renderer() override = default;
    void set_memory(uint8_t* mem);
    void render_frame();

    uint32_t* get_frame_buffer();
signals:
    void frame_ready();
private:

    uint16_t TILE_DATA_BASE = 0x8000;

    MMU* mem;
    uint32_t frame_buffer[144][160];

    Background background;
    Background window;
    Sprites sprites;
};

#endif // RENDERER_HH
