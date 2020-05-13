#include "ppu.hh"

#include "bitmanip.hh"
#include "memory.hh"
#include <cassert>
#include <iostream>
#include <thread>

using namespace std;

Ppu::Ppu(Memory* mmu_, Irc* irc_) :
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
    frame_ready = false;
    cpu_cycles_spent_in_mode = CPU_CYCLES_PER_VBLANK_MODE - 60;
    unemulated_cpu_cycles = 0;
    // BGB:
    // Cycles until mode changes: 60
    // Cycles until the first LY update: 96.
    cpu_cycles_left_in_mode = 60;
    cpu_cycles_until_ly = 96;

    current_mode = OamScan;
    clocksum = 0;
    scanline = 0;
}

void Ppu::step(uint64_t cpu_cycles)
{
    clocksum += cpu_cycles;

    static uint64_t total_cycles = 0;
    total_cycles += cpu_cycles;

    bool stop = false;

    // Launch DMA if a request has been made.
    if (has_dma_request()) launch_dma(dma_src_address());

    /*

    mem->hff41_stat &= ~(current_mode & MODE_FLAG_MASK);
    mem->hff41_stat |= current_mode & MODE_FLAG_MASK;
    mode_task_complete = false;

    */

    while (!stop)
    {
        // Update registers.
        mem->hff41_stat &= ~MODE_FLAG_MASK;
        mem->hff41_stat |= current_mode & MODE_FLAG_MASK;
        mem->hff44_ly = (total_cycles / 456) % 154;

        // Check or clear LYC interrupt condition.
        if (mem->hff44_ly == mem->hff45_lyc
            && get_bit(&mem->hff41_stat, Memory::LycInt))
        {
            set_bit(&mem->hff41_stat, Memory::LycCoincidence);
            cerr << "sent LCD STAT IRQ" << endl;
            irc->request_interrupt(Irc::LcdStatInt);
        }
        else clear_bit(&mem->hff41_stat, Memory::LycCoincidence);

        switch (current_mode)
        {
            case OamScan:
                if (clocksum >= 80)
                {
                    clocksum -= 80;
                    current_mode = LineScan;
                }
                else stop = true;
                break;

            case LineScan:
                if (clocksum >= 291)
                {
                    clocksum -= 291;
                    current_mode = Hblank;
                    // Hblank interrupt
                    if (get_bit(&mem->hff41_stat, Memory::HBlankInterrupt))
                        irc->request_interrupt(Irc::LcdStatInt);
                }
                else stop = true;
                break;

            case Hblank:
            {
                if (clocksum >= 85)
                {
                    clocksum -= 85;
                    if (mem->hff44_ly < 144)
                    {
                        current_mode = OamScan;
                        // OAM interrupt
                        if (get_bit(&mem->hff41_stat, Memory::OamInt))
                            irc->request_interrupt(Irc::LcdStatInt);
                    }
                    else
                    {
                        current_mode = Vblank;
                        renderer->render_frame();
                        irc->request_interrupt(Irc::VBlankInterrupt);
                        if (get_bit(&mem->hff41_stat, Memory::VBlankInterrupt))
                            irc->request_interrupt(Irc::LcdStatInt);
                    }
                }
                else stop = true;
                break;
            }

            case Vblank:
                if (clocksum >= 4560)
                {
                    //cerr << "vblank begin @ " << total_cycles - clocksum << endl;

                    clocksum -= 4560;
                    current_mode = OamScan;
                    // OAM interrupt
                    if (get_bit(&mem->hff41_stat, Memory::OamInt))
                        irc->request_interrupt(Irc::LcdStatInt);

                    //cerr << "vblank end @ " << total_cycles - clocksum << endl;

                    total_cycles = clocksum;

                    auto start = chrono::high_resolution_clock::now();
                    while ((chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1) < 17);
                }
                else stop = true;
                break;
        }
    }

    return;
}

void Ppu::emulate_mode1(uint64_t cpu_cycles)
{

}

void Ppu::emulate_mode2(uint64_t cpu_cycles)
{
    cpu_cycles_spent_in_mode += cpu_cycles;
}

void Ppu::emulate_mode3(uint64_t cpu_cycles)
{
    cpu_cycles_spent_in_mode += cpu_cycles;
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
    return cpu_cycles_spent_in_mode >= MODE_DURATION[current_mode];
}

Ppu::Mode Ppu::get_next_mode()
{
    switch (current_mode)
    {
        case Mode::OamScan:
            return Mode::LineScan;
        case Mode::LineScan:
            return Mode::Hblank;
        case Mode::Hblank:
            return mem->hff44_ly < 143 ? Mode::OamScan : Mode::Vblank;
        case Mode::Vblank:
            return Mode::OamScan;
    }
}

