#pragma once

#include "memorybus.hh"

class Dma
{
public:
    Dma(MemoryBus& memory_bus);

    void launch_oam_dma(addr_t destination, addr_t source, addr_t size);
    void emulate(uint64_t cpu_cycles);
    void end_oam_dma();
    void clear_dma_status();

protected:
    MemoryBus& memory_bus_;

    uint64_t unemulated_cpu_cycles_;
    uint64_t cpu_cycles_left_;
    addr_t src_pointer_;
    addr_t dest_pointer_;
    addr_t src_low_;
    addr_t src_high_;
    addr_t size_;
};
