#pragma once

#include "memory.hh"

class Dma
{
public:
    Memory* mem;

    struct
    {
        uint64_t unemulated_cpu_cycles;
        uint64_t cpu_cycles_left;
        memaddr_t src_pointer;
        memaddr_t dest_pointer;
        memaddr_t src_low;
        memaddr_t src_high;
        memaddr_t size;
    } dma_status;

    Dma(Memory* mem);
    void launch_oam_dma(memaddr_t destination,
                        memaddr_t source,
                        memaddr_t size);
    void emulate(uint64_t cpu_cycles);
    void emulate_oam_dma(uint64_t cpu_cycles);
    void end_oam_dma();
    void clear_dma_status();
};
