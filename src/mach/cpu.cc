#include "cpu.hh"

#include "../util/disassembler.hh"
#include <iomanip>
#include <iostream>

Disassembler disassembler;

CPU::CPU(MMU& mmu_, IRC& irc_) :
    mmu(mmu_),
    irc(irc_)
{
    init();
}

void CPU::restart()
{
    init();
}

void CPU::init()
{
    reg = {0};
    curr_instr = nullptr;
    branch_taken = false;
    DI_status = IMEStatus::DO_NOTHING;
    EI_status = IMEStatus::DO_NOTHING;
    is_halted = false;
    is_stopped = false;
    clock_cycles = 0;
    is_interrupted = false;

    irc.ime_flag_clear();
    mmu.write(HWREG_IF_ADDR, 0x00);
    mmu.write(HWREG_IF_ADDR, 0x00);
    AF = 0x01B0;
    BC = 0x0013;
    DE = 0x00D8;
    HL = 0x014D;
    SP = 0xFFFE;
    mmu.write(0xFF05, 0x00); // TIMA
    mmu.write(0xFF06, 0x00); // TMA
    mmu.write(0xFF07, 0x00); // TAC
    mmu.write(0xFF10, 0x80); // NR10
    mmu.write(0xFF11, 0xBF); // NR11
    mmu.write(0xFF12, 0xF3); // NR12
    mmu.write(0xFF14, 0xBF); // NR14
    mmu.write(0xFF16, 0x3F); // NR21
    mmu.write(0xFF17, 0x00); // NR22
    mmu.write(0xFF19, 0xBF); // NR24
    mmu.write(0xFF1A, 0x7F); // NR30
    mmu.write(0xFF1B, 0xFF); // NR31
    mmu.write(0xFF1C, 0x9F); // NR32
    mmu.write(0xFF1E, 0xBF); // NR33
    mmu.write(0xFF20, 0xFF); // NR41
    mmu.write(0xFF21, 0x00); // NR42
    mmu.write(0xFF22, 0x00); // NR43
    mmu.write(0xFF23, 0xBF); // NR44
    mmu.write(0xFF24, 0x77); // NR50
    mmu.write(0xFF25, 0xF3); // NR51
    mmu.write(0xFF26, 0xF1); // NR52
    mmu.write(0xFF40, 0x91); // LCDC
    mmu.write(0xFF42, 0x00); // SCY
    mmu.write(0xFF43, 0x00); // SCX
    mmu.write(0xFF45, 0x00); // LYC
    mmu.write(0xFF47, 0xFC); // BGP
    mmu.write(0xFF48, 0xFF); // OBP0
    mmu.write(0xFF49, 0xFF); // OBP1
    mmu.write(0xFF4A, 0x00); // WY
    mmu.write(0xFF4B, 0x00); // WX
    mmu.write(0xFFFF, 0x00); // IE

    PC = 0x0100;
}

void CPU::execute(const uint8_t* instruction)
{
    if (!instruction) instruction = &mmu.mem[PC];
    curr_instr = instruction;
    branch_taken = false;

    // Nested interrupts are possible if the user has set IME
    // in the handler, so no if statement here.
    if (irc.has_active_requests())
    {
        auto interrupt = irc.accept_next_request();
        if (interrupt.source != IRC::NoInterrupt)
        {
            if (is_halted) is_halted = false;
            if (is_stopped && interrupt.source == IRC::JoypadInterrupt)
                is_stopped = false;
            jump_to_isr(interrupt.vector_address);
            return;
        }

    }

    if (is_halted) return;
    if (is_stopped) return;

    if (DI_status == IMEStatus::RESET_NEXT_CYCLE)
        DI_status = IMEStatus::RESET_THIS_CYCLE;
    else if (EI_status == IMEStatus::SET_NEXT_CYCLE)
        EI_status = IMEStatus::SET_THIS_CYCLE;

    const InstrInfo* op_info = (*curr_instr == 0xCB) ?
                                &CB_INSTR_TABLE[curr_instr[1]] :
                                &INSTR_TABLE[*curr_instr];

/*
    std::cout << "@" << std::setw(5) << std::left << PC << ":   "
              << std::setw(16) << std::left << disassembler.disassemble(const_cast<uint8_t*>(instruction))
              << static_cast<int>(op_info->len_bytes) << " bytes"
              << std::endl;
*/
    // PC has to be incremented before instruction execution.
    PC += op_info->len_bytes;

    if (op_info->handler) (this->*(op_info->handler))();
    else invalid_opcode();

    if (branch_taken)
        clock_cycles += op_info->cycles_on_action;
    else
    {
        clock_cycles += op_info->cycles_on_no_action;
        //PC += op_info->len_bytes;
    }

    if (DI_status == IMEStatus::RESET_THIS_CYCLE) irc.ime_flag_clear();
    else if (EI_status == IMEStatus::SET_THIS_CYCLE) irc.ime_flag_set();
}

uint8_t CPU::extract_immediate8(const uint8_t* instruction)
{
    if (!instruction) instruction = curr_instr;
    return curr_instr[1];
}

uint16_t CPU::extract_immediate16(const uint8_t* instruction)
{
    if (!instruction) instruction = curr_instr;
    return (static_cast<uint16_t>(instruction[1])) |
           (static_cast<uint16_t>(instruction[2]) << 8);
}

uint64_t CPU::get_cycles()
{
    return clock_cycles;
}

void CPU::reset_cycles()
{
    clock_cycles = 0;
}

void CPU::invalid_opcode()
{
    throw OpcodeError(0, *curr_instr);
}
