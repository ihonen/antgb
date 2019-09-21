#include "ppu.hh"

GBMachine::PPU::PPU(CPU& cpu, uint8_t interrupt_line) :
    GBMachine::Peripheral(cpu, interrupt_line)
{

}
