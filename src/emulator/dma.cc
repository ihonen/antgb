#include "dma.hh"

void Memory::step(uint64_t cpu_cycles)
{
    dma_status.unemulated_cpu_cycles += cpu_cycles;

    while (dma_status.unemulated_cpu_cycles >= 4 && dma_status.cpu_cycles_left >= 4)
    {
        *get(dma_status.dest_pointer) = *get(dma_status.src_pointer);
        write(dma_status.dest_pointer, read(dma_status.src_pointer));
        dma_status.unemulated_cpu_cycles -= 4;
        dma_status.cpu_cycles_left -= 4;
    }

    if (dma_status.cpu_cycles_left < 4)
    {
        end_oam_dma();
    }
}

void Memory::launch_oam_dma(memaddr_t destination, memaddr_t source, memaddr_t size)
{
    dma_status.unemulated_cpu_cycles = 0;
    dma_status.cpu_cycles_left = 640;
    dma_status.dest_pointer = destination;
    dma_status.src_pointer = source;
    dma_status.src_low = source;
    dma_status.src_high = source + size - 1;
    dma_status.size = size;
}

void Memory::end_oam_dma()
{
    clear_dma_status();
}

void Memory::clear_dma_status()
{
    dma_status.unemulated_cpu_cycles = 0;
    dma_status.cpu_cycles_left = 0;
    dma_status.src_pointer = 0x0000;
    dma_status.dest_pointer = 0x0000;
    dma_status.src_low = 0x0000;
    dma_status.src_high = 0x0000;
    dma_status.size = 0;
}
