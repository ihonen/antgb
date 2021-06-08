#pragma once

#include "joypadregisters.hh"
#include <map>

class Cpu;

class Joypad
{
public:
    Joypad(JoypadRegisters& registers, Cpu* cpu);
    void button_pressed(JoypadButton button);
    void button_released(JoypadButton button);
protected:
    struct ButtonState
    {
        JoypadRegisters::BitPos bit_pos;
        bool pressed;
    };

    JoypadRegisters& registers;
    Cpu* cpu;

    std::map<JoypadButton, ButtonState> button_status;
};
