#include "machine.hh"

#include <iostream>
#include <cstring>

using namespace std;

Machine::Machine()
{
    mmu = new MMU();
    irc = new IRC(*mmu);
    cpu = new CPU(*mmu, *irc);
    ppu = new PPU(*mmu);
    joypad = new Joypad(*mmu, *irc);
}

Machine::~Machine()
{
    delete cpu;
    delete mmu;
    delete ppu;
}

void Machine::load_rom(void* rom, size_t size)
{
    memcpy(mmu->mem.data, rom, size);
    cpu->restart();
}

void Machine::tick()
{
    cpu->execute();
}

void Machine::button_pressed(Joypad::Button button)
{
    joypad->button_pressed(button);
}

void Machine::button_released(Joypad::Button button)
{
    joypad->button_released(button);
}
