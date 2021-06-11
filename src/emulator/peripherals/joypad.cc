#include "joypad.hh"

#include "emulator/common/bitmanip.hh"
#include "emulator/memory/memorybus.hh"
#include <iostream>

Joypad::Joypad(JoypadRegisters& registers, Interrupts& interrupts)
    : registers(registers)
    , interrupts(interrupts)
{
    button_status =
    {
        {JoypadUp,     {JoypadRegisters::Up,      false}},
        {JoypadDown,   {JoypadRegisters::Down,    false}},
        {JoypadLeft,   {JoypadRegisters::Left,    false}},
        {JoypadRight,  {JoypadRegisters::Right,   false}},
        {JoypadSelect, {JoypadRegisters::Select,  false}},
        {JoypadStart,  {JoypadRegisters::Start,   false}},
        {JoypadA,      {JoypadRegisters::ButtonA, false}},
        {JoypadB,      {JoypadRegisters::ButtonB, false}},
    };
}

void Joypad::button_pressed(JoypadButton button)
{
    if (button == JoypadNone) return;

    if (registers.read(JOYP_ADDR) & JoypadRegisters::DirectionKeysSelect)
    {
        switch (button)
        {
            case JoypadRight:
            case JoypadLeft:
            case JoypadUp:
            case JoypadDown:
                // TODO: Register interface violation.
                clear_bit(registers.get(JOYP_ADDR), button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }
    else if (registers.read(JOYP_ADDR) & JoypadRegisters::ButtonKeysSelect)
    {
        switch (button)
        {
            case JoypadA:
            case JoypadB:
            case JoypadSelect:
            case JoypadStart:
                // TODO: Register interface violation.
                clear_bit(registers.get(JOYP_ADDR), button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }

    if (button_status[button].pressed == false)
    {
        button_status[button].pressed = true;
        interrupts.request_interrupt(Interrupts::JoypadInterrupt);
    }
}

void Joypad::button_released(JoypadButton button)
{
    if (button == JoypadNone) return;

    if (registers.read(JOYP_ADDR) & JoypadRegisters::DirectionKeysSelect)
    {
        switch (button)
        {
            case JoypadRight:
            case JoypadLeft:
            case JoypadUp:
            case JoypadDown:
                // TODO: Register interface violation.
                set_bit(registers.get(JOYP_ADDR), button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }
    else if (registers.read(JOYP_ADDR) & JoypadRegisters::ButtonKeysSelect)
    {
        switch (button)
        {
            case JoypadA:
            case JoypadB:
            case JoypadSelect:
            case JoypadStart:
                // TODO: Register interface violation.
                set_bit(registers.get(JOYP_ADDR), button_status[button].bit_pos);
                break;
            default:
                break;
        }
    }

    button_status[button].pressed = false;
}
