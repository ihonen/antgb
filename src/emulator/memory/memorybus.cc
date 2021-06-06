#include "emulator/memory/memorybus.hh"

#include "emulator/memory/bootrom.hh"
#include "emulator/memory/cartridge.hh"
#include "emulator/memory/hram.hh"
#include "emulator/memory/oam.hh"
#include "emulator/memory/vram.hh"
#include "emulator/memory/wram0.hh"
#include "emulator/memory/wram1.hh"

#include "emulator/cpu/cpuregisters.hh"
#include "emulator/peripherals/apuregisters.hh"
#include "emulator/peripherals/joypadregisters.hh"
#include "emulator/peripherals/serialregisters.hh"
#include "emulator/peripherals/timerregisters.hh"
#include "emulator/ppu/ppuregisters.hh"

/*
#include "emulator/fileio.hh"

#include <QFile>

#include <cstring>
#include <iostream>

MemoryBus::MemoryBus()
{
    cartridge = nullptr;
    hard_reset();
    clear_dma_status();
}

void MemoryBus::hard_reset()
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
*/

MemoryBus::MemoryBus(
    BootRom& bootrom,
    Hram& hram,
    Oam& oam,
    Vram& vram,
    Wram0& wram0,
    Wram1& wram1,
    Cartridge& cartridge,
    ApuRegisters& apu_registers,
    CpuRegisters& cpu_registers,
    JoypadRegisters& joypad_registers,
    PpuRegisters& ppu_registers,
    SerialRegisters& serial_registers,
    TimerRegisters& timer_registers
)
    : bootrom_(bootrom)
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
}

void MemoryBus::hard_reset()
{
    assert(false);
}

uint8_t* MemoryBus::get(memaddr_t address)
{
    if (address >= BOOTROM_LOW && address <= BOOTROM_HIGH && !bootrom_.is_locked())
    {
        return bootrom_.get(address);
    }
    else if (address >= ROM0_LOW && address <= ROM0_HIGH)
    {
        return cartridge_.get(address);
    }
    else if (address >= ROM1_LOW && address <= ROM1_HIGH)
    {
        return cartridge_.get(address);
    }
    else if (address >= VRAM_LOW && address <= VRAM_HIGH)
    {
        return vram_.get(address);
    }
    else if (address >= ERAM_LOW && address <= ERAM_HIGH)
    {
        return cartridge_.get(address);
    }
    else if (address >= WRAM0_LOW && address <= WRAM0_HIGH)
    {
        return wram0_.get(address);
    }
    else if (address >= WRAM1_LOW && address <= WRAM1_HIGH)
    {
        return wram1_.get(address);
    }
    else if (address >= ECHO_LOW && address <= ECHO_HIGH)
    {
        return wram0_.get(address - ECHO_LOW + WRAM0_LOW);
    }
    else if (address >= OAM_LOW && address <= OAM_HIGH)
    {
        return oam_.get(address);
    }
    else if (address >= UNUSABLE_LOW && address <= UNUSABLE_HIGH)
    {
        assert(false);
        return nullptr;
    }
    else if (address >= IO_LOW && address <= IO_HIGH)
    {
        if (address == IE_ADDR)
        {
            return &const_cast<uint8_t&>(cpu_registers_.get_IE());
        }
        else if (address == IF_ADDR)
        {
            return &const_cast<uint8_t&>(cpu_registers_.get_IF());
        }
        else if (address >= APU_LOW && address <= APU_HIGH)
        {
            return apu_registers_.get(address);
        }
        else if (address >= JOYP_ADDR)
        {
            return joypad_registers_.get(address);
        }
        else if (address >= PPU_LOW && address <= PPU_HIGH)
        {
            return ppu_registers_.get(address);
        }
        else if (address >= SERIAL_LOW && address <= SERIAL_HIGH)
        {
            return serial_registers_.get(address);
        }
        else if (address >= TIMER_LOW && address <= TIMER_HIGH)
        {
            return timer_registers_.get(address);
        }
    }

    return nullptr;
}

