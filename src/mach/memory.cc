#include "memory.hh"

#include <iostream>
#include <cstring>

Memory::Memory()
{
    hard_reset();
    cartridge = nullptr;
    clear_dma_status();
}

void Memory::hard_reset()
{
    memset(h8000_vram, 0x00, VRAM.size);
    memset(hc000_wram0, 0x00, WRAM0.size);
    memset(hd000_wram1, 0x00, WRAM1.size);
    memset(hfe00_oam, 0x00, OAM.size);
    memset(hff00_io, 0x00, IO.size);
    memset(hff80_hram, 0x00, HRAM.size);
    memset(hffff_ie, 0x00, IE.size);

    hff05_tima = 0x00;
    hff06_tma = 0x00;
    hff07_tac = 0x00;
    hff10_nr10 = 0x80;
    hff11_nr11 = 0xBF;
    hff12_nr12 = 0xF3;
    hff14_nr14 = 0xBF;
    hff16_nr21 = 0x3F;
    hff17_nr22 = 0x00;
    hff19_nr24 = 0xBF;
    hff1a_nr30 = 0x7F;
    hff1b_nr31 = 0xFF;
    hff1c_nr32 = 0x9F;

    // TODO: Check if correct
    hff1e_nr34 = 0x9F;

    hff20_nr_41 = 0xFF;
    hff21_nr_42 = 0x00;
    hff22_nr_43 = 0x00;
    hff23_nr_44 = 0xBF;
    hff24_nr_50 = 0x77;
    hff25_nr_51 = 0xF3;
    hff26_nr_52 = 0xF1;
    hff40_lcdc = 0x91;
    hff42_scy = 0x00;
    hff43_scx = 0x00;
    hff45_lyc = 0x00;
    hff47_bgp = 0xFC;
    hff48_obp0 = 0xFF;
    hff49_obp1 = 0xFF;
    hff4a_wy = 0x00;
    hff4b_wx = 0x00;
    *hffff_ie = 0x00;
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
    if (source)
    {
        return *source;
    }
    return 0xFF;
}

bool Memory::write(memaddr_t address, uint8_t value)
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
