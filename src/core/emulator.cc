#include "emulator.hh"

#include <iostream>
#include <cstring>
#include <thread>

namespace antgb
{
Emulator::Emulator()
{
    renderer      = nullptr;
    mem           = new Mmu();
    cpu           = new Cpu(mem, mem->get(IE_ADDR), mem->get(IF_ADDR));
    ppu = new Ppu(mem, (Ppu::Registers*)mem->get(PPU_LOW_ADDR), cpu, renderer);
    joypad        = new Joypad(mem, cpu);
    timer_divider = new Timer((Timer::Registers*)mem->get(TIMER_LOW_ADDR), cpu);
    serial    = new Serial((Serial::Registers*)mem->get(SERIAL_LOW_ADDR), cpu);
    cartridge = nullptr;
}

Emulator::~Emulator()
{
    delete mem;
    delete ppu;
    delete cpu;
    delete joypad;
    delete timer_divider;
    delete serial;
    delete cartridge;
}

void Emulator::set_renderer(iRenderer* renderer)
{
    ppu->set_renderer(renderer);
}

void Emulator::load_rom(const void* rom, size_t size)
{
    if (cartridge) delete cartridge;
    cartridge = new Cartridge;
    memcpy(cartridge->data, rom, size);
    mem->set_cartridge(cartridge);
    reset_emulation();
}

void Emulator::button_pressed(JoypadButton button)
{
    joypad->button_pressed(button);
}

void Emulator::button_released(JoypadButton button)
{
    joypad->button_released(button);
}

void Emulator::reset_emulation()
{
    mem->hard_reset();
    cpu->hard_reset();
}

} // namespace antgb
