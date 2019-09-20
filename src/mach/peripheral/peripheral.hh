#ifndef PERIPHERAL_HH
#define PERIPHERAL_HH

#include "../machine.hh"
#include "../cpu/cpu.hh"
#include <cstdint>

class GBMachine::Peripheral
{
public:
    Peripheral(CPU& cpu, uint8_t interrupt_line) :
        cpu_(cpu), interrupt_line_(interrupt_line)
    {}

    uint8_t* get_io_base()
    {
        return io_base_;
    }

    void assign_int_line(uint8_t line)
    {
        interrupt_line_ = line;
    }
private:
    void request_interrupt()
    {
        cpu_.request_interrupt(interrupt_line_);
    }
    CPU& cpu_;
    uint8_t interrupt_line_ = 0;
    uint8_t* io_base_ = nullptr;
};

#endif // PERIPHERAL_HH
