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

    static constexpr memaddr_t SB_ADDRESS = 0xFF01;
    static constexpr memaddr_t SC_ADDRESS = 0xFF02;
    static constexpr memaddr_t LOW_ADDRESS = SB_ADDRESS;
    static constexpr memaddr_t HIGH_ADDRESS = SC_ADDRESS;

    static constexpr uint64_t CPU_CYCLES_PER_BYTE = 4096;

    Serial(Memory* memory, Irc* irc);
    void emulate(uint64_t cpu_cycles);

    uint64_t cpu_cycles_left_in_transfer;
    Irc* irc;
    Memory* mem;
};
