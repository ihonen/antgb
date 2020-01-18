#include "ppu.hh"

#include "bitmanip.hh"
#include <cassert>
#include <iostream>
using namespace std;

PPU::PPU(MMU& mmu_, IRC& irc_) :
    irc(irc_),
    mmu(mmu_)
{
    lcdc = &mmu.mem[LCDC_ADDRESS];
    stat = &mmu.mem[STAT_ADDRESS];
    scy = &mmu.mem[SCY_ADDRESS];
    ly = &mmu.mem[LY_ADDRESS];
    lyc = &mmu.mem[LYC_ADDRESS];
    wy = &mmu.mem[WY_ADDRESS];
    bgp = &mmu.mem[BGP_ADDRESS];
    obp0 = &mmu.mem[OBP0_ADDRESS];
    obp1 = &mmu.mem[OBP1_ADDRESS];
    dma = &mmu.mem[DMA_ADDRESS];

    status.mode_task_complete = false;
    status.frame_ready = false;
    status.unemulated_cpu_cycles = 0;
    status.cpu_cycles_spent_in_mode = 0;
    status.current_mode = ScanningOAM;
    memset(display_buffer.data(), 0x00, 144 * 160);
}

void PPU::emulate(uint64_t cpu_cycles)
{
    status.unemulated_cpu_cycles += cpu_cycles;

    if (has_dma_request())
    {
        launch_dma(dma_src_address());
    }

    process_mode();
    status.unemulated_cpu_cycles -= cpu_cycles;
    status.cpu_cycles_spent_in_mode += cpu_cycles;
    if (mode_ending())
    {
        next_mode();
    }
}

bool PPU::has_dma_request()
{
    return *dma != 0x00;
}

memaddr_t PPU::dma_src_address()
{
    return *dma * 0x100;
}

void PPU::launch_dma(memaddr_t src_address)
{
    cout << "DMA" << endl;
    *dma = 0x00;
    mmu.launch_oam_dma(0xFE00, src_address, 160);
}

void PPU::process_mode()
{
    switch (status.current_mode)
    {
        case Mode::ScanningOAM:
            scan_oam();
            break;
        case Mode::DrawingLine:
            break;
        case Mode::HBlanking:
            break;
        case Mode::VBlanking:
            break;
    }

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
        status.cpu_cycles_spent_in_mode -= MODE_DURATION[status.current_mode];
        status.unemulated_cpu_cycles = status.cpu_cycles_spent_in_mode;
    }
    else
    {
        status.cpu_cycles_spent_in_mode = 0;
    }

    // TODO: Only interrupt when moving from "no interrupt condition"
    // to "any interrupt condition".
    switch (status.current_mode)
    {
        case Mode::ScanningOAM:
            status.current_mode = Mode::DrawingLine;
            ++(*ly);
            if (get_bit(stat, LYCInterrupt) && *ly == *lyc)
            {
                set_bit(stat, CoincidenceFlag);
                irc.request_interrupt(IRC::LcdStatInterrupt);
            }
            else
            {
                clear_bit(stat, CoincidenceFlag);
            }
            break;

        case Mode::DrawingLine:
            status.current_mode = Mode::HBlanking;
            if (get_bit(stat, HBlankInterrupt))
            {
                irc.request_interrupt(IRC::LcdStatInterrupt);
            }
            break;

        case Mode::HBlanking:
            status.current_mode = Mode::VBlanking;
            if (get_bit(stat, VBlankInterrupt))
            {
                irc.request_interrupt(IRC::VBlankInterrupt);
            }
            break;

        case Mode::VBlanking:
            status.current_mode = Mode::ScanningOAM;
            if (get_bit(stat, OAMInterrupt))
            {
                irc.request_interrupt(IRC::LcdStatInterrupt);
            }
            break;
    }

    *stat |= status.current_mode & MODE_FLAG_MASK;

    status.mode_task_complete = false;
    status.frame_ready = false;
    status.cpu_cycles_spent_in_mode = 0;
}

void PPU::scan_oam()
{
    if (status.mode_task_complete) return;

    status.mode_task_complete;
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
