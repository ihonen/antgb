#pragma once

#include "../emulator/joypad.hh"
#include <QKeyEvent>

const std::map<int, JoypadButton> JOYPAD_KEYMAP
{
    {Qt::Key_W,         JoypadUp},
    {Qt::Key_S,         JoypadDown},
    {Qt::Key_A,         JoypadLeft},
    {Qt::Key_D,         JoypadRight},
    {Qt::Key_Return,    JoypadA},
    {Qt::Key_Backspace, JoypadB},
    {Qt::Key_Comma,     JoypadSelect},
    {Qt::Key_Period,    JoypadStart}
};
