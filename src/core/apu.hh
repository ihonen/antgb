#pragma once

#include "types.hh"

namespace antgb
{

class Apu
{
public:
    struct
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
