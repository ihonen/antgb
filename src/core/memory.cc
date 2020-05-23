#include "memory.hh"

#include "fileio.hh"
#include <cstring>
#include <iostream>

#include <QFile>

/*
FF07 - TAC - Timer Control (R/W)

 Bit  2   - Timer Enable
 Bits 1-0 - Input Clock Select
            00: CPU Clock / 1024 (DMG, CGB:   4096 Hz, SGB:   ~4194 Hz)
            01: CPU Clock / 16   (DMG, CGB: 262144 Hz, SGB: ~268400 Hz)
            10: CPU Clock / 64   (DMG, CGB:  65536 Hz, SGB:  ~67110 Hz)
            11: CPU Clock / 256  (DMG, CGB:  16384 Hz, SGB:  ~16780 Hz)

 Note: The "Timer Enable" bit only affects the timer, the divider is ALWAYS counting.

*/

Memory::Memory()
{
    cartridge = nullptr;
    hard_reset();
    clear_dma_status();
}

void Memory::hard_reset()
{
    memset(h8000_vram, 0xFF, VRAM.size);
    memset(hc000_wram0, 0xFF, WRAM0.size);
    memset(hd000_wram1, 0xFF, WRAM1.size);
    memset(hfe00_oam, 0xFF, OAM.size);
    memset(hff00_io, 0xFF, IO.size);
    memset(hff80_hram, 0xFF, HRAM.size);
    memset(&hffff_ie, 0xFF, 1);

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

void Memory::emulate(uint64_t cpu_cycles)
{
    emulate_oam_dma(cpu_cycles);
}

void Memory::emulate_oam_dma(uint64_t cpu_cycles)
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

void Memory::set_cartridge(Cartridge* cartridge_)
{
    cartridge = cartridge_;
}
