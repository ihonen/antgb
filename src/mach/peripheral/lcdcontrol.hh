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
    static const uint8_t IO_REGION_SIZE = 12;
private:
    array<uint8_t, 12> registers_;
    uint8_t& LCDC = registers_[0];
    uint8_t& STAT = registers_[1];
    uint8_t& SCY = registers_[2];
    uint8_t& SCX = registers_[3];
    uint8_t& LY = registers_[4];
    uint8_t& LYC = registers_[5];
    uint8_t& DMA = registers_[6];
    uint8_t& BGP = registers_[7];
    uint8_t& OBP0 = registers_[8];
    uint8_t& OBP1 = registers_[9];
    uint8_t& WY = registers_[10];
    uint8_t& WX = registers_[11];

    uint8_t* io_base_ = &registers_[0];
};

#endif // LCDCONTROL_HH
