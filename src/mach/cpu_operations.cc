#include "cpu.hh"

/* ADC */

void Cpu::ADC_A_HL()
{
    ADC_A_n8(mem->read(HL));
}

void Cpu::ADC_A_n8(uint8_t n8)
{
    uint16_t result = A + n8 + C_flag_get();
    H_flag_update(((A & 0x0F) + (n8 & 0x0F) + C_flag_get()) > 0x0F);
    // Can't call before H_flag_update.
    C_flag_update(result > 0xFF);
    N_flag_reset();
    Z_flag_update((uint8_t)result == 0);
    A = static_cast<uint8_t>(result);
}

void Cpu::ADC_A_r8(uint8_t& r8)
{
    ADC_A_n8(r8);
}

/* ADD */

void Cpu::ADD_A_HL()
{
    ADD_A_n8(mem->read(HL));
}

void Cpu::ADD_A_n8(uint8_t n8)
{
    uint16_t result = A + n8;
    C_flag_update(result > 0xFF);
    H_flag_update((A & 0x0F) + (n8 & 0x0F) > 0x0F);
    N_flag_reset();
    Z_flag_update((uint8_t)result == 0);
    A = static_cast<uint8_t>(result);
}

void Cpu::ADD_A_r8(uint8_t& r8)
{
    ADD_A_n8(r8);
}

void Cpu::ADD_HL_r16(uint16_t& r16)
{
    uint32_t result = HL + r16;
    C_flag_update(result > 0xFFFF);
    H_flag_update(((HL & 0x0FFF) + (r16 & 0x0FFF)) > 0x0FFF);
    N_flag_reset();
    HL = static_cast<uint16_t>(result);
}

void Cpu::ADD_SP_e8(int8_t e8)
{
    uint32_t result = static_cast<uint32_t>(SP) + e8;
    C_flag_update(((SP & 0x00FF) + static_cast<uint8_t>(e8)) > 0x00FF);
    H_flag_update(((SP & 0x000F) + (static_cast<uint8_t>(e8 & 0x0F))) > 0x0F);
    N_flag_reset();
    Z_flag_reset();
    SP = static_cast<uint16_t>(result);
}

/* AND */

void Cpu::AND_n8(uint8_t n8)
{
    A &= n8;
    C_flag_reset();
    H_flag_set();
    N_flag_reset();
    Z_flag_update(A == 0);
}

void Cpu::AND_HL()
{
    AND_n8(mem->read(HL));
}

void Cpu::AND_r8(uint8_t& r8)
{
    AND_n8(r8);
}

/* BIT */
void Cpu::BIT_n3_HL(uint8_t n3)
{
    uint8_t temp = mem->read(HL);
    BIT_n3_r8(n3, temp);
    mem->write(HL, temp);
}

void Cpu::BIT_n3_r8(uint8_t n3, uint8_t& r8)
{
    H_flag_set();
    N_flag_reset();
    Z_flag_update(!((r8 >> n3) & 0x01));
}

/* CALL */

void Cpu::CALL_n16(uint16_t n16)
{
    PUSH_r16(PC);
    PC = n16;
}

void Cpu::CALL_cc_n16(bool cc, uint16_t n16)
{
    if (cc)
    {
        CALL_n16(n16);
        branch_taken = true;
    }
    else branch_taken = false;
}

/* CCF */

void Cpu::CCF()
{
    C_flag_update(!C_flag_get());
    H_flag_reset();
    N_flag_reset();
}

/* CP */

void Cpu::CP_HL()
{
    CP_n8(mem->read(HL));
}

void Cpu::CP_n8(uint8_t n8)
{
    C_flag_update(A < n8);
    H_flag_update((A & 0x0F) < (n8 & 0x0F));
    N_flag_set();
    Z_flag_update(A == n8);
}

void Cpu::CP_r8(uint8_t& r8)
{
    CP_n8(r8);
}

/* CPL */

void Cpu::CPL()
{
    A = ~(A);
    H_flag_set();
    N_flag_set();
}

/* DAA */

