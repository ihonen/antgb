#include "emulator.hh"

#include "renderer.hh"
#include <iostream>
#include <cstring>
#include <thread>

using namespace std;

Emulator::Emulator()
{
    mem = new Memory();
    irc = new Irc(&mem->hff0f_if, &mem->hffff_ie);
    ppu = new Ppu(mem, &mem->ppureg, irc);
    cpu = new Cpu(mem, irc);
    joypad = new Joypad(mem, irc);
    timer_divider = new Timer(&mem->timerreg, irc);
    serial = new Serial(&mem->serialreg, irc);
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
