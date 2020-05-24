#pragma once

#include "types.hh"

namespace antgb
{

class Apu
{
public:
    static constexpr memaddr_t NR10_ADDRESS = 0xFF10;
    static constexpr memaddr_t NR11_ADDRESS = 0xFF11;
    static constexpr memaddr_t NR12_ADDRESS = 0xFF12;
    static constexpr memaddr_t NR13_ADDRESS = 0xFF13;
    static constexpr memaddr_t NR14_ADDRESS = 0xFF14;
    static constexpr memaddr_t NR21_ADDRESS = 0xFF16;
    static constexpr memaddr_t NR22_ADDRESS = 0xFF17;
    static constexpr memaddr_t NR23_ADDRESS = 0xFF18;
    static constexpr memaddr_t NR24_ADDRESS = 0xFF19;
    static constexpr memaddr_t NR30_ADDRESS = 0xFF1A;
    static constexpr memaddr_t NR31_ADDRESS = 0xFF1B;
    static constexpr memaddr_t NR32_ADDRESS = 0xFF1C;
    static constexpr memaddr_t NR33_ADDRESS = 0xFF1D;
    static constexpr memaddr_t NR34_ADDRESS = 0xFF1E;

    static constexpr memaddr_t NR41_ADDRESS = 0xFF20;
    static constexpr memaddr_t NR42_ADDRESS = 0xFF21;
    static constexpr memaddr_t NR43_ADDRESS = 0xFF22;
    static constexpr memaddr_t NR44_ADDRESS = 0xFF23;
    static constexpr memaddr_t NR50_ADDRESS = 0xFF24;
    static constexpr memaddr_t NR51_ADDRESS = 0xFF25;
    static constexpr memaddr_t NR52_ADDRESS = 0xFF26;

    static constexpr memaddr_t LOW_ADDRESS = NR10_ADDRESS;
    static constexpr memaddr_t HIGH_ADDRESS = 0xFF3F;

    struct Registers
    {
        uint8_t nr10;
        uint8_t nr11;
        uint8_t nr12;
        uint8_t nr13;
        uint8_t nr14;
        uint8_t unused1;
        uint8_t nr21;
        uint8_t nr22;
        uint8_t nr23;
        uint8_t nr24;
        uint8_t nr30;
        uint8_t nr31;
        uint8_t nr32;
        uint8_t nr33;
        uint8_t nr34;
        uint8_t unused2;
        uint8_t nr_41;
        uint8_t nr_42;
        uint8_t nr_43;
        uint8_t nr_44;
        uint8_t nr_50;
        uint8_t nr_51;
        uint8_t nr_52;
        uint8_t unused[9];
        uint8_t wave_pattern_ram[16];
    } __attribute__((packed));
};

} // namespace antgb
