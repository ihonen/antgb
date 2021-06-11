#pragma once

#include "emulator/cpu/interrupts.hh"
#include "joypadregisters.hh"
#include <map>

class Cpu;

class Joypad
{
public:
    Joypad(JoypadRegisters& registers, Interrupts& interrupts);
    void button_pressed(JoypadButton button);
    void button_released(JoypadButton button);
protected:
    struct ButtonState
    {
        JoypadRegisters::BitPos bit_pos;
        bool pressed;
    };

    JoypadRegisters& registers;
    Interrupts& interrupts;

    std::map<JoypadButton, ButtonState> button_status;
};
