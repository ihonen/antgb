#ifndef MMU_HH
#define MMU_HH

#include "memory.hh"
#include <cstdint>
#include <array>

using std::array;

class MMU
{
public:
    MMU();

    uint8_t read(memaddr_t address);

    bool write(memaddr_t address, uint8_t value);

    bool can_read(memaddr_t address);

    bool can_write(memaddr_t address);

    Memory mem;
};

#endif // MMU_HH
