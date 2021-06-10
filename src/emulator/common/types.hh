#pragma once

#include "debugger/types.hh"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

using regid_t = int32_t;
using addr_t = uint16_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

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
