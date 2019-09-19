#include "cpu.hh"
#include <iostream>

/* ADC */

void CPU::ADC_A_HL()
{
    ADC_A_n8(mem[HL]);
}

void CPU::ADC_A_n8(uint8_t n8)
{
    uint16_t result = A + n8 + C_flag_get();
    C_flag_update(result > 0xFF);
    H_flag_update((A & 0x0F) + (n8 & 0x0F) + C_flag_get() > 0x0F);
    N_flag_reset();
    Z_flag_update(result == 0);
    A = static_cast<uint8_t>(result);
}

void CPU::ADC_A_r8(uint8_t& r8)
{
    ADC_A_n8(r8);
}

/* ADD */

void CPU::ADD_A_HL()
{
    ADD_A_n8(mem[HL]);
}

void CPU::ADD_A_n8(uint8_t n8)
{
    uint16_t result = A + n8;
    C_flag_update(result > 0xFF);
    H_flag_update((A & 0x0F) + (n8 & 0x0F) > 0x0F);
    N_flag_reset();
    Z_flag_update(result == 0);
    A = static_cast<uint8_t>(result);
}

void CPU::ADD_A_r8(uint8_t& r8)
{
    ADD_A_n8(r8);
}

void CPU::ADD_HL_r16(uint16_t& r16)
{
    uint32_t result = HL + r16;
    C_flag_update(result > 0xFFFF);
    H_flag_update(((HL >> 8) & 0x0F) + ((r16 >> 8) & 0x0F) > 0x0F);
    N_flag_reset();
    HL = static_cast<uint16_t>(result);
}

void CPU::ADD_SP_e8(int8_t e8)
{
    uint32_t result = static_cast<uint32_t>(SP) + e8;
    C_flag_update(result > 0xFFFF);
    H_flag_update(((SP & 0x000F) + (static_cast<uint8_t>(e8 & 0x0F))) > 0x0F);
    N_flag_reset();
    Z_flag_reset();
    SP = static_cast<uint16_t>(result);
}

/* AND */

void CPU::AND_n8(uint8_t n8)
{
    A &= n8;
    C_flag_reset();
    H_flag_set();
    N_flag_reset();
    Z_flag_update(A == 0);
}

void CPU::AND_HL()
{
    AND_n8(mem[HL]);
}

void CPU::AND_r8(uint8_t& r8)
{
    AND_n8(r8);
}

/* BIT */
void CPU::BIT_n3_HL(uint8_t n3)
{
    BIT_n3_r8(n3, mem[HL]);
}

void CPU::BIT_n3_r8(uint8_t n3, uint8_t& r8)
{
    H_flag_set();
    N_flag_reset();
    Z_flag_update(!((r8 >> n3) & 0x01));
}

/* CALL */

void CPU::CALL_n16(uint16_t n16)
{
    PUSH_r16(PC);
    PC = n16;
}

void CPU::CALL_cc_n16(bool cc, uint16_t n16)
{
    if (cc)
    {
        CALL_n16(n16);
        branch_taken = BranchTaken::YES;
    }
    else branch_taken = BranchTaken::NO;
}

/* CCF */

void CPU::CCF()
{
    C_flag_update(!C_flag_get());
    H_flag_reset();
    N_flag_reset();
}

/* CP */

void CPU::CP_HL()
{
    CP_n8(mem[HL]);
}

void CPU::CP_n8(uint8_t n8)
{
    C_flag_update(A < n8);
    H_flag_update((A & 0x0F) < (n8 & 0x0F));
    N_flag_set();
    Z_flag_update(A == n8);
}

void CPU::CP_r8(uint8_t& r8)
{
    CP_n8(r8);
}

/* CPL */

void CPU::CPL()
{
    A = ~(A);
}

/* DAA */

void CPU::DAA()
{
    if (!N_flag_get())
    {
        if (C_flag_get() || A > 0x99)
        {
            A += 0x60;
            C_flag_set();
        }
        if (H_flag_get() || (A & 0x0F) < 0x09)
        {
            A += 0x06;
        }
    }
    else
    {
        if (C_flag_get()) A -= 0x60;
        if (H_flag_get()) A -= 0x06;
    }

    H_flag_reset();
    Z_flag_update(A == 0);
}

/* DEC */

void CPU::DEC_HL()
{
    DEC_r8(mem[HL]);
}

void CPU::DEC_r16(uint16_t& r16)
{
    --(r16);
}

void CPU::DEC_r8(uint8_t& r8)
{
    --(r8);
    H_flag_update((r8 & 0x0F) == 0x0F);
    N_flag_set();
    Z_flag_update(r8 == 0);
}

/* DI */

void CPU::DI()
{
    if (DI_status == IMEStatus::DO_NOTHING)
        DI_status = IMEStatus::RESET_NEXT_CYCLE;
}

/* EI */
void CPU::EI()
{
    if (EI_status == IMEStatus::DO_NOTHING)
        EI_status = IMEStatus::SET_NEXT_CYCLE;
}

/* HALT */

void CPU::HALT()
{
    is_halted = true;
}

/* INC */

