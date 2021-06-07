#include "ppuregisters.hh"

namespace
{

    constexpr uint8_t LCDC_WRITE_MASK = 0b11111111;
    constexpr uint8_t LCDC_READ_MASK  = 0b11111111;

    constexpr uint8_t STAT_WRITE_MASK = 0b01111000;
    constexpr uint8_t STAT_READ_MASK  = 0b01111111;

    constexpr uint8_t SCY_WRITE_MASK  = 0b11111111;
    constexpr uint8_t SCY_READ_MASK   = 0b11111111;

    constexpr uint8_t SCX_WRITE_MASK  = 0b11111111;
    constexpr uint8_t SCX_READ_MASK   = 0b11111111;

    constexpr uint8_t LY_WRITE_MASK   = 0b00000000;
    constexpr uint8_t LY_READ_MASK    = 0b11111111;

    constexpr uint8_t LYC_WRITE_MASK  = 0b11111111;
    constexpr uint8_t LYC_READ_MASK   = 0b11111111;

    constexpr uint8_t DMA_WRITE_MASK  = 0b11111111;
    constexpr uint8_t DMA_READ_MASK   = 0b11111111;

    constexpr uint8_t BGP_WRITE_MASK  = 0b11111111;
    constexpr uint8_t BGP_READ_MASK   = 0b11111111;

    constexpr uint8_t OBP0_WRITE_MASK = 0b11111111;
    constexpr uint8_t OBP0_READ_MASK  = 0b11111111;

    constexpr uint8_t OBP1_WRITE_MASK = 0b11111111;
    constexpr uint8_t OBP1_READ_MASK  = 0b11111111;

    constexpr uint8_t WY_WRITE_MASK   = 0b11111111;
    constexpr uint8_t WY_READ_MASK    = 0b11111111;

    constexpr uint8_t WX_WRITE_MASK   = 0b11111111;
    constexpr uint8_t WX_READ_MASK    = 0b11111111;

    uint8_t read_mask(addr_t address)
    {
        switch (address)
        {
            case LCDC_ADDR: return LCDC_READ_MASK;
            case STAT_ADDR: return STAT_READ_MASK;
            case SCY_ADDR:  return SCY_READ_MASK;
            case SCX_ADDR:  return SCX_READ_MASK;
            case LY_ADDR:   return LY_READ_MASK;
            case LYC_ADDR:  return LYC_READ_MASK;
            case DMA_ADDR:  return DMA_READ_MASK;
            case BGP_ADDR:  return BGP_READ_MASK;
            case OBP0_ADDR: return OBP0_READ_MASK;
            case OBP1_ADDR: return OBP1_READ_MASK;
            case WY_ADDR:   return WY_READ_MASK;
            case WX_ADDR:   return WX_READ_MASK;
        }
        return 0xFF;
    }

    uint8_t write_mask(addr_t address)
    {
        switch (address)
        {
            case LCDC_ADDR: return LCDC_WRITE_MASK;
            case STAT_ADDR: return STAT_WRITE_MASK;
            case SCY_ADDR:  return SCY_WRITE_MASK;
            case SCX_ADDR:  return SCX_WRITE_MASK;
            case LY_ADDR:   return LY_WRITE_MASK;
            case LYC_ADDR:  return LYC_WRITE_MASK;
            case DMA_ADDR:  return DMA_WRITE_MASK;
            case BGP_ADDR:  return BGP_WRITE_MASK;
            case OBP0_ADDR: return OBP0_WRITE_MASK;
            case OBP1_ADDR: return OBP1_WRITE_MASK;
            case WY_ADDR:   return WY_WRITE_MASK;
            case WX_ADDR:   return WX_WRITE_MASK;
        }
        return 0xFF;
    }
}

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

void PpuRegisters::post_bootram_reset()
{
    LCDC = 0x91;
    STAT = 0x85;
    SCY  = 0x00;
    SCX  = 0x00;
    LYC  = 0x00;
    BGP  = 0xFC;
    OBP0 = 0xFF;
    OBP1 = 0xFF;
    WY   = 0x00;
    WX   = 0x00;
}

PpuRegisters::~PpuRegisters() = default;

bool PpuRegisters::owns(addr_t address)
{
    return address >= PPU_LOW && address <= PPU_HIGH;
}

uint8_t* PpuRegisters::get(addr_t address)
{
    assert(owns(address));
    switch (address)
    {
        case LCDC_ADDR: return &LCDC;
        case STAT_ADDR: return &STAT;
        case SCY_ADDR:  return &SCY;
        case SCX_ADDR:  return &SCX;
        case LY_ADDR:   return &LY;
        case LYC_ADDR:  return &LYC;
        case DMA_ADDR:  return &DMA;
        case BGP_ADDR:  return &BGP;
        case OBP0_ADDR: return &OBP0;
        case OBP1_ADDR: return &OBP1;
        case WY_ADDR:   return &WY;
        case WX_ADDR:   return &WX;
    }
    return nullptr;
}

uint8_t PpuRegisters::read(addr_t address)
{
    assert(owns(address));
    if (const auto* byte = get(address))
    {
        return *byte & read_mask(address);
    }
    return 0xFF;
}

void PpuRegisters::write(addr_t address, uint8_t value)
{
    assert(owns(address));
    if (auto* byte = get(address))
    {
        *byte = value & write_mask(address);
    }
}
