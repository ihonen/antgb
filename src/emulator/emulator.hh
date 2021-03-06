#pragma once

#include "debugger/iemulator.hh"

#include "emulator/cpu/cpu.hh"
#include "emulator/common/macros.hh"
#include "emulator/memory/memorybus.hh"
#include "emulator/ppu/ppu.hh"
#include "emulator/peripherals/joypad.hh"
#include "emulator/peripherals/serial.hh"
#include "emulator/peripherals/timer.hh"

#include "emulator/peripherals/apuregisters.hh"
#include "emulator/cpu/cpuregisters.hh"
#include "emulator/peripherals/joypadregisters.hh"
#include "emulator/ppu/ppuregisters.hh"
#include "emulator/peripherals/serialregisters.hh"
#include "emulator/peripherals/timerregisters.hh"

#include "emulator/memory/dma.hh"
#include "emulator/memory/bootrom.hh"
#include "emulator/memory/echoram.hh"
#include "emulator/memory/hram.hh"
#include "emulator/memory/oam.hh"
#include "emulator/memory/vram.hh"
#include "emulator/memory/wram0.hh"
#include "emulator/memory/wram1.hh"

#include <iostream>
#include <memory>

class Cartridge;
class Irc;
class Joypad;
class MemoryBus;
class Ppu;
class Serial;
class Timer;

class Emulator : public iEmulator
{
public:
    Emulator();
    virtual ~Emulator() override;

    virtual void set_joypad_press_callback(iFrontend::JoypadCallback callback) override;
    virtual void set_joypad_release_callback(iFrontend::JoypadCallback callback) override;
    virtual void set_render_callback(iFrontend::RenderCallback callback) override;
    virtual void set_serial_callback(iFrontend::SerialCallback callback) override;

    virtual void load_rom(const std::string& filepath) override;
    inline virtual int execute_next() override;
    virtual void button_pressed(JoypadButton button) override;
    virtual void button_released(JoypadButton button) override;
    virtual void reset_emulation() override;

    inline virtual uint16_t read(regid_t register_id) override;
    inline virtual void write(regid_t register_id, uint16_t value) override;

    iFrontend::JoypadCallback joypad_press_callback;
    iFrontend::JoypadCallback joypad_release_callback;
    iFrontend::RenderCallback render_callback;
    iFrontend::SerialCallback serial_callback;

    std::unique_ptr<BootRom> bootrom;
    std::unique_ptr<EchoRam> echoram;
    std::unique_ptr<Hram> hram;
    std::unique_ptr<Oam> oam;
    std::unique_ptr<Vram> vram;
    std::unique_ptr<Wram0> wram0;
    std::unique_ptr<Wram1> wram1;

    std::unique_ptr<ApuRegisters> apu_registers;
    std::unique_ptr<CpuRegisters> cpu_registers;
    std::unique_ptr<JoypadRegisters> joypad_registers;
    std::unique_ptr<PpuRegisters> ppu_registers;
    std::unique_ptr<SerialRegisters> serial_registers;
    std::unique_ptr<TimerRegisters> timer_registers;

    std::unique_ptr<MemoryBus> mem;

    std::unique_ptr<Interrupts> interrupts;

    std::unique_ptr<Cpu> cpu;
    std::unique_ptr<Dma> dma;
    std::unique_ptr<Ppu> ppu;
    std::unique_ptr<Joypad> joypad;
    std::unique_ptr<Timer> timer;
    std::unique_ptr<Serial> serial;
    std::unique_ptr<Cartridge> cartridge;
};

FORCE_INLINE int Emulator::execute_next()
{
    dma->pre_cpu_exec_tick();
    interrupts->pre_cpu_exec_tick();
    ppu->pre_cpu_exec_tick();
    serial->pre_cpu_exec_tick();
    timer->pre_cpu_exec_tick();

    auto elapsed_tcycles_before = cpu->get_elapsed_tcycles();

    cpu->execute_next();

    auto tcycles_passed = cpu->get_elapsed_tcycles() - elapsed_tcycles_before;

    dma->post_cpu_exec_tick(tcycles_passed);
    interrupts->post_cpu_exec_tick(tcycles_passed);
    serial->post_cpu_exec_tick(tcycles_passed);
    timer->post_cpu_exec_tick(tcycles_passed);
    ppu->post_cpu_exec_tick(tcycles_passed);

    return static_cast<int>(tcycles_passed);
}

FORCE_INLINE uint16_t Emulator::read(regid_t register_id)
{
    if (register_id <= 0xFFFF)
        return mem->read(static_cast<addr_t>(register_id));

    switch (register_id)
    {
        case RegA:
            return cpu->get_registers().read_A();
        case RegF:
            return cpu->get_registers().read_F();
        case RegAF:
            return cpu->get_registers().read_AF();
        case RegB:
            return cpu->get_registers().read_B();
        case RegC:
            return cpu->get_registers().read_C();
        case RegBC:
            return cpu->get_registers().read_BC();
        case RegD:
            return cpu->get_registers().read_D();
        case RegE:
            return cpu->get_registers().read_E();
        case RegDE:
            return cpu->get_registers().read_DE();
        case RegH:
            return cpu->get_registers().read_H();
        case RegL:
            return cpu->get_registers().read_L();
        case RegHL:
            return cpu->get_registers().read_HL();
        case RegPC:
            return cpu->get_registers().read_PC();
        case RegSP:
            return cpu->get_registers().read_SP();
        case RegIME:
            return cpu->get_registers().read_IME();
        default:
            std::cerr << std::hex << register_id << std::endl;
            assert(false);
            break;
    }

    return 0xFFFF;
}

FORCE_INLINE void Emulator::write(regid_t register_id, uint16_t value)
{
    if (register_id <= 0xFFFF)
    {
        mem->write(static_cast<addr_t>(register_id), static_cast<uint8_t>(value));
        return;
    }

    switch (register_id)
    {
        case RegA:
            cpu->get_registers().write_A(static_cast<uint8_t>(value));
            break;
        case RegF:
            cpu->get_registers().write_F(static_cast<uint8_t>(value));
            break;
        case RegAF:
            cpu->get_registers().write_AF(value);
            break;
        case RegB:
            cpu->get_registers().write_B(static_cast<uint8_t>(value));
            break;
        case RegC:
            cpu->get_registers().write_C(static_cast<uint8_t>(value));
            break;
        case RegBC:
            cpu->get_registers().write_BC(value);
            break;
        case RegD:
            cpu->get_registers().write_D(static_cast<uint8_t>(value));
            break;
        case RegE:
            cpu->get_registers().write_E(static_cast<uint8_t>(value));
            break;
        case RegDE:
            cpu->get_registers().write_DE(value);
            break;
        case RegH:
            cpu->get_registers().write_H(static_cast<uint8_t>(value));
            break;
        case RegL:
            cpu->get_registers().write_L(static_cast<uint8_t>(value));
            break;
        case RegHL:
            cpu->get_registers().write_HL(value);
            break;
        case RegPC:
            cpu->get_registers().write_PC(value);
            break;
        case RegSP:
            cpu->get_registers().write_SP(value);
            break;
        case RegIME:
            cpu->get_registers().write_IME((value != 0) ? 1 : 0);
            break;
        default:
            std::cerr << std::hex << register_id << std::endl;
            assert(false);
            break;
    }
}
