#include "ppu.hh"

#include "bitmanip.hh"
#include <cassert>
#include <iostream>
using namespace std;

PPU::PPU(MMU& mmu_, IRC& irc_) :
    irc(irc_),
    mmu(mmu_)
{
    reg.lcdc = &mmu.mem[reg.LCDC_ADDRESS];
    reg.stat = &mmu.mem[reg.STAT_ADDRESS];
    reg.scy = &mmu.mem[reg.SCY_ADDRESS];
    reg.scx = &mmu.mem[reg.SCX_ADDRESS];
    reg.ly = &mmu.mem[reg.LY_ADDRESS];
    reg.lyc = &mmu.mem[reg.LYC_ADDRESS];
    reg.wy = &mmu.mem[reg.WY_ADDRESS];
    reg.bgp = &mmu.mem[reg.BGP_ADDRESS];
    reg.obp0 = &mmu.mem[reg.OBP0_ADDRESS];
    reg.obp1 = &mmu.mem[reg.OBP1_ADDRESS];
    reg.dma = &mmu.mem[reg.DMA_ADDRESS];

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
    return *reg.dma != 0x00;
}

memaddr_t PPU::dma_src_address()
{
    return *reg.dma * 0x100;
}

void PPU::launch_dma(memaddr_t src_address)
{
    cout << "DMA" << endl;
    *reg.dma = 0x00;
    /*
    mmu.launch_oam_dma(0xFE00, src_address, 160);
    */
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
            ++(*reg.ly);
            if (get_bit(reg.stat, reg.LYCInterrupt) && *reg.ly == *reg.lyc)
            {
                set_bit(reg.stat, reg.CoincidenceFlag);
                irc.request_interrupt(IRC::LcdStatInterrupt);
            }
            else
            {
                clear_bit(reg.stat, reg.CoincidenceFlag);
            }
            break;

        case Mode::DrawingLine:
            status.current_mode = Mode::HBlanking;
            if (get_bit(reg.stat, reg.HBlankInterrupt))
            {
                irc.request_interrupt(IRC::LcdStatInterrupt);
            }
            break;

        case Mode::HBlanking:
            status.current_mode = Mode::VBlanking;
            if (get_bit(reg.stat, reg.VBlankInterrupt))
            {
                irc.request_interrupt(IRC::VBlankInterrupt);
            }
            break;

        case Mode::VBlanking:
            status.current_mode = Mode::ScanningOAM;
            if (get_bit(reg.stat, reg.OAMInterrupt))
            {
                irc.request_interrupt(IRC::LcdStatInterrupt);
            }
            break;
    }

    *reg.stat |= status.current_mode & MODE_FLAG_MASK;

    status.mode_task_complete = false;
    status.frame_ready = false;
    status.cpu_cycles_spent_in_mode = 0;
}

void PPU::scan_oam()
{
    if (status.mode_task_complete) return;

    sprite_buffer.clear();
    sprite_buffer.reserve(10);
/*
    auto sprite = (SpriteAttribute*)&(mmu.mem[SPRITE_ATTRIBUTE_TABLE_ADDRESS]);

    for (int i = 0; i < SPRITE_ATTRIBUTE_TABLE_SIZE_B; ++i)
    {
        auto sprite_height = *lcdc & ObjSize ? 16 : 8;
        if (sprite->y_pos - sprite_height >= *ly)
        {
            sprite_buffer.push_back(sprite);
        }
        ++sprite;
    }
*/
    status.mode_task_complete = true;
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