void Cpu::DAA()
{
    if (!N_flag_get())
    {
        if (C_flag_get() || A > 0x99)
        {
            A += 0x60;
            C_flag_set();
        }
        if (H_flag_get() || (A & 0x0F) > 0x09)
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

void Cpu::DEC_HL()
{
    uint8_t temp = mem->read(HL);
    DEC_r8(temp);
    mem->write(HL, temp);
}

void Cpu::DEC_r16(uint16_t& r16)
{
    --(r16);
}

void Cpu::DEC_r8(uint8_t& r8)
{
    --(r8);
    H_flag_update((r8 & 0x0F) == 0x0F);
    N_flag_set();
    Z_flag_update(r8 == 0);
}

/* DI */

void Cpu::DI()
{
    DI_action = IMEStatus::RESET_NEXT_CYCLE;
}

/* EI */
void Cpu::EI()
{
    EI_action = IMEStatus::SET_NEXT_CYCLE;
}

/* HALT */

void Cpu::HALT()
{
    // This is documented here:
    // https://www.reddit.com/r/EmuDev/comments/5bfb2t/a_subtlety_about_the_gameboy_z80_halt_instruction/

    // TODO: Implement more accurately.
    is_halted = true;

    PC += 1; // Next instruction is skipped (DMG bug)
}

/* INC */

void Cpu::INC_HL()
{
    uint8_t temp = mem->read(HL);
    INC_r8(temp);
    mem->write(HL, temp);
}

void Cpu::INC_r16(uint16_t& r16)
{
    ++(r16);
}

void Cpu::INC_r8(uint8_t& r8)
{
    uint16_t result = r8 + 1;
    H_flag_update((r8 & 0x0F) + 1 > 0x0F);
    N_flag_reset();
    Z_flag_update((uint8_t)result == 0);
    r8 = static_cast<uint8_t>(result);
}

/* JP */

void Cpu::JP_HL()
{
    JP_n16(HL);
    branch_taken = true;
}

void Cpu::JP_cc_n16(bool cc, uint16_t n16)
{
    if (cc)
    {
        JP_n16(n16);
        branch_taken = true;
    }
    else branch_taken = false;
}

void Cpu::JP_n16(uint16_t n16)
{
    PC = n16;
    branch_taken = true;
}

/* JR */

void Cpu::JR_cc_n8(bool cc, int8_t n8)
{
    if (cc)
    {
        JR_n8(n8);
        branch_taken = true;
    }
    else branch_taken = false;
}

void Cpu::JR_n8(int8_t n8)
{
    PC += n8;
    branch_taken = true;
}

/* LD */

void Cpu::LD_C_A()
{
    mem->write(0xFF00 + C, A);
}

void Cpu::LD_HL_n8(uint8_t n8)
{
    mem->write(HL, n8);
}

void Cpu::LD_HL_r8(uint8_t& r8)
{
    LD_HL_n8(r8);
}

void Cpu::LD_n16_A(uint16_t n16)
{
    mem->write(n16, A);
}

void Cpu::LD_n16_SP(uint16_t n16)
{
    mem->write(n16, SP & 0x00FF);
    mem->write(n16 + 1, (SP & 0xFF00) >> 8);
}

void Cpu::LD_r16_A(uint16_t& r16)
{
    mem->write(r16, A);
}

void Cpu::LD_A_C()
{
    A = mem->read(0xFF00 + C);
}

void Cpu::LD_A_n16(uint16_t n16)
{
    A = mem->read(n16);
}

void Cpu::LD_A_r16(uint16_t& r16)
{
    LD_A_n16(r16);
}

void Cpu::LD_HL_SP_e8(int8_t e8)
{
    uint32_t result = static_cast<uint32_t>(SP) + e8;
    C_flag_update(((SP & 0x00FF) + static_cast<uint8_t>(e8)) > 0x00FF);
//    C_flag_update(result > 0xFFFF);
    H_flag_update((SP & 0x000F) + static_cast<uint16_t>(e8 & 0x0F) > 0x0F);
    N_flag_reset();
    Z_flag_reset();
    HL = static_cast<uint16_t>(result);
}

void Cpu::LD_r16_n16(uint16_t& r16, uint16_t n16)
{
    r16 = n16;
}

void Cpu::LD_r8_HL(uint8_t& r8)
{
    r8 = mem->read(HL);
}

void Cpu::LD_r8_n8(uint8_t& r8, uint8_t n8)
{
    r8 = n8;
}

void Cpu::LD_r8_r8(uint8_t& r8_1, uint8_t& r8_2)
{
    r8_1 = r8_2;
}

void Cpu::LD_SP_HL()
{
    SP = HL;
}

void Cpu::LDD_HL_A()
{
    mem->write(HL, A);
    --(HL);
}

void Cpu::LDD_A_HL()
{
    A = mem->read(HL);
    --(HL);
}

void Cpu::LDH_n8_A(uint8_t n8)
{
    mem->write(0xFF00 + n8,  A);
}

void Cpu::LDH_A_n8(uint8_t n8)
{
    A = mem->read(0xFF00 + n8);
}

void Cpu::LDI_HL_A()
{
    mem->write(HL, A);
    ++(HL);
}

void Cpu::LDI_A_HL()
{
    A = mem->read(HL);
    ++(HL);
}

/* NOP */

void Cpu::NOP()
{

}

/* OR */

void Cpu::OR_HL()
{
    OR_n8(mem->read(HL));
}

void Cpu::OR_n8(uint8_t n8)
{
    A |= n8;
    C_flag_reset();
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(A == 0);
}

void Cpu::OR_r8(uint8_t& r8)
{
    OR_n8(r8);
}

/* POP */

void Cpu::POP_r16(uint16_t& r16)
{
    r16 = static_cast<uint16_t>(mem->read(SP));
    ++(SP);
    r16 |= static_cast<uint16_t>(mem->read(SP)) << 8;
    ++(SP);
}

/* PUSH */

void Cpu::PUSH_r16(uint16_t& r16)
{
    --(SP);
    mem->write(SP, static_cast<uint8_t>(r16 >> 8));
    --(SP);
    mem->write(SP, static_cast<uint8_t>(r16));
}

/* RES */

void Cpu::RES_n3_HL(uint8_t n3)
{
    uint8_t temp = mem->read(HL);
    RES_n3_r8(n3, temp);
    mem->write(HL, temp);
}

void Cpu::RES_n3_r8(uint8_t n3, uint8_t& r8)
{
    r8 &= ~(0x01 << n3);
}

/* RET */

void Cpu::RET()
{
    POP_r16(PC);
}

void Cpu::RET_cc(bool cc)
{
    if (cc)
    {
        RET();
        branch_taken = true;
    }
    else branch_taken = false;
}

/* RETI */

void Cpu::RETI()
{
    RET();
    irc->ime_flag_set();
    EI_action = IMEStatus::DO_NOTHING;
    DI_action = IMEStatus::DO_NOTHING;
}

void Cpu::RL_HL()
{
    uint8_t temp = mem->read(HL);
    RL_r8(temp);
    mem->write(HL, temp);
}

void Cpu::RL_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= C_flag_get();
    C_flag_update(msbit != 0);
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(result == 0);
    r8 = result;
}

/* RLA */

void Cpu::RLA()
{
    RL_r8(A);
    // NOTE: The Z flag is also updated in RL_r8.
    Z_flag_reset();
}

/* RLC */

void Cpu::RLC_HL()
{
    uint8_t temp = mem->read(HL);
    RLC_r8(temp);
    mem->write(HL, temp);
}

void Cpu::RLC_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= msbit;
    C_flag_update(msbit != 0);
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(result == 0);
    r8 = result;
}

