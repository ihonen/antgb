#ifndef GAMEBOY_HH
#define GAMEBOY_HH

#include "cartridge.hh"
#include "cpu.hh"
#include "interrupts.hh"
#include "joypad.hh"
#include "memory.hh"
#include "ppu.hh"
#include "timer.hh"
#include "../gfx/renderer.hh"
#include "../util/typedefs.hh"

class Machine
{
public:
    Machine();
    ~Machine();
    void insert_cartridge(Cartridge* cartridge);
    void load_rom(void* rom, size_t size);
    void tick();
    uint64_t cpu_tick();
    void button_pressed(Joypad::Button button);
    void button_released(Joypad::Button button);

    Cpu* cpu;
    InterruptController* irc;
    Memory* mem;
    Ppu* ppu;
    Joypad* joypad;
    Timer* timer_divider;
    Renderer* renderer;
    Cartridge* cartridge;
};

#endif // GAMEBOY_HH
