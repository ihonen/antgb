#pragma once

#include <cstdint>

class Instruction
{
public:
    Instruction(void* memory);

protected:
    uint8_t& bytes_;
};
