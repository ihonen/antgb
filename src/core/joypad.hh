#pragma once

#include "interrupts.hh"
#include "memory.hh"
#include "types.hh"
#include <map>

using namespace std;

class Joypad
{
public:

    enum class Button
    {
        None,
        Right,
        Left,
        Up,
        Down,
        A,
        B,
        Select,
        Start
    };

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
    void button_pressed(Button button);
    void button_released(Button button);

    const size_t BUTTON_COUNT = 8;
    const memaddr_t IO_REG_ADDRESS = 0xFF00;
    uint8_t* io_register;
    Memory* mem;
    Irc* irc;

    map<Button, ButtonState> button_status;
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
