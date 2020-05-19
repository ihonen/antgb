#pragma once

#include "interrupts.hh"
#include "types.hh"
#include "../antdbg/src/core/types.hh"
#include <map>

using namespace std;

class Joypad
{
public:
    struct Registers
    {
        uint8_t joyp;
    } reg;

    static constexpr memaddr_t JOYP_ADDRESS = 0xFF00;
    static constexpr memaddr_t LOW_ADDRESS = JOYP_ADDRESS;
    static constexpr memaddr_t HIGH_ADDRESS = JOYP_ADDRESS;

    enum BitPos
    {
        Right = 0,
        Left = 1,
        Up = 2,
        Down = 3,
        ButtonA = 0,
        ButtonB = 1,
        Select = 2,
        Start = 3,
        DirectionKeysSelect = 4,
        ButtonKeysSelect = 5
    };

    struct ButtonState
    {
        BitPos bit_pos;
        bool pressed;
    };

    Joypad(Memory* mem, Irc* irc);
    void button_pressed(JoypadButton button);
    void button_released(JoypadButton button);

    const size_t BUTTON_COUNT = 8;
    const memaddr_t IO_REG_ADDRESS = 0xFF00;
    uint8_t* io_register;
    Memory* mem;
    Irc* irc;

    map<JoypadButton, ButtonState> button_status;
};

typedef struct __attribute__((packed))
{
    uint8_t right_or_button_a : 1;
    uint8_t left_or_button_b : 1;
    uint8_t up_or_select : 1;
    uint8_t down_or_start : 1;
    uint8_t direction_keys_select : 1;
    uint8_t button_keys_select : 1;
    uint8_t __unused : 2;
} JoypadRegister;
