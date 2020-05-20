#pragma once

#include "../antdbg/src/core/types.hh"
#include "../antdbg/src/core/iemulator.hh"
#include "cpu.hh"
#include "interrupts.hh"
#include "joypad.hh"
#include "memory.hh"
#include "ppu.hh"
#include "serial.hh"
#include "timer.hh"
#include "renderer.hh"
#include "types.hh"

class Emulator : public iEmulator
{
public:
    Emulator();
    virtual ~Emulator() override;
    virtual void load_rom(const void* rom, size_t size) override;
    virtual int execute_next() override;
    virtual void button_pressed(JoypadButton button) override;
    virtual void button_released(JoypadButton button) override;
    virtual void reset_emulation() override;
    virtual void set_render_callback(void (*callback)(const framebuf_t *, int)) override;

    virtual uint16_t read(regid_t register_id) override;
    virtual void write(regid_t register_id, uint16_t value) override;

    Cpu* cpu;
    Irc* irc;
    Memory* mem;
    Ppu* ppu;
    Joypad* joypad;
    Timer* timer_divider;
    Serial* serial;
    Cartridge* cartridge;
};
