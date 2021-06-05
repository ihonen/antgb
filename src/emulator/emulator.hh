#pragma once

#include "debugger/iemulator.hh"
#include "memory.hh"
#include <memory>

class Cartridge;
class Cpu;
class Irc;
class Joypad;
class Memory;
class Ppu;
class Serial;
class Timer;

class Emulator : public iEmulator
{
public:
    Emulator();
    virtual ~Emulator() override;

    virtual void set_frontend(iFrontend* frontend) override;

    virtual void load_rom(const std::string& filepath) override;
    inline virtual int execute_next() override;
    virtual void button_pressed(JoypadButton button) override;
    virtual void button_released(JoypadButton button) override;
    virtual void reset_emulation() override;

    inline virtual uint16_t read(regid_t register_id) override;
    inline virtual void write(regid_t register_id, uint16_t value) override;

    std::unique_ptr<Cpu> cpu;
    std::unique_ptr<Irc> irc;
    std::unique_ptr<Memory> mem;
    std::unique_ptr<Ppu> ppu;
    std::unique_ptr<Joypad> joypad;
    std::unique_ptr<Timer> timer_divider;
    std::unique_ptr<Serial> serial;
    std::unique_ptr<Cartridge> cartridge;
};

FORCE_INLINE int Emulator::execute_next()
{
    uint64_t cpu_cycle_count_before = cpu->get_cycles();
    cpu->execute();
    uint64_t cpu_cycle_count_after = cpu->get_cycles();
    uint64_t clock_cycles = cpu_cycle_count_after - cpu_cycle_count_before;

    timer_divider->emulate(clock_cycles);
    mem->emulate(clock_cycles);
    ppu->step(clock_cycles);
    serial->emulate(clock_cycles);

    return static_cast<int>(clock_cycles);
}

FORCE_INLINE uint16_t Emulator::read(regid_t register_id)
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

FORCE_INLINE void Emulator::write(regid_t register_id, uint16_t value)
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
