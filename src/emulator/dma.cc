#include "dma.hh"

// TODO: Verify correctness.

Dma::Dma(MemoryBus& memory_bus)
    : memory_bus_(memory_bus)
    , unemulated_cpu_cycles_(0)
    , cpu_cycles_left_(0)
    , src_pointer_(0)
    , dest_pointer_(0)
    , src_low_(0)
    , src_high_(0)
    , size_(0)
{

}

void Dma::launch_oam_dma(addr_t destination, addr_t source, addr_t size)
{
    unemulated_cpu_cycles_ = 0;
    cpu_cycles_left_ = 640;
    dest_pointer_ = destination;
    src_pointer_ = source;
    src_low_ = source;
    src_high_ = source + size - 1;
    size_ = size;
}

void Dma::emulate(uint64_t cpu_cycles)
{
    unemulated_cpu_cycles_ += cpu_cycles;

    while (unemulated_cpu_cycles_ >= 4 && cpu_cycles_left_ >= 4)
    {
        *memory_bus_.get(dest_pointer_) = *memory_bus_.get(src_pointer_);
        memory_bus_.write(dest_pointer_, memory_bus_.read(src_pointer_));
        unemulated_cpu_cycles_ -= 4;
        cpu_cycles_left_ -= 4;
    }

    if (cpu_cycles_left_ < 4)
    {
        end_oam_dma();
    }
}

void Dma::end_oam_dma()
{
    clear_dma_status();
}

void Dma::clear_dma_status()
{
    unemulated_cpu_cycles_ = 0;
    cpu_cycles_left_ = 0;
    src_pointer_ = 0x0000;
    dest_pointer_ = 0x0000;
    src_low_ = 0x0000;
    src_high_ = 0x0000;
    size_ = 0;
}
