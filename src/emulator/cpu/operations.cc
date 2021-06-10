#include "cpu.hh"

#include "emulator/memory/memorybus.hh"

void Cpu::ADC_A_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    ADC_A_n8(&temp, nullptr);
}

void Cpu::ADC_A_n8(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    u16 result = reg.read_A() + n8 + reg.read_C_flag();
    reg.update_H_flag(((reg.read_A() & 0x0F) + (n8 & 0x0F) + reg.read_C_flag()) > 0x0F);
    // Can't call before H_flag_update.
    reg.update_C_flag(result > 0xFF);
    reg.clear_N_flag();
    reg.update_Z_flag(static_cast<u8>(result) == 0);
    reg.write_A(static_cast<u8>(result));
}

void Cpu::ADC_A_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    ADC_A_n8(&r8, nullptr);
}

void Cpu::ADD_A_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());

    ADD_A_n8(&temp);
}

void Cpu::ADD_A_n8(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    u16 result = reg.read_A() + n8;
    reg.update_C_flag(result > 0xFF);
    reg.update_H_flag((reg.read_A() & 0x0F) + (n8 & 0x0F) > 0x0F);
    reg.clear_N_flag();
    reg.update_Z_flag(static_cast<u8>(result) == 0);
    reg.write_A(static_cast<u8>(result));
}

void Cpu::ADD_A_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    ADD_A_n8(&r8);
}

void Cpu::ADD_HL_r16(void* r16_, void*)
{
    u16& r16 = *reinterpret_cast<u16*>(r16_);

    u32 result = reg.read_HL() + r16;
    reg.update_C_flag(result > 0xFFFF);
    reg.update_H_flag(((reg.read_HL() & 0x0FFF) + (r16 & 0x0FFF)) > 0x0FFF);
    reg.clear_N_flag();
    reg.write_HL(static_cast<u16>(result));
}

void Cpu::ADD_SP_e8(void* e8_, void*)
{
    i8 e8 = *reinterpret_cast<i8*>(e8_);

    u32 result = static_cast<u32>(reg.read_SP()) + e8;
    reg.update_C_flag(((reg.read_SP() & 0x00FF) + static_cast<u8>(e8)) > 0x00FF);
    reg.update_H_flag(((reg.read_SP() & 0x000F) + (static_cast<u8>(e8 & 0x0F))) > 0x0F);
    reg.clear_N_flag();
    reg.clear_Z_flag();
    reg.write_SP(static_cast<u16>(result));
}

void Cpu::AND_n8(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    reg.write_A(reg.read_A() & n8);
    reg.clear_C_flag();
    reg.set_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(reg.read_A() == 0);
}

void Cpu::AND_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());

    AND_n8(&temp);
}

void Cpu::AND_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    AND_n8(&r8);
}

void Cpu::BIT_n3_HL(void* n3_, void*)
{
    u8& n3 = *reinterpret_cast<u8*>(n3_);

    u8 temp = mem->read(reg.read_HL());
    BIT_n3_r8(&n3, &temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::BIT_n3_r8(void* n3_, void* r8_)
{
     u8& n3 = *reinterpret_cast<u8*>(n3_);
     u8& r8 = *reinterpret_cast<u8*>(r8_);

    reg.set_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(!((r8 >> n3) & 0x01));
}

void Cpu::CALL_n16(void* n16_, void*)
{
    u16& n16 = *reinterpret_cast<u16*>(n16_);

    PUSH_r16(&reg.get_PC());
    reg.write_PC(n16);
}

void Cpu::CALL_cc_n16(void* cc_, void* n16_)
{
    bool& cc = *reinterpret_cast<bool*>(cc_);
    u16& n16 = *reinterpret_cast<u16*>(n16_);

    if (cc)
    {
        CALL_n16(&n16);
        branch_taken = true;
    }
    else branch_taken = false;
}

void Cpu::CCF(void*, void*)
{
    reg.update_C_flag(!reg.read_C_flag());
    reg.clear_H_flag();
    reg.clear_N_flag();
}

void Cpu::CP_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());

    CP_n8(&temp);
}

void Cpu::CP_n8(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    reg.update_C_flag(reg.read_A() < n8);
    reg.update_H_flag((reg.read_A() & 0x0F) < (n8 & 0x0F));
    reg.set_N_flag();
    reg.update_Z_flag(reg.read_A() == n8);
}

void Cpu::CP_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    CP_n8(&r8);
}

void Cpu::CPL(void*, void*)
{
    reg.write_A(~(reg.read_A()));
    reg.set_H_flag();
    reg.set_N_flag();
}

