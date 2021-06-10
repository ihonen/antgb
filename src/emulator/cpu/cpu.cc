#include "cpu.hh"

#include "emulator/memory/memorybus.hh"
#include <array>
#include <iomanip>
#include <iostream>

Cpu::Cpu(MemoryBus* mem, CpuRegisters& registers)
    : mem(mem)
    , reg(registers)
{
    trace_log.open("trace.log", std::ofstream::out);
    hard_reset();
}

Cpu::~Cpu()
{
    std::cerr << "vblank interrupts: " << vblank_irqs << std::endl;
    std::cerr << "lcdstat interrupts: " << lcdstat_irqs << std::endl;
    std::cerr << "timer interrupts: " << timer_irqs << std::endl;
    std::cerr << "serial interrupts: " << serial_irqs << std::endl;
    std::cerr << "joypad interrupts: " << joypad_irqs << std::endl;
}

void Cpu::set_PC(uint16_t value)
{
    reg.write_PC(value);
}

void Cpu::restart()
{
    hard_reset();
}

void Cpu::hard_reset()
{
    reg = CpuRegisters();
    reg.post_bootram_reset();

    current_instruction = nullptr;
    branch_taken = false;
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
    is_halted = false;
    is_stopped = false;
    clock_cycles = 0;
}

void Cpu::execute(const uint8_t* instruction)
{
    if (!instruction) instruction = mem->get(reg.read_PC());
    assert(instruction);
    current_instruction = instruction;
    branch_taken = false;

    if (EI_countdown > 0)
        --EI_countdown;

    if (has_pending_requests())
    {
        auto interrupt = next_request();

        if (interrupt.source != NoInterrupt)
        {
            bool was_halted = is_halted;
            is_halted = false;
            if (interrupt.source == JoypadInterrupt)
            {
                // TODO: Does this require an actual
                // interrupt or just a button press?
                is_stopped = false;
            }

            if (reg.read_IME())
            {
                if (was_halted)
                    clock_cycles += 4;
                reg.write_IME(0);
                clear_interrupt(interrupt.source);
                jump_to_isr(interrupt.vector_address);
                return;
            }
        }
    }

    if (!is_halted && !is_stopped)
    {
        const InstructionInfo* op_info = (*current_instruction == 0xCB) ?
                                    &CB_INSTRUCTION_TABLE[current_instruction[1]] :
                                    &INSTRUCTION_TABLE[*current_instruction];

        reg.write_PC(reg.read_PC() + op_info->len_bytes);

        decode_and_dispatch(current_instruction);

        if (branch_taken)
            clock_cycles += op_info->cycles_on_action;
        else
            clock_cycles += op_info->cycles_on_no_action;
    }
    else
    {
        // Halted or stopped, so advance time by one machine cycle.
        clock_cycles += 4;
    }

    if (DI_countdown == 0)
    {
        reg.write_IME(0);
        DI_countdown = NO_COUNTDOWN;
        EI_countdown = NO_COUNTDOWN;
    }
    else if (EI_countdown == 0)
    {
        reg.write_IME(1);
        DI_countdown = NO_COUNTDOWN;
        EI_countdown = NO_COUNTDOWN;
    }
}

void Cpu::invalid_opcode()
{
    throw OpcodeError(0, *current_instruction);
}

void Cpu::jump_to_isr(addr_t vector_address)
{
    if (vector_address == 0x00) return;

    // cerr << "Jumping to interrupt vector @ " << std::hex << vector_address << endl;
    // trace_log << "Jumping to interrupt vector @ " << std::hex << vector_address << endl;

    reg.write_IME(0);
    PUSH_r16(reg.get_PC());
    reg.write_PC(vector_address);
    current_instruction = mem->get(reg.read_PC());
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
    clock_cycles += 20;
}

/* ADC */

void Cpu::ADC_A_HL()
{
    ADC_A_n8(mem->read(reg.read_HL()));
}

void Cpu::ADC_A_n8(uint8_t n8)
{
    uint16_t result = reg.read_A() + n8 + reg.read_C_flag();
    reg.update_H_flag(((reg.read_A() & 0x0F) + (n8 & 0x0F) + reg.read_C_flag()) > 0x0F);
    // Can't call before H_flag_update.
    reg.update_C_flag(result > 0xFF);
    reg.clear_N_flag();
    reg.update_Z_flag(static_cast<uint8_t>(result) == 0);
    reg.write_A(static_cast<uint8_t>(result));
}

void Cpu::ADC_A_r8(uint8_t& r8)
{
    ADC_A_n8(r8);
}

/* ADD */

