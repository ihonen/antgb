#ifndef GAMEBOY_HH
#define GAMEBOY_HH

#include "../gfx/renderer.hh"
#include "cpu.hh"
#include "irc.hh"
#include "joypad.hh"
#include "memory.hh"
#include "mmu.hh"
#include "ppu.hh"
#include "timerdivider.hh"
#include <cstdint>

class Machine
{
public:
    Machine();
    ~Machine();
    void load_rom(void* rom, size_t size);
    void tick();
    uint64_t cpu_tick();
    void button_pressed(Joypad::Button button);
    void button_released(Joypad::Button button);

    CPU* cpu;
    IRC* irc;
    MMU* mmu;
    PPU* ppu;
    Joypad* joypad;
    TimerDivider* timer_divider;
    Renderer* renderer;
};

#endif // GAMEBOY_HH