void Cpu::DAA(void*, void*)
{
    if (!reg.read_N_flag())
    {
        if (reg.read_C_flag() || reg.read_A() > 0x99)
        {
            reg.write_A(reg.read_A() + 0x60);
            reg.set_C_flag();
        }
        if (reg.read_H_flag() || (reg.read_A() & 0x0F) > 0x09)
        {
            reg.write_A(reg.read_A() + 0x06);
        }
    }
    else
    {
        if (reg.read_C_flag()) reg.write_A(reg.read_A() - 0x60);
        if (reg.read_H_flag()) reg.write_A(reg.read_A() - 0x06);
    }

    reg.clear_H_flag();
    reg.update_Z_flag(reg.read_A() == 0);
}

void Cpu::DEC_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    DEC_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::DEC_r16(void* r16_, void*)
{
    u16& r16 = *reinterpret_cast<u16*>(r16_);

    --(r16);
}

void Cpu::DEC_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    --(r8);
    reg.update_H_flag((r8 & 0x0F) == 0x0F);
    reg.set_N_flag();
    reg.update_Z_flag(r8 == 0);
}

void Cpu::DI(void*, void*)
{
    // No delay in DI.
    DI_countdown = 0;
}

void Cpu::EI(void*, void*)
{
    EI_countdown = 1;
}

void Cpu::HALT(void*, void*)
{
    // This is documented here:
    // https://www.reddit.com/r/EmuDev/comments/5bfb2t/a_subtlety_about_the_gameboy_z80_halt_instruction/

    // TODO: Implement more accurately.
    is_halted = true;

    reg.write_PC(reg.read_PC() + 1); // Next instruction is skipped (DMG bug)
}

/* INC */

void Cpu::INC_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    INC_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::INC_r16(void* r16_, void*)
{
    u16& r16 = *reinterpret_cast<u16*>(r16_);
    ++(r16);
}

void Cpu::INC_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    u16 result = r8 + 1;
    reg.update_H_flag((r8 & 0x0F) + 1 > 0x0F);
    reg.clear_N_flag();
    reg.update_Z_flag(static_cast<u8>(result) == 0);
    r8 = static_cast<u8>(result);
}

/* JP */

void Cpu::JP_HL(void*, void*)
{
    u16 temp = reg.read_HL();

    JP_n16(&temp);
    branch_taken = true;
}

void Cpu::JP_cc_n16(void* cc_, void* n16_)
{
    bool& cc = *reinterpret_cast<bool*>(cc_);
    u16& n16 = *reinterpret_cast<u16*>(n16_);

    if (cc)
    {
        JP_n16(&n16);
        branch_taken = true;
    }
    else branch_taken = false;
}

void Cpu::JP_n16(void* n16_, void*)
{
    u16& n16 = *reinterpret_cast<u16*>(n16_);

    reg.write_PC(n16);
    branch_taken = true;
}

/* JR */

void Cpu::JR_cc_n8(void* cc_, void* n8_)
{
    bool& cc = *reinterpret_cast<bool*>(cc_);
    i8& n8 = *reinterpret_cast<i8*>(n8_);

    if (cc)
    {
        JR_n8(&n8);
        branch_taken = true;
    }
    else branch_taken = false;
}

void Cpu::JR_n8(void* n8_, void*)
{
    i8& n8 = *reinterpret_cast<i8*>(n8_);

    reg.write_PC(reg.read_PC() + n8);
    branch_taken = true;
}

/* LD */

void Cpu::LD_C_A(void*, void*)
{
    mem->write(0xFF00 + reg.read_C(), reg.read_A());
}

void Cpu::LD_HL_n8(void* n8_, void*)
{
    i8& n8 = *reinterpret_cast<i8*>(n8_);

    mem->write(reg.get_HL(), n8);
}

void Cpu::LD_HL_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    LD_HL_n8(&r8);
}

void Cpu::LD_n16_A(void* n16_, void*)
{
    u16& n16 = *reinterpret_cast<u16*>(n16_);

    mem->write(n16, reg.read_A());
}

void Cpu::LD_n16_SP(void* n16_, void*)
{
    u16& n16 = *reinterpret_cast<u16*>(n16_);

    mem->write(n16, reg.read_SP() & 0x00FF);
    mem->write(n16 + 1, (reg.read_SP() & 0xFF00) >> 8);
}

void Cpu::LD_r16_A(void* r16_, void*)
{
    u16& r16 = *reinterpret_cast<u16*>(r16_);

    mem->write(r16, reg.read_A());
}

void Cpu::LD_A_C(void*, void*)
{
    reg.write_A(mem->read(0xFF00 + reg.read_C()));
}

void Cpu::LD_A_n16(void* n16_, void*)
{
    u16& n16 = *reinterpret_cast<u16*>(n16_);

    reg.write_A(mem->read(n16));
}

void Cpu::LD_A_r16(void* r16_, void*)
{
    u16& r16 = *reinterpret_cast<u16*>(r16_);

    LD_A_n16(&r16);
}