void Cpu::ADD_A_HL()
{
    ADD_A_n8(mem->read(reg.read_HL()));
}

void Cpu::ADD_A_n8(uint8_t n8)
{
    uint16_t result = reg.read_A() + n8;
    reg.update_C_flag(result > 0xFF);
    reg.update_H_flag((reg.read_A() & 0x0F) + (n8 & 0x0F) > 0x0F);
    reg.clear_N_flag();
    reg.update_Z_flag(static_cast<uint8_t>(result) == 0);
    reg.write_A(static_cast<uint8_t>(result));
}

void Cpu::ADD_A_r8(uint8_t& r8)
{
    ADD_A_n8(r8);
}

void Cpu::ADD_HL_r16(uint16_t& r16)
{
    uint32_t result = reg.read_HL() + r16;
    reg.update_C_flag(result > 0xFFFF);
    reg.update_H_flag(((reg.read_HL() & 0x0FFF) + (r16 & 0x0FFF)) > 0x0FFF);
    reg.clear_N_flag();
    reg.write_HL(static_cast<uint16_t>(result));
}

void Cpu::ADD_SP_e8(int8_t e8)
{
    uint32_t result = static_cast<uint32_t>(reg.read_SP()) + e8;
    reg.update_C_flag(((reg.read_SP() & 0x00FF) + static_cast<uint8_t>(e8)) > 0x00FF);
    reg.update_H_flag(((reg.read_SP() & 0x000F) + (static_cast<uint8_t>(e8 & 0x0F))) > 0x0F);
    reg.clear_N_flag();
    reg.clear_Z_flag();
    reg.write_SP(static_cast<uint16_t>(result));
}

/* AND */

void Cpu::AND_n8(uint8_t n8)
{
    reg.write_A(reg.read_A() & n8);
    reg.clear_C_flag();
    reg.set_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(reg.read_A() == 0);
}

void Cpu::AND_HL()
{
    AND_n8(mem->read(reg.read_HL()));
}

void Cpu::AND_r8(uint8_t& r8)
{
    AND_n8(r8);
}

