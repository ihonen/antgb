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

/* NOP */

void Cpu::NOP()
{

}
