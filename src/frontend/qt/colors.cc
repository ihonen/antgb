#include "colors.hh"

QColor* Colors::BG_DARK_GRAY = nullptr;
QColor* Colors::BG_GRAY = nullptr;
QColor* Colors::FG_BLUE = nullptr;
QColor* Colors::FG_LIGHT_GRAY = nullptr;
QColor* Colors::FG_RED = nullptr;
QColor* Colors::FG_WHITE = nullptr;

void Colors::load_all()
{
    BG_DARK_GRAY = new QColor(0x232629);
    BG_GRAY = new QColor(0x31363B);
    FG_BLUE = new QColor(0x3DAEE9);
    FG_LIGHT_GRAY = new QColor(0xAFAFAF);
    FG_RED = new QColor(0xC4464A); // breakpoint red
    FG_WHITE = new QColor(0xFFFFFF);
}
