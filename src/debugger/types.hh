#pragma once

#include <cstdint>

typedef int32_t regid_t;

enum CpuReg : regid_t
{
    // The first invalid memory address.
    RegNone = 0x10000,
    RegA,
    RegF,
    RegAF,
    RegB,
    RegC,
    RegBC,
    RegD,
    RegE,
    RegDE,
    RegH,
    RegL,
    RegHL,
    RegSP,
    RegPC,
    RegIME
};

/*
enum JoypadButton
{
    JoypadNone,
    JoypadUp,
    JoypadDown,
    JoypadLeft,
    JoypadRight,
    JoypadSelect,
    JoypadStart,
    JoypadA,
    JoypadB
};
*/
