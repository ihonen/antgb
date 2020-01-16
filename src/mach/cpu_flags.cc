#include "cpu.hh"

uint8_t CPU::get_ALU_flag(enum ALUFlagPos pos)
{
    return (F >> static_cast<uint8_t>(pos)) & 0x01;
}

void CPU::assign_ALU_flag(enum ALUFlagPos pos, uint8_t val)
{
    if (val == 0) F &= ~(0x01 << static_cast<uint8_t>(pos));
    else F |= 0x01 << static_cast<uint8_t>(pos);
}

uint8_t CPU::C_flag_get()
{
    return get_ALU_flag(ALUFlagPos::C_FLAG);
}

uint8_t CPU::H_flag_get()
{
    return get_ALU_flag(ALUFlagPos::H_FLAG);
}

uint8_t CPU::N_flag_get()
{
    return get_ALU_flag(ALUFlagPos::N_FLAG);
}

uint8_t CPU::Z_flag_get()
{
    return get_ALU_flag(ALUFlagPos::Z_FLAG);
}

void CPU::C_flag_set()
{
    assign_ALU_flag(ALUFlagPos::C_FLAG, 1);
}

void CPU::H_flag_set()
{
    assign_ALU_flag(ALUFlagPos::H_FLAG, 1);
}

void CPU::N_flag_set()
{
    assign_ALU_flag(ALUFlagPos::N_FLAG, 1);
}

void CPU::Z_flag_set()
{
    assign_ALU_flag(ALUFlagPos::Z_FLAG, 1);
}

void CPU::C_flag_reset()
{
    assign_ALU_flag(ALUFlagPos::C_FLAG, 0);
}

void CPU::H_flag_reset()
{
    assign_ALU_flag(ALUFlagPos::H_FLAG, 0);
}

void CPU::N_flag_reset()
{
    assign_ALU_flag(ALUFlagPos::N_FLAG, 0);
}

void CPU::Z_flag_reset()
{
    assign_ALU_flag(ALUFlagPos::Z_FLAG, 0);
}

void CPU::C_flag_update(bool cond)
{
    if (cond) C_flag_set();
    else C_flag_reset();
}

void CPU::H_flag_update(bool cond)
{
    if (cond) H_flag_set();
    else H_flag_reset();
}

void CPU::N_flag_update(bool cond)
{
    if (cond) N_flag_set();
    else C_flag_reset();
}

void CPU::Z_flag_update(bool cond)
{
    if (cond) Z_flag_set();
    else Z_flag_reset();
}

uint8_t CPU::IME_flag_get()
{
    return IME_flag;
}

void CPU::IME_flag_set()
{
    IME_flag = 0x01;
}

void CPU::IME_flag_reset()
{
    IME_flag = 0x00;
}
