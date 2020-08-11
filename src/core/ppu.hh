#pragma once

#include "bits.hh"
#include "cpu.hh"
#include "types.hh"
#include <chrono>
#include <map>
#include <vector>

namespace antgb
{
using namespace std;

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

    enum BitPos
    {
        // LCDC
        BgAndWindowDisplayEnable   = 0,
        ObjDisplayEnable           = 1,
        ObjSize                    = 2,
        BgTileMapDisplaySelect     = 3,
        BgAndWindowTileDataSelect  = 4,
        WindowDisplayEnable        = 5,
        WindowTileMapDisplaySelect = 6,
        DisplayEnable              = 7,

        // STAT
        LycInt          = 6,
        OamInt          = 5,
        VBlankInterrupt = 4,
        HBlankInterrupt = 3,
        LycCoincidence  = 2,
        ModeFlag1       = 1,
        ModeFlag0       = 0
    };

    enum Mode
    {
        Hblank   = 0x00,
        Vblank   = 0x01,
        LineScan = 0x02,
        OamScan  = 0x03
    };

    static constexpr uint64_t CPU_CYCLES_PER_LY_INCREMENT = 456;

    static constexpr uint64_t CPU_CYCLES_PER_HBLANK_MODE   = 85; // Minimum
    static constexpr uint64_t CPU_CYCLES_PER_VBLANK_MODE   = 4560;
    static constexpr uint64_t CPU_CYCLES_PER_DRAWING_MODE  = 291; // Maximum
    static constexpr uint64_t CPU_CYCLES_PER_OAM_SCAN_MODE = 80;

    Registers* reg = nullptr;

    std::map<Mode, uint64_t> MODE_DURATION{{Hblank, 85},
                                           {Vblank, 4560},
                                           {LineScan, 291},
                                           {OamScan, 80}};

    bool     mode_task_complete;
    bool     frame_ready;
    uint64_t cpu_cycles_spent_in_mode;
    Mode     current_mode;
    uint64_t unemulated_cpu_cycles;

    uint64_t cpu_cycles_left_in_mode;
    uint64_t cpu_cycles_until_ly;

    uint64_t clocksum = 0;
    uint64_t scanline = 0;

    const uint8_t MODE_FLAG_MASK = 0x03; // Bits 0-1

    Ppu(Mmu* mem, Registers* reg, Cpu* cpu, iRenderer* renderer = nullptr);
    ~Ppu();
    void          set_renderer(iRenderer* renderer);
    void          hard_reset();
    void          step(uint64_t cpu_cycles);
    bool          has_dma_request();
    addr_t        dma_src_address();
    void          launch_dma(addr_t src_address);

    Cpu*      cpu;
    Mmu*      mem;
    Renderer* renderer;
};

} // namespace antgb