/* RLCA */

void Cpu::RLCA()
{
    RLC_r8(A);
    // NOTE: The Z flag is also updated in RLC_r8.
    Z_flag_reset();
}

/* RR */

void Cpu::RR_HL()
{
    uint8_t temp = mem->read(HL);
    RR_r8(temp);
    mem->write(HL, temp);
}

void Cpu::RR_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t lsbit = result & 0x01;
    result >>= 1;
    result |= (C_flag_get() << 7);
    C_flag_update(lsbit != 0);
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(result == 0);
    r8 = result;
}

/* RRA */

void Cpu::RRA()
{
    RR_r8(A);
    // NOTE: The Z flag is also updated in RR_r8.
    Z_flag_reset();
}

/* RRC */

void Cpu::RRC_HL()
{
    uint8_t temp = mem->read(HL);
    RRC_r8(temp);
    mem->write(HL, temp);
}

void Cpu::RRC_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t lsbit = result & 0x01;
    result >>= 1;
    result |= (lsbit << 7);
    C_flag_update(lsbit != 0);
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(result == 0);
    r8 = result;
}

/* RRCA */

void Cpu::RRCA()
{
    RRC_r8(A);
    // NOTE: The Z flag is also updated in RRC_r8.
    Z_flag_reset();
}

void Cpu::RST_f(uint8_t f)
{
    PUSH_r16(PC);
    PC = 0x0000 + f;
}

