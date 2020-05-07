#include "pixmaps.hh"

/* TOOLBAR */

QPixmap* Pixmaps::ACCELERATE = nullptr;;
QPixmap* Pixmaps::ACCELERATE_HOVER = nullptr;;
QPixmap* Pixmaps::ACCELERATE_PRESS = nullptr;;

QPixmap* Pixmaps::DECELERATE = nullptr;;
QPixmap* Pixmaps::DECELERATE_HOVER = nullptr;;
QPixmap* Pixmaps::DECELERATE_PRESS = nullptr;;

QPixmap* Pixmaps::DUMP = nullptr;
QPixmap* Pixmaps::DUMP_HOVER = nullptr;
QPixmap* Pixmaps::DUMP_PRESS = nullptr;

QPixmap* Pixmaps::GEAR = nullptr;
QPixmap* Pixmaps::GEAR_HOVER = nullptr;
QPixmap* Pixmaps::GEAR_PRESS = nullptr;

QPixmap* Pixmaps::PAUSE = nullptr;
QPixmap* Pixmaps::PAUSE_HOVER = nullptr;
QPixmap* Pixmaps::PAUSE_PRESS = nullptr;

QPixmap* Pixmaps::ROM = nullptr;
QPixmap* Pixmaps::ROM_HOVER = nullptr;
QPixmap* Pixmaps::ROM_PRESS = nullptr;

QPixmap* Pixmaps::RESTART = nullptr;
QPixmap* Pixmaps::RESTART_HOVER = nullptr;
QPixmap* Pixmaps::RESTART_PRESS = nullptr;

QPixmap* Pixmaps::RUN = nullptr;
QPixmap* Pixmaps::RUN_HOVER = nullptr;
QPixmap* Pixmaps::RUN_PRESS = nullptr;

QPixmap* Pixmaps::SAVE_GAME = nullptr;
QPixmap* Pixmaps::SAVE_GAME_HOVER = nullptr;
QPixmap* Pixmaps::SAVE_GAME_PRESS = nullptr;

QPixmap* Pixmaps::STEP_INTO = nullptr;
QPixmap* Pixmaps::STEP_INTO_HOVER = nullptr;
QPixmap* Pixmaps::STEP_INTO_PRESS = nullptr;

QPixmap* Pixmaps::STEP_OUT = nullptr;
QPixmap* Pixmaps::STEP_OUT_HOVER = nullptr;
QPixmap* Pixmaps::STEP_OUT_PRESS = nullptr;

QPixmap* Pixmaps::STEP_OVER = nullptr;
QPixmap* Pixmaps::STEP_OVER_HOVER = nullptr;
QPixmap* Pixmaps::STEP_OVER_PRESS = nullptr;

QPixmap* Pixmaps::TERMINATE = nullptr;
QPixmap* Pixmaps::TERMINATE_HOVER = nullptr;
QPixmap* Pixmaps::TERMINATE_PRESS = nullptr;

/* JOYPAD */

QPixmap* Pixmaps::JOYPAD_CENTER = nullptr;
QPixmap* Pixmaps::JOYPAD_CENTER_HOVER = nullptr;
QPixmap* Pixmaps::JOYPAD_CENTER_PRESS = nullptr;

QPixmap* Pixmaps::JOYPAD_UP = nullptr;
QPixmap* Pixmaps::JOYPAD_UP_HOVER = nullptr;
QPixmap* Pixmaps::JOYPAD_UP_PRESS = nullptr;

QPixmap* Pixmaps::JOYPAD_DOWN = nullptr;
QPixmap* Pixmaps::JOYPAD_DOWN_HOVER = nullptr;
QPixmap* Pixmaps::JOYPAD_DOWN_PRESS = nullptr;

QPixmap* Pixmaps::JOYPAD_LEFT = nullptr;
QPixmap* Pixmaps::JOYPAD_LEFT_HOVER = nullptr;
QPixmap* Pixmaps::JOYPAD_LEFT_PRESS = nullptr;

