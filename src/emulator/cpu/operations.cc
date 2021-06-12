#include "cpu.hh"

#include "emulator/memory/memorybus.hh"

// ADC

void Cpu::ADC_A_HL()
{
    ADC_A_n8(memory_.read(reg_.read_HL()));
}

void Cpu::ADC_A_n8(uint8_t n8)
{
    uint16_t result = reg_.read_A() + n8 + reg_.read_C_flag();
    reg_.update_H_flag(((reg_.read_A() & 0x0F) + (n8 & 0x0F) + reg_.read_C_flag()) > 0x0F);
    // Can't call before H_flag_update.
    reg_.update_C_flag(result > 0xFF);
    reg_.clear_N_flag();
    reg_.update_Z_flag(static_cast<uint8_t>(result) == 0);
    reg_.write_A(static_cast<uint8_t>(result));
}

void Cpu::ADC_A_r8(uint8_t& r8)
{
    ADC_A_n8(r8);
}

// ADD

void Cpu::ADD_A_HL()
{
    ADD_A_n8(memory_.read(reg_.read_HL()));
}

void Cpu::ADD_A_n8(uint8_t n8)
{
    uint16_t result = reg_.read_A() + n8;
    reg_.update_C_flag(result > 0xFF);
    reg_.update_H_flag((reg_.read_A() & 0x0F) + (n8 & 0x0F) > 0x0F);
    reg_.clear_N_flag();
    reg_.update_Z_flag(static_cast<uint8_t>(result) == 0);
    reg_.write_A(static_cast<uint8_t>(result));
}

void Cpu::ADD_A_r8(uint8_t& r8)
{
    ADD_A_n8(r8);
}

void Cpu::ADD_HL_r16(uint16_t& r16)
{
    uint32_t result = reg_.read_HL() + r16;
    reg_.update_C_flag(result > 0xFFFF);
    reg_.update_H_flag(((reg_.read_HL() & 0x0FFF) + (r16 & 0x0FFF)) > 0x0FFF);
    reg_.clear_N_flag();
    reg_.write_HL(static_cast<uint16_t>(result));
}

void Cpu::ADD_SP_e8(int8_t e8)
{
    uint32_t result = static_cast<uint32_t>(reg_.read_SP()) + e8;
    reg_.update_C_flag(((reg_.read_SP() & 0x00FF) + static_cast<uint8_t>(e8)) > 0x00FF);
    reg_.update_H_flag(((reg_.read_SP() & 0x000F) + (static_cast<uint8_t>(e8 & 0x0F))) > 0x0F);
    reg_.clear_N_flag();
    reg_.clear_Z_flag();
    reg_.write_SP(static_cast<uint16_t>(result));
}

// AND

void Cpu::AND_n8(uint8_t n8)
{
    reg_.write_A(reg_.read_A() & n8);
    reg_.clear_C_flag();
    reg_.set_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(reg_.read_A() == 0);
}

void Cpu::AND_HL()
{
    AND_n8(memory_.read(reg_.read_HL()));
}

void Cpu::AND_r8(uint8_t& r8)
{
    AND_n8(r8);
}

