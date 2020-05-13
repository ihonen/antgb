#pragma once

#include "renderer.hh"
#include "cpu.hh"
#include "interrupts.hh"
#include "types.hh"
#include <chrono>
#include <map>
#include <vector>

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

    Ppu(Memory* mem, Irc* irc);
    ~Ppu();
    void hard_reset();
    void step(uint64_t cpu_cycles);
    void emulate_current_mode(uint64_t cpu_cycles);
    void emulate_mode0(uint64_t cpu_cycles);
    void emulate_mode1(uint64_t cpu_cycles);
    void emulate_mode2(uint64_t cpu_cycles);
    void emulate_mode3(uint64_t cpu_cycles);
    bool has_dma_request();
    memaddr_t dma_src_address();
    void launch_dma(memaddr_t src_address);
    bool mode_ending();
    Mode get_next_mode();
    void transition_to_mode(Mode mode);
    void scan_oam();
    vector<vector<uint8_t>> read_tile(void* address);

    Irc* irc;
    Memory* mem;
    Renderer* renderer;
};
