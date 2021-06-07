#pragma once

#include "addresses.hh"
#include "imemorybusnode.hh"

#include <array>
#include <cstdint>

class PpuRegisters : public iMemoryBusNode
{
public:
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

    PpuRegisters();
    virtual ~PpuRegisters() override;

    void post_bootram_reset();

    virtual bool owns(addr_t address) override;
    virtual uint8_t* get(addr_t address) override;
    virtual uint8_t read(addr_t address) override;
    virtual void write(addr_t address, uint8_t value) override;

protected:
    uint8_t LCDC;
    uint8_t STAT;
    uint8_t SCY;
    uint8_t SCX;
    uint8_t LY;
    uint8_t LYC;
    uint8_t DMA;
    uint8_t BGP;
    uint8_t OBP0;
    uint8_t OBP1;
    uint8_t WY;
    uint8_t WX;
};
