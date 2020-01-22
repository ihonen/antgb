#ifndef PPUREGISTERS_HH
#define PPUREGISTERS_HH

#include "mmu.hh"

struct PPUReg
{
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

    memaddr_t LCDC_ADDRESS = 0xFF40;
    memaddr_t STAT_ADDRESS = 0xFF41;
    memaddr_t SCY_ADDRESS = 0xFF42;
    memaddr_t SCX_ADDRESS = 0xFF43;
    memaddr_t LY_ADDRESS = 0xFF44;
    memaddr_t LYC_ADDRESS = 0xFF45;
    memaddr_t WY_ADDRESS = 0xFF4A;
    memaddr_t BGP_ADDRESS = 0xFF47;
    memaddr_t OBP0_ADDRESS = 0xFF48;
    memaddr_t OBP1_ADDRESS = 0xFF49;
    memaddr_t DMA_ADDRESS = 0xFF46;

    uint8_t* lcdc;
    uint8_t* stat;
    uint8_t* scx;
    uint8_t* scy;
    uint8_t* ly;
    uint8_t* lyc;
    uint8_t* wy;
    uint8_t* wx;
    uint8_t* bgp;
    uint8_t* obp0;
    uint8_t* obp1;
    uint8_t* dma;
};

#endif // PPUREGISTERS_HH
