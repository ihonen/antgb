#include "emulator.hh"

#include "addresses.hh"

#include <iostream>
#include <cstring>
#include <thread>

namespace antgb
{
Emulator::Emulator(iRenderer* gui_renderer)
{
    cartridge     = new Cartridge("C:\\Users\\anton\\Desktop\\antgb\\testbin\\tetris_jue_v1_1.gb");
    mem           = new Mmu();
    cpu           = new Cpu(mem, mem->get(REG_IE), mem->get(REG_IF));
    ppu           = new Ppu(mem, (Ppu::Registers*)mem->get(PPU_LOW), cpu, gui_renderer);
    joypad        = new Joypad(mem, cpu);
    timer_divider = new Timer((Timer::Registers*)mem->get(TIMER_LOW), cpu);
    serial        = new Serial((Serial::Registers*)mem->get(SERIAL_LOW), cpu);

    mem->set_cartridge(cartridge);
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
