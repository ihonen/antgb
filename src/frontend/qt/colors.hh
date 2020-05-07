#pragma once

#include <QColor>

struct Colors
{
    static void load_all();

    static QColor* FG_BLUE;
    static QColor* FG_WHITE;
    static QColor* FG_LIGHT_GRAY;
    static QColor* BG_GRAY;
    static QColor* BG_DARK_GRAY;
};
