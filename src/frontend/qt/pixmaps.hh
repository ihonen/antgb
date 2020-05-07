#pragma once

#include <QPixmap>

struct Pixmaps
{
    static void load_all();

    /* TOOLBAR */

    static QPixmap* ACCELERATE;
    static QPixmap* ACCELERATE_HOVER;
    static QPixmap* ACCELERATE_PRESS;

    static QPixmap* DECELERATE;
    static QPixmap* DECELERATE_HOVER;
    static QPixmap* DECELERATE_PRESS;

    static QPixmap* DUMP;
    static QPixmap* DUMP_HOVER;
    static QPixmap* DUMP_PRESS;

    static QPixmap* GEAR;
    static QPixmap* GEAR_HOVER;
    static QPixmap* GEAR_PRESS;

    static QPixmap* PAUSE;
    static QPixmap* PAUSE_HOVER;
    static QPixmap* PAUSE_PRESS;

    static QPixmap* RESTART;
    static QPixmap* RESTART_HOVER;
    static QPixmap* RESTART_PRESS;

    static QPixmap* ROM;
    static QPixmap* ROM_HOVER;
    static QPixmap* ROM_PRESS;

    static QPixmap* RUN;
    static QPixmap* RUN_HOVER;
    static QPixmap* RUN_PRESS;

    static QPixmap* SAVE_GAME;
    static QPixmap* SAVE_GAME_HOVER;
    static QPixmap* SAVE_GAME_PRESS;

    static QPixmap* STEP_INTO;
    static QPixmap* STEP_INTO_HOVER;
    static QPixmap* STEP_INTO_PRESS;

    static QPixmap* STEP_OUT;
    static QPixmap* STEP_OUT_HOVER;
    static QPixmap* STEP_OUT_PRESS;

    static QPixmap* STEP_OVER;
    static QPixmap* STEP_OVER_HOVER;
    static QPixmap* STEP_OVER_PRESS;

    static QPixmap* TERMINATE;
    static QPixmap* TERMINATE_HOVER;
    static QPixmap* TERMINATE_PRESS;

    /* JOYPAD */

    static QPixmap* JOYPAD_CENTER;
    static QPixmap* JOYPAD_CENTER_HOVER;
    static QPixmap* JOYPAD_CENTER_PRESS;

    static QPixmap* JOYPAD_UP;
    static QPixmap* JOYPAD_UP_HOVER;
    static QPixmap* JOYPAD_UP_PRESS;

    static QPixmap* JOYPAD_DOWN;
    static QPixmap* JOYPAD_DOWN_HOVER;
    static QPixmap* JOYPAD_DOWN_PRESS;

    static QPixmap* JOYPAD_LEFT;
    static QPixmap* JOYPAD_LEFT_HOVER;
    static QPixmap* JOYPAD_LEFT_PRESS;

    static QPixmap* JOYPAD_RIGHT;
    static QPixmap* JOYPAD_RIGHT_HOVER;
    static QPixmap* JOYPAD_RIGHT_PRESS;

    static QPixmap* JOYPAD_A;
    static QPixmap* JOYPAD_A_HOVER;
    static QPixmap* JOYPAD_A_PRESS;

    static QPixmap* JOYPAD_B;
    static QPixmap* JOYPAD_B_HOVER;
    static QPixmap* JOYPAD_B_PRESS;

    static QPixmap* JOYPAD_START;
    static QPixmap* JOYPAD_START_HOVER;
    static QPixmap* JOYPAD_START_PRESS;

    static QPixmap* JOYPAD_SELECT;
    static QPixmap* JOYPAD_SELECT_HOVER;
    static QPixmap* JOYPAD_SELECT_PRESS;

    /* DEBUGGER */

    static QPixmap* BREAKPOINT;
    static QPixmap* BREAKPOINT_HOVER;
    static QPixmap* BREAKPOINT_PRESS;

    static QPixmap* CURRENT_ARROW;
    static QPixmap* CURRENT_ARROW_HOVER;
    static QPixmap* CURRENT_ARROW_PRESS;

    static QPixmap* DATA_BREAKPOINT;
    static QPixmap* DATA_BREAKPOINT_HOVER;
    static QPixmap* DATA_BREAKPOINT_PRESS;
};