void Cpu::LD_HL_SP_e8(void* e8_, void*)
{
    i8& e8 = *reinterpret_cast<i8*>(e8_);

    uint32_t result = static_cast<uint32_t>(reg.read_SP()) + e8;
    reg.update_C_flag(((reg.read_SP() & 0x00FF) + static_cast<u8>(e8)) > 0x00FF);
//    reg.update_C_flag(result > 0xFFFF);
    reg.update_H_flag((reg.read_SP() & 0x000F) + static_cast<u16>(e8 & 0x0F) > 0x0F);
    reg.clear_N_flag();
    reg.clear_Z_flag();
    reg.write_HL(static_cast<u16>(result));
}

void Cpu::LD_r16_n16(void* r16_, void* n16_)
{
    u16& r16 = *reinterpret_cast<u16*>(r16_);
    u16& n16 = *reinterpret_cast<u16*>(n16_);

    r16 = n16;
}

void Cpu::LD_r8_HL(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    r8 = mem->read(reg.read_HL());
}

void Cpu::LD_r8_n8(void* r8_, void* n8_)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    r8 = n8;
}

void Cpu::LD_r8_r8(void* r8_1_, void* r8_2_)
{
    u8& r8_1 = *reinterpret_cast<u8*>(r8_1_);
    u8& r8_2 = *reinterpret_cast<u8*>(r8_2_);

    r8_1 = r8_2;
}

void Cpu::LD_SP_HL(void*, void*)
{
    reg.write_SP(reg.read_HL());
}

void Cpu::LDD_HL_A(void*, void*)
{
    mem->write(reg.read_HL(), reg.read_A());
    reg.write_HL(reg.read_HL() - 1);
}

void Cpu::LDD_A_HL(void*, void*)
{
    reg.write_A(mem->read(reg.read_HL()));
    reg.write_HL(reg.read_HL() - 1);
}

void Cpu::LDH_n8_A(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    mem->write(0xFF00 + n8,  reg.read_A());
}

void Cpu::LDH_A_n8(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    reg.write_A(mem->read(0xFF00 + n8));
}

void Cpu::LDI_HL_A(void*, void*)
{
    mem->write(reg.read_HL(), reg.read_A());
    reg.write_HL(reg.read_HL() + 1);
}

void Cpu::LDI_A_HL(void*, void*)
{
    reg.write_A(mem->read(reg.read_HL()));
    reg.write_HL(reg.read_HL() + 1);
}

void Cpu::NOP(void*, void*)
{

}

void Cpu::OR_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());

    OR_n8(&temp);
}

void Cpu::OR_n8(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    reg.write_A(reg.read_A() | n8);
    reg.clear_C_flag();
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(reg.read_A() == 0);
}

void Cpu::OR_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    OR_n8(&r8);
}

void Cpu::POP_r16(void* r16_, void*)
{
    u16& r16 = *reinterpret_cast<u16*>(r16_);

    r16 = static_cast<u16>(mem->read(reg.read_SP()));
    reg.write_SP(reg.read_SP() + 1);
    r16 |= static_cast<u16>(mem->read(reg.read_SP())) << 8;
    reg.write_SP(reg.read_SP() + 1);
}

void Cpu::PUSH_r16(void* r16_, void*)
{
    u16& r16 = *reinterpret_cast<u16*>(r16_);

    reg.write_SP(reg.read_SP() - 1);
    mem->write(reg.read_SP(), static_cast<u8>(r16 >> 8));
    reg.write_SP(reg.read_SP() - 1);
    mem->write(reg.read_SP(), static_cast<u8>(r16));
}

