#include "ppu.hh"

#include <cassert>
#include <iostream>
using namespace std;

PPU::PPU(MMU& mmu_, IRC& irc_) :
    mmu(mmu_),
    irc(irc_)
{
    uint8_t* lcdc = &mmu.mem[LCDC_ADDRESS];
    uint8_t* stat = &mmu.mem[STAT_ADDRESS];
    uint8_t* scy = &mmu.mem[SCY_ADDRESS];
    uint8_t* ly = &mmu.mem[LY_ADDRESS];
    uint8_t* lyc = &mmu.mem[LYC_ADDRESS];
    uint8_t* wy = &mmu.mem[WY_ADDRESS];
    uint8_t* bgp = &mmu.mem[BGP_ADDRESS];
    uint8_t* obp0 = &mmu.mem[OBP0_ADDRESS];
    uint8_t* obp1 = &mmu.mem[OBP1_ADDRESS];
    uint8_t* dma = &mmu.mem[DMA_ADDRESS];

    status.unemulated_cpu_cycles = 0;
    status.cpu_cycles_spent_in_mode = 0;
    status.current_mode = ScanningOAM;
    memset(display_buffer.data(), 0x00, 144 * 160);
}

void PPU::emulate(uint64_t cpu_cycles)
{
    status.unemulated_cpu_cycles += cpu_cycles;
    status.cpu_cycles_spent_in_mode += cpu_cycles;

    process_mode();
    status.unemulated_cpu_cycles -= cpu_cycles;
    if (mode_ending())
    {
        next_mode();
    }
}

void PPU::process_mode()
{
    return;
}

bool PPU::mode_ending()
{
    return status.cpu_cycles_spent_in_mode >= MODE_DURATION[status.current_mode];
}

void PPU::next_mode()
{
    if (status.cpu_cycles_spent_in_mode > MODE_DURATION[status.current_mode])
    {
        //cout << status.cpu_cycles_spent_in_mode << endl;
        status.cpu_cycles_spent_in_mode -= MODE_DURATION[status.current_mode];
    }
    else
    {
        status.cpu_cycles_spent_in_mode = 0;
    }

    switch (status.current_mode)
    {
        case Mode::ScanningOAM:
            status.current_mode = Mode::DrawingLine;
            break;
        case Mode::DrawingLine:
            status.current_mode = Mode::HBlanking;
            break;
        case Mode::HBlanking:
            status.current_mode = Mode::VBlanking;
            break;
        case Mode::VBlanking:
            irc.request_interrupt(IRC::VBlankInterrupt);
            status.current_mode = Mode::ScanningOAM;
            break;
    }
}

vector<vector<uint8_t>> PPU::read_tile(void* address_)
{
    vector<vector<uint8_t>> pixels;
    uint8_t* tile_start = (uint8_t*)address_;

    for (uint8_t row = 0; row < 8; ++row)
    {
        pixels.push_back({});
        const uint8_t& lower_byte = tile_start[row + 1];
        const uint8_t& upper_byte = tile_start[row];

        for (int8_t column = 7; column >= 0; --column)
        {
            // Bit 0 from lower byte, bit 1 from upper byte:
            uint8_t lower_bit = (upper_byte & (0x01 << column))
                                  >> column;
            uint8_t upper_bit = (lower_byte & (0x01 << column))
                                  >> column;
            uint8_t pixel_value = (upper_bit << 1) | lower_bit;
            pixels.at(row).push_back(pixel_value);
        }
    }

    return pixels;
}
