#include "emulator/peripherals/apuregisters.hh"

namespace
{

constexpr uint8_t NR10_MASK = 0b01111111;
constexpr uint8_t NR30_MASK = 0b10000000;
constexpr uint8_t NR32_MASK = 0b01100000;
constexpr uint8_t NR34_MASK = 0b11000011;
constexpr uint8_t NR41_MASK = 0b00111111;
constexpr uint8_t NR44_MASK = 0b11000000;
constexpr uint8_t NR52_MASK = 0b10001111;

inline uint8_t mask(memaddr_t address)
{
    switch (address)
    {
        case NR10_ADDR: return NR10_MASK;
        case NR30_ADDR: return NR30_MASK;
        case NR32_ADDR: return NR32_MASK;
        case NR34_ADDR: return NR34_MASK;
        case NR41_ADDR: return NR41_MASK;
        case NR44_ADDR: return NR44_MASK;
        case NR52_ADDR: return NR52_MASK;
    }
    return 0xFF;
}

} // namespace

ApuRegisters::ApuRegisters()
    : NR10(0x00)
    , NR11(0x00)
    , NR12(0x00)
    , NR13(0x00)
    , NR14(0x00)
    , unused1(0x00)
    , NR21(0x00)
    , NR22(0x00)
    , NR23(0x00)
    , NR24(0x00)
    , NR30(0x00)
    , NR31(0x00)
    , NR32(0x00)
    , NR33(0x00)
    , NR34(0x00)
    , unused2(0x00)
    , NR41(0x00)
    , NR42(0x00)
    , NR43(0x00)
    , NR44(0x00)
    , NR50(0x00)
    , NR51(0x00)
    , NR52(0x00)
    , waveram({})
{
}

ApuRegisters::~ApuRegisters() = default;

uint8_t* ApuRegisters::get(memaddr_t address)
{
    switch (address)
    {
        case NR10_ADDR: return &NR10;
        case NR11_ADDR: return &NR11;
        case NR12_ADDR: return &NR12;
        case NR13_ADDR: return &NR13;
        case NR14_ADDR: return &NR14;
        case NR21_ADDR: return &NR21;
        case NR22_ADDR: return &NR22;
        case NR23_ADDR: return &NR23;
        case NR24_ADDR: return &NR24;
        case NR30_ADDR: return &NR30;
        case NR31_ADDR: return &NR31;
        case NR32_ADDR: return &NR32;
        case NR33_ADDR: return &NR33;
        case NR34_ADDR: return &NR34;
        case NR41_ADDR: return &NR41;
        case NR42_ADDR: return &NR42;
        case NR43_ADDR: return &NR43;
        case NR44_ADDR: return &NR44;
        case NR50_ADDR: return &NR50;
        case NR51_ADDR: return &NR51;
        case NR52_ADDR: return &NR52;
    }
    if (address >= WAVERAM_LOW && address <= WAVERAM_HIGH)
    {
        return &waveram.at(address - WAVERAM_LOW);
    }
    assert(false);
    return nullptr;
}

uint8_t ApuRegisters::read(memaddr_t address)
{
    if (auto* byte = get(address))
    {
        return *byte & mask(address);
    }
    assert(false);
    return 0xFF;
}

void ApuRegisters::write(memaddr_t address, uint8_t value)
{
    if (auto* byte = get(address))
    {
        *byte = value & mask(address);
    }
    assert(false);
}
