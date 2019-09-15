#ifndef CPU_HH
#define CPU_HH

#include <array>
#include <cstdint>
#include <functional>

class Cpu
{
public:
    Cpu();
private:
    // BC, DE, HL, AF, PC, SP
    uint16_t reg[6];
    uint16_t* BC = &reg[0];
    uint16_t* DE = &reg[1];
    uint16_t* HL = &reg[2];
    uint16_t* AF = &reg[3];
    uint16_t* PC = &reg[5];
    uint16_t* SP = &reg[4];
    // Note: the following assumes little-endian.
    uint8_t*  B  = reinterpret_cast<uint8_t*>(BC) + 1;
    uint8_t*  C  = reinterpret_cast<uint8_t*>(BC) + 0;
    uint8_t*  D  = reinterpret_cast<uint8_t*>(DE) + 1;
    uint8_t*  E  = reinterpret_cast<uint8_t*>(DE) + 0;
    uint8_t*  H  = reinterpret_cast<uint8_t*>(HL) + 1;
    uint8_t*  L  = reinterpret_cast<uint8_t*>(HL) + 0;
    uint8_t*  A  = reinterpret_cast<uint8_t*>(AF) + 1;
    uint8_t*  F  = reinterpret_cast<uint8_t*>(AF) + 0;
    // Some instructions "index" the registers (both 16- and 8-bit)
    // so store them accordingly.
    const std::array<uint16_t*, 6> reg16 = {BC, DE, HL, AF, PC, SP};
    const std::array<uint8_t*, 8>  reg8  = {B, C, D, E, H, L, A, F};
};

#endif // CPU_HH
