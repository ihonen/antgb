#pragma once

#include "debugger/types.hh"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

typedef int32_t regid_t;
typedef int32_t memaddr_t;

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
