#include "mmu.hh"

#include "fileio.hh"
#include <cstring>
#include <iostream>

#include <QFile>

namespace antgb
{
Mmu::Mmu()
{
    cartridge = nullptr;
    hard_reset();
    clear_dma_status();
}

void Mmu::hard_reset()
{
    memset(bytes, 0xFF, 0x10000);

    // Load memory contents after boot ROM execution.
    // TODO: Get rid of Qt dependency.

    QString dump_filepath = ":/memdump/afterboot.dump";
    uint8_t* afterboot_dump = new uint8_t[0x10000];
    load_rom(dump_filepath, afterboot_dump);
    for (size_t i = 0; i < 0x10000; ++i)
    {
        if (i >= VRAM.low)
        {
            force_write(i, afterboot_dump[i]);
        }
    }
    delete[] afterboot_dump;
}

void Mmu::launch_oam_dma(addr_t destination, addr_t source, addr_t size)
{
    dma_status.unemulated_cpu_cycles = 0;
    dma_status.cpu_cycles_left = 640;
    dma_status.dest_pointer = destination;
    dma_status.src_pointer = source;
    dma_status.src_low = source;
    dma_status.src_high = source + size - 1;
    dma_status.size = size;
}

void Mmu::emulate(uint64_t cpu_cycles)
{
    emulate_oam_dma(cpu_cycles);
}

void Mmu::emulate_oam_dma(uint64_t cpu_cycles)
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

void Mmu::end_oam_dma()
{
    clear_dma_status();
}

void Mmu::clear_dma_status()
{
    dma_status.unemulated_cpu_cycles = 0;
    dma_status.cpu_cycles_left = 0;
    dma_status.src_pointer = 0x0000;
    dma_status.dest_pointer = 0x0000;
    dma_status.src_low = 0x0000;
    dma_status.src_high = 0x0000;
    dma_status.size = 0;
}

void Mmu::set_cartridge(Cartridge* cartridge_)
{
    cartridge = cartridge_;
}

} // namespace antgb
