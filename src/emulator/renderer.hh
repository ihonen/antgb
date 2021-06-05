#pragma once

#include "debugger/ifrontend.hh"
#include "background.hh"
#include "sprites.hh"
#include "types.hh"
#include <array>
#include <cstdint>
#include <iostream>

class Renderer
{
public:
    Renderer(Memory* mem);
    void set_frontend(iFrontend* frontend);
    void set_memory(uint8_t* mem);
    void render_frame();
private:

    uint16_t TILE_DATA_BASE = 0x8000;

    Memory* mem;
    //uint32_t frame_buffer[144][160];

    Background background;
    Background window;
    Sprites sprites;

    iFrontend* frontend = nullptr;
    iFrontend::Pixels pixels;
};
