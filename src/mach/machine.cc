#include "machine.hh"

#include <iostream>
#include <cstring>

using namespace std;

Machine::Machine()
{
    mmu = new MMU();
    cpu = new CPU(*mmu);
    ppu = new PPU(*mmu);
}

Machine::~Machine()
{
    delete cpu;
    delete mmu;
    delete ppu;
}

void Machine::load_rom(void* rom, size_t size)
{
    memcpy(mmu->mem.data, rom, size);
    cpu->restart();
}

void Machine::tick()
{
    cpu->execute();
}

void Machine::button_pressed(JoypadButton button)
{
    JoypadRegister& joypad_reg = *((JoypadRegister*)&mmu->mem[0xFF00]);

    if (joypad_reg.button_keys_select)
    {
        switch (button)
        {
            case JoypadButton::A:
                joypad_reg.right_or_button_a = 0;
                cout << "A" << endl;
                break;
            case JoypadButton::B:
                joypad_reg.left_or_button_b = 0;
                cout << "B" << endl;
                break;
            case JoypadButton::Start:
                joypad_reg.down_or_start = 0;
                cout << "Start" << endl;
                break;
            case JoypadButton::Select:
                joypad_reg.up_or_select = 0;
                cout << "Select" << endl;
                break;
            default:
                break;
        }
    }
    else if (joypad_reg.direction_keys_select)
    {
        switch (button)
        {
            case JoypadButton::Up:
                joypad_reg.up_or_select = 0;
                cout << "Up" << endl;
                break;
            case JoypadButton::Down:
                joypad_reg.down_or_start = 0;
                cout << "Down" << endl;
                break;
            case JoypadButton::Left:
                joypad_reg.left_or_button_b = 0;
                cout << "Left" << endl;
                break;
            case JoypadButton::Right:
                joypad_reg.right_or_button_a = 0;
                cout << "Right" << endl;
                break;
            default:
                break;
        }
    }

    cpu->request_joypad_interrupt();
}

void Machine::button_released(JoypadButton button)
{
    JoypadRegister& joypad_reg = *((JoypadRegister*)&mmu->mem[0xFF00]);

    if (joypad_reg.button_keys_select)
    {
        switch (button)
        {
            case JoypadButton::A:
                joypad_reg.right_or_button_a = 1;
                break;
            case JoypadButton::B:
                joypad_reg.left_or_button_b = 1;
                break;
            case JoypadButton::Start:
                joypad_reg.down_or_start = 1;
                break;
            case JoypadButton::Select:
                joypad_reg.up_or_select = 1;
                break;
            default:
                break;
        }
    }
    else if (joypad_reg.direction_keys_select)
    {
        switch (button)
        {
            case JoypadButton::Up:
                joypad_reg.up_or_select = 1;
                break;
            case JoypadButton::Down:
                joypad_reg.down_or_start = 1;
                break;
            case JoypadButton::Left:
                joypad_reg.left_or_button_b = 1;
                break;
            case JoypadButton::Right:
                joypad_reg.right_or_button_a = 1;
                break;
            default:
                break;
        }
    }
}
