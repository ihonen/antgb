#include "cpu.hh"
#include <iostream>

/* ADC */

void Cpu::ADC_A_HL()
{
    ADC_A_n8(mem[*HL]);
}

void Cpu::ADC_A_n8(uint8_t n8)
{
    ADD_A_n8(n8 + get_cf());
}

void Cpu::ADC_A_r8(uint8_t* r8)
{
    ADC_A_n8(*r8);
}

/* ADD */

void Cpu::ADD_A_HL()
{
    ADD_A_n8(mem[*HL]);
}

void Cpu::ADD_A_n8(uint8_t n8)
{
    uint16_t result = *A + n8;
    update_cf(result > 0xFF);
    update_hf((*A & 0x0F) + (n8 & 0x0F) > 0x0F);
    clear_nf();
    update_zf(result == 0);
    *A = static_cast<uint8_t>(result);
}

void Cpu::ADD_A_r8(uint8_t* r8)
{
    ADD_A_n8(*r8);
}

void Cpu::ADD_HL_r16(uint16_t* r16)
{
    uint32_t result = *HL + *r16;
    update_cf(result > 0xFFFF);
    update_hf(((*HL >> 8) & 0x0F) + ((*r16 >> 8) & 0x0F) > 0x0F);
    clear_nf();
    // Zero flag is not updated.
    *HL = static_cast<uint16_t>(result);
}

void Cpu::ADD_SP_e8(int8_t e8)
{
    uint32_t result = static_cast<uint32_t>(*SP + static_cast<int16_t>(e8));
    update_cf(result > 0xFFFF);
    update_hf(((*SP & 0x000F) + (static_cast<uint8_t>(e8 & 0x0F))) > 0x0F);
    clear_nf();
    clear_zf();
    *SP = static_cast<uint16_t>(result);
}

/* AND */

void Cpu::AND_n8(uint8_t n8)
{
    *A &= n8;
    clear_cf();
    set_hf();
    clear_nf();
    update_zf(*A == 0);
}

void Cpu::AND_HL()
{
    AND_n8(mem[*HL]);
}

void Cpu::AND_r8(uint8_t* r8)
{
    AND_n8(*r8);
}

/* BIT */
void Cpu::BIT_n3_HL(uint8_t n3)
{
    BIT_n3_r8(n3, &mem[*HL]);
}

void Cpu::BIT_n3_r8(uint8_t n3, uint8_t* r8)
{
    set_hf();
    clear_nf();
    update_zf(!((*r8 >> n3) & 0x01));
}

/* CALL */

void Cpu::CALL_n16(uint16_t n16)
{
    --SP;
    mem[*SP] = *PC >> 8;
    --SP;
    mem[*SP] = static_cast<uint8_t>(*PC);

    *PC = n16;
}

void Cpu::CALL_cc_n16(bool cc, uint16_t n16)
{
    if (cc)
    {
        CALL_n16(n16);
        op_success = true;
    }
    else op_success = false;
}

/* CCF */

void Cpu::CCF()
{
    update_cf(!get_cf());
    clear_hf();
    clear_nf();
}

/* CP */

void Cpu::CP_HL()
{
    CP_n8(mem[*HL]);
}

void Cpu::CP_n8(uint8_t n8)
{
    update_cf(*A < n8);
    update_hf((*A & 0x0F) < (n8 & 0x0F));
    set_nf();
    update_zf(*A == n8);
}

void Cpu::CP_r8(uint8_t* r8)
{
    CP_n8(*r8);
}

/* CPL */

void Cpu::CPL()
{
    *A = ~(*A);
}

/* DEC */

void Cpu::DEC_HL()
{
    DEC_r8(&mem[*HL]);
}

void Cpu::DEC_r16(uint16_t* r16)
{
    --(*r16);
}

void Cpu::DEC_r8(uint8_t* r8)
{
    --(*r8);
    update_hf((*r8 & 0x0F) == 0x0F);
    set_nf();
    update_zf(*r8 == 0);
}

/* DI */

void Cpu::DI()
{
    if (DI_status == IntStatusChange::NOT_SCHEDULED)
    {
        DI_status = IntStatusChange::SCHEDULED;
    }
}

/* EI */
void Cpu::EI()
{
    if (EI_status == IntStatusChange::NOT_SCHEDULED)
    {
        EI_status = IntStatusChange::SCHEDULED;
    }
}

/* INC */

void Cpu::INC_HL()
{
    INC_r8(&mem[*HL]);
}

void Cpu::INC_r16(uint16_t* r16)
{
    ++(*r16);
}

void Cpu::INC_r8(uint8_t* r8)
{
    uint16_t result = *r8 + 1;
    update_hf((*r8 & 0x0F) + 1 > 0x0F);
    clear_nf();
    update_zf(result == 0);
    *r8 = static_cast<uint8_t>(result);
}

/* JP */

void Cpu::JP_HL()
{
    JP_n16(*HL);
}

void Cpu::JP_cc_n16(bool cc, uint16_t n16)
{
    if (cc)
    {
        JP_n16(n16);
        op_success = true;
    }
    else op_success = false;
}

void Cpu::JP_n16(uint16_t n16)
{
    *PC = n16;
}

/* JR */

void Cpu::JR_cc_n8(bool cc, int8_t n8)
{
    if (cc)
    {
        JR_n8(n8);
        op_success = true;
    }
    else op_success = false;
}

void Cpu::JR_n8(int8_t n8)
{
    *PC += n8;
}

/* LD */

