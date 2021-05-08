#pragma once

#pragma once

#include <cstdint>

typedef int32_t regid_t;
typedef uint16_t memaddr_t;

struct framebuf_t
{
    uint8_t data[144][160];
};

class Breakpoint;
class DebugEvent;
class DebugCore;
class iDebugObserver;
class iEmulator;

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


#include "../debugger/addresses.hh"
#include "../emulator/types.hh"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::pair;
using std::string;

//typedef uint16_t memaddr_t;
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
