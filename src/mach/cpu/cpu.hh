#ifndef CPU_HH
#define CPU_HH

#include <array>
#include <cstdint>
#include <vector>

class Cpu
{
public:
    void execute(uint8_t* opcode);
private:
    uint8_t get_flag(uint8_t pos);
    void    set_flag(uint8_t pos, uint8_t val);
    uint8_t get_cf();
    uint8_t get_hf();
    uint8_t get_nf();
    uint8_t get_zf();
    void    set_cf();
    void    set_hf();
    void    set_nf();
    void    set_zf();
    void    clear_cf();
    void    clear_hf();
    void    clear_nf();
    void    clear_zf();
    void    update_cf(bool cond);
    void    update_hf(bool cond);
    void    update_nf(bool cond);
    void    update_zf(bool cond);

    void    disable_interrupts();
    void    enable_interrupts();
    uint8_t get_IME();
    void    set_IME();
    void    clear_IME();

    // Positions of the flag bits in flag register (F).
    static const uint8_t CF_BIT_POS = 4;
    static const uint8_t HF_BIT_POS = 5;
    static const uint8_t NF_BIT_POS = 6;
    static const uint8_t ZF_BIT_POS = 7;

    // Main memory.
    uint8_t mem[0xFFFF + 1];

    // BC, DE, HL, AF, PC, SP
    uint16_t reg[6] = {0};
    uint16_t* BC = &reg[0];
    uint16_t* DE = &reg[1];
    uint16_t* HL = &reg[2];
    uint16_t* AF = &reg[3];
    uint16_t* PC = &reg[5];
    uint16_t* SP = &reg[4];
    // NOTE: the following assumes little-endian.
    uint8_t*  B  = reinterpret_cast<uint8_t*>(BC) + 0;
    uint8_t*  C  = reinterpret_cast<uint8_t*>(BC) + 1;
    uint8_t*  D  = reinterpret_cast<uint8_t*>(DE) + 0;
    uint8_t*  E  = reinterpret_cast<uint8_t*>(DE) + 1;
    uint8_t*  H  = reinterpret_cast<uint8_t*>(HL) + 0;
    uint8_t*  L  = reinterpret_cast<uint8_t*>(HL) + 1;
    uint8_t*  A  = reinterpret_cast<uint8_t*>(AF) + 0;
    uint8_t*  F  = reinterpret_cast<uint8_t*>(AF) + 1;
    // Some instructions "index" the registers (both 16- and 8-bit)
    // so store them accordingly.
    const std::array<uint16_t*, 6> reg16 = {BC, DE, HL, AF, PC, SP};
    const std::array<uint8_t*, 8>  reg8  = {B, C, D, E, H, L, A, F};

    typedef struct
    {
        // The length of the instruction in bytes.
        uint8_t len = 1;
        // The duration of the instruction in clock cycles if action
        // was taken.
        uint8_t dur_s = 0;
        // The duration of the instruction in clock cycles if action
        // was not taken.
        uint8_t dur_f = 0;
        // The function that is called when the instruction
        // is encountered.
        void (Cpu::*handler)() = nullptr;
    } OpcodeInfo;

    // This table will contain the information related to "normal" opcodes.
    static const OpcodeInfo OP_INFO[256];
    // This table will contain the information related to opcodes prefixed
    // with 0xCB. The 0xCB prefix only means that another byte should be
    // fetched and that byte defines the operation to be taken.
    static const OpcodeInfo CB_OP_INFO[256];

    // Pointer to the current instruction in execution. This is not necessarily
    // the same as PC since execute() can take a pointer to any location.
    uint8_t* curr_op;

    // Indicates whether the instruction that was just executed was executed
    // successfully. This is relevant because some instructions take a varying
    // amount of clock cycles based on whether a certain action was taken or
    // not.
    bool op_success;

    enum class IntStatusChange {NOT_SCHEDULED, SCHEDULED, TRIGGER};
    // If either of these values is TRIGGER after
    // executing an instuction, do the corresponding operation on the
    // Interrupt Master Enable flag.
    // TODO: If this is to be done accurately, IME has to be changed
    // after one machine cycle, not one instruction.
    // TODO: What to do if EI or DI is called twice or more in a row?
    enum IntStatusChange DI_status = IntStatusChange::NOT_SCHEDULED;
    enum IntStatusChange EI_status = IntStatusChange::NOT_SCHEDULED;