void Cpu::LD_C_A()
{
    mem[0xFF00 + *C] = *A;
}

void Cpu::LD_HL_n8(uint8_t n8)
{
    mem[*HL] = n8;
}

void Cpu::LD_HL_r8(uint8_t* r8)
{
    LD_HL_n8(*r8);
}

void Cpu::LD_n16_A(uint16_t n16)
{
    mem[n16] = *A;
}

void Cpu::LD_n16_SP(uint16_t n16)
{
    // TODO: Check if both bytes of SP should be written.
    mem[n16] = static_cast<uint8_t>(*SP);
}

void Cpu::LD_r16_A(uint16_t* r16)
{
    LD_n16_A(*r16);
}

void Cpu::LD_A_C()
{
    *A = mem[0xFF00 + *C];
}

void Cpu::LD_A_n16(uint16_t n16)
{
    *A = mem[n16];
}

void Cpu::LD_A_r16(uint16_t* r16)
{
    LD_A_n16(*r16);
}

void Cpu::LD_HL_SP_e8(int8_t e8)
{
    // TODO: Check correctness.
    uint32_t result =static_cast<uint32_t>(*SP + e8);
    update_cf(result > 0xFFFF);
    update_hf((*SP & 0x000F) + static_cast<uint16_t>(e8 & 0x0F) > 0x0F);
    clear_nf();
    clear_zf();
    *HL = static_cast<uint16_t>(result);
}

void Cpu::LD_r16_n16(uint16_t* r16, uint16_t n16)
{
    *r16 = n16;
}

void Cpu::LD_r8_HL(uint8_t* r8)
{
    *r8 = mem[*HL];
}

void Cpu::LD_r8_n8(uint8_t* r8, uint8_t n8)
{
    *r8 = n8;
}

void Cpu::LD_r8_r8(uint8_t* r8_1, uint8_t* r8_2)
{
    *r8_1 = *r8_2;
}

void Cpu::LD_SP_HL()
{
    *SP = *HL;
}

void Cpu::LDD_HL_A()
{
    mem[*HL] = *A;
    --(*HL);
}

void Cpu::LDD_A_HL()
{
    *A = mem[*HL];
    --(*HL);
}

void Cpu::LDH_n8_A(uint8_t n8)
{
    mem[0xFF00 + n8] = *A;
}

void Cpu::LDH_A_n8(uint8_t n8)
{
    *A = mem[0xFF00 + n8];
}

void Cpu::LDI_HL_A()
{
    mem[*HL] = *A;
    ++(*HL);
}

void Cpu::LDI_A_HL()
{
    *A = mem[*HL];
    ++(*HL);
}

/* NOP */

void Cpu::NOP()
{

}

/* OR */

void Cpu::OR_HL()
{
    OR_n8(mem[*HL]);
}

void Cpu::OR_n8(uint8_t n8)
{
    *A |= n8;
    clear_cf();
    clear_hf();
    clear_nf();
    update_zf(*A == 0);
}

void Cpu::OR_r8(uint8_t* r8)
{
    OR_n8(*r8);
}

/* POP */

void Cpu::POP_r16(uint16_t* r16)
{
    ++SP;
    *r16 |= mem[*SP];
    ++SP;
    *r16 |= mem[*SP] << 8;
}

/* PUSH */

void Cpu::PUSH_r16(uint16_t* r16)
{
    mem[*SP] = static_cast<uint8_t>(*r16);
    --SP;
    mem[*SP] = static_cast<uint8_t>(*r16 >> 8);
    --SP;
}

/* RES */

void Cpu::RES_n3_HL(uint8_t n3)
{
    RES_n3_r8(n3, &mem[*HL]);
}

void Cpu::RES_n3_r8(uint8_t n3, uint8_t* r8)
{
    *r8 &= ~(0x01 << n3);
}

/* RET */

void Cpu::RET()
{
    ++SP;
    *PC |= mem[*SP];
    ++SP;
    *PC |= mem[*SP] << 8;
}

void Cpu::RET_cc(bool cc)
{
    if (cc)
    {
        RET();
        op_success = true;
    }
    else op_success = false;
}

/* RETI */

void Cpu::RETI()
{
    RET();
    enable_interrupts();
}

void Cpu::RL_HL()
{
    RL_r8(&mem[*HL]);
}

void Cpu::RL_r8(uint8_t* r8)
{
    uint8_t result = *r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= get_cf();
    update_cf(msbit != 0);
    clear_hf();
    clear_nf();
    update_cf(result == 0);
    *r8 = result;
}

/* RLA */

void Cpu::RLA()
{
    RL_r8(A);
}

/* RLC */

void Cpu::RLC_HL()
{
    RLC_r8(&mem[*HL]);
}

void Cpu::RLC_r8(uint8_t* r8)
{
    uint8_t result = *r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= msbit;
    update_cf(msbit != 0);
    clear_hf();
    clear_nf();
    update_cf(result == 0);
    *r8 = result;
}

/* RLCA */

void Cpu::RLCA()
{
    RLC_r8(A);
}

/* RR */

void Cpu::RR_HL()
{
    RR_r8(&mem[*HL]);
}

void Cpu::RR_r8(uint8_t* r8)
{
    uint8_t result = *r8;
    uint8_t lsbit = result & 0x01;
    result >>= 1;
    result |= (get_cf() << 7);
    update_cf(lsbit != 0);
    clear_hf();
    clear_nf();
    update_cf(result == 0);
    *r8 = result;
}
