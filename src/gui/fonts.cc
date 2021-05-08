#include "fonts.hh"

QFont* Fonts::COURIER = nullptr;

void Fonts::load_all()
{
    COURIER = new QFont();
    COURIER->setFamily("Courier");
}
