#include "memory.hh"

#include "fileio.hh"
#include <cstring>
#include <iostream>

#include <QFile>

// TODO: Write-only (such as NR11)
struct Mask
{
    uint8_t valid = 0xFF;
    uint8_t invalid = 0x00;
    uint8_t readonly = 0xFF;
};

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

static std::map<memaddr_t, Mask> MASK
{
    // JOYP
    {0xFF00, {0b00111111, 0b11000000, 0b00001111}},
    // TAC
    {0xFF07, {0b00000111, 0b11111000, 0b11111000}},
    // NR52
    {0xFF26, {0b11111111, 0b00000000, 0b00001111}},
    //STAT
    {0xFF41, {0b01111111, 0b10000000, 0b10000111}},
    // IF
    {0xFF00, {0b00011111, 0b11100000, 0b11100000}},
    // IE
    {0xFFFF, {0b00011111, 0b11100000, 0b11100000}}
};

Memory::Memory()
{
    hard_reset();
    cartridge = nullptr;
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
    memset(hffff_ie, 0xFF, IE.size);

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

uint8_t* Memory::get(memaddr_t address)
{
    if (address <= ROM1.high)
    {
        if (cartridge) return &cartridge->data[address];
        else return nullptr;
    }
    else if (address <= VRAM.high)
    {
        return &h8000_vram[address - VRAM.low];
    }
    else if (address <= ERAM.high)
    {
        return nullptr;
    }
    else if (address <= WRAM0.high)
    {
        return &hc000_wram0[address - WRAM0.low];
    }
    else if (address <= WRAM1.high)
    {
        return &hd000_wram1[address -WRAM1.low];
    }
    else if (address <= ECHO.high)
    {
        return get(WRAM0.low + (address - ECHO.low));
    }
    else if (address <= OAM.high)
    {
        return &hfe00_oam[address - OAM.low];
    }
    else if (address <= UNUSABLE.high)
    {
        return nullptr;
    }
    else if (address <= IO.high)
    {
        return &hff00_io[address - IO.low];
    }
    else if (address <= HRAM.high)
    {
        return &hff80_hram[address - HRAM.low];
    }
    else
    {
        return hffff_ie;
    }
}

uint8_t Memory::read(memaddr_t address)
{
    uint8_t* source = get(address);
    if (!source) return 0xFF;

    // Some addresses have bits that can't be read, so make sure to return 1
    // in the places of those bits.
    uint8_t invalid_mask = 0x00;
    if (MASK.count(address))
        invalid_mask = MASK[address].invalid;

    return *source | (invalid_mask & 0xFF);
}

bool Memory::write(memaddr_t address, uint8_t value)
{
    uint8_t* dest = get(address);
    if (!dest) return false;

    // Some addresses have read-only bits, so make sure not to change them.
    uint8_t readonly_mask = 0x00;
    if (MASK.count(address))
        readonly_mask = MASK[address].readonly;

    // DIV
    if (address == 0xFF04) value = 0x00;

    *dest = (*dest & readonly_mask) | (value & ~readonly_mask);
    return true;
}

bool Memory::force_write(memaddr_t address, uint8_t value)
{
    auto dest = get(address);
    if (dest) *dest = value;
    return 0;
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
