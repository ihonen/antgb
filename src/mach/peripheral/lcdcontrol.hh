#ifndef LCDCONTROL_HH
#define LCDCONTROL_HH

#include "../machine.hh"
#include "peripheral.hh"
#include <array>

using std::array;

class GBMachine::LCDControl : public GBMachine::Peripheral
{
public:
    LCDControl(CPU& cpu, uint8_t interrupt_line);
private:
    array<uint8_t, 12> registers_;
    uint8_t* io_base_ = &registers_[0];
};

#endif // LCDCONTROL_HH
