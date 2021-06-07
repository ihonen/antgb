#include "apuregisters.hh"

namespace
{

constexpr uint8_t NR10_WRITE_MASK = 0b01111111;
constexpr uint8_t NR10_READ_MASK  = 0b01111111;

constexpr uint8_t NR11_WRITE_MASK = 0b11111111;
constexpr uint8_t NR11_READ_MASK  = 0b11000000;

constexpr uint8_t NR12_WRITE_MASK = 0b11111111;
constexpr uint8_t NR12_READ_MASK  = 0b11111111;

constexpr uint8_t NR13_WRITE_MASK = 0b11111111;
constexpr uint8_t NR13_READ_MASK  = 0b00000000;

constexpr uint8_t NR14_WRITE_MASK = 0b11000111;
constexpr uint8_t NR14_READ_MASK  = 0b01000000;

constexpr uint8_t NR21_WRITE_MASK = 0b11111111;
constexpr uint8_t NR21_READ_MASK  = 0b11000000;

constexpr uint8_t NR22_WRITE_MASK = 0b11111111;
constexpr uint8_t NR22_READ_MASK  = 0b11111111;

constexpr uint8_t NR23_WRITE_MASK = 0b11111111;
constexpr uint8_t NR23_READ_MASK  = 0b00000000;

constexpr uint8_t NR24_WRITE_MASK = 0b11000111;
constexpr uint8_t NR24_READ_MASK  = 0b01000000;

constexpr uint8_t NR30_WRITE_MASK = 0b10000000;
constexpr uint8_t NR30_READ_MASK  = 0b10000000;

constexpr uint8_t NR31_WRITE_MASK = 0b11111111;
constexpr uint8_t NR31_READ_MASK  = 0b11111111;

constexpr uint8_t NR32_WRITE_MASK = 0b01100000;
constexpr uint8_t NR32_READ_MASK  = 0b01100000;

constexpr uint8_t NR33_WRITE_MASK = 0b11111111;
constexpr uint8_t NR33_READ_MASK  = 0b00000000;

constexpr uint8_t NR34_WRITE_MASK = 0b11000111;
constexpr uint8_t NR34_READ_MASK  = 0b01000000;

constexpr uint8_t NR41_WRITE_MASK = 0b00111111;
constexpr uint8_t NR41_READ_MASK  = 0b00111111;

constexpr uint8_t NR42_WRITE_MASK = 0b11111111;
constexpr uint8_t NR42_READ_MASK  = 0b11111111;

constexpr uint8_t NR43_WRITE_MASK = 0b11111111;
constexpr uint8_t NR43_READ_MASK  = 0b11111111;

constexpr uint8_t NR44_WRITE_MASK = 0b11000000;
constexpr uint8_t NR44_READ_MASK  = 0b01000000;

constexpr uint8_t NR50_WRITE_MASK = 0b11111111;
constexpr uint8_t NR50_READ_MASK  = 0b11111111;

constexpr uint8_t NR51_WRITE_MASK = 0b11111111;
constexpr uint8_t NR51_READ_MASK  = 0b11111111;

constexpr uint8_t NR52_WRITE_MASK = 0b10000000;
constexpr uint8_t NR52_READ_MASK  = 0b10001111;

inline uint8_t write_mask(memaddr_t address)
{
    switch (address)
    {
        case NR10_ADDR: return NR10_WRITE_MASK;
        case NR11_ADDR: return NR11_WRITE_MASK;
        case NR12_ADDR: return NR12_WRITE_MASK;
        case NR13_ADDR: return NR13_WRITE_MASK;
        case NR14_ADDR: return NR14_WRITE_MASK;
        case NR21_ADDR: return NR21_WRITE_MASK;
        case NR22_ADDR: return NR22_WRITE_MASK;
        case NR23_ADDR: return NR23_WRITE_MASK;
        case NR24_ADDR: return NR24_WRITE_MASK;
        case NR30_ADDR: return NR30_WRITE_MASK;
        case NR31_ADDR: return NR31_WRITE_MASK;
        case NR32_ADDR: return NR32_WRITE_MASK;
        case NR33_ADDR: return NR33_WRITE_MASK;
        case NR34_ADDR: return NR34_WRITE_MASK;
        case NR41_ADDR: return NR41_WRITE_MASK;
        case NR42_ADDR: return NR42_WRITE_MASK;
        case NR43_ADDR: return NR43_WRITE_MASK;
        case NR44_ADDR: return NR44_WRITE_MASK;
        case NR50_ADDR: return NR50_WRITE_MASK;
        case NR51_ADDR: return NR51_WRITE_MASK;
        case NR52_ADDR: return NR52_WRITE_MASK;
    }
    return 0xFF;
}

inline uint8_t read_mask(memaddr_t address)
{
    switch (address)
    {
        case NR10_ADDR: return NR10_READ_MASK;
        case NR11_ADDR: return NR11_READ_MASK;
        case NR12_ADDR: return NR12_READ_MASK;
        case NR13_ADDR: return NR13_READ_MASK;
        case NR14_ADDR: return NR14_READ_MASK;
        case NR21_ADDR: return NR21_READ_MASK;
        case NR22_ADDR: return NR22_READ_MASK;
        case NR23_ADDR: return NR23_READ_MASK;
        case NR24_ADDR: return NR24_READ_MASK;
        case NR30_ADDR: return NR30_READ_MASK;
        case NR31_ADDR: return NR31_READ_MASK;
        case NR32_ADDR: return NR32_READ_MASK;
        case NR33_ADDR: return NR33_READ_MASK;
        case NR34_ADDR: return NR34_READ_MASK;
        case NR41_ADDR: return NR41_READ_MASK;
        case NR42_ADDR: return NR42_READ_MASK;
        case NR43_ADDR: return NR43_READ_MASK;
        case NR44_ADDR: return NR44_READ_MASK;
        case NR50_ADDR: return NR50_READ_MASK;
        case NR51_ADDR: return NR51_READ_MASK;
        case NR52_ADDR: return NR52_READ_MASK;
    }
    return 0xFF;
}
} // namespace

// TODO: Proper initialization.
ApuRegisters::ApuRegisters()
    : NR10(0x00)
    , NR11(0x00)
    , NR12(0x00)
    , NR13(0x00)
    , NR14(0x00)
    , NR21(0x00)
    , NR22(0x00)
    , NR23(0x00)
    , NR24(0x00)
    , NR30(0x00)
    , NR31(0x00)
    , NR32(0x00)
    , NR33(0x00)
    , NR34(0x00)
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
    return nullptr;
}

uint8_t ApuRegisters::read(memaddr_t address)
{
    if (auto* byte = get(address))
    {
        return *byte & read_mask(address);
    }
    return 0xFF;
}

void ApuRegisters::write(memaddr_t address, uint8_t value)
{
    if (auto* byte = get(address))
    {
        *byte = value & write_mask(address);
    }
}

// TODO: Wave RAM behavior when accessed while CH3 is disabled.
