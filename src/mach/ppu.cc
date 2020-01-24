#include "ppu.hh"

#include "bitmanip.hh"
#include <cassert>
#include <iostream>
using namespace std;

PPU::PPU(MMU* mmu_, IRC* irc_) :
    irc(irc_),
    mmu(mmu_)
{    
    reg = new PPUReg;
    renderer = new Renderer(reg, mmu->mem.data);

    reg->lcdc = &mmu->mem[reg->LCDC_ADDRESS];
    reg->stat = &mmu->mem[reg->STAT_ADDRESS];
    reg->scy = &mmu->mem[reg->SCY_ADDRESS];
    reg->scx = &mmu->mem[reg->SCX_ADDRESS];
    reg->ly = &mmu->mem[reg->LY_ADDRESS];
    reg->lyc = &mmu->mem[reg->LYC_ADDRESS];
    reg->wy = &mmu->mem[reg->WY_ADDRESS];
    reg->bgp = &mmu->mem[reg->BGP_ADDRESS];
    reg->obp0 = &mmu->mem[reg->OBP0_ADDRESS];
    reg->obp1 = &mmu->mem[reg->OBP1_ADDRESS];
    reg->dma = &mmu->mem[reg->DMA_ADDRESS];

    status.mode_task_complete = false;
    status.frame_ready = false;
    status.unemulated_cpu_cycles = 0;
    status.cpu_cycles_spent_in_mode = 0;
    status.current_mode = ScanningOAM;
}

PPU::~PPU()
{
    delete renderer;
    delete reg;
}

void PPU::emulate(uint64_t cpu_cycles)
{
    status.unemulated_cpu_cycles += cpu_cycles;

    if (has_dma_request())
    {
        launch_dma(dma_src_address());
    }

    emulate_current_mode();
    status.unemulated_cpu_cycles -= cpu_cycles;
    status.cpu_cycles_spent_in_mode += cpu_cycles;
    if (mode_ending())
    {
        transition_to_mode(next_mode());
    }
}

void PPU::emulate_mode0()
{

}

void PPU::emulate_mode1()
{

}

void PPU::emulate_mode2()
{

}

void PPU::emulate_mode3()
{

}

bool PPU::has_dma_request()
{
    return *reg->dma != 0x00;
}

memaddr_t PPU::dma_src_address()
{
    return *reg->dma * 0x100;
}

void PPU::launch_dma(memaddr_t src_address)
{
    *reg->dma = 0x00;
    mmu->launch_oam_dma(0xFE00, src_address, 160);
}

void PPU::emulate_current_mode()
{
    switch (status.current_mode)
    {
        case Mode::ScanningOAM:
            emulate_mode2();
            break;
        case Mode::DrawingLine:
            emulate_mode3();
            break;
        case Mode::HBlanking:
            emulate_mode0();
            break;
        case Mode::VBlanking:
            emulate_mode1();
            break;
    }

    return;
}

bool PPU::mode_ending()
{
    return status.cpu_cycles_spent_in_mode >= MODE_DURATION[status.current_mode];
}

PPU::Mode PPU::next_mode()
{
    switch (status.current_mode)
    {
        case Mode::ScanningOAM:
            return Mode::DrawingLine;
        case Mode::DrawingLine:
            return Mode::HBlanking;
        case Mode::HBlanking:
            return *reg->ly < 144 ? Mode::ScanningOAM : Mode::VBlanking;
        case Mode::VBlanking:
            return Mode::ScanningOAM;
    }
}

void PPU::transition_to_mode(PPU::Mode mode)
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

    switch (mode)
    {
        case Mode::ScanningOAM:
            if (get_bit(reg->stat, reg->OAMInterrupt))
            {
                irc->request_interrupt(IRC::LcdStatInterrupt);
            }

            ++(*reg->ly);
            if (get_bit(reg->stat, reg->LYCInterrupt)
                && *reg->ly == *reg->lyc)
            {
                set_bit(reg->stat, reg->CoincidenceFlag);
                irc->request_interrupt(IRC::LcdStatInterrupt);
            }
            else
            {
                clear_bit(reg->stat, reg->CoincidenceFlag);
            }
            break;

        case Mode::DrawingLine:
            break;

        case Mode::HBlanking:
            if (get_bit(reg->stat, reg->HBlankInterrupt))
            {
                irc->request_interrupt(IRC::LcdStatInterrupt);
            }
            break;

        case Mode::VBlanking:
            if (get_bit(reg->stat, reg->VBlankInterrupt))
            {
                irc->request_interrupt(IRC::VBlankInterrupt);
            }
            break;
    }

    *reg->stat &= ~(status.current_mode & MODE_FLAG_MASK);
    *reg->stat |= status.current_mode & MODE_FLAG_MASK;
    status.mode_task_complete = false;
    status.frame_ready = false;
    status.current_mode = mode;
}
