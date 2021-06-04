#include "emulator.hh"

#include "addresses.hh"
#include "renderer.hh"

#include <iostream>
#include <cstring>
#include <thread>

Emulator::Emulator()
{
    mem = new Memory();
    irc = new Irc(mem->get(IF_ADDR), mem->get(IE_ADDR));
    ppu = new Ppu(mem, (Ppu::Registers*)mem->get(PPU_LOW_ADDR), irc);
    cpu = new Cpu(mem, irc);
    joypad = new Joypad(mem, irc);
    timer_divider = new Timer((Timer::Registers*)mem->get(TIMER_LOW_ADDR), irc);
    serial = new Serial((Serial::Registers*)mem->get(SERIAL_LOW_ADDR), irc);
    cartridge = nullptr;
}

Emulator::~Emulator()
{
    delete mem;
    delete irc;
    delete ppu;
    delete cpu;
    delete joypad;
    delete timer_divider;
    delete serial;
    delete cartridge;
}

void Emulator::load_rom(const std::string& filepath)
{
    if (cartridge != nullptr)
    {
        delete cartridge;
        cartridge = nullptr;
    }

    cartridge = new Cartridge(filepath);

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
    irc->hard_reset();
}

void Emulator::set_render_callback(void (*callback)(const framebuf_t*, int))
{

}
