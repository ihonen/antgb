#pragma once

#include "../antdbg/src/core/types.hh"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace antgb
{

using namespace antdbg;

using std::cerr;
using std::cout;
using std::endl;
using std::pair;
using std::string;

typedef uint16_t memaddr_t;
typedef uint16_t memsize_t;
typedef uint16_t lcdsize_t;

class Background;
class Cpu;
class Emulator;
class Irc;
class Joypad;
class Memory;
class Ppu;
class Renderer;
class Serial;
class Sprite;
class Sprites;
class Tile;
class Timer;

} // namespace antgb
