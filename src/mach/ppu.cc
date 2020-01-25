#include "ppu.hh"

#include "bitmanip.hh"
#include <cassert>
#include <iostream>
using namespace std;

PPU::PPU(MMU* mmu_, IRC* irc_) :
    irc(irc_),
    mmu(mmu_)
{    
    renderer = new Renderer(mmu);

    status.mode_task_complete = false;
    status.frame_ready = false;
    status.unemulated_cpu_cycles = 0;
    status.cpu_cycles_spent_in_mode = 0;
    status.current_mode = ScanningOAM;
}

PPU::~PPU()
{
    delete renderer;
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
    return mmu->hff46_dma;
}

memaddr_t PPU::dma_src_address()
{
    return mmu->hff46_dma * 0x100;
}

void PPU::launch_dma(memaddr_t src_address)
{
    mmu->hff46_dma = 0x00;
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
            return mmu->hff44_ly < 144 ? Mode::ScanningOAM : Mode::VBlanking;
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
            if (get_bit(&mmu->hff41_stat, MMU::OAMInterrupt))
            {
                irc->request_interrupt(IRC::LcdStatInterrupt);
            }

            ++(mmu->hff44_ly);
            if (get_bit(&mmu->hff41_stat, MMU::LYCInterrupt)
                && mmu->hff44_ly == mmu->hff45_lyc)
            {
                set_bit(&mmu->hff41_stat, MMU::CoincidenceFlag);
                irc->request_interrupt(IRC::LcdStatInterrupt);
            }
            else
            {
                clear_bit(&mmu->hff41_stat, MMU::CoincidenceFlag);
            }
            break;

        case Mode::DrawingLine:
            break;

        case Mode::HBlanking:
            if (get_bit(&mmu->hff41_stat, MMU::HBlankInterrupt))
            {
                irc->request_interrupt(IRC::LcdStatInterrupt);
            }
            break;

        case Mode::VBlanking:
            if (get_bit(&mmu->hff41_stat, MMU::VBlankInterrupt))
            {
                irc->request_interrupt(IRC::VBlankInterrupt);
            }
            break;
    }

    mmu->hff41_stat &= ~(status.current_mode & MODE_FLAG_MASK);
    mmu->hff41_stat |= status.current_mode & MODE_FLAG_MASK;
    status.mode_task_complete = false;
    status.frame_ready = false;
    status.current_mode = mode;
}
