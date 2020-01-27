#ifndef SERIAL_HH
#define SERIAL_HH

#include "interrupts.hh"
#include "memory.hh"
#include "../util/typedefs.hh"

class Serial
{
public:
    Serial(Memory* memory, InterruptController* irc);
    void emulate(uint64_t cpu_cycles);

    static constexpr uint64_t CPU_CYCLES_PER_BYTE = 4096;
    uint64_t cpu_cycles_left_in_transfer;
    InterruptController* irc;
    Memory* mem;
};

#endif // SERIAL_HH
