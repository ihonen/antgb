#include "ppu.hh"

#include "../util/bitmanip.hh"
#include <cassert>
#include <iostream>
#include <thread>

using namespace std;

Ppu::Ppu(Memory* mmu_, InterruptController* irc_) :
    irc(irc_),
    mem(mmu_)
{    
    renderer = new Renderer(mem);
    hard_reset();
}

Ppu::~Ppu()
{
    delete renderer;
}

void Ppu::hard_reset()
{
    status.mode_task_complete = false;
    status.frame_ready = false;
    status.unemulated_cpu_cycles = 0;
    status.cpu_cycles_spent_in_mode = 0;
    status.current_mode = ScanningOAM;
}

void Ppu::emulate(uint64_t cpu_cycles)
{
    status.unemulated_cpu_cycles += cpu_cycles;

    if (has_dma_request())
    {
        launch_dma(dma_src_address());
    }

    emulate_current_mode(cpu_cycles);
    if (mode_ending())
    {
        if (status.current_mode == Mode::VBlanking)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        transition_to_mode(get_next_mode());
    }
}

void Ppu::emulate_current_mode(uint64_t cpu_cycles)
{
    switch (status.current_mode)
    {
        case Mode::ScanningOAM:
            emulate_mode2(cpu_cycles);
            break;

        case Mode::DrawingLine:
            emulate_mode3(cpu_cycles);
            break;

        case Mode::HBlanking:
            emulate_mode0(cpu_cycles);
            break;

        case Mode::VBlanking:
            emulate_mode1(cpu_cycles);
            break;
    }

    return;
}

void Ppu::emulate_mode0(uint64_t cpu_cycles)
{
    status.cpu_cycles_spent_in_mode += cpu_cycles;
}

void Ppu::emulate_mode1(uint64_t cpu_cycles)
{
    while (status.unemulated_cpu_cycles >= CPU_CYCLES_PER_LY_INCREMENT)
    {
        status.cpu_cycles_spent_in_mode += CPU_CYCLES_PER_LY_INCREMENT;
        status.unemulated_cpu_cycles -= CPU_CYCLES_PER_LY_INCREMENT;

        ++mem->hff44_ly;
        if (get_bit(&mem->hff41_stat, Memory::LYCInterrupt)
            && mem->hff44_ly == mem->hff45_lyc)
        {
            set_bit(&mem->hff41_stat, Memory::CoincidenceFlag);
            irc->request_interrupt(InterruptController::LcdStatInterrupt);
        }
    }
}

void Ppu::emulate_mode2(uint64_t cpu_cycles)
{
    status.cpu_cycles_spent_in_mode += cpu_cycles;
}

void Ppu::emulate_mode3(uint64_t cpu_cycles)
{
    status.cpu_cycles_spent_in_mode += cpu_cycles;
}

bool Ppu::has_dma_request()
{
    return mem->hff46_dma;
}

memaddr_t Ppu::dma_src_address()
{
    return mem->hff46_dma * 0x100;
}

void Ppu::launch_dma(memaddr_t src_address)
{
    mem->hff46_dma = 0x00;
    mem->launch_oam_dma(0xFE00, src_address, 160);
}

bool Ppu::mode_ending()
{
    return status.cpu_cycles_spent_in_mode >= MODE_DURATION[status.current_mode];
}

Ppu::Mode Ppu::get_next_mode()
{
    switch (status.current_mode)
    {
        case Mode::ScanningOAM:
            return Mode::DrawingLine;
        case Mode::DrawingLine:
            return Mode::HBlanking;
        case Mode::HBlanking:
            return mem->hff44_ly < 143 ? Mode::ScanningOAM : Mode::VBlanking;
        case Mode::VBlanking:
            return Mode::ScanningOAM;
    }
}

void Ppu::transition_to_mode(Ppu::Mode mode)
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
            if (get_bit(&mem->hff41_stat, Memory::OAMInterrupt))
            {
                irc->request_interrupt(InterruptController::LcdStatInterrupt);
            }

            if (mem->hff44_ly >= 144)
            {
                mem->hff44_ly = 0;
            }
            else
            {
                ++mem->hff44_ly;
            }

            if (get_bit(&mem->hff41_stat, Memory::LYCInterrupt)
                && mem->hff44_ly == mem->hff45_lyc)
            {
                set_bit(&mem->hff41_stat, Memory::CoincidenceFlag);
                irc->request_interrupt(InterruptController::LcdStatInterrupt);
            }
            else
            {
                clear_bit(&mem->hff41_stat, Memory::CoincidenceFlag);
            }

            status.frame_ready = false;
            break;

        case Mode::DrawingLine:
            break;

        case Mode::HBlanking:
            if (get_bit(&mem->hff41_stat, Memory::HBlankInterrupt))
            {
                irc->request_interrupt(InterruptController::LcdStatInterrupt);
            }
            break;

        case Mode::VBlanking:
            renderer->render_frame();
            ++mem->hff44_ly;
            if (get_bit(&mem->hff41_stat, Memory::VBlankInterrupt))
            {
                irc->request_interrupt(InterruptController::VBlankInterrupt);
            }
            break;
    }

    mem->hff41_stat &= ~(status.current_mode & MODE_FLAG_MASK);
    mem->hff41_stat |= status.current_mode & MODE_FLAG_MASK;
    status.mode_task_complete = false;
    status.current_mode = mode;
}
