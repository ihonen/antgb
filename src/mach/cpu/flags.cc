#include "cpu.hh"
#include <cassert>

uint8_t Cpu::get_flag(uint8_t pos)
{
    assert(pos >= 4 && pos <= 7);
    return (*F >> pos) & 0x01;
}

void Cpu::assign_flag(uint8_t pos, uint8_t val)
{
    assert(val == 0 || val == 1);
    if (val == 0) *F &= ~(0x01 << pos);
    else *F |= 0x01 << pos;
}

uint8_t Cpu::C_flag_get()
{
    return get_flag(CF_BIT_POS);
}

uint8_t Cpu::H_flag_get()
{
    return get_flag(HF_BIT_POS);
}

uint8_t Cpu::N_flag_get()
{
    return get_flag(NF_BIT_POS);
}

uint8_t Cpu::Z_flag_get()
{
    return get_flag(ZF_BIT_POS);
}

void Cpu::C_flag_set()
{
    assign_flag(CF_BIT_POS, 1);
}

void Cpu::H_flag_set()
{
    assign_flag(HF_BIT_POS, 1);
}

void Cpu::N_flag_set()
{
    assign_flag(NF_BIT_POS, 1);
}

void Cpu::Z_flag_set()
{
    assign_flag(ZF_BIT_POS, 1);
}

void Cpu::C_flag_reset()
{
    assign_flag(CF_BIT_POS, 0);
}

void Cpu::H_flag_reset()
{
    assign_flag(HF_BIT_POS, 0);
}

void Cpu::N_flag_reset()
{
    assign_flag(NF_BIT_POS, 0);
}

void Cpu::Z_flag_reset()
{
    assign_flag(ZF_BIT_POS, 0);
}

void Cpu::C_flag_update(bool cond)
{
    if (cond) C_flag_set();
    else C_flag_reset();
}

void Cpu::H_flag_update(bool cond)
{
    if (cond) H_flag_set();
    else H_flag_reset();
}

void Cpu::N_flag_update(bool cond)
{
    if (cond) N_flag_set();
    else C_flag_reset();
}

void Cpu::Z_flag_update(bool cond)
{
    if (cond) Z_flag_set();
    else Z_flag_reset();
}

uint8_t Cpu::IME_flag_get()
{
    return mem[0xFFFF];
}

void Cpu::IME_flag_set()
{
    mem[0xFFFF] = 0x01;
}

void Cpu::IME_flag_reset()
{
    mem[0xFFFF] = 0x00;
}
