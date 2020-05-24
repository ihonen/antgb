#include "emulator.hh"

#include "renderer.hh"
#include <iostream>
#include <cstring>
#include <thread>

namespace antgb
{

Emulator::Emulator()
{
    mem = new Memory();
    irc = new Irc(mem->get(Irc::IF_ADDRESS), mem->get(Irc::IE_ADDRESS));
    ppu = new Ppu(mem, (Ppu::Registers*)mem->get(Ppu::LOW_ADDRESS), irc);
    cpu = new Cpu(mem, irc);
    joypad = new Joypad(mem, irc);
    timer_divider = new Timer((Timer::Registers*)mem->get(Timer::LOW_ADDRESS), irc);
    serial = new Serial((Serial::Registers*)mem->get(Serial::LOW_ADDRESS), irc);
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
    irc->hard_reset();
}

void Emulator::set_render_callback(void (*callback)(const framebuf_t*, int))
{

}

} // namespace antgb