    void ADC_A_HL();
    void ADC_A_n8(uint8_t u8);
    void ADC_A_r8(uint8_t* r8);
    void ADD_A_HL();
    void ADD_A_n8(uint8_t u8);
    void ADD_A_r8(uint8_t* r8);
    void ADD_HL_r16(uint16_t* r16);
    void ADD_SP_e8(int8_t s8);
    void AND_n8(uint8_t u8);
    void AND_HL();
    void AND_r8(uint8_t* r8);
    void BIT_n3_HL(uint8_t n);
    void BIT_n3_r8(uint8_t n, uint8_t* r8);
    void CALL_cc_n16(bool cc, uint16_t n16);
    void CALL_n16(uint16_t n16);
    void CCF();
    void CP_HL();
    void CP_n8(uint8_t n8);
    void CP_r8(uint8_t* r8);
    void CPL();
    void DEC_HL();
    void DEC_r16(uint16_t* r16);
    void DEC_r8(uint8_t* r8);
    void DI();
    void EI();
    void INC_HL();
    void INC_r16(uint16_t* r16);
    void INC_r8(uint8_t* r8);
    void JP_HL();
    void JP_cc_n16(bool cc, uint16_t n16);
    void JP_n16(uint16_t n16);
    void JR_cc_n8(bool cc, int8_t n8);
    void JR_n8(int8_t n8);
    void LD_C_A();
    void LD_HL_n8(uint8_t n8);
    void LD_HL_r8(uint8_t* r8);
    void LD_n16_A(uint16_t n16);
    void LD_n16_SP(uint16_t n16);
    void LD_r16_A(uint16_t* r16);
    void LD_A_C();
    void LD_A_n16(uint16_t n16);
    void LD_A_r16(uint16_t* r16);
    void LD_HL_SP_e8(int8_t e8);
    void LD_r16_n16(uint16_t* r16, uint16_t n16);
    void LD_r8_HL(uint8_t* r8);
    void LD_r8_n8(uint8_t* r8, uint8_t n8);
    void LD_r8_r8(uint8_t* r8_1, uint8_t* r8_2);
    void LD_SP_HL();
    void LDD_HL_A();
    void LDD_A_HL();
    void LDH_n8_A(uint8_t n8);
    void LDH_A_n8(uint8_t n8);
    void LDI_HL_A();
    void LDI_A_HL();
    void NOP();

