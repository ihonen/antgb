#ifndef MEMORY_HH
#define MEMORY_HH

#include "cartridge.hh"
#include "../util/typedefs.hh"

using namespace std;

class Memory
{
public:

    struct MemoryRegion
    {
        memaddr_t low;
        memaddr_t high;
        memaddr_t size;
    };

    Memory();

    void hard_reset();
    uint8_t* get(memaddr_t address);
    uint8_t read(memaddr_t address);
    bool write(memaddr_t address, uint8_t value);
    bool can_read(memaddr_t address);
    bool can_write(memaddr_t address);
    void launch_oam_dma(memaddr_t destination,
                        memaddr_t source,
                        memaddr_t size);
    void emulate(uint64_t cpu_cycles);
    void emulate_oam_dma(uint64_t cpu_cycles);
    void end_oam_dma();
    void clear_dma_status();
    void set_cartridge(Cartridge* cartridge);

    Cartridge* cartridge;

    struct
    {
        uint64_t unemulated_cpu_cycles;
        uint64_t cpu_cycles_left;
        memaddr_t src_pointer;
        memaddr_t dest_pointer;
        memaddr_t src_low;
        memaddr_t src_high;
        memaddr_t size;
    } dma_status;

    // Main regions
    static constexpr MemoryRegion BOOT = {0x0000, 0x00FF, 0x0100};
    static constexpr MemoryRegion ROM0 = {0x0000, 0x3FFF, 0x4000};
    static constexpr MemoryRegion ROM1 = {0x4000, 0x7FFF, 0x4000};
    static constexpr MemoryRegion VRAM = {0x8000, 0x9FFF, 0x2000};
    static constexpr MemoryRegion ERAM = {0xA000, 0xBFFF, 0x2000};
    static constexpr MemoryRegion WRAM0 = {0xC000, 0xCFFF, 0x1000};
    static constexpr MemoryRegion WRAM1 = {0xD000, 0xDFFF, 0x1000};
    static constexpr MemoryRegion ECHO = {0xE000, 0xFDFF, 0x1E00};
    static constexpr MemoryRegion OAM = {0xFE00, 0xFE9F, 0x00A0};
    static constexpr MemoryRegion UNUSABLE = {0xFEA0, 0xFEFF, 0x0060};
    static constexpr MemoryRegion IO = {0xFF00, 0xFF7F, 0x0080};
    static constexpr MemoryRegion HRAM = {0xFF80, 0xFFFE, 0x007F};
    static constexpr MemoryRegion IE = {0xFFFF, 0xFFFF, 0x0001};

    // Subregions
    static constexpr MemoryRegion TILEDATA0 = {0x8000, 0x87FF, 0x0800};
    static constexpr MemoryRegion TILEDATA1 = {0x8800, 0x8FFF, 0x0800};
    static constexpr MemoryRegion TILEDATA2 = {0x9000, 0x97FF, 0x0800};

    memaddr_t LCDC_ADDRESS = 0xFF40;
    memaddr_t STAT_ADDRESS = 0xFF41;
    memaddr_t SCY_ADDRESS = 0xFF42;
    memaddr_t SCX_ADDRESS = 0xFF43;
    memaddr_t LY_ADDRESS = 0xFF44;
    memaddr_t LYC_ADDRESS = 0xFF45;
    memaddr_t WY_ADDRESS = 0xFF4A;
    memaddr_t WX_ADDRESS = 0xFF4B;
    memaddr_t BGP_ADDRESS = 0xFF47;
    memaddr_t OBP0_ADDRESS = 0xFF48;
    memaddr_t OBP1_ADDRESS = 0xFF49;
    memaddr_t DMA_ADDRESS = 0xFF46;

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

    union __attribute__((packed))
    {
        uint8_t h8000_vram[VRAM.size];
        struct __attribute__((packed))
        {
            uint8_t h8000_tilemap0[TILEDATA0.size];
            uint8_t h8800_tilemap1[TILEDATA1.size];
            uint8_t h8800_tilemap2[TILEDATA2.size];
        };
    };
    uint8_t hc000_wram0[WRAM0.size];
    uint8_t hd000_wram1[WRAM1.size];
    uint8_t hfe00_oam[OAM.size];
    union __attribute__((packed))
    {
        uint8_t hff00_io[IO.size];
        struct __attribute__((packed))
        {
            // Joypad
            uint8_t hff00_joyp;

            // Serial link
            uint8_t hff01_sb;
            uint8_t hff02_sc;

            uint8_t hff03_io_unused_in_dmg_1__;

            // Timer/divider
            uint8_t hff04_div;
            uint8_t hff05_tima;
            uint8_t hff06_tma;
            uint8_t hff07_tac;

            uint8_t hff08_io_unused_in_dmg_2__[7];

            // Interrupt flags
            uint8_t hff0f_if;

            // Sound controller
            uint8_t hff10_nr10;
            uint8_t hff11_nr11;
            uint8_t hff12_nr12;
            uint8_t hff13_nr13;
            uint8_t hff14_nr14;
            uint8_t hff15_io_unused_in_dmg_3__;
            uint8_t hff16_nr21;
            uint8_t hff17_nr22;
            uint8_t hff18_nr23;
            uint8_t hff19_nr24;
            uint8_t hff1a_nr30;
            uint8_t hff1b_nr31;
            uint8_t hff1c_nr32;
            uint8_t hff1d_nr33;
            uint8_t hff1e_nr34;

            uint8_t hff1f_unused_in_dmg_4__;

            uint8_t hff20_nr_41;
            uint8_t hff21_nr_42;
            uint8_t hff22_nr_43;
            uint8_t hff23_nr_44;
            uint8_t hff24_nr_50;
            uint8_t hff25_nr_51;
            uint8_t hff26_nr_52;
            uint8_t hff27_io_unused_in_dmg_5__[9];
            uint8_t hff30_wave_pattern_ram[16];

            // LCD controller
            uint8_t hff40_lcdc;
            uint8_t hff41_stat;
            uint8_t hff42_scy;
            uint8_t hff43_scx;
            uint8_t hff44_ly;
            uint8_t hff45_lyc;
            uint8_t hff46_dma;
            uint8_t hff47_bgp;
            uint8_t hff48_obp0;
            uint8_t hff49_obp1;
            uint8_t hff4a_wy;
            uint8_t hff4b_wx;
        };
    };
    uint8_t hff80_hram[HRAM.size];
    uint8_t hffff_ie[IE.size];
};

#endif // MEMORY_HH
