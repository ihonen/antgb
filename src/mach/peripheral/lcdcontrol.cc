#include "lcdcontrol.hh"

GBMachine::LCDControl::LCDControl(CPU& cpu, uint8_t interrupt_line) :
    GBMachine::Peripheral(cpu, interrupt_line)
{

}