// BIT
void Cpu::BIT_n3_HL(uint8_t n3)
{
    uint8_t temp = memory_.read(reg_.read_HL());
    BIT_n3_r8(n3, temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::BIT_n3_r8(uint8_t n3, uint8_t& r8)
{
    reg_.set_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(!((r8 >> n3) & 0x01));
}

// CALL

void Cpu::CALL_n16(uint16_t n16)
{
    PUSH_r16(reg_.get_PC());
    reg_.write_PC(n16);
}

void Cpu::CALL_cc_n16(bool cc, uint16_t n16)
{
    if (cc)
    {
        CALL_n16(n16);
        branch_was_taken_ = true;
    }
    else branch_was_taken_ = false;
}

// CCF

void Cpu::CCF()
{
    reg_.update_C_flag(!reg_.read_C_flag());
    reg_.clear_H_flag();
    reg_.clear_N_flag();
}

// CP

void Cpu::CP_HL()
{
    CP_n8(memory_.read(reg_.read_HL()));
}

void Cpu::CP_n8(uint8_t n8)
{
    reg_.update_C_flag(reg_.read_A() < n8);
    reg_.update_H_flag((reg_.read_A() & 0x0F) < (n8 & 0x0F));
    reg_.set_N_flag();
    reg_.update_Z_flag(reg_.read_A() == n8);
}

void Cpu::CP_r8(uint8_t& r8)
{
    CP_n8(r8);
}

// CPL

void Cpu::CPL()
{
    reg_.write_A(~(reg_.read_A()));
    reg_.set_H_flag();
    reg_.set_N_flag();
}

// DAA

void Cpu::DAA()
{
    if (!reg_.read_N_flag())
    {
        if (reg_.read_C_flag() || reg_.read_A() > 0x99)
        {
            reg_.write_A(reg_.read_A() + 0x60);
            reg_.set_C_flag();
        }
        if (reg_.read_H_flag() || (reg_.read_A() & 0x0F) > 0x09)
        {
            reg_.write_A(reg_.read_A() + 0x06);
        }
    }
    else
    {
        if (reg_.read_C_flag()) reg_.write_A(reg_.read_A() - 0x60);
        if (reg_.read_H_flag()) reg_.write_A(reg_.read_A() - 0x06);
    }

    reg_.clear_H_flag();
    reg_.update_Z_flag(reg_.read_A() == 0);
}

// DEC

void Cpu::DEC_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    DEC_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::DEC_r16(uint16_t& r16)
{
    --(r16);
}

void Cpu::DEC_r8(uint8_t& r8)
{
    --(r8);
    reg_.update_H_flag((r8 & 0x0F) == 0x0F);
    reg_.set_N_flag();
    reg_.update_Z_flag(r8 == 0);
}

// DI

void Cpu::DI()
{
    // No delay in DI.
    interrupts_.disable_interrupts_now();
}

// EI
void Cpu::EI()
{
    interrupts_.enable_interrupts_after(1);
}

// HALT

void Cpu::HALT()
{
    // This is documented here:
    // https://www.reddit.com/r/EmuDev/comments/5bfb2t/a_subtlety_about_the_gameboy_z80_halt_instruction/

    // TODO: Implement more accurately.
    is_halted_ = true;

    reg_.write_PC(reg_.read_PC() + 1); // Next instruction is skipped (DMG bug)
}

// INC

void Cpu::INC_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    INC_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::INC_r16(uint16_t& r16)
{
    ++(r16);
}

void Cpu::INC_r8(uint8_t& r8)
{
    uint16_t result = r8 + 1;
    reg_.update_H_flag((r8 & 0x0F) + 1 > 0x0F);
    reg_.clear_N_flag();
    reg_.update_Z_flag(static_cast<uint8_t>(result) == 0);
    r8 = static_cast<uint8_t>(result);
}

// JP

void Cpu::JP_HL()
{
    JP_n16(reg_.read_HL());
    branch_was_taken_ = true;
}

void Cpu::JP_cc_n16(bool cc, uint16_t n16)
{
    if (cc)
    {
        JP_n16(n16);
        branch_was_taken_ = true;
    }
    else branch_was_taken_ = false;
}

void Cpu::JP_n16(uint16_t n16)
{
    reg_.write_PC(n16);
    branch_was_taken_ = true;
}

// JR

void Cpu::JR_cc_n8(bool cc, int8_t n8)
{
    if (cc)
    {
        JR_n8(n8);
        branch_was_taken_ = true;
    }
    else branch_was_taken_ = false;
}

void Cpu::JR_n8(int8_t n8)
{
    reg_.write_PC(reg_.read_PC() + n8);
    branch_was_taken_ = true;
}

// LD

void Cpu::LD_C_A()
{
    memory_.write(0xFF00 + reg_.read_C(), reg_.read_A());
}

void Cpu::LD_HL_n8(uint8_t n8)
{
    memory_.write(reg_.get_HL(), n8);
}

void Cpu::LD_HL_r8(uint8_t& r8)
{
    LD_HL_n8(r8);
}

void Cpu::LD_n16_A(uint16_t n16)
{
    memory_.write(n16, reg_.read_A());
}

void Cpu::LD_n16_SP(uint16_t n16)
{
    memory_.write(n16, reg_.read_SP() & 0x00FF);
    memory_.write(n16 + 1, (reg_.read_SP() & 0xFF00) >> 8);
}

void Cpu::LD_r16_A(uint16_t& r16)
{
    memory_.write(r16, reg_.read_A());
}

void Cpu::LD_A_C()
{
    reg_.write_A(memory_.read(0xFF00 + reg_.read_C()));
}

void Cpu::LD_A_n16(uint16_t n16)
{
    reg_.write_A(memory_.read(n16));
}

void Cpu::LD_A_r16(uint16_t& r16)
{
    LD_A_n16(r16);
}

void Cpu::LD_HL_SP_e8(int8_t e8)
{
    uint32_t result = static_cast<uint32_t>(reg_.read_SP()) + e8;
    reg_.update_C_flag(((reg_.read_SP() & 0x00FF) + static_cast<uint8_t>(e8)) > 0x00FF);
//    reg.update_C_flag(result > 0xFFFF);
    reg_.update_H_flag((reg_.read_SP() & 0x000F) + static_cast<uint16_t>(e8 & 0x0F) > 0x0F);
    reg_.clear_N_flag();
    reg_.clear_Z_flag();
    reg_.write_HL(static_cast<uint16_t>(result));
}

void Cpu::LD_r16_n16(uint16_t& r16, uint16_t n16)
{
    r16 = n16;
}

void Cpu::LD_r8_HL(uint8_t& r8)
{
    r8 = memory_.read(reg_.read_HL());
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
    reg_.write_SP(reg_.read_HL());
}

void Cpu::LDD_HL_A()
{
    memory_.write(reg_.read_HL(), reg_.read_A());
    reg_.write_HL(reg_.read_HL() - 1);
}

void Cpu::LDD_A_HL()
{
    reg_.write_A(memory_.read(reg_.read_HL()));
    reg_.write_HL(reg_.read_HL() - 1);
}

void Cpu::LDH_n8_A(uint8_t n8)
{
    memory_.write(0xFF00 + n8,  reg_.read_A());
}

void Cpu::LDH_A_n8(uint8_t n8)
{
    reg_.write_A(memory_.read(0xFF00 + n8));
}

void Cpu::LDI_HL_A()
{
    memory_.write(reg_.read_HL(), reg_.read_A());
    reg_.write_HL(reg_.read_HL() + 1);
}

void Cpu::LDI_A_HL()
{
    reg_.write_A(memory_.read(reg_.read_HL()));
    reg_.write_HL(reg_.read_HL() + 1);
}

// NOP

void Cpu::NOP()
{

}

// OR

void Cpu::OR_HL()
{
    OR_n8(memory_.read(reg_.read_HL()));
}

void Cpu::OR_n8(uint8_t n8)
{
    reg_.write_A(reg_.read_A() | n8);
    reg_.clear_C_flag();
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(reg_.read_A() == 0);
}

void Cpu::OR_r8(uint8_t& r8)
{
    OR_n8(r8);
}

// POP

void Cpu::POP_r16(uint16_t& r16)
{
    r16 = static_cast<uint16_t>(memory_.read(reg_.read_SP()));
    reg_.write_SP(reg_.read_SP() + 1);
    r16 |= static_cast<uint16_t>(memory_.read(reg_.read_SP())) << 8;
    reg_.write_SP(reg_.read_SP() + 1);
}

// PUSH

void Cpu::PUSH_r16(uint16_t& r16)
{
    reg_.write_SP(reg_.read_SP() - 1);
    memory_.write(reg_.read_SP(), static_cast<uint8_t>(r16 >> 8));
    reg_.write_SP(reg_.read_SP() - 1);
    memory_.write(reg_.read_SP(), static_cast<uint8_t>(r16));
}

// RES

void Cpu::RES_n3_HL(uint8_t n3)
{
    uint8_t temp = memory_.read(reg_.read_HL());
    RES_n3_r8(n3, temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::RES_n3_r8(uint8_t n3, uint8_t& r8)
{
    r8 &= ~(0x01 << n3);
}

// RET

void Cpu::RET()
{
    POP_r16(reg_.get_PC());
}

void Cpu::RET_cc(bool cc)
{
    if (cc)
    {
        RET();
        branch_was_taken_ = true;
    }
    else branch_was_taken_ = false;
}

// RETI

void Cpu::RETI()
{
    RET();
    interrupts_.enable_interrupts_now();
}

void Cpu::RL_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    RL_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::RL_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= reg_.read_C_flag();
    reg_.update_C_flag(msbit != 0);
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(result == 0);
    r8 = result;
}

// RLA

void Cpu::RLA()
{
    RL_r8(reg_.get_A());
    // NOTE: The Z flag is also updated in RL_r8.
    reg_.clear_Z_flag();
}

// RLC

void Cpu::RLC_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    RLC_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::RLC_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= msbit;
    reg_.update_C_flag(msbit != 0);
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(result == 0);
    r8 = result;
}

// RLCA

void Cpu::RLCA()
{
    RLC_r8(reg_.get_A());
    // NOTE: The Z flag is also updated in RLC_r8.
    reg_.clear_Z_flag();
}

// RR

void Cpu::RR_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    RR_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::RR_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t lsbit = result & 0x01;
    result >>= 1;
    result |= (reg_.read_C_flag() << 7);
    reg_.update_C_flag(lsbit != 0);
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(result == 0);
    r8 = result;
}

// RRA

void Cpu::RRA()
{
    RR_r8(reg_.get_A());
    // NOTE: The Z flag is also updated in RR_r8.
    reg_.clear_Z_flag();
}

// RRC

void Cpu::RRC_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    RRC_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::RRC_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t lsbit = result & 0x01;
    result >>= 1;
    result |= (lsbit << 7);
    reg_.update_C_flag(lsbit != 0);
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(result == 0);
    r8 = result;
}

