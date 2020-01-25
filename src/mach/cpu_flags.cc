#include "cpu.hh"

uint8_t Cpu::get_ALU_flag(enum ALUFlagPos pos)
{
    return (F >> static_cast<uint8_t>(pos)) & 0x01;
}

void Cpu::assign_ALU_flag(enum ALUFlagPos pos, uint8_t val)
{
    if (val == 0) F &= ~(0x01 << static_cast<uint8_t>(pos));
    else F |= 0x01 << static_cast<uint8_t>(pos);
}

uint8_t Cpu::C_flag_get()
{
    return get_ALU_flag(ALUFlagPos::C_FLAG);
}

uint8_t Cpu::H_flag_get()
{
    return get_ALU_flag(ALUFlagPos::H_FLAG);
}

uint8_t Cpu::N_flag_get()
{
    return get_ALU_flag(ALUFlagPos::N_FLAG);
}

uint8_t Cpu::Z_flag_get()
{
    return get_ALU_flag(ALUFlagPos::Z_FLAG);
}

void Cpu::C_flag_set()
{
    assign_ALU_flag(ALUFlagPos::C_FLAG, 1);
}

void Cpu::H_flag_set()
{
    assign_ALU_flag(ALUFlagPos::H_FLAG, 1);
}

void Cpu::N_flag_set()
{
    assign_ALU_flag(ALUFlagPos::N_FLAG, 1);
}

void Cpu::Z_flag_set()
{
    assign_ALU_flag(ALUFlagPos::Z_FLAG, 1);
}

void Cpu::C_flag_reset()
{
    assign_ALU_flag(ALUFlagPos::C_FLAG, 0);
}

void Cpu::H_flag_reset()
{
    assign_ALU_flag(ALUFlagPos::H_FLAG, 0);
}

void Cpu::N_flag_reset()
{
    assign_ALU_flag(ALUFlagPos::N_FLAG, 0);
}

void Cpu::Z_flag_reset()
{
    assign_ALU_flag(ALUFlagPos::Z_FLAG, 0);
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
