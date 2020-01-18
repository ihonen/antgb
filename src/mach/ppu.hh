#ifndef PPU_HH
#define PPU_HH

#include "cpu.hh"
#include "irc.hh"
#include "mmu.hh"
#include "sprite.hh"
#include <chrono>
#include <map>
#include <vector>

using std::vector;

class PPU
{
public:

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
        LYCInterrupt = 6,
        OAMInterrupt = 5,
        VBlankInterrupt = 4,
        HBlankInterrupt = 3,
        CoincidenceFlag = 2,
        ModeFlag1 = 1,
        ModeFlag0 = 0
    };

    enum Mode
    {
        HBlanking = 0x00,
        VBlanking = 0x01,
        DrawingLine = 0x02,
        ScanningOAM = 0x03
    };

    const uint64_t CPU_CYCLES_PER_HBLANK_MODE = 85; // Minimum
    const uint64_t CPU_CYCLES_PER_VBLANK_MODE = 4560;
    const uint64_t CPU_CYCLES_PER_DRAWING_MODE = 291; // Maximum
    const uint64_t CPU_CYCLES_PER_OAM_SCAN_MODE = 80;

    std::map<Mode, uint64_t> MODE_DURATION
    {
        {HBlanking, 85},
        {VBlanking, 4560},
        {DrawingLine, 291},
        {ScanningOAM, 80}
    };

    struct
    {
        bool mode_task_complete;
        bool frame_ready;
        uint64_t cpu_cycles_spent_in_mode;
        Mode current_mode;
        uint64_t unemulated_cpu_cycles;
    } status;

    const uint8_t MODE_FLAG_MASK = 0x03; // Bits 0-1

    PPU(MMU& mmu, IRC& irc);
    void emulate(uint64_t cpu_cycles);
    bool has_dma_request();
    memaddr_t dma_src_address();
    void launch_dma(memaddr_t src_address);
    void process_mode();
    bool mode_ending();
    void next_mode();
    void scan_oam();
    vector<vector<uint8_t>> read_tile(void* address);

    IRC& irc;
    MMU& mmu;

    memaddr_t SPRITE_ATTRIBUTE_TABLE_ADDRESS = 0xFE00;
    memaddr_t SPRITE_ATTRIBUTE_TABLE_SIZE_B = 160;

    memaddr_t LCDC_ADDRESS = 0xFF40;
    memaddr_t STAT_ADDRESS = 0xFF41;
    memaddr_t SCY_ADDRESS = 0xFF42;
    memaddr_t LY_ADDRESS = 0xFF44;
    memaddr_t LYC_ADDRESS = 0xFF45;
    memaddr_t WY_ADDRESS = 0xFF4A;
    memaddr_t BGP_ADDRESS = 0xFF47;
    memaddr_t OBP0_ADDRESS = 0xFF48;
    memaddr_t OBP1_ADDRESS = 0xFF49;
    memaddr_t DMA_ADDRESS = 0xFF46;

    uint8_t* lcdc;
    uint8_t* stat;
    uint8_t* scy;
    uint8_t* ly;
    uint8_t* lyc;
    uint8_t* wy;
    uint8_t* bgp;
    uint8_t* obp0;
    uint8_t* obp1;
    uint8_t* dma;

    vector<SpriteAttribute*> sprite_attribute_buffer;
    array<array<uint8_t, 144>, 160> display_buffer;

};

#endif // PPU_HH
