#pragma once

#include "interrupts.hh"
#include "types.hh"

class Serial
{
public:
    struct Registers
    {
        uint8_t sb;
        uint8_t sc;
    };

    static constexpr uint64_t CPU_CYCLES_PER_BYTE = 4096;

    Serial(Registers* reg, Irc* irc);
    inline void emulate(uint64_t cpu_cycles);

    uint64_t cpu_cycles_left_in_transfer;
    Irc* irc;
    Registers* reg;
};

FORCE_INLINE void Serial::emulate(uint64_t cpu_cycles)
{
    if (reg->sc == 0x81 && !cpu_cycles_left_in_transfer)
    {
        cpu_cycles_left_in_transfer = CPU_CYCLES_PER_BYTE;
    }

    if (cpu_cycles_left_in_transfer <= cpu_cycles && cpu_cycles_left_in_transfer != 0)
    {
        irc->request_interrupt(Irc::SerialInterrupt);
        cpu_cycles_left_in_transfer = 0;
        reg->sb = 0x00;
        reg->sc = 0x01;
    }
    else
    {
        cpu_cycles_left_in_transfer -= cpu_cycles;
    }

}