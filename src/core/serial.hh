#pragma once

#include "interrupts.hh"
#include "types.hh"

class Serial
{
public:
    Serial(Memory* memory, Irc* irc);
    void emulate(uint64_t cpu_cycles);

    static constexpr uint64_t CPU_CYCLES_PER_BYTE = 4096;
    uint64_t cpu_cycles_left_in_transfer;
    Irc* irc;
    Memory* mem;
};