/* SBC */

void Cpu::SBC_A_HL()
{
    uint8_t temp = mem->read(HL);
    SBC_A_n8(temp);
    mem->write(HL, temp);
}

void Cpu::SBC_A_n8(uint8_t n8)
{
    uint16_t result = (uint16_t)A - n8 - C_flag_get();
    H_flag_update(((n8 & 0x0F) + C_flag_get()) > (A & 0x0F));
    C_flag_update(A < n8 + C_flag_get());
    N_flag_set();
    Z_flag_update((uint8_t)result == 0);
    A = static_cast<uint8_t>(result);
}

void Cpu::SBC_A_r8(uint8_t& r8)
{
    SBC_A_n8(r8);
}

/* SCF */

void Cpu::SCF()
{
    C_flag_set();
    H_flag_reset();
    N_flag_reset();
}

/* SET */

void Cpu::SET_n3_HL(uint8_t n3)
{
    uint8_t temp = mem->read(HL);
    SET_n3_r8(n3, temp);
    mem->write(HL, temp);
}

void Cpu::SET_n3_r8(uint8_t n3, uint8_t& r8)
{
    r8 |= 0x01 << n3;
}

/* SLA */

void Cpu::SLA_HL()
{
    uint8_t temp = mem->read(HL);
    SLA_r8(temp);
    mem->write(HL, temp);
}

void Cpu::SLA_r8(uint8_t& r8)
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

void Cpu::SRA_HL()
{
    uint8_t temp = mem->read(HL);
    SRA_r8(temp);
    mem->write(HL, temp);
}

void Cpu::SRA_r8(uint8_t& r8)
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

void Cpu::SRL_HL()
{
    uint8_t temp = mem->read(HL);
    SRL_r8(temp);
    mem->write(HL, temp);
}

void Cpu::SRL_r8(uint8_t& r8)
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

void Cpu::STOP()
{
    is_stopped = true;
    // TODO: Turn off display
}

/* SUB */

void Cpu::SUB_A_HL()
{
    uint8_t temp = mem->read(HL);
    SUB_A_n8(temp);
    mem->write(HL, temp);
}

void Cpu::SUB_A_n8(uint8_t n8)
{
    uint16_t result = A - n8;
    C_flag_update(A < n8);
//    C_flag_update(result > 0xFF);
    H_flag_update((n8 & 0x0F) > (A & 0x0F));
    N_flag_set();
    Z_flag_update((uint8_t)result == 0);
    A = static_cast<uint8_t>(result);
}

void Cpu::SUB_A_r8(uint8_t& r8)
{
    SUB_A_n8(r8);
}

/* SWAP */

void Cpu::SWAP_HL()
{
    uint8_t temp = mem->read(HL);
    SWAP_r8(temp);
    mem->write(HL, temp);
}

void Cpu::SWAP_r8(uint8_t& r8)
{
    r8 = static_cast<uint8_t>(((r8 & 0x0F) << 4) | ((r8 & 0xF0) >> 4));
    C_flag_reset();
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(r8 == 0);
}

/* XOR */

void Cpu::XOR_HL()
{
    uint8_t temp = mem->read(HL);
    XOR_n8(temp);
    mem->write(HL, temp);
}

void Cpu::XOR_n8(uint8_t n8)
{
    A ^= n8;
    C_flag_reset();
    H_flag_reset();
    N_flag_reset();
    Z_flag_update(A == 0);
}

void Cpu::XOR_r8(uint8_t& r8)
{
    XOR_n8(r8);
}