uint8_t MemoryBus::read(memaddr_t address)
{
    if (address >= BOOTROM_LOW && address <= BOOTROM_HIGH && !bootrom_.is_locked())
    {
        return bootrom_.read(address);
    }
    else if (address >= ROM0_LOW && address <= ROM0_HIGH)
    {
        return cartridge_.read(address);
    }
    else if (address >= ROM1_LOW && address <= ROM1_HIGH)
    {
        return cartridge_.read(address);
    }
    else if (address >= VRAM_LOW && address <= VRAM_HIGH)
    {
        return vram_.read(address);
    }
    else if (address >= ERAM_LOW && address <= ERAM_HIGH)
    {
        return cartridge_.read(address);
    }
    else if (address >= WRAM0_LOW && address <= WRAM0_HIGH)
    {
        return wram0_.read(address);
    }
    else if (address >= WRAM1_LOW && address <= WRAM1_HIGH)
    {
        return wram1_.read(address);
    }
    else if (address >= ECHO_LOW && address <= ECHO_HIGH)
    {
        return wram0_.read(address - ECHO_LOW + WRAM0_LOW);
    }
    else if (address >= OAM_LOW && address <= OAM_HIGH)
    {
        return oam_.read(address);
    }
    else if (address >= UNUSABLE_LOW && address <= UNUSABLE_HIGH)
    {
        assert(false);
        return 0xFF;
    }
    else if (address >= IO_LOW && address <= IO_HIGH)
    {
        if (address == IE_ADDR)
        {
            return cpu_registers_.read_IE();
        }
        else if (address == IF_ADDR)
        {
            return cpu_registers_.read_IF();
        }
        else if (address >= APU_LOW && address <= APU_HIGH)
        {
            return apu_registers_.read(address);
        }
        else if (address >= JOYP_ADDR)
        {
            return joypad_registers_.read(address);
        }
        else if (address >= PPU_LOW && address <= PPU_HIGH)
        {
            return ppu_registers_.read(address);
        }
        else if (address >= SERIAL_LOW && address <= SERIAL_HIGH)
        {
            return serial_registers_.read(address);
        }
        else if (address >= TIMER_LOW && address <= TIMER_HIGH)
        {
            return timer_registers_.read(address);
        }
    }

    return 0xFF;
}

bool MemoryBus::write(memaddr_t address, uint8_t value)
{
    if (address >= BOOTROM_LOW && address <= BOOTROM_HIGH && !bootrom_.is_locked())
    {
        bootrom_.write(address, value);
    }
    else if (address >= ROM0_LOW && address <= ROM0_HIGH)
    {
        cartridge_.write(address, value);
    }
    else if (address >= ROM1_LOW && address <= ROM1_HIGH)
    {
        cartridge_.write(address, value);
    }
    else if (address >= VRAM_LOW && address <= VRAM_HIGH)
    {
        vram_.write(address, value);
    }
    else if (address >= ERAM_LOW && address <= ERAM_HIGH)
    {
        cartridge_.write(address, value);
    }
    else if (address >= WRAM0_LOW && address <= WRAM0_HIGH)
    {
        wram0_.write(address, value);
    }
    else if (address >= WRAM1_LOW && address <= WRAM1_HIGH)
    {
        wram1_.write(address, value);
    }
    else if (address >= ECHO_LOW && address <= ECHO_HIGH)
    {
        wram0_.write(address - ECHO_LOW + WRAM0_LOW, value);
    }
    else if (address >= OAM_LOW && address <= OAM_HIGH)
    {
        oam_.write(address, value);
    }
    else if (address >= UNUSABLE_LOW && address <= UNUSABLE_HIGH)
    {
        assert(false);
    }
    else if (address >= IO_LOW && address <= IO_HIGH)
    {
        if (address == IE_ADDR)
        {
            cpu_registers_.write_IE(value);
        }
        else if (address == IF_ADDR)
        {
            cpu_registers_.write_IF(value);
        }
        else if (address >= APU_LOW && address <= APU_HIGH)
        {
            apu_registers_.write(address, value);
        }
        else if (address >= JOYP_ADDR)
        {
            joypad_registers_.write(address, value);
        }
        else if (address >= PPU_LOW && address <= PPU_HIGH)
        {
            ppu_registers_.write(address, value);
        }
        else if (address >= SERIAL_LOW && address <= SERIAL_HIGH)
        {
            serial_registers_.write(address, value);
        }
        else if (address >= TIMER_LOW && address <= TIMER_HIGH)
        {
            timer_registers_.write(address, value);
        }
    }
}

void MemoryBus::launch_oam_dma(memaddr_t destination, memaddr_t source, memaddr_t size)
{

}

void MemoryBus::emulate(uint64_t cpu_cycles)
{

}

void MemoryBus::emulate_oam_dma(uint64_t cpu_cycles)
{

}

void MemoryBus::end_oam_dma()
{

}

void MemoryBus::clear_dma_status()
{

}
