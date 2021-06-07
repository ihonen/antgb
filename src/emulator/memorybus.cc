#include "memorybus.hh"

#include "fileio.hh"
#include <cstring>
#include <iostream>

#include <QFile>

MemoryBus::MemoryBus(
    BootRom& bootrom,
    EchoRam& echoram,
    Hram& hram,
    Oam& oam,
    Vram& vram,
    Wram0& wram0,
    Wram1& wram1,
    ApuRegisters& apu_registers,
    CpuRegisters& cpu_registers,
    JoypadRegisters& joypad_registers,
    PpuRegisters& ppu_registers,
    SerialRegisters& serial_registers,
    TimerRegisters& timer_registers
)
    : bootrom_(bootrom)
    , echoram_(echoram)
    , hram_(hram)
    , oam_(oam)
    , vram_(vram)
    , wram0_(wram0)
    , wram1_(wram1)
    , apu_registers_(apu_registers)
    , cpu_registers_(cpu_registers)
    , joypad_registers_(joypad_registers)
    , ppu_registers_(ppu_registers)
    , serial_registers_(serial_registers)
    , timer_registers_(timer_registers)
{
    bootrom_.set_locked(true);

    cartridge = nullptr;
    hard_reset();
    clear_dma_status();
}

void MemoryBus::hard_reset()
{
    // Load memory contents after boot ROM execution.
    // TODO: Get rid of Qt dependency.

    QString dump_filepath = ":/memdump/afterboot.dump";
    uint8_t* afterboot_dump = new uint8_t[0x10000];
    load_rom(dump_filepath, afterboot_dump);
    for (size_t i = 0; i < 0x10000; ++i)
    {
        if (i >= VRAM_LOW)
        {
            force_write(i, afterboot_dump[i]);
        }
    }
    delete[] afterboot_dump;
}

void MemoryBus::launch_oam_dma(memaddr_t destination, memaddr_t source, memaddr_t size)
{
    dma_status.unemulated_cpu_cycles = 0;
    dma_status.cpu_cycles_left = 640;
    dma_status.dest_pointer = destination;
    dma_status.src_pointer = source;
    dma_status.src_low = source;
    dma_status.src_high = source + size - 1;
    dma_status.size = size;
}

void MemoryBus::emulate(uint64_t cpu_cycles)
{
    emulate_oam_dma(cpu_cycles);
}

void MemoryBus::emulate_oam_dma(uint64_t cpu_cycles)
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

void MemoryBus::end_oam_dma()
{
    clear_dma_status();
}

void MemoryBus::clear_dma_status()
{
    dma_status.unemulated_cpu_cycles = 0;
    dma_status.cpu_cycles_left = 0;
    dma_status.src_pointer = 0x0000;
    dma_status.dest_pointer = 0x0000;
    dma_status.src_low = 0x0000;
    dma_status.src_high = 0x0000;
    dma_status.size = 0;
}

void MemoryBus::set_cartridge(Cartridge* cartridge_)
{
    cartridge = cartridge_;
}