void CPU::INC_HL()
{
    INC_r8(mem[HL]);
}

void CPU::INC_r16(uint16_t& r16)
{
    ++(r16);
}

void CPU::INC_r8(uint8_t& r8)
{
    uint16_t result = r8 + 1;
    H_flag_update((r8 & 0x0F) + 1 > 0x0F);
    N_flag_reset();
    Z_flag_update(result == 0);
    r8 = static_cast<uint8_t>(result);
}

/* JP */

void CPU::JP_HL()
{
    JP_n16(HL);
}

void CPU::JP_cc_n16(bool cc, uint16_t n16)
{
    if (cc)
    {
        JP_n16(n16);
        branch_taken = BranchTaken::YES;
    }
    else branch_taken = BranchTaken::NO;
}

void CPU::JP_n16(uint16_t n16)
{
    PC = n16;
}

/* JR */

void CPU::JR_cc_n8(bool cc, int8_t n8)
{
    if (cc)
    {
        JR_n8(n8);
        branch_taken = BranchTaken::YES;
    }
    else branch_taken = BranchTaken::NO;
}

void CPU::JR_n8(int8_t n8)
{
    PC += n8;
}

/* LD */

void CPU::LD_C_A()
{
    mem[0xFF00 + C] = A;
}

void CPU::LD_HL_n8(uint8_t n8)
{
    mem[HL] = n8;
}

void CPU::LD_HL_r8(uint8_t& r8)
{
    LD_HL_n8(r8);
}

void CPU::LD_n16_A(uint16_t n16)
{
    mem[n16] = A;
}

void CPU::LD_n16_SP(uint16_t n16)
{
    // TODO: Check if both bytes of SP should be written.
    mem[n16] = static_cast<uint8_t>(SP);
}

void CPU::LD_r16_A(uint16_t& r16)
{
    LD_n16_A(r16);
}

void CPU::LD_A_C()
{
    A = mem[0xFF00 + C];
}

void CPU::LD_A_n16(uint16_t n16)
{
    A = mem[n16];
}

void CPU::LD_A_r16(uint16_t& r16)
{
    LD_A_n16(r16);
}

void CPU::LD_HL_SP_e8(int8_t e8)
{
    uint32_t result =static_cast<uint32_t>(SP) + e8;
    C_flag_update(result > 0xFFFF);
    H_flag_update((SP & 0x000F) + static_cast<uint16_t>(e8 & 0x0F) > 0x0F);
    N_flag_reset();
    Z_flag_reset();
    HL = static_cast<uint16_t>(result);
}

void CPU::LD_r16_n16(uint16_t& r16, uint16_t n16)
{
    r16 = n16;
}

void CPU::LD_r8_HL(uint8_t& r8)
{
    r8 = mem[HL];
}

void CPU::LD_r8_n8(uint8_t& r8, uint8_t n8)
{
    r8 = n8;
}

void CPU::LD_r8_r8(uint8_t& r8_1, uint8_t& r8_2)
{
    r8_1 = r8_2;
}

void CPU::LD_SP_HL()
{
    SP = HL;
}

void CPU::LDD_HL_A()
{
    mem[HL] = A;
    --(HL);
}

void CPU::LDD_A_HL()
{
    A = mem[HL];
    --(HL);
}

void CPU::LDH_n8_A(uint8_t n8)
{
    mem[0xFF00 + n8] = A;
}

void CPU::LDH_A_n8(uint8_t n8)
{
    A = mem[0xFF00 + n8];
}

void CPU::LDI_HL_A()
{
    mem[HL] = A;
    ++(HL);
}

void CPU::LDI_A_HL()
{
    A = mem[HL];
    ++(HL);
}

/* NOP */

void CPU::NOP()
{

}

/* OR */

void CPU::OR_HL()
{
    OR_n8(mem[HL]);
}

void CPU::OR_n8(uint8_t n8)
{
    A |= n8;
    C_flag_reset();
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(A == 0);
}

void CPU::OR_r8(uint8_t& r8)
{
    OR_n8(r8);
}

/* POP */

void CPU::POP_r16(uint16_t& r16)
{
    ++(SP);
    r16 |= mem[SP];
    ++(SP);
    r16 |= mem[SP] << 8;
}

/* PUSH */

void CPU::PUSH_r16(uint16_t& r16)
{
    mem[SP] = static_cast<uint8_t>(r16);
    --(SP);
    mem[SP] = static_cast<uint8_t>(r16 >> 8);
    --(SP);
}

/* RES */

void CPU::RES_n3_HL(uint8_t n3)
{
    RES_n3_r8(n3, mem[HL]);
}

void CPU::RES_n3_r8(uint8_t n3, uint8_t& r8)
{
    r8 &= ~(0x01 << n3);
}

/* RET */

void CPU::RET()
{
    ++(SP);
    PC |= mem[SP];
    ++(SP);
    PC |= mem[SP] << 8;
}

void CPU::RET_cc(bool cc)
{
    if (cc)
    {
        RET();
        branch_taken = BranchTaken::YES;
    }
    else branch_taken = BranchTaken::NO;
}

/* RETI */

