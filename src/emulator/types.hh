#pragma once

#include "debugger/types.hh"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

using regid_t = int32_t;
using addr_t = uint16_t;

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
