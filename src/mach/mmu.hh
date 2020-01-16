#ifndef MMU_HH
#define MMU_HH

#include "memory.hh"
#include <cstdint>
#include <array>

using std::array;

class MMU
{
public:
    MMU()
    {
        mem = Memory();
    }

    uint8_t& operator[](uint16_t address)
    {
        return mem[address];
    }

private:
    Memory mem;
};

#endif // MMU_HH