QPixmap* Pixmaps::JOYPAD_RIGHT = nullptr;
QPixmap* Pixmaps::JOYPAD_RIGHT_HOVER = nullptr;
QPixmap* Pixmaps::JOYPAD_RIGHT_PRESS = nullptr;

QPixmap* Pixmaps::JOYPAD_A = nullptr;
QPixmap* Pixmaps::JOYPAD_A_HOVER = nullptr;
QPixmap* Pixmaps::JOYPAD_A_PRESS = nullptr;

QPixmap* Pixmaps::JOYPAD_B = nullptr;
QPixmap* Pixmaps::JOYPAD_B_HOVER = nullptr;
QPixmap* Pixmaps::JOYPAD_B_PRESS = nullptr;

QPixmap* Pixmaps::JOYPAD_START = nullptr;
QPixmap* Pixmaps::JOYPAD_START_HOVER = nullptr;
QPixmap* Pixmaps::JOYPAD_START_PRESS = nullptr;

QPixmap* Pixmaps::JOYPAD_SELECT = nullptr;
QPixmap* Pixmaps::JOYPAD_SELECT_HOVER = nullptr;
QPixmap* Pixmaps::JOYPAD_SELECT_PRESS = nullptr;

/* DEBUGGER */

QPixmap* Pixmaps::BREAKPOINT = nullptr;
QPixmap* Pixmaps::BREAKPOINT_HOVER = nullptr;
QPixmap* Pixmaps::BREAKPOINT_PRESS = nullptr;

QPixmap* Pixmaps::CURRENT_ARROW = nullptr;
QPixmap* Pixmaps::CURRENT_ARROW_HOVER = nullptr;
QPixmap* Pixmaps::CURRENT_ARROW_PRESS = nullptr;

QPixmap* Pixmaps::DATA_BREAKPOINT = nullptr;
QPixmap* Pixmaps::DATA_BREAKPOINT_HOVER = nullptr;
QPixmap* Pixmaps::DATA_BREAKPOINT_PRESS = nullptr;

