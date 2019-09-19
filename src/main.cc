#include "mach/cpu/cpu.hh"

#include <cstdint>
#include <iostream>

int main()
{
    uint8_t op[3] {0x00, 0x00, 0x00};
    CPU cpu;
    cpu.execute(op);

    for (volatile size_t i = 0; i < 0xFFFF; ++i);

    return 0;
}