void Ppu::transition_to_mode(Ppu::Mode mode)
{
    if (cpu_cycles_spent_in_mode > MODE_DURATION[current_mode])
    {
        cpu_cycles_spent_in_mode -= MODE_DURATION[current_mode];
        unemulated_cpu_cycles = cpu_cycles_spent_in_mode;
    }
    else
    {
        cpu_cycles_spent_in_mode = 0;
    }

    // TODO: Only interrupt when moving from "no interrupt condition"
    // to "any interrupt condition".

    switch (mode)
    {
        case Mode::OamScan:
            if (get_bit(&mem->hff41_stat, Memory::OamInt))
            {
                irc->request_interrupt(Irc::LcdStatInt);
            }

            if (mem->hff44_ly >= 144)
            {
                mem->hff44_ly = 0;
            }
            else
            {
                ++mem->hff44_ly;
            }

            if (get_bit(&mem->hff41_stat, Memory::LycInt)
                && mem->hff44_ly == mem->hff45_lyc)
            {
                set_bit(&mem->hff41_stat, Memory::LycCoincidence);
                irc->request_interrupt(Irc::LcdStatInt);
            }
            else
            {
                clear_bit(&mem->hff41_stat, Memory::LycCoincidence);
            }

            frame_ready = false;
            break;

        case Mode::LineScan:
            break;

        case Mode::Hblank:
            if (get_bit(&mem->hff41_stat, Memory::HBlankInterrupt))
            {
                irc->request_interrupt(Irc::LcdStatInt);
            }
            break;

        case Mode::Vblank:
            renderer->render_frame();
            ++mem->hff44_ly;
            if (get_bit(&mem->hff41_stat, Memory::VBlankInterrupt))
            {
                irc->request_interrupt(Irc::VBlankInterrupt);
            }
            break;
    }

    mem->hff41_stat &= ~(current_mode & MODE_FLAG_MASK);
    mem->hff41_stat |= current_mode & MODE_FLAG_MASK;
    mode_task_complete = false;
    current_mode = mode;
}


/*

void Ppu::emulate(uint64_t cpu_cycles)
{
    unemulated_cpu_cycles += cpu_cycles;

    if (has_dma_request())
    {
        launch_dma(dma_src_address());
    }

    emulate_current_mode(cpu_cycles);
    if (mode_ending())
    {
        if (current_mode == Mode::VBlanking)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        transition_to_mode(get_next_mode());
    }
}


void Ppu::emulate_current_mode(uint64_t cpu_cycles)
{
    switch (current_mode)
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
    cpu_cycles_spent_in_mode += cpu_cycles;
}

void Ppu::emulate_mode1(uint64_t cpu_cycles)
{
    while (unemulated_cpu_cycles >= CPU_CYCLES_PER_LY_INCREMENT)
    {
        cpu_cycles_spent_in_mode += CPU_CYCLES_PER_LY_INCREMENT;
        unemulated_cpu_cycles -= CPU_CYCLES_PER_LY_INCREMENT;

        ++mem->hff44_ly;
        if (get_bit(&mem->hff41_stat, Memory::LYCInterrupt)
            && mem->hff44_ly == mem->hff45_lyc)
        {
            set_bit(&mem->hff41_stat, Memory::CoincidenceFlag);
            irc->request_interrupt(Irc::LcdStatInterrupt);
        }
    }
}

void Ppu::emulate_mode2(uint64_t cpu_cycles)
{
    cpu_cycles_spent_in_mode += cpu_cycles;
}

void Ppu::emulate_mode3(uint64_t cpu_cycles)
{
    cpu_cycles_spent_in_mode += cpu_cycles;
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
    return cpu_cycles_spent_in_mode >= MODE_DURATION[current_mode];
}

Ppu::Mode Ppu::get_next_mode()
{
    switch (current_mode)
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
    if (cpu_cycles_spent_in_mode > MODE_DURATION[current_mode])
    {
        cpu_cycles_spent_in_mode -= MODE_DURATION[current_mode];
        unemulated_cpu_cycles = cpu_cycles_spent_in_mode;
    }
    else
    {
        cpu_cycles_spent_in_mode = 0;
    }

    // TODO: Only interrupt when moving from "no interrupt condition"
    // to "any interrupt condition".

    switch (mode)
    {
        case Mode::ScanningOAM:
            if (get_bit(&mem->hff41_stat, Memory::OAMInterrupt))
            {
                irc->request_interrupt(Irc::LcdStatInterrupt);
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
                irc->request_interrupt(Irc::LcdStatInterrupt);
            }
            else
            {
                clear_bit(&mem->hff41_stat, Memory::CoincidenceFlag);
            }

            frame_ready = false;
            break;

        case Mode::DrawingLine:
            break;

        case Mode::HBlanking:
            if (get_bit(&mem->hff41_stat, Memory::HBlankInterrupt))
            {
                irc->request_interrupt(Irc::LcdStatInterrupt);
            }
            break;

        case Mode::VBlanking:
            renderer->render_frame();
            ++mem->hff44_ly;
            if (get_bit(&mem->hff41_stat, Memory::VBlankInterrupt))
            {
                irc->request_interrupt(Irc::VBlankInterrupt);
            }
            break;
    }

    mem->hff41_stat &= ~(current_mode & MODE_FLAG_MASK);
    mem->hff41_stat |= current_mode & MODE_FLAG_MASK;
    mode_task_complete = false;
    current_mode = mode;
}

*/
