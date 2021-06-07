#include "memory.hh"

#include "fileio.hh"
#include <cstring>
#include <iostream>

#include <QFile>

Memory::Memory(
    ApuRegisters& apu_registers,
    CpuRegisters& cpu_registers,
    JoypadRegisters& joypad_registers
)
    : apu_registers_(apu_registers)
    , cpu_registers_(cpu_registers)
    , joypad_registers_(joypad_registers)
{
    cartridge = nullptr;
    hard_reset();
    clear_dma_status();
}

void Memory::hard_reset()
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