void Pixmaps::load_all()
{
    /* TOOLBAR */

    ACCELERATE = new QPixmap(":/img/accelerate.png");
    ACCELERATE_HOVER = new QPixmap(":/img/accelerate_hover.png");
    ACCELERATE_PRESS = new QPixmap(":/img/accelerate_press.png");

    DECELERATE = new QPixmap(":/img/decelerate.png");
    DECELERATE_HOVER = new QPixmap(":/img/decelerate_hover.png");
    DECELERATE_PRESS = new QPixmap(":/img/decelerate_press.png");

    DUMP = new QPixmap(":/img/dump.png");
    DUMP_HOVER = new QPixmap(":/img/dump_hover.png");
    DUMP_PRESS = new QPixmap(":/img/dump_press.png");

    GEAR = new QPixmap(":/img/gear.png");
    GEAR_HOVER = new QPixmap(":/img/gear_hover.png");
    GEAR_PRESS = new QPixmap(":/img/gear_press.png");

    PAUSE = new QPixmap(":/img/pause.png");
    PAUSE_HOVER = new QPixmap(":/img/pause_hover.png");
    PAUSE_PRESS = new QPixmap(":/img/pause_press.png");

    RESTART = new QPixmap(":/img/restart2.png");
    RESTART_HOVER = new QPixmap(":/img/restart2_hover.png");
    RESTART_PRESS = new QPixmap(":/img/restart2_press.png");

    ROM = new QPixmap(":/img/rom2.png");
    ROM_HOVER = new QPixmap(":/img/rom2_hover.png");
    ROM_PRESS = new QPixmap(":/img/rom2_press.png");

    RUN = new QPixmap(":/img/run.png");
    RUN_HOVER = new QPixmap(":/img/run_hover.png");
    RUN_PRESS = new QPixmap(":/img/run_press.png");

    SAVE_GAME = new QPixmap(":/img/save_game.png");
    SAVE_GAME_HOVER = new QPixmap(":/img/save_game_hover.png");
    SAVE_GAME_PRESS = new QPixmap(":/img/save_game_press.png");

    STEP_INTO = new QPixmap(":/img/step_into.png");
    STEP_INTO_HOVER = new QPixmap(":/img/step_into_hover.png");
    STEP_INTO_PRESS = new QPixmap(":/img/step_into_press.png");

    STEP_OUT = new QPixmap(":/img/step_out.png");
    STEP_OUT_HOVER = new QPixmap(":/img/step_out_hover.png");
    STEP_OUT_PRESS = new QPixmap(":/img/step_out_press.png");

    STEP_OVER = new QPixmap(":/img/step_over.png");
    STEP_OVER_HOVER = new QPixmap(":/img/step_over_hover.png");
    STEP_OVER_PRESS = new QPixmap(":/img/step_over_press.png");

    TERMINATE = new QPixmap(":/img/terminate.png");
    TERMINATE_HOVER = new QPixmap(":/img/terminate_hover.png");
    TERMINATE_PRESS = new QPixmap(":/img/terminate_press.png");

    /* JOYPAD */

    QTransform rotate90;
    rotate90.rotate(90);

    JOYPAD_CENTER = new QPixmap(":/img/joypad_center.png");
    JOYPAD_CENTER_HOVER = new QPixmap(":/img/joypad_center_hover.png");
    JOYPAD_CENTER_PRESS = new QPixmap(":/img/joypad_center_press.png");

    JOYPAD_UP = new QPixmap(":/img/joypad_up.png");
    JOYPAD_UP_HOVER = new QPixmap(":/img/joypad_up_hover.png");
    JOYPAD_UP_PRESS = new QPixmap(":/img/joypad_up_press.png");

    JOYPAD_RIGHT = new QPixmap(JOYPAD_UP->transformed(rotate90));
    JOYPAD_RIGHT_HOVER = new QPixmap(JOYPAD_UP_HOVER->transformed(rotate90));
    JOYPAD_RIGHT_PRESS = new QPixmap(JOYPAD_UP_PRESS->transformed(rotate90));

    JOYPAD_DOWN = new QPixmap(JOYPAD_RIGHT->transformed(rotate90));
    JOYPAD_DOWN_HOVER = new QPixmap(JOYPAD_RIGHT_HOVER->transformed(rotate90));
    JOYPAD_DOWN_PRESS = new QPixmap(JOYPAD_RIGHT_PRESS->transformed(rotate90));

    JOYPAD_LEFT = new QPixmap(JOYPAD_DOWN->transformed(rotate90));
    JOYPAD_LEFT_HOVER = new QPixmap(JOYPAD_DOWN_HOVER->transformed(rotate90));
    JOYPAD_LEFT_PRESS = new QPixmap(JOYPAD_DOWN_PRESS->transformed(rotate90));

    JOYPAD_A = new QPixmap(":/img/a_button3.png");
    JOYPAD_A_HOVER = new QPixmap(":/img/a_button3_hover.png");
    JOYPAD_A_PRESS = new QPixmap(":/img/a_button3_press.png");

    JOYPAD_B = new QPixmap(":/img/b_button3.png");
    JOYPAD_B_HOVER = new QPixmap(":/img/b_button3_hover.png");
    JOYPAD_B_PRESS = new QPixmap(":/img/b_button3_press.png");

    JOYPAD_START = new QPixmap(":/img/start_button3.png");
    JOYPAD_START_HOVER = new QPixmap(":/img/start_button3_hover.png");
    JOYPAD_START_PRESS = new QPixmap(":/img/start_button3_press.png");

    JOYPAD_SELECT = new QPixmap(":/img/select_button3.png");
    JOYPAD_SELECT_HOVER = new QPixmap(":/img/select_button3_hover.png");
    JOYPAD_SELECT_PRESS = new QPixmap(":/img/select_button3_press.png");

    /* DEBUGGER */

    BREAKPOINT = new QPixmap(":/img/breakpoint.png");

    DATA_BREAKPOINT = new QPixmap(":/img/data_breakpoint.png");

    CURRENT_ARROW = new QPixmap(":/img/current_arrow.png");
}
