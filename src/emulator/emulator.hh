#pragma once

#include "debugger/iemulator.hh"

#include "macros.hh"
#include "memory.hh"

#include <memory>

class CpuRegisters;

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

    std::unique_ptr<CpuRegisters> cpu_registers;

    std::unique_ptr<Cpu> cpu;
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

    //timer_divider->emulate(clock_cycles);
    //mem->emulate(clock_cycles);
    //ppu->step(clock_cycles);
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
            return cpu->reg.read_A();
        case RegF:
            return cpu->reg.read_F();
        case RegAF:
            return cpu->reg.read_AF();
        case RegB:
            return cpu->reg.read_B();
        case RegC:
            return cpu->reg.read_C();
        case RegBC:
            return cpu->reg.read_BC();
        case RegD:
            return cpu->reg.read_D();
        case RegE:
            return cpu->reg.read_E();
        case RegDE:
            return cpu->reg.read_DE();
        case RegH:
            return cpu->reg.read_H();
        case RegL:
            return cpu->reg.read_L();
        case RegHL:
            return cpu->reg.read_HL();
        case RegPC:
            return cpu->reg.read_PC();
        case RegSP:
            return cpu->reg.read_SP();
        case RegIME:
            return cpu->reg.read_IME();
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
            cpu->reg.write_A(static_cast<uint8_t>(value));
            break;
        case RegF:
            cpu->reg.write_F(static_cast<uint8_t>(value));
            break;
        case RegAF:
            cpu->reg.write_AF(value);
            break;
        case RegB:
            cpu->reg.write_B(static_cast<uint8_t>(value));
            break;
        case RegC:
            cpu->reg.write_C(static_cast<uint8_t>(value));
            break;
        case RegBC:
            cpu->reg.write_BC(value);
            break;
        case RegD:
            cpu->reg.write_D(static_cast<uint8_t>(value));
            break;
        case RegE:
            cpu->reg.write_E(static_cast<uint8_t>(value));
            break;
        case RegDE:
            cpu->reg.write_DE(value);
            break;
        case RegH:
            cpu->reg.write_H(static_cast<uint8_t>(value));
            break;
        case RegL:
            cpu->reg.write_L(static_cast<uint8_t>(value));
            break;
        case RegHL:
            cpu->reg.write_HL(value);
            break;
        case RegPC:
            cpu->reg.write_PC(value);
            break;
        case RegSP:
            cpu->reg.write_SP(value);
            break;
        case RegIME:
            cpu->reg.write_IME((value != 0) ? 1 : 0);
            break;
        default:
            cerr << std::hex << register_id << endl;
            assert(false);
            break;
    }
}