void Cpu::RES_n3_HL(void* n3_, void*)
{
    u8& n3 = *reinterpret_cast<u8*>(n3_);

    u8 temp = mem->read(reg.read_HL());
    RES_n3_r8(&n3, &temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RES_n3_r8(void* n3_, void* r8_)
{
    u8& n3 = *reinterpret_cast<u8*>(n3_);
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    r8 &= ~(0x01 << n3);
}

void Cpu::RET(void*, void*)
{
    POP_r16(&reg.get_PC());
}

void Cpu::RET_cc(void* cc_, void*)
{
    bool& cc = *reinterpret_cast<bool*>(cc_);

    if (cc)
    {
        RET();
        branch_taken = true;
    }
    else branch_taken = false;
}

void Cpu::RETI(void*, void*)
{
    RET();
    reg.write_IME(1);
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
}

void Cpu::RL_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    RL_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RL_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    u8 result = r8;
    u8 msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= reg.read_C_flag();
    reg.update_C_flag(msbit != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

void Cpu::RLA(void*, void*)
{
    RL_r8(&reg.get_A());
    // NOTE: The Z flag is also updated in RL_r8.
    reg.clear_Z_flag();
}

void Cpu::RLC_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    RLC_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RLC_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    u8 result = r8;
    u8 msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= msbit;
    reg.update_C_flag(msbit != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

void Cpu::RLCA(void*, void*)
{
    RLC_r8(&reg.get_A());
    // NOTE: The Z flag is also updated in RLC_r8.
    reg.clear_Z_flag();
}

void Cpu::RR_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    RR_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RR_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    u8 result = r8;
    u8 lsbit = result & 0x01;
    result >>= 1;
    result |= (reg.read_C_flag() << 7);
    reg.update_C_flag(lsbit != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

void Cpu::RRA(void*, void*)
{
    RR_r8(&reg.get_A());
    // NOTE: The Z flag is also updated in RR_r8.
    reg.clear_Z_flag();
}

void Cpu::RRC_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    RRC_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RRC_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    u8 result = r8;
    u8 lsbit = result & 0x01;
    result >>= 1;
    result |= (lsbit << 7);
    reg.update_C_flag(lsbit != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

void Cpu::RRCA(void*, void*)
{
    RRC_r8(&reg.get_A());
    // NOTE: The Z flag is also updated in RRC_r8.
    reg.clear_Z_flag();
}

void Cpu::RST_f(void* f_, void*)
{
    u8& f = *reinterpret_cast<u8*>(f_);

    PUSH_r16(&reg.get_PC());
    reg.write_PC(0x0000 + f);
}

void Cpu::SBC_A_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    SBC_A_n8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SBC_A_n8(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    u16 result = static_cast<u16>(reg.read_A()) - n8 - reg.read_C_flag();
    reg.update_H_flag(((n8 & 0x0F) + reg.read_C_flag()) > (reg.read_A() & 0x0F));
    reg.update_C_flag(reg.read_A() < n8 + reg.read_C_flag());
    reg.set_N_flag();
    reg.update_Z_flag(static_cast<u8>(result) == 0);
    reg.write_A(static_cast<u8>(result));
}

void Cpu::SBC_A_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    SBC_A_n8(&r8);
}

void Cpu::SCF(void*, void*)
{
    reg.set_C_flag();
    reg.clear_H_flag();
    reg.clear_N_flag();
}

void Cpu::SET_n3_HL(void* n3_, void*)
{
    u8& n3 = *reinterpret_cast<u8*>(n3_);

    u8 temp = mem->read(reg.read_HL());
    SET_n3_r8(&n3, &temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SET_n3_r8(void* n3_, void* r8_)
{
    u8& n3 = *reinterpret_cast<u8*>(n3_);
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    r8 |= 0x01 << n3;
}

void Cpu::SLA_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    SLA_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SLA_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    u8 result = r8;
    result <<= 1;
    result &= ~(0x01);
    reg.update_C_flag(((r8 >> 7) & 0x01) != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

void Cpu::SRA_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    SRA_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SRA_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    u8 result = r8;
    u8 msb = r8 & (0x01 << 7);
    result >>= 1;
    result |= msb;
    reg.update_C_flag(r8 & 0x01);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

void Cpu::SRL_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    SRL_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SRL_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    u8 result = r8;
    result >>= 1;
    result &= ~(0x01 << 7);
    reg.update_C_flag(r8 & 0x01);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

void Cpu::STOP(void*, void*)
{
    is_stopped = true;
    // TODO: Turn off display
}

void Cpu::SUB_A_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    SUB_A_n8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SUB_A_n8(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    u16 result = reg.read_A() - n8;
    reg.update_C_flag(reg.read_A() < n8);
//    reg.update_C_flag(result > 0xFF);
    reg.update_H_flag((n8 & 0x0F) > (reg.read_A() & 0x0F));
    reg.set_N_flag();
    reg.update_Z_flag(static_cast<u8>(result) == 0);
    reg.write_A(static_cast<u8>(result));
}

void Cpu::SUB_A_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    SUB_A_n8(&r8);
}

void Cpu::SWAP_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    SWAP_r8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SWAP_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    r8 = static_cast<u8>(((r8 & 0x0F) << 4) | ((r8 & 0xF0) >> 4));
    reg.clear_C_flag();
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(r8 == 0);
}

void Cpu::XOR_HL(void*, void*)
{
    u8 temp = mem->read(reg.read_HL());
    XOR_n8(&temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::XOR_n8(void* n8_, void*)
{
    u8& n8 = *reinterpret_cast<u8*>(n8_);

    reg.write_A(reg.get_A() ^ n8);
    reg.clear_C_flag();
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(reg.read_A() == 0);
}

void Cpu::XOR_r8(void* r8_, void*)
{
    u8& r8 = *reinterpret_cast<u8*>(r8_);

    XOR_n8(&r8);
}
