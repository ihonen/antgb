#pragma once

#include "bitmanip.hh"
#include "renderer.hh"
#include "cpu.hh"
#include "interrupts.hh"
#include "types.hh"
#include <chrono>
#include <map>
#include <vector>

namespace antgb
{

using std::vector;

class Ppu
{
public:
    struct Registers
    {
        uint8_t lcdc;
        uint8_t stat;
        uint8_t scy;
        uint8_t scx;
        uint8_t ly;
        uint8_t lyc;
        uint8_t dma;
        uint8_t bgp;
        uint8_t obp0;
        uint8_t obp1;
        uint8_t wy;
        uint8_t wx;
    };

    static constexpr memaddr_t LCDC_ADDRESS = 0xFF40;
    static constexpr memaddr_t STAT_ADDRESS = 0xFF41;
    static constexpr memaddr_t SCY_ADDRESS = 0xFF42;
    static constexpr memaddr_t SCX_ADDRESS = 0xFF43;
    static constexpr memaddr_t LY_ADDRESS = 0xFF44;
    static constexpr memaddr_t LYC_ADDRESS = 0xFF45;
    static constexpr memaddr_t DMA_ADDRESS = 0xFF46;
    static constexpr memaddr_t BGP_ADDRESS = 0xFF47;
    static constexpr memaddr_t OBP0_ADDRESS = 0xFF48;
    static constexpr memaddr_t OBP1_ADDRESS = 0xFF49;
    static constexpr memaddr_t WY_ADDRESS = 0xFF4A;
    static constexpr memaddr_t WX_ADDRESS = 0xFF4B;
    static constexpr memaddr_t LOW_ADDRESS = LCDC_ADDRESS;
    static constexpr memaddr_t HIGH_ADDRESS = WX_ADDRESS;

    enum BitPos
    {
        // LCDC
        BgAndWindowDisplayEnable = 0,
        ObjDisplayEnable = 1,
        ObjSize = 2,
        BgTileMapDisplaySelect = 3,
        BgAndWindowTileDataSelect = 4,
        WindowDisplayEnable = 5,
        WindowTileMapDisplaySelect = 6,
        DisplayEnable = 7,

        // STAT
        LycInt = 6,
        OamInt = 5,
        VBlankInterrupt = 4,
        HBlankInterrupt = 3,
        LycCoincidence = 2,
        ModeFlag1 = 1,
        ModeFlag0 = 0
    };

    enum Mode
    {
        Hblank = 0x00,
        Vblank = 0x01,
        LineScan = 0x02,
        OamScan = 0x03
    };

    static constexpr uint64_t CPU_CYCLES_PER_LY_INCREMENT = 456;

    static constexpr uint64_t CPU_CYCLES_PER_HBLANK_MODE = 85; // Minimum
    static constexpr uint64_t CPU_CYCLES_PER_VBLANK_MODE = 4560;
    static constexpr uint64_t CPU_CYCLES_PER_DRAWING_MODE = 291; // Maximum
    static constexpr uint64_t CPU_CYCLES_PER_OAM_SCAN_MODE = 80;

    Registers* reg = nullptr;

    std::map<Mode, uint64_t> MODE_DURATION
    {
        {Hblank, 85},
        {Vblank, 4560},
        {LineScan, 291},
        {OamScan, 80}
    };

    bool mode_task_complete;
    bool frame_ready;
    uint64_t cpu_cycles_spent_in_mode;
    Mode current_mode;
    uint64_t unemulated_cpu_cycles;

    uint64_t cpu_cycles_left_in_mode;
    uint64_t cpu_cycles_until_ly;

    uint64_t clocksum = 0;
    uint64_t scanline = 0;

    const uint8_t MODE_FLAG_MASK = 0x03; // Bits 0-1

    Ppu(Memory* mem, Registers* reg, Irc* irc);
    ~Ppu();
    void hard_reset();
    inline void step(uint64_t cpu_cycles);
    inline bool has_dma_request();
    inline memaddr_t dma_src_address();
    void launch_dma(memaddr_t src_address);

    Irc* irc;
    Memory* mem;
    Renderer* renderer;
};

ANTDBG_ALWAYS_INLINE void Ppu::step(uint64_t cpu_cycles)
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
        reg->stat &= ~MODE_FLAG_MASK;
        reg->stat |= current_mode & MODE_FLAG_MASK;
        reg->ly = (total_cycles / 456) % 154;

        // Check or clear LYC interrupt condition.
        if (reg->ly == reg->lyc
            && get_bit(&reg->stat, Ppu::LycInt))
        {
            set_bit(&reg->stat, Ppu::LycCoincidence);
            cerr << "sent LCD STAT IRQ" << endl;
            irc->request_interrupt(Irc::LcdStatInt);
        }
        else clear_bit(&reg->stat, Ppu::LycCoincidence);

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
                    if (get_bit(&reg->stat, HBlankInterrupt))
                        irc->request_interrupt(Irc::LcdStatInt);
                }
                else stop = true;
                break;

            case Hblank:
            {
                if (clocksum >= 85)
                {
                    clocksum -= 85;
                    if (reg->ly < 144)
                    {
                        current_mode = OamScan;
                        // OAM interrupt
                        if (get_bit(&reg->stat, OamInt))
                            irc->request_interrupt(Irc::LcdStatInt);
                    }
                    else
                    {
                        current_mode = Vblank;
                        renderer->render_frame();
                        irc->request_interrupt(Irc::VBlankInterrupt);
                        if (get_bit(&reg->stat, VBlankInterrupt))
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
                    if (get_bit(&reg->stat, OamInt))
                        irc->request_interrupt(Irc::LcdStatInt);

                    //cerr << "vblank end @ " << total_cycles - clocksum << endl;

                    total_cycles = clocksum;

                    auto start = chrono::high_resolution_clock::now();
                    while ((chrono::high_resolution_clock::now() - start) / chrono::milliseconds(1) < 10);

                }
                else stop = true;
                break;
        }
    }

    return;
}

ANTDBG_ALWAYS_INLINE bool Ppu::has_dma_request()
{
    return reg->dma;
}

ANTDBG_ALWAYS_INLINE memaddr_t Ppu::dma_src_address()
{
    return reg->dma * 0x100;
}

} // namespace antgb