    void op_00(); void op_01(); void op_02(); void op_03();
    void op_04(); void op_05(); void op_06(); void op_07();
    void op_08(); void op_09(); void op_0A(); void op_0B();
    void op_0C(); void op_0D(); void op_0E(); void op_0F();
    void op_10(); void op_11(); void op_12(); void op_13();
    void op_14(); void op_15(); void op_16(); void op_17();
    void op_18(); void op_19(); void op_1A(); void op_1B();
    void op_1C(); void op_1D(); void op_1E(); void op_1F();
    void op_20(); void op_21(); void op_22(); void op_23();
    void op_24(); void op_25(); void op_26(); void op_27();
    void op_28(); void op_29(); void op_2A(); void op_2B();
    void op_2C(); void op_2D(); void op_2E(); void op_2F();
    void op_30(); void op_31(); void op_32(); void op_33();
    void op_34(); void op_35(); void op_36(); void op_37();
    void op_38(); void op_39(); void op_3A(); void op_3B();
    void op_3C(); void op_3D(); void op_3E(); void op_3F();
    void op_40(); void op_41(); void op_42(); void op_43();
    void op_44(); void op_45(); void op_46(); void op_47();
    void op_48(); void op_49(); void op_4A(); void op_4B();
    void op_4C(); void op_4D(); void op_4E(); void op_4F();
    void op_50(); void op_51(); void op_52(); void op_53();
    void op_54(); void op_55(); void op_56(); void op_57();
    void op_58(); void op_59(); void op_5A(); void op_5B();
    void op_5C(); void op_5D(); void op_5E(); void op_5F();
    void op_60(); void op_61(); void op_62(); void op_63();
    void op_64(); void op_65(); void op_66(); void op_67();
    void op_68(); void op_69(); void op_6A(); void op_6B();
    void op_6C(); void op_6D(); void op_6E(); void op_6F();
    void op_70(); void op_71(); void op_72(); void op_73();
    void op_74(); void op_75(); void op_76(); void op_77();
    void op_78(); void op_79(); void op_7A(); void op_7B();
    void op_7C(); void op_7D(); void op_7E(); void op_7F();
    void op_80(); void op_81(); void op_82(); void op_83();
    void op_84(); void op_85(); void op_86(); void op_87();
    void op_88(); void op_89(); void op_8A(); void op_8B();
    void op_8C(); void op_8D(); void op_8E(); void op_8F();
    void op_90(); void op_91(); void op_92(); void op_93();
    void op_94(); void op_95(); void op_96(); void op_97();
    void op_98(); void op_99(); void op_9A(); void op_9B();
    void op_9C(); void op_9D(); void op_9E(); void op_9F();
    void op_A0(); void op_A1(); void op_A2(); void op_A3();
    void op_A4(); void op_A5(); void op_A6(); void op_A7();
    void op_A8(); void op_A9(); void op_AA(); void op_AB();
    void op_AC(); void op_AD(); void op_AE(); void op_AF();
    void op_B0(); void op_B1(); void op_B2(); void op_B3();
    void op_B4(); void op_B5(); void op_B6(); void op_B7();
    void op_B8(); void op_B9(); void op_BA(); void op_BB();
    void op_BC(); void op_BD(); void op_BE(); void op_BF();
    void op_C0(); void op_C1(); void op_C2(); void op_C3();
    void op_C4(); void op_C5(); void op_C6(); void op_C7();
    void op_C8(); void op_C9(); void op_CA(); void op_CB();
    void op_CC(); void op_CD(); void op_CE(); void op_CF();
    void op_D0(); void op_D1(); void op_D2(); void op_D3();
    void op_D4(); void op_D5(); void op_D6(); void op_D7();
    void op_D8(); void op_D9(); void op_DA(); void op_DB();
    void op_DC(); void op_DD(); void op_DE(); void op_DF();
    void op_E0(); void op_E1(); void op_E2(); void op_E3();
    void op_E4(); void op_E5(); void op_E6(); void op_E7();
    void op_E8(); void op_E9(); void op_EA(); void op_EB();
    void op_EC(); void op_ED(); void op_EE(); void op_EF();
    void op_F0(); void op_F1(); void op_F2(); void op_F3();
    void op_F4(); void op_F5(); void op_F6(); void op_F7();
    void op_F8(); void op_F9(); void op_FA(); void op_FB();
    void op_FC(); void op_FD(); void op_FE(); void op_FF();

