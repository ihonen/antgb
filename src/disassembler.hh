#ifndef DISASSEMBLER_HH
#define DISASSEMBLER_HH

#include <string>


class Disassembler
{
public:
    Disassembler();
    std::string disassemble(void* instruction);
};

#endif // DISASSEMBLER_HH
