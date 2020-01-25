#include "mmu.hh"

#include <iostream>

using namespace std;

MMU::MMU()
{
    memset(h8000_vram, 0x00, VRAM.size);
    memset(hc000_wram0, 0x00, WRAM0.size);
    memset(hd000_wram1, 0x00, WRAM1.size);
    memset(hfe00_oam, 0x00, OAM.size);
    memset(hff00_io, 0x00, IO.size);
    memset(hff80_hram, 0x00, HRAM.size);
    memset(hffff_ie, 0x00, IE.size);

    cartridge = nullptr;
    clear_dma_status();
}

uint8_t* MMU::get(memaddr_t address)
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

uint8_t MMU::read(memaddr_t address)
{
    uint8_t* source = get(address);
    if (source)
    {
        return *source;
    }
    return 0xFF;
}

bool MMU::write(memaddr_t address, uint8_t value)
{
    uint8_t* dest = get(address);
    if (!dest)
    {
        return false;
    }

    *dest = value;
    if (address == 0xFF02)
    {
        cout << *get(0xFF01);
    }

    return true;
}

void MMU::launch_oam_dma(memaddr_t destination, memaddr_t source, memaddr_t size)
{
    dma_status.unemulated_cpu_cycles = 0;
    dma_status.cpu_cycles_left = 640;
    dma_status.dest_pointer = destination;
    dma_status.src_pointer = source;
    dma_status.src_low = source;
    dma_status.src_high = source + size - 1;
    dma_status.size = size;
}

void MMU::emulate(uint64_t cpu_cycles)
{
    emulate_oam_dma(cpu_cycles);
}

void MMU::emulate_oam_dma(uint64_t cpu_cycles)
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

void MMU::end_oam_dma()
{
    clear_dma_status();
}

void MMU::clear_dma_status()
{
    dma_status.unemulated_cpu_cycles = 0;
    dma_status.cpu_cycles_left = 0;
    dma_status.src_pointer = 0x0000;
    dma_status.dest_pointer = 0x0000;
    dma_status.src_low = 0x0000;
    dma_status.src_high = 0x0000;
    dma_status.size = 0;
}

void MMU::set_cartridge(Cartridge* cartridge_)
{
    cartridge = cartridge_;
}
