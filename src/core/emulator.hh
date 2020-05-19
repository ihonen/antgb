#pragma once

#include "../antdbg/src/core/cartridge.hh"
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
    void insert_cartridge(Cartridge* cartridge);
    void tick();
    uint64_t cpu_tick();
    void button_pressed(Joypad::Button button);
    void button_released(Joypad::Button button);
    void hard_reset();

    Cpu* cpu;
    Irc* irc;
    Memory* mem;
    Ppu* ppu;
    Joypad* joypad;
    Timer* timer_divider;
    Serial* serial;
    Cartridge* cartridge;
};