    void op_CB_00(); void op_CB_01(); void op_CB_02(); void op_CB_03();
    void op_CB_04(); void op_CB_05(); void op_CB_06(); void op_CB_07();
    void op_CB_08(); void op_CB_09(); void op_CB_0A(); void op_CB_0B();
    void op_CB_0C(); void op_CB_0D(); void op_CB_0E(); void op_CB_0F();
    void op_CB_10(); void op_CB_11(); void op_CB_12(); void op_CB_13();
    void op_CB_14(); void op_CB_15(); void op_CB_16(); void op_CB_17();
    void op_CB_18(); void op_CB_19(); void op_CB_1A(); void op_CB_1B();
    void op_CB_1C(); void op_CB_1D(); void op_CB_1E(); void op_CB_1F();
    void op_CB_20(); void op_CB_21(); void op_CB_22(); void op_CB_23();
    void op_CB_24(); void op_CB_25(); void op_CB_26(); void op_CB_27();
    void op_CB_28(); void op_CB_29(); void op_CB_2A(); void op_CB_2B();
    void op_CB_2C(); void op_CB_2D(); void op_CB_2E(); void op_CB_2F();
    void op_CB_30(); void op_CB_31(); void op_CB_32(); void op_CB_33();
    void op_CB_34(); void op_CB_35(); void op_CB_36(); void op_CB_37();
    void op_CB_38(); void op_CB_39(); void op_CB_3A(); void op_CB_3B();
    void op_CB_3C(); void op_CB_3D(); void op_CB_3E(); void op_CB_3F();
    void op_CB_40(); void op_CB_41(); void op_CB_42(); void op_CB_43();
    void op_CB_44(); void op_CB_45(); void op_CB_46(); void op_CB_47();
    void op_CB_48(); void op_CB_49(); void op_CB_4A(); void op_CB_4B();
    void op_CB_4C(); void op_CB_4D(); void op_CB_4E(); void op_CB_4F();
    void op_CB_50(); void op_CB_51(); void op_CB_52(); void op_CB_53();
    void op_CB_54(); void op_CB_55(); void op_CB_56(); void op_CB_57();
    void op_CB_58(); void op_CB_59(); void op_CB_5A(); void op_CB_5B();
    void op_CB_5C(); void op_CB_5D(); void op_CB_5E(); void op_CB_5F();
    void op_CB_60(); void op_CB_61(); void op_CB_62(); void op_CB_63();
    void op_CB_64(); void op_CB_65(); void op_CB_66(); void op_CB_67();
    void op_CB_68(); void op_CB_69(); void op_CB_6A(); void op_CB_6B();
    void op_CB_6C(); void op_CB_6D(); void op_CB_6E(); void op_CB_6F();
    void op_CB_70(); void op_CB_71(); void op_CB_72(); void op_CB_73();
    void op_CB_74(); void op_CB_75(); void op_CB_76(); void op_CB_77();
    void op_CB_78(); void op_CB_79(); void op_CB_7A(); void op_CB_7B();
    void op_CB_7C(); void op_CB_7D(); void op_CB_7E(); void op_CB_7F();
    void op_CB_80(); void op_CB_81(); void op_CB_82(); void op_CB_83();
    void op_CB_84(); void op_CB_85(); void op_CB_86(); void op_CB_87();
    void op_CB_88(); void op_CB_89(); void op_CB_8A(); void op_CB_8B();
    void op_CB_8C(); void op_CB_8D(); void op_CB_8E(); void op_CB_8F();
    void op_CB_90(); void op_CB_91(); void op_CB_92(); void op_CB_93();
    void op_CB_94(); void op_CB_95(); void op_CB_96(); void op_CB_97();
    void op_CB_98(); void op_CB_99(); void op_CB_9A(); void op_CB_9B();
    void op_CB_9C(); void op_CB_9D(); void op_CB_9E(); void op_CB_9F();
    void op_CB_A0(); void op_CB_A1(); void op_CB_A2(); void op_CB_A3();
    void op_CB_A4(); void op_CB_A5(); void op_CB_A6(); void op_CB_A7();
    void op_CB_A8(); void op_CB_A9(); void op_CB_AA(); void op_CB_AB();
    void op_CB_AC(); void op_CB_AD(); void op_CB_AE(); void op_CB_AF();
    void op_CB_B0(); void op_CB_B1(); void op_CB_B2(); void op_CB_B3();
    void op_CB_B4(); void op_CB_B5(); void op_CB_B6(); void op_CB_B7();
    void op_CB_B8(); void op_CB_B9(); void op_CB_BA(); void op_CB_BB();
    void op_CB_BC(); void op_CB_BD(); void op_CB_BE(); void op_CB_BF();
    void op_CB_C0(); void op_CB_C1(); void op_CB_C2(); void op_CB_C3();
    void op_CB_C4(); void op_CB_C5(); void op_CB_C6(); void op_CB_C7();
    void op_CB_C8(); void op_CB_C9(); void op_CB_CA(); void op_CB_CB();
    void op_CB_CC(); void op_CB_CD(); void op_CB_CE(); void op_CB_CF();
    void op_CB_D0(); void op_CB_D1(); void op_CB_D2(); void op_CB_D3();
    void op_CB_D4(); void op_CB_D5(); void op_CB_D6(); void op_CB_D7();
    void op_CB_D8(); void op_CB_D9(); void op_CB_DA(); void op_CB_DB();
    void op_CB_DC(); void op_CB_DD(); void op_CB_DE(); void op_CB_DF();
    void op_CB_E0(); void op_CB_E1(); void op_CB_E2(); void op_CB_E3();
    void op_CB_E4(); void op_CB_E5(); void op_CB_E6(); void op_CB_E7();
    void op_CB_E8(); void op_CB_E9(); void op_CB_EA(); void op_CB_EB();
    void op_CB_EC(); void op_CB_ED(); void op_CB_EE(); void op_CB_EF();
    void op_CB_F0(); void op_CB_F1(); void op_CB_F2(); void op_CB_F3();
    void op_CB_F4(); void op_CB_F5(); void op_CB_F6(); void op_CB_F7();
    void op_CB_F8(); void op_CB_F9(); void op_CB_FA(); void op_CB_FB();
    void op_CB_FC(); void op_CB_FD(); void op_CB_FE(); void op_CB_FF();

};

#endif // CPU_HH
