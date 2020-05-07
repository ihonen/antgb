#include "colors.hh"

QColor* Colors::FG_BLUE = nullptr;
QColor* Colors::FG_WHITE = nullptr;
QColor* Colors::FG_LIGHT_GRAY = nullptr;
QColor* Colors::BG_GRAY = nullptr;
QColor* Colors::BG_DARK_GRAY = nullptr;

void Colors::load_all()
{
    FG_BLUE = new QColor(0x3DAEE9);
    FG_WHITE = new QColor(0xFFFFFF);
    FG_LIGHT_GRAY = new QColor(0xAFAFAF);
    BG_GRAY = new QColor(0x31363B);
    BG_DARK_GRAY = new QColor(0x232629);
}