// RRCA

void Cpu::RRCA()
{
    RRC_r8(reg_.get_A());
    // NOTE: The Z flag is also updated in RRC_r8.
    reg_.clear_Z_flag();
}

void Cpu::RST_f(uint8_t f)
{
    PUSH_r16(reg_.get_PC());
    reg_.write_PC(0x0000 + f);
}

// SBC

void Cpu::SBC_A_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    SBC_A_n8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::SBC_A_n8(uint8_t n8)
{
    uint16_t result = static_cast<uint16_t>(reg_.read_A()) - n8 - reg_.read_C_flag();
    reg_.update_H_flag(((n8 & 0x0F) + reg_.read_C_flag()) > (reg_.read_A() & 0x0F));
    reg_.update_C_flag(reg_.read_A() < n8 + reg_.read_C_flag());
    reg_.set_N_flag();
    reg_.update_Z_flag(static_cast<uint8_t>(result) == 0);
    reg_.write_A(static_cast<uint8_t>(result));
}

void Cpu::SBC_A_r8(uint8_t& r8)
{
    SBC_A_n8(r8);
}

// SCF

void Cpu::SCF()
{
    reg_.set_C_flag();
    reg_.clear_H_flag();
    reg_.clear_N_flag();
}

// SET

void Cpu::SET_n3_HL(uint8_t n3)
{
    uint8_t temp = memory_.read(reg_.read_HL());
    SET_n3_r8(n3, temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::SET_n3_r8(uint8_t n3, uint8_t& r8)
{
    r8 |= 0x01 << n3;
}

// SLA

void Cpu::SLA_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    SLA_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::SLA_r8(uint8_t& r8)
{
    uint8_t result = r8;
    result <<= 1;
    result &= ~(0x01);
    reg_.update_C_flag(((r8 >> 7) & 0x01) != 0);
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(result == 0);
    r8 = result;
}

// SRA

void Cpu::SRA_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    SRA_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::SRA_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msb = r8 & (0x01 << 7);
    result >>= 1;
    result |= msb;
    reg_.update_C_flag(r8 & 0x01);
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(result == 0);
    r8 = result;
}

// SRL

void Cpu::SRL_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    SRL_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::SRL_r8(uint8_t& r8)
{
    uint8_t result = r8;
    result >>= 1;
    result &= ~(0x01 << 7);
    reg_.update_C_flag(r8 & 0x01);
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(result == 0);
    r8 = result;
}

// STOP

void Cpu::STOP()
{
    is_stopped_ = true;
    // TODO: Turn off display
}

// SUB

void Cpu::SUB_A_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    SUB_A_n8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::SUB_A_n8(uint8_t n8)
{
    uint16_t result = reg_.read_A() - n8;
    reg_.update_C_flag(reg_.read_A() < n8);
//    reg.update_C_flag(result > 0xFF);
    reg_.update_H_flag((n8 & 0x0F) > (reg_.read_A() & 0x0F));
    reg_.set_N_flag();
    reg_.update_Z_flag(static_cast<uint8_t>(result) == 0);
    reg_.write_A(static_cast<uint8_t>(result));
}

void Cpu::SUB_A_r8(uint8_t& r8)
{
    SUB_A_n8(r8);
}

// SWAP

void Cpu::SWAP_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    SWAP_r8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::SWAP_r8(uint8_t& r8)
{
    r8 = static_cast<uint8_t>(((r8 & 0x0F) << 4) | ((r8 & 0xF0) >> 4));
    reg_.clear_C_flag();
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(r8 == 0);
}

// XOR

void Cpu::XOR_HL()
{
    uint8_t temp = memory_.read(reg_.read_HL());
    XOR_n8(temp);
    memory_.write(reg_.read_HL(), temp);
}

void Cpu::XOR_n8(uint8_t n8)
{
    reg_.write_A(reg_.get_A() ^ n8);
    reg_.clear_C_flag();
    reg_.clear_H_flag();
    reg_.clear_N_flag();
    reg_.update_Z_flag(reg_.read_A() == 0);
}

void Cpu::XOR_r8(uint8_t& r8)
{
    XOR_n8(r8);
}
