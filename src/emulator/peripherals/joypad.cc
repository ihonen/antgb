#include "joypad.hh"

#include "emulator/common/bitmanip.hh"
#include "emulator/memory/memorybus.hh"
#include <iostream>

Joypad::Joypad(JoypadRegisters& registers, Interrupts& interrupts)
    : registers_(registers)
    , interrupts_(interrupts)
{
}

void Joypad::button_pressed(JoypadButton button)
{
    if (registers_.read(JOYP_ADDR) & JoypadRegisters::DirectionKeysSelect)
    {
        uint8_t bit_pos = 0xFF;

        switch (button)
        {
            case JoypadRight:
                bit_pos = JoypadRegisters::Right;
                break;
            case JoypadLeft:
                bit_pos = JoypadRegisters::Left;
                break;
            case JoypadUp:
                bit_pos = JoypadRegisters::Up;
                break;
            case JoypadDown:
                bit_pos = JoypadRegisters::Down;
                break;
            default:
                bit_pos = 0;
                break;
        }

        if (bit_pos != 0xFF)
        {
            uint8_t value = registers_.read(JOYP_ADDR);
            set_bit(&value, bit_pos);
            registers_.write(JOYP_ADDR, value);
            interrupts_.request_interrupt(Interrupts::Joypad);
        }
    }

    if (registers_.read(JOYP_ADDR) & JoypadRegisters::ButtonKeysSelect)
    {
        uint8_t bit_pos = 0xFF;

        switch (button)
        {
            case JoypadA:
                bit_pos = JoypadRegisters::ButtonA;
                break;
            case JoypadB:
                bit_pos = JoypadRegisters::ButtonB;
                break;
            case JoypadSelect:
                bit_pos = JoypadRegisters::Select;
                break;
            case JoypadStart:
                bit_pos = JoypadRegisters::Start;
                break;
            default:
                bit_pos = 0;
                break;
        }

        if (bit_pos != 0xFF)
        {
            uint8_t value = registers_.read(JOYP_ADDR);
            set_bit(&value, bit_pos);
            registers_.write(JOYP_ADDR, value);
            interrupts_.request_interrupt(Interrupts::Joypad);
        }
    }
}

void Joypad::button_released(JoypadButton button)
{
    if (registers_.read(JOYP_ADDR) & JoypadRegisters::DirectionKeysSelect)
    {
        uint8_t bit_pos = 0xFF;

        switch (button)
        {
            case JoypadRight:
                bit_pos = JoypadRegisters::Right;
                break;
            case JoypadLeft:
                bit_pos = JoypadRegisters::Left;
                break;
            case JoypadUp:
                bit_pos = JoypadRegisters::Up;
                break;
            case JoypadDown:
                bit_pos = JoypadRegisters::Down;
                break;
            default:
                bit_pos = 0;
                break;
        }

        if (bit_pos != 0xFF)
        {
            uint8_t value = registers_.read(JOYP_ADDR);
            clear_bit(&value, bit_pos);
            registers_.write(JOYP_ADDR, value);
        }
    }

    if (registers_.read(JOYP_ADDR) & JoypadRegisters::ButtonKeysSelect)
    {
        uint8_t bit_pos = 0xFF;

        switch (button)
        {
            case JoypadA:
                bit_pos = JoypadRegisters::ButtonA;
                break;
            case JoypadB:
                bit_pos = JoypadRegisters::ButtonB;
                break;
            case JoypadSelect:
                bit_pos = JoypadRegisters::Select;
                break;
            case JoypadStart:
                bit_pos = JoypadRegisters::Start;
                break;
            default:
                bit_pos = 0;
                break;
        }

        if (bit_pos != 0xFF)
        {
            uint8_t value = registers_.read(JOYP_ADDR);
            clear_bit(&value, bit_pos);
            registers_.write(JOYP_ADDR, value);
        }
    }
}
