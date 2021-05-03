#pragma once

//#include "../antdbg/src/core/addresses.hh"
//#include "../antdbg/src/core/macros.hh"
//#include "../antdbg/src/core/types.hh"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace antgb
{

using std::cerr;
using std::cout;
using std::endl;
using std::pair;
using std::string;

typedef int32_t  addr_t;
typedef uint16_t memsize_t;
typedef uint16_t lcdsize_t;

class iRenderer;

class Background;
class Cpu;
class Emulator;
class Joypad;
class Mmu;
class Ppu;
class Renderer;
class Serial;
class Sprite;
class Sprites;
class Tile;
class Timer;

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

} // namespace antgb
