#ifndef RENDERER_HH
#define RENDERER_HH

#include "background.hh"
#include <array>
#include <cstdint>
#include <iostream>
#include <QObject>

using namespace std;

class Renderer : public QObject
{
    Q_OBJECT
public:
    Renderer(PPUReg* ppu_reg, uint8_t* memory, QObject* parent = nullptr);
    virtual ~Renderer() override = default;
    void set_memory(uint8_t* memory);
    void render_frame();

    uint32_t* get_frame_buffer();
signals:
    void frame_ready();
private:

    uint16_t TILE_DATA_BASE = 0x8000;

    uint8_t* memory;
    uint32_t frame_buffer[144][160];

    PPUReg* ppureg;
    Background background;
    Background window;
};

#endif // RENDERER_HH
