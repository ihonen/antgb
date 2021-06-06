#include "emulator/ppu/ppuregisters.hh"

#include "emulator/memory/addresses.hh"

namespace
{

constexpr uint8_t STAT_MASK = 0b01111111;

inline uint8_t mask(memaddr_t address)
{
    switch (address)
    {
        case STAT_ADDR: return STAT_MASK;
    }
    return 0xFF;
}

} // namespace

PpuRegisters::PpuRegisters()
    : LCDC(0x00)
    , STAT(0x00)
    , SCY(0x00)
    , SCX(0x00)
    , LY(0x00)
    , LYC(0x00)
    , DMA(0x00)
    , BGP(0x00)
    , OBP0(0x00)
    , OBP1(0x00)
    , WY(0x00)
    , WX(0x00)
{
}

PpuRegisters::~PpuRegisters() = default;

uint8_t* PpuRegisters::get(memaddr_t address)
{
    switch (address)
    {
        case LCDC_ADDR: return &LCDC;
        case STAT_ADDR: return &STAT;
        case SCY_ADDR: return &SCY;
        case SCX_ADDR: return &SCX;
        case LY_ADDR: return &LY;
        case LYC_ADDR: return &LYC;
        case DMA_ADDR: return &DMA;
        case BGP_ADDR: return &BGP;
        case OBP0_ADDR: return &OBP0;
        case OBP1_ADDR: return &OBP1;
        case WY_ADDR: return &WY;
        case WX_ADDR: return &WX;
    }
    assert(false);
    return nullptr;
}

uint8_t PpuRegisters::read(memaddr_t address)
{
    if (const auto* byte = get(address))
    {
        return *byte & mask(address);
    }

    assert(false);
    return 0xFF;
}

void PpuRegisters::write(memaddr_t address, uint8_t value)
{
    if (auto* byte = get(address))
    {
        *byte = value & mask(address);
    }
    assert(false);
}