/* BIT */
void Cpu::BIT_n3_HL(uint8_t n3)
{
    uint8_t temp = mem->read(reg.read_HL());
    BIT_n3_r8(n3, temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::BIT_n3_r8(uint8_t n3, uint8_t& r8)
{
    reg.set_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(!((r8 >> n3) & 0x01));
}

/* CALL */

void Cpu::CALL_n16(uint16_t n16)
{
    PUSH_r16(reg.get_PC());
    reg.write_PC(n16);
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
    reg.update_C_flag(!reg.read_C_flag());
    reg.clear_H_flag();
    reg.clear_N_flag();
}

/* CP */

void Cpu::CP_HL()
{
    CP_n8(mem->read(reg.read_HL()));
}

void Cpu::CP_n8(uint8_t n8)
{
    reg.update_C_flag(reg.read_A() < n8);
    reg.update_H_flag((reg.read_A() & 0x0F) < (n8 & 0x0F));
    reg.set_N_flag();
    reg.update_Z_flag(reg.read_A() == n8);
}

void Cpu::CP_r8(uint8_t& r8)
{
    CP_n8(r8);
}

/* CPL */

void Cpu::CPL()
{
    reg.write_A(~(reg.read_A()));
    reg.set_H_flag();
    reg.set_N_flag();
}

/* DAA */

void Cpu::DAA()
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

/* DEC */

void Cpu::DEC_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    DEC_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::DEC_r16(uint16_t& r16)
{
    --(r16);
}

void Cpu::DEC_r8(uint8_t& r8)
{
    --(r8);
    reg.update_H_flag((r8 & 0x0F) == 0x0F);
    reg.set_N_flag();
    reg.update_Z_flag(r8 == 0);
}

/* DI */

void Cpu::DI()
{
    // No delay in DI.
    DI_countdown = 0;
}

/* EI */
void Cpu::EI()
{
    EI_countdown = 1;
}

/* HALT */

void Cpu::HALT()
{
    // This is documented here:
    // https://www.reddit.com/r/EmuDev/comments/5bfb2t/a_subtlety_about_the_gameboy_z80_halt_instruction/

    // TODO: Implement more accurately.
    is_halted = true;

    reg.write_PC(reg.read_PC() + 1); // Next instruction is skipped (DMG bug)
}

/* INC */

void Cpu::INC_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    INC_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::INC_r16(uint16_t& r16)
{
    ++(r16);
}

void Cpu::INC_r8(uint8_t& r8)
{
    uint16_t result = r8 + 1;
    reg.update_H_flag((r8 & 0x0F) + 1 > 0x0F);
    reg.clear_N_flag();
    reg.update_Z_flag(static_cast<uint8_t>(result) == 0);
    r8 = static_cast<uint8_t>(result);
}

/* JP */

void Cpu::JP_HL()
{
    JP_n16(reg.read_HL());
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
    reg.write_PC(n16);
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
    reg.write_PC(reg.read_PC() + n8);
    branch_taken = true;
}

/* LD */

void Cpu::LD_C_A()
{
    mem->write(0xFF00 + reg.read_C(), reg.read_A());
}

void Cpu::LD_HL_n8(uint8_t n8)
{
    mem->write(reg.get_HL(), n8);
}

void Cpu::LD_HL_r8(uint8_t& r8)
{
    LD_HL_n8(r8);
}

void Cpu::LD_n16_A(uint16_t n16)
{
    mem->write(n16, reg.read_A());
}

void Cpu::LD_n16_SP(uint16_t n16)
{
    mem->write(n16, reg.read_SP() & 0x00FF);
    mem->write(n16 + 1, (reg.read_SP() & 0xFF00) >> 8);
}

void Cpu::LD_r16_A(uint16_t& r16)
{
    mem->write(r16, reg.read_A());
}

void Cpu::LD_A_C()
{
    reg.write_A(mem->read(0xFF00 + reg.read_C()));
}

void Cpu::LD_A_n16(uint16_t n16)
{
    reg.write_A(mem->read(n16));
}

void Cpu::LD_A_r16(uint16_t& r16)
{
    LD_A_n16(r16);
}

void Cpu::LD_HL_SP_e8(int8_t e8)
{
    uint32_t result = static_cast<uint32_t>(reg.read_SP()) + e8;
    reg.update_C_flag(((reg.read_SP() & 0x00FF) + static_cast<uint8_t>(e8)) > 0x00FF);
//    reg.update_C_flag(result > 0xFFFF);
    reg.update_H_flag((reg.read_SP() & 0x000F) + static_cast<uint16_t>(e8 & 0x0F) > 0x0F);
    reg.clear_N_flag();
    reg.clear_Z_flag();
    reg.write_HL(static_cast<uint16_t>(result));
}

void Cpu::LD_r16_n16(uint16_t& r16, uint16_t n16)
{
    r16 = n16;
}

void Cpu::LD_r8_HL(uint8_t& r8)
{
    r8 = mem->read(reg.read_HL());
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
    reg.write_SP(reg.read_HL());
}

void Cpu::LDD_HL_A()
{
    mem->write(reg.read_HL(), reg.read_A());
    reg.write_HL(reg.read_HL() - 1);
}

void Cpu::LDD_A_HL()
{
    reg.write_A(mem->read(reg.read_HL()));
    reg.write_HL(reg.read_HL() - 1);
}

void Cpu::LDH_n8_A(uint8_t n8)
{
    mem->write(0xFF00 + n8,  reg.read_A());
}

void Cpu::LDH_A_n8(uint8_t n8)
{
    reg.write_A(mem->read(0xFF00 + n8));
}

void Cpu::LDI_HL_A()
{
    mem->write(reg.read_HL(), reg.read_A());
    reg.write_HL(reg.read_HL() + 1);
}

void Cpu::LDI_A_HL()
{
    reg.write_A(mem->read(reg.read_HL()));
    reg.write_HL(reg.read_HL() + 1);
}

/* NOP */

void Cpu::NOP()
{

}

/* OR */

void Cpu::OR_HL()
{
    OR_n8(mem->read(reg.read_HL()));
}

void Cpu::OR_n8(uint8_t n8)
{
    reg.write_A(reg.read_A() | n8);
    reg.clear_C_flag();
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(reg.read_A() == 0);
}

void Cpu::OR_r8(uint8_t& r8)
{
    OR_n8(r8);
}

/* POP */

void Cpu::POP_r16(uint16_t& r16)
{
    r16 = static_cast<uint16_t>(mem->read(reg.read_SP()));
    reg.write_SP(reg.read_SP() + 1);
    r16 |= static_cast<uint16_t>(mem->read(reg.read_SP())) << 8;
    reg.write_SP(reg.read_SP() + 1);
}

/* PUSH */

void Cpu::PUSH_r16(uint16_t& r16)
{
    reg.write_SP(reg.read_SP() - 1);
    mem->write(reg.read_SP(), static_cast<uint8_t>(r16 >> 8));
    reg.write_SP(reg.read_SP() - 1);
    mem->write(reg.read_SP(), static_cast<uint8_t>(r16));
}

/* RES */

void Cpu::RES_n3_HL(uint8_t n3)
{
    uint8_t temp = mem->read(reg.read_HL());
    RES_n3_r8(n3, temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RES_n3_r8(uint8_t n3, uint8_t& r8)
{
    r8 &= ~(0x01 << n3);
}

/* RET */

void Cpu::RET()
{
    POP_r16(reg.get_PC());
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
    reg.write_IME(1);
    DI_countdown = NO_COUNTDOWN;
    EI_countdown = NO_COUNTDOWN;
}

void Cpu::RL_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    RL_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RL_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= reg.read_C_flag();
    reg.update_C_flag(msbit != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

/* RLA */

void Cpu::RLA()
{
    RL_r8(reg.get_A());
    // NOTE: The Z flag is also updated in RL_r8.
    reg.clear_Z_flag();
}

/* RLC */

void Cpu::RLC_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    RLC_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RLC_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msbit = (result >> 7) & 0x01;
    result <<= 1;
    result |= msbit;
    reg.update_C_flag(msbit != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

/* RLCA */

void Cpu::RLCA()
{
    RLC_r8(reg.get_A());
    // NOTE: The Z flag is also updated in RLC_r8.
    reg.clear_Z_flag();
}

/* RR */

void Cpu::RR_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    RR_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RR_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t lsbit = result & 0x01;
    result >>= 1;
    result |= (reg.read_C_flag() << 7);
    reg.update_C_flag(lsbit != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

/* RRA */

void Cpu::RRA()
{
    RR_r8(reg.get_A());
    // NOTE: The Z flag is also updated in RR_r8.
    reg.clear_Z_flag();
}

/* RRC */

void Cpu::RRC_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    RRC_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::RRC_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t lsbit = result & 0x01;
    result >>= 1;
    result |= (lsbit << 7);
    reg.update_C_flag(lsbit != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

/* RRCA */

void Cpu::RRCA()
{
    RRC_r8(reg.get_A());
    // NOTE: The Z flag is also updated in RRC_r8.
    reg.clear_Z_flag();
}

void Cpu::RST_f(uint8_t f)
{
    PUSH_r16(reg.get_PC());
    reg.write_PC(0x0000 + f);
}

/* SBC */

void Cpu::SBC_A_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    SBC_A_n8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SBC_A_n8(uint8_t n8)
{
    uint16_t result = static_cast<uint16_t>(reg.read_A()) - n8 - reg.read_C_flag();
    reg.update_H_flag(((n8 & 0x0F) + reg.read_C_flag()) > (reg.read_A() & 0x0F));
    reg.update_C_flag(reg.read_A() < n8 + reg.read_C_flag());
    reg.set_N_flag();
    reg.update_Z_flag(static_cast<uint8_t>(result) == 0);
    reg.write_A(static_cast<uint8_t>(result));
}

void Cpu::SBC_A_r8(uint8_t& r8)
{
    SBC_A_n8(r8);
}

/* SCF */

void Cpu::SCF()
{
    reg.set_C_flag();
    reg.clear_H_flag();
    reg.clear_N_flag();
}

/* SET */

void Cpu::SET_n3_HL(uint8_t n3)
{
    uint8_t temp = mem->read(reg.read_HL());
    SET_n3_r8(n3, temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SET_n3_r8(uint8_t n3, uint8_t& r8)
{
    r8 |= 0x01 << n3;
}

/* SLA */

void Cpu::SLA_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    SLA_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SLA_r8(uint8_t& r8)
{
    uint8_t result = r8;
    result <<= 1;
    result &= ~(0x01);
    reg.update_C_flag(((r8 >> 7) & 0x01) != 0);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

/* SRA */

void Cpu::SRA_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    SRA_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SRA_r8(uint8_t& r8)
{
    uint8_t result = r8;
    uint8_t msb = r8 & (0x01 << 7);
    result >>= 1;
    result |= msb;
    reg.update_C_flag(r8 & 0x01);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
    r8 = result;
}

/* SRL */

void Cpu::SRL_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    SRL_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SRL_r8(uint8_t& r8)
{
    uint8_t result = r8;
    result >>= 1;
    result &= ~(0x01 << 7);
    reg.update_C_flag(r8 & 0x01);
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(result == 0);
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
    uint8_t temp = mem->read(reg.read_HL());
    SUB_A_n8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SUB_A_n8(uint8_t n8)
{
    uint16_t result = reg.read_A() - n8;
    reg.update_C_flag(reg.read_A() < n8);
//    reg.update_C_flag(result > 0xFF);
    reg.update_H_flag((n8 & 0x0F) > (reg.read_A() & 0x0F));
    reg.set_N_flag();
    reg.update_Z_flag(static_cast<uint8_t>(result) == 0);
    reg.write_A(static_cast<uint8_t>(result));
}

void Cpu::SUB_A_r8(uint8_t& r8)
{
    SUB_A_n8(r8);
}

/* SWAP */

void Cpu::SWAP_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    SWAP_r8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::SWAP_r8(uint8_t& r8)
{
    r8 = static_cast<uint8_t>(((r8 & 0x0F) << 4) | ((r8 & 0xF0) >> 4));
    reg.clear_C_flag();
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(r8 == 0);
}

/* XOR */

void Cpu::XOR_HL()
{
    uint8_t temp = mem->read(reg.read_HL());
    XOR_n8(temp);
    mem->write(reg.read_HL(), temp);
}

void Cpu::XOR_n8(uint8_t n8)
{
    reg.write_A(reg.get_A() ^ n8);
    reg.clear_C_flag();
    reg.clear_H_flag();
    reg.clear_N_flag();
    reg.update_Z_flag(reg.read_A() == 0);
}

void Cpu::XOR_r8(uint8_t& r8)
{
    XOR_n8(r8);
}

const std::array<const Cpu::InstructionInfo, 256> Cpu::INSTRUCTION_TABLE =
{{
    {1, 4,  4, },
    {3, 12, 12,},
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {2, 8,  8, },
    {1, 4,  4, },
    {3, 20, 20,},
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {2, 8,  8, },
    {1, 4,  4, },
    {2, 4,  4, },
    {3, 12, 12,},
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {2, 8,  8, },
    {1, 4,  4, },
    {2, 12, 12,},
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {2, 8,  8, },
    {1, 4,  4, },
    {2, 12, 8, },
    {3, 12, 12,},
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {2, 8,  8, },
    {1, 4,  4, },
    {2, 12, 8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {2, 8,  8, },
    {1, 4,  4, },
    {2, 12, 8, },
    {3, 12, 12,},
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 12, 12,},
    {1, 12, 12,},
    {2, 12, 12,},
    {1, 4,  4, },
    {2, 12, 8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {2, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 4,  4, },
    {1, 8,  8, },
    {1, 4,  4, },
    {1, 20, 8, },
    {1, 12, 12,},
    {3, 16, 12,},
    {3, 16, 16,},
    {3, 24, 12,},
    {1, 16, 16,},
    {2, 8,  8, },
    {1, 16, 16,},
    {1, 20, 8, },
    {1, 16, 16,},
    {3, 16, 12,},
    {1, 4,  4, },
    {3, 24, 12,},
    {3, 24, 24,},
    {2, 8,  8, },
    {1, 16, 16,},
    {1, 20, 8, },
    {1, 12, 12,},
    {3, 16, 12,},
    {0, 0,  0, },
    {3, 24, 12,},
    {1, 16, 16,},
    {2, 8,  8, },
    {1, 16, 16,},
    {1, 20, 8, },
    {1, 16, 16,},
    {3, 16, 12,},
    {0, 0,  0, },
    {3, 24, 12,},
    {0, 0,  0, },
    {2, 8,  8, },
    {1, 16, 16,},
    {2, 12, 12,},
    {1, 12, 12,},
    {1, 8,  8, },
    {0, 0,  0, },
    {0, 0,  0, },
    {1, 16, 16,},
    {2, 8,  8, },
    {1, 16, 16,},
    {2, 16, 16,},
    {1, 4,  4, },
    {3, 16, 16,},
    {0, 0,  0, },
    {0, 0,  0, },
    {0, 0,  0, },
    {2, 8,  8, },
    {1, 16, 16,},
    {2, 12, 12,},
    {1, 12, 12,},
    {1, 8,  8, },
    {1, 4,  4, },
    {0, 0,  0, },
    {1, 16, 16,},
    {2, 8,  8, },
    {1, 16, 16,},
    {2, 12, 12,},
    {1, 8,  8, },
    {3, 16, 16,},
    {1, 1,  4, },
    {0, 0,  0, },
    {0, 0,  0, },
    {2, 8,  8, },
    {1, 16, 16,}
}};

const std::array<const Cpu::InstructionInfo, 256> Cpu::CB_INSTRUCTION_TABLE =
{{
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 12, 12,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 12, 12,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 12, 12,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 12, 12,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 12, 12,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 12, 12,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 12, 12,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 12, 12,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 8,  8, },
    {2, 16, 16,},
    {2, 8,  8, }
}};
