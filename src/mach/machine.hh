#ifndef GAMEBOY_HH
#define GAMEBOY_HH

#include "cpu.hh"
#include "irc.hh"
#include "joypad.hh"
#include "memory.hh"
#include "mmu.hh"
#include "ppu.hh"
#include <cstdint>

class Machine
{
public:
    Machine();
    ~Machine();
    void load_rom(void* rom, size_t size);
    void tick();
    void button_pressed(JoypadButton button);
    void button_released(JoypadButton button);

    CPU* cpu;
    IRC* irc;
    MMU* mmu;
    PPU* ppu;
};

#endif // GAMEBOY_HH
