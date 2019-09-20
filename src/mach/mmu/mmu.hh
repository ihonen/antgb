#ifndef MMU_HH
#define MMU_HH

#include "../machine.hh"
#include <cstdint>
#include <array>

using std::array;

class GBMachine::MMU
{
public:
    MMU()
    {
        for (uint32_t i = 0; i < mem_space_size(); ++i)
        {
            memory_space_.at(i) = &ram_.at(i);
        }
        clear_memory();
    }

    uint8_t& operator[](uint16_t address)
    {
        return *memory_space_[address];
    }

    void clear_memory()
    {
        for (uint32_t i = 0; i < mem_space_size(); ++i)
        {
            *memory_space_[i] = 0x00;
        }
    }

    void map_region(uint16_t map_from, uint16_t map_size, uint8_t* map_to)
    {
        for (uint32_t i = 0; i < map_size; ++i)
        {
            memory_space_[map_from + i] = map_to;
        }
    }

    uint16_t mem_space_size()
    {
        return static_cast<uint16_t>(memory_space_.size());
    }
private:
    array<uint8_t, 0x100>  ram_ = {0};
    array<uint8_t*, 0x100> memory_space_;
};

#endif // MMU_HH
