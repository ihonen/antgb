#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::pair;
using std::string;

typedef uint16_t memaddr_t;
typedef uint16_t memsize_t;
typedef uint16_t lcdsize_t;

class Background;
class Cartridge;
class Cpu;
class Emulator;
class Irc;
class Joypad;
class Memory;
class Ppu;
class Renderer;
class Serial;
class Sprite;
class Tile;
class Timer;