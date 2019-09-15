#ifndef CPU_HH
#define CPU_HH

#include <array>
#include <cstdint>

class Cpu
{
public:
    Cpu();
private:
    uint8_t get_flag(uint8_t pos);
    void    set_flag(uint8_t pos, uint8_t val);
    uint8_t get_cf();
    uint8_t get_hf();
    uint8_t get_nf();
    uint8_t get_zf();
    void    set_cf(bool cond = true);
    void    set_hf(bool cond = true);
    void    set_nf(bool cond = true);
    void    set_zf(bool cond = true);
    void    clear_cf(bool cond = true);
    void    clear_hf(bool cond = true);
    void    clear_nf(bool cond = true);
    void    clear_zf(bool cond = true);

    // Positions of the flag bits in flag register (F).
    static const uint8_t CF_BIT_POS = 4;
    static const uint8_t HF_BIT_POS = 5;
    static const uint8_t NF_BIT_POS = 6;
    static const uint8_t ZF_BIT_POS = 7;

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

    // Current opcode in execution. Maximum instruction length is 3 bytes.
    uint8_t op[3];
};

#endif // CPU_HH
