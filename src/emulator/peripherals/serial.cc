#include "serial.hh"

#include "emulator/memory/memorybus.hh"

Serial::Serial(SerialRegisters& reg, Interrupts& interrupts, iFrontend* frontend)
    : interrupts(interrupts)
    , reg(reg)
    , frontend_(frontend)
    , callback_(nullptr)
{
    tcycles_left_in_transfer = 0;
}

void Serial::set_serial_callback(iFrontend::SerialCallback callback)
{
    callback_ = callback;
}
