#pragma once

#include "cpuregisters.hh"
#include "interrupts.hh"
#include "emulator/common/exceptions.hh"
#include "emulator/common/macros.hh"
#include "emulator/common/types.hh"
#include <array>
#include <fstream>


class MemoryBus;
class Timer;

class Cpu
{
public:
    struct OpcodeInfo
    {
        uint8_t len_bytes;
        uint8_t tcycles_if_branch_taken;
        uint8_t tcycles_if_branch_not_taken;
    };

    static constexpr uint64_t CLK_FREQ_Hz = 4194304;

    static const std::array<const OpcodeInfo, 256> OPCODE_TABLE;
    static const std::array<const OpcodeInfo, 256> CB_OPCODE_TABLE;

    Cpu(CpuRegisters& registers,
        Interrupts& interrupts,
        MemoryBus& mem,
        Timer& timer
    );
    ~Cpu();

    CpuRegisters& get_registers();

    inline emutime_t get_elapsed_tcycles();
    inline void set_elapsed_tcycles(emutime_t tcycles);

    void restart();
    void post_bootram_reset();

    void execute_next();

protected:
    void dispatch(const uint8_t* instruction);
    void jump_to_interrupt_handler(const Interrupts::Irq& interrupt);

    inline uint8_t get_immediate8(const uint8_t* instruction);
    inline uint16_t get_immediate16(const uint8_t* instruction);

    [[noreturn]] void invalid_opcode();

    void ADC_A_HL();
    void ADC_A_n8(uint8_t u8);
    void ADC_A_r8(uint8_t& r8);
    void ADD_A_HL();
    void ADD_A_n8(uint8_t u8);
    void ADD_A_r8(uint8_t& r8);
    void ADD_HL_r16(uint16_t& r16);
    void ADD_SP_e8(int8_t s8);
    void AND_n8(uint8_t u8);
    void AND_HL();
    void AND_r8(uint8_t& r8);
    void BIT_n3_HL(uint8_t n);
    void BIT_n3_r8(uint8_t n, uint8_t& r8);
    void CALL_cc_n16(bool cc, uint16_t n16);
    void CALL_n16(uint16_t n16);
    void CCF();
    void CP_HL();
    void CP_n8(uint8_t n8);
    void CP_r8(uint8_t& r8);
    void CPL();
    void DAA();
    void DEC_HL();
    void DEC_r16(uint16_t& r16);
    void DEC_r8(uint8_t& r8);
    void DI();
    void EI();
    void HALT();
    void INC_HL();
    void INC_r16(uint16_t& r16);
    void INC_r8(uint8_t& r8);
    void JP_HL();
    void JP_cc_n16(bool cc, uint16_t n16);
    void JP_n16(uint16_t n16);
    void JR_cc_n8(bool cc, int8_t n8);
    void JR_n8(int8_t n8);
    void LD_C_A();
    void LD_HL_n8(uint8_t n8);
    void LD_HL_r8(uint8_t& r8);
    void LD_n16_A(uint16_t n16);
    void LD_n16_SP(uint16_t n16);
    void LD_r16_A(uint16_t& r16);
    void LD_A_C();
    void LD_A_n16(uint16_t n16);
    void LD_A_r16(uint16_t& r16);
    void LD_HL_SP_e8(int8_t e8);
    void LD_r16_n16(uint16_t& r16, uint16_t n16);
    void LD_r8_HL(uint8_t& r8);
    void LD_r8_n8(uint8_t& r8, uint8_t n8);
    void LD_r8_r8(uint8_t& r8_1, uint8_t& r8_2);
    void LD_SP_HL();
    void LDD_HL_A();
    void LDD_A_HL();
    void LDH_n8_A(uint8_t n8);
    void LDH_A_n8(uint8_t n8);
    void LDI_HL_A();
    void LDI_A_HL();
    void NOP();
    void OR_HL();
    void OR_n8(uint8_t n8);
    void OR_r8(uint8_t& r8);
    void POP_r16(uint16_t& r16);
    void PUSH_r16(uint16_t& r16);
    void RES_n3_HL(uint8_t n3);
    void RES_n3_r8(uint8_t n3, uint8_t& r8);
    void RET();
    void RET_cc(bool cc);
    void RETI();
    void RL_HL();
    void RL_r8(uint8_t& r8);
    void RLA();
    void RLC_HL();
    void RLC_r8(uint8_t& r8);
    void RLCA();
    void RR_HL();
    void RR_r8(uint8_t& r8);
    void RRA();
    void RRC_HL();
    void RRC_r8(uint8_t& r8);
    void RRCA();
    void RST_f(uint8_t f);
    void SBC_A_HL();
    void SBC_A_n8(uint8_t n8);
    void SBC_A_r8(uint8_t& r8);
    void SCF();
    void SET_n3_HL(uint8_t n3);
    void SET_n3_r8(uint8_t n3, uint8_t& r8);
    void SLA_HL();
    void SLA_r8(uint8_t& r8);
    void SRA_HL();
    void SRA_r8(uint8_t& r8);
    void SRL_HL();
    void SRL_r8(uint8_t& r8);
    void STOP();
    void SUB_A_HL();
    void SUB_A_n8(uint8_t n8);
    void SUB_A_r8(uint8_t& r8);
    void SWAP_HL();
    void SWAP_r8(uint8_t& r8);
    void XOR_HL();
    void XOR_n8(uint8_t n8);
    void XOR_r8(uint8_t& r8);

    CpuRegisters& reg_;
    Interrupts& interrupts_;
    MemoryBus& memory_;
    Timer& timer_;

    std::ofstream trace_log_;

    const uint8_t* current_instruction_;
    bool branch_was_taken_;
    bool is_halted_;
    bool is_stopped_;
    emutime_t elapsed_tcycles_;
};

FORCE_INLINE emutime_t Cpu::get_elapsed_tcycles()
{
    return elapsed_tcycles_;
}

FORCE_INLINE void Cpu::set_elapsed_tcycles(emutime_t tcycles)
{
    elapsed_tcycles_ = tcycles;
}

FORCE_INLINE uint8_t Cpu::get_immediate8(const uint8_t* instruction)
{
    return instruction[1];
}

FORCE_INLINE uint16_t Cpu::get_immediate16(const uint8_t* instruction)
{
    return (static_cast<uint16_t>(instruction[1]))
           | (static_cast<uint16_t>(instruction[2]) << 8);
}