void CPU::RETI()
{
    RET();
    enable_interrupts_now();
}

void CPU::RL_HL()
{
    RL_r8(mem[HL]);
}

void CPU::RL_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= C_flag_get();
    C_flag_update(msbit != 0);
    H_flag_reset();
    N_flag_reset();
    C_flag_update(result == 0);
    r8 = result;
}

/* RLA */

void CPU::RLA()
{
    RL_r8(A);
}

/* RLC */

void CPU::RLC_HL()
{
    RLC_r8(mem[HL]);
}

void CPU::RLC_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= msbit;
    C_flag_update(msbit != 0);
    H_flag_reset();
    N_flag_reset();
    C_flag_update(result == 0);
    r8 = result;
}

/* RLCA */

void CPU::RLCA()
{
    RLC_r8(A);
}

/* RR */

void CPU::RR_HL()
{
    RR_r8(mem[HL]);
}

void CPU::RR_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t lsbit = result & 0x01;
    result >>= 1;
    result |= (C_flag_get() << 7);
    C_flag_update(lsbit != 0);
    H_flag_reset();
    N_flag_reset();
    C_flag_update(result == 0);
    r8 = result;
}

/* RRA */

void CPU::RRA()
{
    RR_r8(A);
}

/* RRC */

void CPU::RRC_HL()
{
    RRC_r8(mem[HL]);
}

void CPU::RRC_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t lsbit = result & 0x01;
    result >>= 1;
    result |= (lsbit << 7);
    C_flag_update(lsbit != 0);
    H_flag_reset();
    N_flag_reset();
    C_flag_update(result == 0);
    r8 = result;
}

/* RRCA */

void CPU::RRCA()
{
    RRC_r8(A);
}

void CPU::RST_f(uint8_t f)
{
    PUSH_r16(PC);
    PC = 0x0000 + f;
}

/* SBC */

void CPU::SBC_A_HL()
{
    SBC_A_n8(mem[HL]);
}

void CPU::SBC_A_n8(uint8_t n8)
{
    uint16_t result = A - n8 - C_flag_get();
    H_flag_update((A & 0x0F) - (n8 & 0x0F) - C_flag_get() > 0x0F);
    C_flag_update(result > 0xFF);
    N_flag_set();
    Z_flag_update(result == 0);
    A = static_cast<uint8_t>(result);
}

void CPU::SBC_A_r8(uint8_t& r8)
{
    SBC_A_n8(r8);
}

void CPU::SUB_A_HL()
{
    SUB_A_n8(mem[HL]);
}

/* SCF */

void CPU::SCF()
{
    C_flag_set();
    H_flag_reset();
    N_flag_reset();
}

/* SET */

void CPU::SET_n3_HL(uint8_t n3)
{
    SET_n3_r8(n3, mem[HL]);
}

void CPU::SET_n3_r8(uint8_t n3, uint8_t& r8)
{
    r8 |= 0x01 << n3;
}

/* SLA */

void CPU::SLA_HL()
{
    SLA_r8(mem[HL]);
}

void CPU::SLA_r8(uint8_t& r8)
{
    uint8_t result = r8;
    result <<= 1;
    result &= ~(0x01);
    C_flag_update(((r8 >> 7) & 0x01) != 0);
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(result == 0);
    r8 = result;
}

/* SRA */

void CPU::SRA_HL()
{
    SRA_r8(mem[HL]);
}

void CPU::SRA_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msb = r8 & (0x01 << 7);
    result >>= 1;
    result |= msb;
    C_flag_update(r8 & 0x01);
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(result == 0);
    r8 = result;
}

/* SRL */

void CPU::SRL_HL()
{
    SRL_r8(mem[HL]);
}

void CPU::SRL_r8(uint8_t& r8)
{
    uint8_t result = r8;
    result >>= 1;
    result &= ~(0x01 << 7);
    C_flag_update(r8 & 0x01);
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(result == 0);
    r8 = result;
}

/* STOP */

void CPU::STOP()
{
    is_stopped = true;
}

/* SUB */

void CPU::SUB_A_n8(uint8_t n8)
{
    uint16_t result = A - n8;
    C_flag_update(result > 0xFF);
    H_flag_update((A & 0x0F) - (n8 & 0x0F) > 0x0F);
    N_flag_set();
    Z_flag_update(result == 0);
    A = static_cast<uint8_t>(result);
}

void CPU::SUB_A_r8(uint8_t& r8)
{
    SUB_A_n8(r8);
}

/* SWAP */

void CPU::SWAP_HL()
{
    SWAP_r8(mem[HL]);
}

void CPU::SWAP_r8(uint8_t& r8)
{
    r8 = static_cast<uint8_t>(((r8 & 0x0F) << 4) | ((r8 & 0xF0) >> 4));
    C_flag_reset();
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(r8 == 0);
}

/* XOR */

void CPU::XOR_HL()
{
    XOR_n8(mem[HL]);
}

void CPU::XOR_n8(uint8_t n8)
{
    A ^= n8;
    C_flag_reset();
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(A == 0);
}

void CPU::XOR_r8(uint8_t& r8)
{
    XOR_n8(r8);
}
