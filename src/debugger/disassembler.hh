#pragma once

#include <string>


class Disassembler
{
public:
    Disassembler();
    std::string disassemble(void* instruction);
};
