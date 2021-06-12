#pragma once

#include "emulator/cpu/interrupts.hh"
#include "joypadregisters.hh"
#include <map>

class Joypad
{
public:
    Joypad(JoypadRegisters& registers, Interrupts& interrupts);
    void button_pressed(JoypadButton button);
    void button_released(JoypadButton button);
protected:
    JoypadRegisters& registers_;
    Interrupts& interrupts_;
};
