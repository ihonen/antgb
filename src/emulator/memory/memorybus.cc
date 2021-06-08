#include "memorybus.hh"

#include "cartridge.hh"
#include "emulator/common/fileio.hh"
#include <cstring>
#include <iostream>

#include <QFile>

MemoryBus::MemoryBus(
    BootRom& bootrom,
    iMemoryBusNode& echoram,
    iMemoryBusNode& hram,
    iMemoryBusNode& oam,
    iMemoryBusNode& vram,
    iMemoryBusNode& wram0,
    iMemoryBusNode& wram1,
    Cartridge& cartridge,
    iMemoryBusNode& apu_registers,
    iMemoryBusNode& cpu_registers,
    iMemoryBusNode& joypad_registers,
    iMemoryBusNode& ppu_registers,
    iMemoryBusNode& serial_registers,
    iMemoryBusNode& timer_registers
)
    : bootrom_(bootrom)
    , echoram_(echoram)
    , hram_(hram)
    , oam_(oam)
    , vram_(vram)
    , wram0_(wram0)
    , wram1_(wram1)
    , cartridge_(cartridge)
    , apu_registers_(apu_registers)
    , cpu_registers_(cpu_registers)
    , joypad_registers_(joypad_registers)
    , ppu_registers_(ppu_registers)
    , serial_registers_(serial_registers)
    , timer_registers_(timer_registers)
{
    bootrom_.set_locked(true);
    hard_reset();
}

void MemoryBus::hard_reset()
{
    // Load memory contents after boot ROM execution.
    // TODO: Get rid of Qt dependency.
    // TODO: Move elsewhere.

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
