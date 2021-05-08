#pragma once

#include "../emulator/types.hh"
#include <cstdint>
#include <string>

class iEmulator
{
public:
    virtual ~iEmulator() = default;

    virtual uint16_t read(regid_t register_id) = 0;

    virtual void write(regid_t register_id, uint16_t value) = 0;

    virtual void load_rom(const void* rom, size_t size) = 0;

    virtual void reset_emulation() = 0;

    virtual void button_pressed(JoypadButton button) = 0;

    virtual void button_released(JoypadButton button) = 0;

    virtual int execute_next() = 0;

    virtual void set_render_callback(void (*callback)(const framebuf_t* frame_buffer, int scanline)) = 0;
};
