#include "cpu.hh"
#include <cassert>

uint8_t Cpu::get_flag(uint8_t pos)
{
    assert(pos >= 4 && pos <= 7);
    return (*F >> pos) & 0x01;
}

void Cpu::set_flag(uint8_t pos, uint8_t val)
{
    assert(val == 0 || val == 1);
    if (val == 0)
    {
        *F &= ~(0x01 << pos);
    }
    else
    {
        *F |= 0x01 << pos;
    }
}

uint8_t Cpu::get_cf()
{
    return get_flag(CF_BIT_POS);
}

uint8_t Cpu::get_hf()
{
    return get_flag(HF_BIT_POS);
}

uint8_t Cpu::get_nf()
{
    return get_flag(NF_BIT_POS);
}

uint8_t Cpu::get_zf()
{
    return get_flag(ZF_BIT_POS);
}

void Cpu::set_cf(bool cond)
{
    if (cond) set_flag(CF_BIT_POS, 1);
}

void Cpu::set_hf(bool cond)
{
    if (cond) set_flag(HF_BIT_POS, 1);
}

void Cpu::set_nf(bool cond)
{
    if (cond) set_flag(NF_BIT_POS, 1);
}

void Cpu::set_zf(bool cond)
{
    if (cond) set_flag(ZF_BIT_POS, 1);
}

void Cpu::clear_cf(bool cond)
{
    if (cond) set_flag(CF_BIT_POS, 0);
}

void Cpu::clear_hf(bool cond)
{
    if (cond) set_flag(HF_BIT_POS, 0);
}

void Cpu::clear_nf(bool cond)
{
    if (cond) set_flag(NF_BIT_POS, 0);
}

void Cpu::clear_zf(bool cond)
{
    if (cond) set_flag(ZF_BIT_POS, 0);
}
