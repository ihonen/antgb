#pragma once

#include "debugger/ifrontend.hh"
#include "emulator/cpu/interrupts.hh"
#include "serialregisters.hh"

class Serial
{
public:
    static constexpr uint64_t CPU_CYCLES_PER_BYTE = 4096;

    Serial(SerialRegisters& reg, Interrupts& interrupts, iFrontend* frontend = nullptr);
    void set_frontend(iFrontend* frontend);
    inline void emulate(uint64_t cpu_cycles);

    uint64_t cpu_cycles_left_in_transfer;
    Interrupts& interrupts;
    SerialRegisters& reg;
    iFrontend* frontend_;
};

FORCE_INLINE void Serial::emulate(uint64_t cpu_cycles)
{
    if (reg.read(SB_ADDR) != 0x00 && reg.read(SC_ADDR) == 0x81)
    {
        if (frontend_ != nullptr)
        {
            frontend_->serial_callback(reg.read(SB_ADDR));
        }
        reg.write(SB_ADDR, 0x00);
        cpu_cycles_left_in_transfer = CPU_CYCLES_PER_BYTE;
    }

    if (cpu_cycles_left_in_transfer <= cpu_cycles && cpu_cycles_left_in_transfer != 0)
    {
        interrupts.request_interrupt(Interrupts::SerialInterrupt);
        cpu_cycles_left_in_transfer = 0;
        reg.write(SB_ADDR, 0x00);
        reg.write(SC_ADDR, 0x01);
    }
    else
    {
        cpu_cycles_left_in_transfer -= cpu_cycles;
    }
}
