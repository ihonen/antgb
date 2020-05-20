#include "emulator.hh"

#include "renderer.hh"
#include <iostream>
#include <cstring>
#include <thread>

using namespace std;

Emulator::Emulator()
{
    mem = new Memory();
    irc = new Irc(mem);
    ppu = new Ppu(mem, irc);
    cpu = new Cpu(mem, irc);
    joypad = new Joypad(mem, irc);
    timer_divider = new Timer(mem, irc);
    serial = new Serial(mem ,irc);
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

int Emulator::execute_next()
{
    uint64_t cpu_cycle_count_before = cpu->get_cycles();
    cpu->execute();
    uint64_t cpu_cycle_count_after = cpu->get_cycles();
    cpu->reset_cycles();
    int clock_cycles = cpu_cycle_count_after - cpu_cycle_count_before;

    timer_divider->emulate(clock_cycles);
    mem->emulate(clock_cycles);
    ppu->step(clock_cycles);
    serial->emulate(clock_cycles);
    static uint64_t cpu_cycles_total = 0;
    cpu_cycles_total += clock_cycles;

    return clock_cycles;
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

uint16_t Emulator::read(regid_t register_id)
{
    if (register_id <= 0xFFFF)
        return mem->read(static_cast<memaddr_t>(register_id));

    switch (register_id)
    {
        case RegA:
            return cpu->A;
        case RegF:
            return cpu->F;
        case RegAF:
            return cpu->AF;
        case RegB:
            return cpu->B;
        case RegC:
            return cpu->C;
        case RegBC:
            return cpu->BC;
        case RegD:
            return cpu->D;
        case RegE:
            return cpu->E;
        case RegDE:
            return cpu->DE;
        case RegH:
            return cpu->H;
        case RegL:
            return cpu->L;
        case RegHL:
            return cpu->HL;
        case RegPC:
            return cpu->PC;
        case RegSP:
            return cpu->SP;
        case RegIME:
            return irc->IME;
        default:
            cerr << std::hex << register_id << endl;
            assert(false);
            break;
    }

    return 0xFFFF;
}

void Emulator::write(regid_t register_id, uint16_t value)
{
    if (register_id <= 0xFFFF)
    {
        mem->write(static_cast<memaddr_t>(register_id), static_cast<uint8_t>(value));
        return;
    }

    switch (register_id)
    {
        case RegA:
            cpu->A = static_cast<uint8_t>(value);
            break;
        case RegF:
            // Only the bottom 4 bits of F are writable.
            cpu->F |= (0x0F) & static_cast<uint8_t>(value);
            break;
        case RegAF:
            cpu->AF = value;
            break;
        case RegB:
            cpu->B = static_cast<uint8_t>(value);
            break;
        case RegC:
            cpu->C = static_cast<uint8_t>(value);
            break;
        case RegBC:
            cpu->BC = value;
            break;
        case RegD:
            cpu->D = static_cast<uint8_t>(value);
            break;
        case RegE:
            cpu->E = static_cast<uint8_t>(value);
            break;
        case RegDE:
            cpu->DE = value;
            break;
        case RegH:
            cpu->H = static_cast<uint8_t>(value);
            break;
        case RegL:
            cpu->L = static_cast<uint8_t>(value);
            break;
        case RegHL:
            cpu->HL = value;
            break;
        case RegPC:
            cpu->PC = value;
            break;
        case RegSP:
            cpu->SP = value;
            break;
        case RegIME:
            irc->IME = (value != 0) ? 1 : 0;
            break;
        default:
            cerr << std::hex << register_id << endl;
            assert(false);
            break;
    }
}
