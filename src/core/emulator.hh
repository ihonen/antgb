#pragma once

#include "../antdbg/src/core/types.hh"
#include "cpu.hh"
#include "interrupts.hh"
#include "joypad.hh"
#include "memory.hh"
#include "ppu.hh"
#include "serial.hh"
#include "timer.hh"
#include "renderer.hh"
#include "types.hh"

class Emulator
{
public:
    Emulator();
    ~Emulator();
    void load_rom(const void* rom, size_t size);
    void tick();
    uint64_t cpu_tick();
    void button_pressed(JoypadButton button);
    void button_released(JoypadButton button);
    void hard_reset();

    uint16_t read(regid_t register_id);
    void write(regid_t register_id, uint16_t value);

    Cpu* cpu;
    Irc* irc;
    Memory* mem;
    Ppu* ppu;
    Joypad* joypad;
    Timer* timer_divider;
    Serial* serial;
    Cartridge* cartridge;
};
