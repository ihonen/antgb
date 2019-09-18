#include "cpu.hh"

void Cpu::op_00()
{
    NOP();
}

void Cpu::op_01()
{
    uint16_t n16 = 0x0000;
    n16 |= static_cast<uint16_t>(curr_op[1]) << 8;
    n16 |= curr_op[2];
    LD_r16_n16(BC, n16);
}

void Cpu::op_02()
{
    LD_r16_A(BC);
}

void Cpu::op_03()
{
    INC_r16(BC);
}

void Cpu::op_04()
{
    INC_r8(B);
}

void Cpu::op_05()
{
    DEC_r8(B);
}

void Cpu::op_06()
{
    LD_r8_n8(B, curr_op[1]);
}

void Cpu::op_07()
{
    RLCA();
}

void Cpu::op_08()
{
    uint16_t n16 = 0x0000;
    n16 |= static_cast<uint16_t>(curr_op[1]) << 8;
    n16 |= curr_op[2];
    LD_n16_SP(n16);
}

void Cpu::op_09()
{
    ADD_HL_r16(BC);
}

void Cpu::op_0A()
{
    LD_A_r16(BC);
}

void Cpu::op_0B()
{
    DEC_r16(BC);
}

void Cpu::op_0C()
{
    INC_r8(C);
}

void Cpu::op_0D()
{
    DEC_r8(C);
}

void Cpu::op_0E()
{
    LD_r8_n8(C, curr_op[1]);
}

void Cpu::op_0F()
{
    RRCA();
}

void Cpu::op_10()
{

}

void Cpu::op_11()
{
    uint16_t n16 = 0x0000;
    n16 |= static_cast<uint16_t>(curr_op[1]) << 8;
    n16 |= curr_op[2];
    LD_r16_n16(DE, n16);
}

void Cpu::op_12()
{
    LD_r16_A(DE);
}

void Cpu::op_13()
{
    INC_r16(DE);
}

void Cpu::op_14()
{
    INC_r8(D);
}

void Cpu::op_15()
{
    DEC_r8(D);
}

void Cpu::op_16()
{
    LD_r8_n8(D, curr_op[1]);
}

void Cpu::op_17()
{
    RLA();
}

void Cpu::op_18()
{
    JR_n8(static_cast<int8_t>(curr_op[1]));
}

void Cpu::op_19()
{
    ADD_HL_r16(DE);
}

void Cpu::op_1A()
{
    LD_A_r16(DE);
}

void Cpu::op_1B()
{
    DEC_r16(DE);
}

void Cpu::op_1C()
{
    INC_r8(E);
}

void Cpu::op_1D()
{
    DEC_r8(E);
}

void Cpu::op_1E()
{
    LD_r8_n8(E, curr_op[1]);
}

void Cpu::op_1F()
{
    RRA();
}

void Cpu::op_20()
{
    JR_cc_n8(!get_zf(), static_cast<int8_t>(curr_op[1]));
}

void Cpu::op_21()
{
    uint16_t n16 = 0x0000;
    n16 |= static_cast<uint16_t>(curr_op[1]) << 8;
    n16 |= curr_op[2];
    LD_r16_n16(HL, n16);
}

void Cpu::op_22()
{
    LDI_HL_A();
}

void Cpu::op_23()
{
    INC_r16(HL);
}

void Cpu::op_24()
{
    INC_r8(H);
}

void Cpu::op_25()
{
    DEC_r8(H);
}

void Cpu::op_26()
{
    LD_r8_n8(H, curr_op[1]);
}

void Cpu::op_27()
{

}

void Cpu::op_28()
{
    JR_cc_n8(get_zf(), static_cast<int8_t>(curr_op[1]));
}

void Cpu::op_29()
{
    ADD_HL_r16(HL);
}

void Cpu::op_2A()
{
    LDI_A_HL();
}

void Cpu::op_2B()
{
    DEC_r16(HL);
}

void Cpu::op_2C()
{
    INC_r8(L);
}

void Cpu::op_2D()
{
    DEC_r8(L);
}

void Cpu::op_2E()
{
    LD_r8_n8(L, curr_op[1]);
}

void Cpu::op_2F()
{
    CPL();
}

void Cpu::op_30()
{
    JR_cc_n8(!get_cf(), static_cast<int8_t>(curr_op[1]));
}

void Cpu::op_31()
{
    uint16_t n16 = 0x0000;
    n16 |= static_cast<uint16_t>(curr_op[1]) << 8;
    n16 |= curr_op[2];
    LD_r16_n16(SP, n16);
}

void Cpu::op_32()
{
    LDD_HL_A();
}

void Cpu::op_33()
{
    INC_r16(SP);
}

void Cpu::op_34()
{
    INC_HL();
}

void Cpu::op_35()
{
    DEC_HL();
}

void Cpu::op_36()
{
    LD_HL_n8(curr_op[1]);
}

void Cpu::op_37()
{
    SCF();
}

void Cpu::op_38()
{
    JR_cc_n8(get_cf(), static_cast<int8_t>(curr_op[1]));
}

void Cpu::op_39()
{
    ADD_HL_r16(SP);
}

void Cpu::op_3A()
{
    LDD_A_HL();
}

void Cpu::op_3B()
{
    DEC_r16(SP);
}

void Cpu::op_3C()
{
    INC_r8(A);
}

void Cpu::op_3D()
{
    DEC_r8(A);
}

void Cpu::op_3E()
{
    LD_r8_n8(A, curr_op[1]);
}

void Cpu::op_3F()
{
    CCF();
}

void Cpu::op_40()
{
    LD_r8_r8(B, B);
}

void Cpu::op_41()
{
    LD_r8_r8(B, C);
}

void Cpu::op_42()
{
    LD_r8_r8(B, D);
}

void Cpu::op_43()
{
    LD_r8_r8(B, E);
}

void Cpu::op_44()
{
    LD_r8_r8(B, H);
}

void Cpu::op_45()
{
    LD_r8_r8(B, L);
}

void Cpu::op_46()
{
    LD_r8_HL(B);
}

void Cpu::op_47()
{
    LD_r8_r8(B, A);
}

void Cpu::op_48()
{
    LD_r8_r8(C, B);
}

void Cpu::op_49()
{
    LD_r8_r8(C, C);
}

void Cpu::op_4A()
{
    LD_r8_r8(C, D);
}

void Cpu::op_4B()
{
    LD_r8_r8(C, E);
}

void Cpu::op_4C()
{
    LD_r8_r8(C, H);
}

void Cpu::op_4D()
{
    LD_r8_r8(C, L);
}

void Cpu::op_4E()
{
    LD_r8_HL(C);
}

void Cpu::op_4F()
{
    LD_r8_r8(C, A);
}

void Cpu::op_50()
{
    LD_r8_r8(D, B);
}

void Cpu::op_51()
{
    LD_r8_r8(D, C);
}

void Cpu::op_52()
{
    LD_r8_r8(D, D);
}

void Cpu::op_53()
{
    LD_r8_r8(D, E);
}

void Cpu::op_54()
{
    LD_r8_r8(D, H);
}

void Cpu::op_55()
{
    LD_r8_r8(D, L);
}

void Cpu::op_56()
{
    LD_r8_HL(D);
}

void Cpu::op_57()
{
    LD_r8_r8(D, A);
}

void Cpu::op_58()
{
    LD_r8_r8(E, B);
}

void Cpu::op_59()
{
    LD_r8_r8(E, C);
}

void Cpu::op_5A()
{
    LD_r8_r8(E, D);
}

void Cpu::op_5B()
{
    LD_r8_r8(E, E);
}

void Cpu::op_5C()
{
    LD_r8_r8(E, H);
}

void Cpu::op_5D()
{
    LD_r8_r8(E, L);
}

void Cpu::op_5E()
{
    LD_r8_HL(E);
}

void Cpu::op_5F()
{
    LD_r8_r8(E, A);
}

void Cpu::op_60()
{
    LD_r8_r8(H, B);
}

void Cpu::op_61()
{
    LD_r8_r8(H, C);
}

void Cpu::op_62()
{
    LD_r8_r8(H, D);
}

void Cpu::op_63()
{
    LD_r8_r8(H, E);
}

void Cpu::op_64()
{
    LD_r8_r8(H, H);
}

void Cpu::op_65()
{
    LD_r8_r8(H, L);
}

void Cpu::op_66()
{
    LD_r8_HL(H);
}

void Cpu::op_67()
{
    LD_r8_r8(H, A);
}

void Cpu::op_68()
{
    LD_r8_r8(L, B);
}

void Cpu::op_69()
{
    LD_r8_r8(L, C);
}

void Cpu::op_6A()
{
    LD_r8_r8(L, D);
}

void Cpu::op_6B()
{
    LD_r8_r8(L, E);
}

void Cpu::op_6C()
{
    LD_r8_r8(L, H);
}

void Cpu::op_6D()
{
    LD_r8_r8(L, L);
}

void Cpu::op_6E()
{
    LD_r8_HL(L);
}

void Cpu::op_6F()
{
    LD_r8_r8(L, A);
}

void Cpu::op_70()
{
    LD_HL_r8(B);
}

void Cpu::op_71()
{
    LD_HL_r8(C);
}

void Cpu::op_72()
{
    LD_HL_r8(D);
}

void Cpu::op_73()
{
    LD_HL_r8(E);
}

void Cpu::op_74()
{
    LD_HL_r8(H);
}

void Cpu::op_75()
{
    LD_HL_r8(L);
}

void Cpu::op_76()
{

}

void Cpu::op_77()
{
    LD_HL_r8(A);
}

void Cpu::op_78()
{
    LD_r8_r8(A, B);
}

void Cpu::op_79()
{
    LD_r8_r8(A, C);
}

void Cpu::op_7A()
{
    LD_r8_r8(A, D);
}

void Cpu::op_7B()
{
    LD_r8_r8(A, E);
}

void Cpu::op_7C()
{
    LD_r8_r8(A, H);
}

void Cpu::op_7D()
{
    LD_r8_r8(A, L);
}

void Cpu::op_7E()
{
    LD_A_r16(HL);
}

void Cpu::op_7F()
{
    LD_r8_r8(A, A);
}

void Cpu::op_80()
{
    ADD_A_r8(B);
}

void Cpu::op_81()
{
    ADD_A_r8(C);
}

void Cpu::op_82()
{
    ADD_A_r8(D);
}

void Cpu::op_83()
{
    ADD_A_r8(E);
}

void Cpu::op_84()
{
    ADD_A_r8(H);
}

void Cpu::op_85()
{
    ADD_A_r8(L);
}

void Cpu::op_86()
{
    ADD_A_HL();
}

void Cpu::op_87()
{
    ADD_A_r8(A);
}

void Cpu::op_88()
{
    ADC_A_r8(B);
}

void Cpu::op_89()
{
    ADC_A_r8(C);
}

void Cpu::op_8A()
{
    ADC_A_r8(D);
}

void Cpu::op_8B()
{
    ADC_A_r8(E);
}

void Cpu::op_8C()
{
    ADC_A_r8(H);
}

void Cpu::op_8D()
{
    ADC_A_r8(L);
}

void Cpu::op_8E()
{
    ADC_A_HL();
}

void Cpu::op_8F()
{
    ADC_A_r8(A);
}

void Cpu::op_90()
{
    SUB_A_r8(B);
}

void Cpu::op_91()
{
    SUB_A_r8(C);
}

void Cpu::op_92()
{
    SUB_A_r8(D);
}

void Cpu::op_93()
{
    SUB_A_r8(E);
}

void Cpu::op_94()
{
    SUB_A_r8(H);
}

void Cpu::op_95()
{
    SUB_A_r8(L);
}

void Cpu::op_96()
{

}

void Cpu::op_97()
{
    SUB_A_r8(A);
}

void Cpu::op_98()
{
    SBC_A_r8(B);
}

void Cpu::op_99()
{
    SBC_A_r8(C);
}

void Cpu::op_9A()
{
    SBC_A_r8(D);
}

void Cpu::op_9B()
{
    SBC_A_r8(E);
}

void Cpu::op_9C()
{
    SBC_A_r8(H);
}

void Cpu::op_9D()
{
    SBC_A_r8(L);
}

void Cpu::op_9E()
{
    SBC_A_HL();
}

void Cpu::op_9F()
{
    SBC_A_r8(A);
}

void Cpu::op_A0()
{
    AND_r8(B);
}

void Cpu::op_A1()
{
    AND_r8(C);
}

void Cpu::op_A2()
{
    AND_r8(D);
}

void Cpu::op_A3()
{
    AND_r8(E);
}

void Cpu::op_A4()
{
    AND_r8(H);
}

void Cpu::op_A5()
{
    AND_r8(L);
}

void Cpu::op_A6()
{
    AND_HL();
}

void Cpu::op_A7()
{
    AND_r8(A);
}

void Cpu::op_A8()
{
    XOR_r8(B);
}

void Cpu::op_A9()
{
    XOR_r8(C);
}

void Cpu::op_AA()
{
    XOR_r8(D);
}

void Cpu::op_AB()
{
    XOR_r8(E);
}

void Cpu::op_AC()
{
    XOR_r8(H);
}

void Cpu::op_AD()
{
    XOR_r8(L);
}

void Cpu::op_AE()
{
    XOR_HL();
}

void Cpu::op_AF()
{
    XOR_r8(A);
}

void Cpu::op_B0()
{
    OR_r8(B);
}

void Cpu::op_B1()
{
    OR_r8(C);
}

void Cpu::op_B2()
{
    OR_r8(D);
}

void Cpu::op_B3()
{
    OR_r8(E);
}

void Cpu::op_B4()
{
    OR_r8(H);
}

void Cpu::op_B5()
{
    OR_r8(L);
}

void Cpu::op_B6()
{
    OR_HL();
}

void Cpu::op_B7()
{
    OR_r8(A);
}

void Cpu::op_B8()
{
    CP_r8(B);
}

void Cpu::op_B9()
{
    CP_r8(C);
}

void Cpu::op_BA()
{
    CP_r8(D);
}

void Cpu::op_BB()
{
    CP_r8(E);
}

void Cpu::op_BC()
{
    CP_r8(H);
}

void Cpu::op_BD()
{
    CP_r8(L);
}

void Cpu::op_BE()
{
    CP_HL();
}

void Cpu::op_BF()
{
    CP_r8(A);
}

void Cpu::op_C0()
{
    RET_cc(!get_zf());
}

void Cpu::op_C1()
{
    POP_r16(BC);
}

void Cpu::op_C2()
{
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    JP_cc_n16(!get_zf(), jump_addr);
}

void Cpu::op_C3()
{
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    JP_n16(jump_addr);
}

void Cpu::op_C4()
{
    // TODO: Verify correctness
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    CALL_cc_n16(!get_zf(), jump_addr);
}

void Cpu::op_C5()
{
    PUSH_r16(BC);
}

void Cpu::op_C6()
{
    ADD_A_n8(curr_op[1]);
}

void Cpu::op_C7()
{

}

void Cpu::op_C8()
{
    RET_cc(get_zf());
}

void Cpu::op_C9()
{
    RET();
}

void Cpu::op_CA()
{
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    JP_cc_n16(get_zf(), jump_addr);
}

void Cpu::op_CB()
{

}

void Cpu::op_CC()
{
    // TODO: Verify correctness
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    CALL_cc_n16(get_zf(), jump_addr);
}

void Cpu::op_CD()
{
    // TODO: Verify correctness
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    CALL_n16(jump_addr);
}

void Cpu::op_CE()
{
    ADC_A_n8(curr_op[1]);
}

void Cpu::op_CF()
{

}

void Cpu::op_D0()
{
    RET_cc(!get_cf());
}

void Cpu::op_D1()
{
    POP_r16(DE);
}

void Cpu::op_D2()
{
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    JP_cc_n16(!get_cf(), jump_addr);
}

void Cpu::op_D3()
{

}

void Cpu::op_D4()
{
    // TODO: Verify correctness
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    CALL_cc_n16(!get_cf(), jump_addr);
}

void Cpu::op_D5()
{
    PUSH_r16(DE);
}

void Cpu::op_D6()
{

}

void Cpu::op_D7()
{

}

void Cpu::op_D8()
{
    RET_cc(get_cf());
}

void Cpu::op_D9()
{
    RETI();
}

void Cpu::op_DA()
{
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    JP_cc_n16(get_cf(), jump_addr);
}

void Cpu::op_DB()
{

}

void Cpu::op_DC()
{
    // TODO: Verify correctness
    uint16_t jump_addr = 0x0000;
    jump_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    jump_addr |= curr_op[2];
    CALL_cc_n16(get_cf(), jump_addr);
}

void Cpu::op_DD()
{

}

void Cpu::op_DE()
{
    SBC_A_n8(curr_op[1]);
}

void Cpu::op_DF()
{

}

void Cpu::op_E0()
{
    LDH_n8_A(curr_op[1]);
}

void Cpu::op_E1()
{
    POP_r16(HL);
}

void Cpu::op_E2()
{
    LD_C_A();
}

void Cpu::op_E3()
{

}

void Cpu::op_E4()
{

}

void Cpu::op_E5()
{
    PUSH_r16(HL);
}

void Cpu::op_E6()
{
    AND_n8(curr_op[1]);
}

void Cpu::op_E7()
{

}

void Cpu::op_E8()
{
    ADD_SP_e8(static_cast<int8_t>(curr_op[1]));
}

void Cpu::op_E9()
{
    JP_n16(*HL);
}

void Cpu::op_EA()
{
    uint16_t load_addr = 0x0000;
    load_addr |= static_cast<uint16_t>(curr_op[1]) << 8;
    load_addr |= curr_op[2];
    LD_n16_A(load_addr);
}

void Cpu::op_EB()
{

}

void Cpu::op_EC()
{

}

void Cpu::op_ED()
{

}

void Cpu::op_EE()
{
    XOR_n8(curr_op[1]);
}

void Cpu::op_EF()
{

}

void Cpu::op_F0()
{
    LDH_A_n8(curr_op[1]);
}

void Cpu::op_F1()
{
    POP_r16(AF);
}

void Cpu::op_F2()
{
    LD_A_C();
}

void Cpu::op_F3()
{
    DI();
}

void Cpu::op_F4()
{

}

void Cpu::op_F5()
{
    PUSH_r16(AF);
}

void Cpu::op_F6()
{
    OR_n8(curr_op[1]);
}

void Cpu::op_F7()
{

}

void Cpu::op_F8()
{
    LD_HL_SP_e8(static_cast<int8_t>(curr_op[1]));
}

void Cpu::op_F9()
{
    LD_SP_HL();
}

void Cpu::op_FA()
{
    uint16_t n16 = 0x0000;
    n16 |= static_cast<uint16_t>(curr_op[1]) << 8;
    n16 |= curr_op[2];
    LD_A_n16(n16);
}

void Cpu::op_FB()
{
    EI();
}

void Cpu::op_FC()
{

}

void Cpu::op_FD()
{

}

void Cpu::op_FE()
{
    CP_n8(curr_op[1]);
}

void Cpu::op_FF()
{

}



void Cpu::op_CB_00()
{
    RLC_r8(B);
}

void Cpu::op_CB_01()
{
    RLC_r8(C);
}

void Cpu::op_CB_02()
{
    RLC_r8(D);
}

void Cpu::op_CB_03()
{
    RLC_r8(E);
}

void Cpu::op_CB_04()
{
    RLC_r8(H);
}

void Cpu::op_CB_05()
{
    RLC_r8(L);
}

void Cpu::op_CB_06()
{
    RLC_HL();
}

void Cpu::op_CB_07()
{
    RLC_r8(A);
}

void Cpu::op_CB_08()
{
    RRC_r8(B);
}

void Cpu::op_CB_09()
{
    RRC_r8(C);
}

void Cpu::op_CB_0A()
{
    RRC_r8(D);
}

void Cpu::op_CB_0B()
{
    RRC_r8(E);
}

void Cpu::op_CB_0C()
{
    RRC_r8(H);
}

void Cpu::op_CB_0D()
{
    RRC_r8(L);
}

void Cpu::op_CB_0E()
{
    RRC_HL();
}

void Cpu::op_CB_0F()
{
    RRC_r8(A);
}

void Cpu::op_CB_10()
{
    RL_r8(B);
}

void Cpu::op_CB_11()
{
    RL_r8(C);
}

void Cpu::op_CB_12()
{
    RL_r8(D);
}

void Cpu::op_CB_13()
{
    RL_r8(E);
}

void Cpu::op_CB_14()
{
    RL_r8(H);
}

void Cpu::op_CB_15()
{
    RL_r8(L);
}

void Cpu::op_CB_16()
{
    RL_HL();
}

void Cpu::op_CB_17()
{
    RL_r8(A);
}

void Cpu::op_CB_18()
{
    RR_r8(B);
}

void Cpu::op_CB_19()
{
    RR_r8(C);
}

void Cpu::op_CB_1A()
{
    RR_r8(D);
}

void Cpu::op_CB_1B()
{
    RR_r8(E);
}

void Cpu::op_CB_1C()
{
    RR_r8(H);
}

void Cpu::op_CB_1D()
{
    RR_r8(L);
}

void Cpu::op_CB_1E()
{
    RR_HL();
}

void Cpu::op_CB_1F()
{
    RR_r8(A);
}

void Cpu::op_CB_20()
{
    SLA_r8(B);
}

void Cpu::op_CB_21()
{
    SLA_r8(C);
}

void Cpu::op_CB_22()
{
    SLA_r8(D);
}

void Cpu::op_CB_23()
{
    SLA_r8(E);
}

void Cpu::op_CB_24()
{
    SLA_r8(H);
}

void Cpu::op_CB_25()
{
    SLA_r8(L);
}

void Cpu::op_CB_26()
{
    SLA_HL();
}

void Cpu::op_CB_27()
{
    SLA_r8(A);
}

void Cpu::op_CB_28()
{
    SRA_r8(B);
}

void Cpu::op_CB_29()
{
    SRA_r8(C);
}

void Cpu::op_CB_2A()
{
    SRA_r8(D);
}

void Cpu::op_CB_2B()
{
    SRA_r8(E);
}

void Cpu::op_CB_2C()
{
    SRA_r8(H);
}

void Cpu::op_CB_2D()
{
    SRA_r8(L);
}

void Cpu::op_CB_2E()
{
    SRA_HL();
}

void Cpu::op_CB_2F()
{
    SRA_r8(A);
}

void Cpu::op_CB_30()
{
    SWAP_r8(B);
}

void Cpu::op_CB_31()
{
    SWAP_r8(C);
}

void Cpu::op_CB_32()
{
    SWAP_r8(D);
}

void Cpu::op_CB_33()
{
    SWAP_r8(E);
}

void Cpu::op_CB_34()
{
    SWAP_r8(H);
}

void Cpu::op_CB_35()
{
    SWAP_r8(L);
}

void Cpu::op_CB_36()
{
    SWAP_HL();
}

void Cpu::op_CB_37()
{
    SWAP_r8(A);
}

void Cpu::op_CB_38()
{
    SRL_r8(B);
}

void Cpu::op_CB_39()
{
    SRL_r8(C);
}

void Cpu::op_CB_3A()
{
    SRL_r8(D);
}

void Cpu::op_CB_3B()
{
    SRL_r8(E);
}

void Cpu::op_CB_3C()
{
    SRL_r8(H);
}

void Cpu::op_CB_3D()
{
    SRL_r8(L);
}

void Cpu::op_CB_3E()
{
    SRL_HL();
}

void Cpu::op_CB_3F()
{
    SRL_r8(A);
}

void Cpu::op_CB_40()
{
    BIT_n3_r8(0, B);
}

void Cpu::op_CB_41()
{
    BIT_n3_r8(0, C);
}

void Cpu::op_CB_42()
{
    BIT_n3_r8(0, D);
}

void Cpu::op_CB_43()
{
    BIT_n3_r8(0, E);
}

void Cpu::op_CB_44()
{
    BIT_n3_r8(0, H);
}

void Cpu::op_CB_45()
{
    BIT_n3_r8(0, L);
}

void Cpu::op_CB_46()
{
    BIT_n3_HL(0);
}

void Cpu::op_CB_47()
{
    BIT_n3_r8(0, A);
}

void Cpu::op_CB_48()
{
    BIT_n3_r8(1, B);
}

void Cpu::op_CB_49()
{
    BIT_n3_r8(1, C);
}

void Cpu::op_CB_4A()
{
    BIT_n3_r8(1, D);
}

void Cpu::op_CB_4B()
{
    BIT_n3_r8(1, E);
}

void Cpu::op_CB_4C()
{
    BIT_n3_r8(1, H);
}

void Cpu::op_CB_4D()
{
    BIT_n3_r8(1, L);
}

void Cpu::op_CB_4E()
{
    BIT_n3_HL(1);
}

void Cpu::op_CB_4F()
{
    BIT_n3_r8(1, A);
}

void Cpu::op_CB_50()
{
    BIT_n3_r8(2, B);
}

void Cpu::op_CB_51()
{
    BIT_n3_r8(2, C);
}

void Cpu::op_CB_52()
{
    BIT_n3_r8(2, D);
}

void Cpu::op_CB_53()
{
    BIT_n3_r8(2, E);
}

void Cpu::op_CB_54()
{
    BIT_n3_r8(2, H);
}

void Cpu::op_CB_55()
{
    BIT_n3_r8(2, L);
}

void Cpu::op_CB_56()
{
    BIT_n3_HL(2);
}

void Cpu::op_CB_57()
{
    BIT_n3_r8(2, A);
}

void Cpu::op_CB_58()
{
    BIT_n3_r8(3, B);
}

void Cpu::op_CB_59()
{
    BIT_n3_r8(3, C);
}

void Cpu::op_CB_5A()
{
    BIT_n3_r8(3, D);
}

void Cpu::op_CB_5B()
{
    BIT_n3_r8(3, E);
}

void Cpu::op_CB_5C()
{
    BIT_n3_r8(3, H);
}

void Cpu::op_CB_5D()
{
    BIT_n3_r8(3, L);
}

void Cpu::op_CB_5E()
{
    BIT_n3_HL(3);
}

void Cpu::op_CB_5F()
{
    BIT_n3_r8(3, A);
}

void Cpu::op_CB_60()
{
    BIT_n3_r8(4, B);
}

void Cpu::op_CB_61()
{
    BIT_n3_r8(4, C);
}

void Cpu::op_CB_62()
{
    BIT_n3_r8(4, D);
}

void Cpu::op_CB_63()
{
    BIT_n3_r8(4, E);
}

void Cpu::op_CB_64()
{
    BIT_n3_r8(4, H);
}

void Cpu::op_CB_65()
{
    BIT_n3_r8(4, L);
}

void Cpu::op_CB_66()
{
    BIT_n3_HL(4);
}

void Cpu::op_CB_67()
{
    BIT_n3_r8(4, A);
}

void Cpu::op_CB_68()
{
    BIT_n3_r8(5, B);
}

void Cpu::op_CB_69()
{
    BIT_n3_r8(5, C);
}

void Cpu::op_CB_6A()
{
    BIT_n3_r8(5, D);
}

void Cpu::op_CB_6B()
{
    BIT_n3_r8(5, E);
}

void Cpu::op_CB_6C()
{
    BIT_n3_r8(5, H);
}

void Cpu::op_CB_6D()
{
    BIT_n3_r8(5, L);
}

void Cpu::op_CB_6E()
{
    BIT_n3_HL(5);
}

void Cpu::op_CB_6F()
{
    BIT_n3_r8(5, A);
}

void Cpu::op_CB_70()
{
    BIT_n3_r8(6, B);
}

void Cpu::op_CB_71()
{
    BIT_n3_r8(6, C);
}

void Cpu::op_CB_72()
{
    BIT_n3_r8(6, D);
}

void Cpu::op_CB_73()
{
    BIT_n3_r8(6, E);
}

void Cpu::op_CB_74()
{
    BIT_n3_r8(6, H);
}

void Cpu::op_CB_75()
{
    BIT_n3_r8(6, L);
}

void Cpu::op_CB_76()
{
    BIT_n3_HL(6);
}

void Cpu::op_CB_77()
{
    BIT_n3_r8(6, A);
}

void Cpu::op_CB_78()
{
    BIT_n3_r8(7, B);
}

void Cpu::op_CB_79()
{
    BIT_n3_r8(7, C);
}

void Cpu::op_CB_7A()
{
    BIT_n3_r8(7, D);
}

void Cpu::op_CB_7B()
{
    BIT_n3_r8(7, E);
}

void Cpu::op_CB_7C()
{
    BIT_n3_r8(7, H);
}

void Cpu::op_CB_7D()
{
    BIT_n3_r8(7, L);
}

void Cpu::op_CB_7E()
{
    BIT_n3_HL(7);
}

void Cpu::op_CB_7F()
{
    BIT_n3_r8(7, A);
}

void Cpu::op_CB_80()
{
    RES_n3_r8(0, B);
}

void Cpu::op_CB_81()
{
    RES_n3_r8(0, C);
}

void Cpu::op_CB_82()
{
    RES_n3_r8(0, D);
}

void Cpu::op_CB_83()
{
    RES_n3_r8(0, E);
}

void Cpu::op_CB_84()
{
    RES_n3_r8(0, H);
}

void Cpu::op_CB_85()
{
    RES_n3_r8(0, L);
}

void Cpu::op_CB_86()
{
    RES_n3_HL(0);
}

void Cpu::op_CB_87()
{
    RES_n3_r8(0, A);
}

void Cpu::op_CB_88()
{
    RES_n3_r8(1, B);
}

void Cpu::op_CB_89()
{
    RES_n3_r8(1, C);
}

void Cpu::op_CB_8A()
{
    RES_n3_r8(1, D);
}

void Cpu::op_CB_8B()
{
    RES_n3_r8(1, E);
}

void Cpu::op_CB_8C()
{
    RES_n3_r8(1, H);
}

void Cpu::op_CB_8D()
{
    RES_n3_r8(1, L);
}

void Cpu::op_CB_8E()
{
    RES_n3_HL(1);
}

void Cpu::op_CB_8F()
{
    RES_n3_r8(1, A);
}

void Cpu::op_CB_90()
{
    RES_n3_r8(2, B);
}

void Cpu::op_CB_91()
{
    RES_n3_r8(2, C);
}

void Cpu::op_CB_92()
{
    RES_n3_r8(2, D);
}

void Cpu::op_CB_93()
{
    RES_n3_r8(2, E);
}

void Cpu::op_CB_94()
{
    RES_n3_r8(2, H);
}

void Cpu::op_CB_95()
{
    RES_n3_r8(2, L);
}

void Cpu::op_CB_96()
{
    RES_n3_HL(2);
}

void Cpu::op_CB_97()
{
    RES_n3_r8(2, A);
}

void Cpu::op_CB_98()
{
    RES_n3_r8(3, B);
}

void Cpu::op_CB_99()
{
    RES_n3_r8(3, C);
}

void Cpu::op_CB_9A()
{
    RES_n3_r8(3, D);
}

void Cpu::op_CB_9B()
{
    RES_n3_r8(3, E);
}

void Cpu::op_CB_9C()
{
    RES_n3_r8(3, H);
}

void Cpu::op_CB_9D()
{
    RES_n3_r8(3, L);
}

void Cpu::op_CB_9E()
{
    RES_n3_HL(3);
}

void Cpu::op_CB_9F()
{
    RES_n3_r8(3, A);
}

void Cpu::op_CB_A0()
{
    RES_n3_r8(4, B);
}

void Cpu::op_CB_A1()
{
    RES_n3_r8(4, C);
}

void Cpu::op_CB_A2()
{
    RES_n3_r8(4, D);
}

void Cpu::op_CB_A3()
{
    RES_n3_r8(4, E);
}

void Cpu::op_CB_A4()
{
    RES_n3_r8(4, H);
}

void Cpu::op_CB_A5()
{
    RES_n3_r8(4, L);
}

void Cpu::op_CB_A6()
{
    RES_n3_HL(4);
}

void Cpu::op_CB_A7()
{
    RES_n3_r8(4, A);
}

void Cpu::op_CB_A8()
{
    RES_n3_r8(5, B);
}

void Cpu::op_CB_A9()
{
    RES_n3_r8(5, C);
}

void Cpu::op_CB_AA()
{
    RES_n3_r8(5, D);
}

void Cpu::op_CB_AB()
{
    RES_n3_r8(5, E);
}

void Cpu::op_CB_AC()
{
    RES_n3_r8(5, H);
}

void Cpu::op_CB_AD()
{
    RES_n3_r8(5, L);
}

void Cpu::op_CB_AE()
{
    RES_n3_HL(5);
}

void Cpu::op_CB_AF()
{
    RES_n3_r8(5, A);
}

void Cpu::op_CB_B0()
{
    RES_n3_r8(6, B);
}

void Cpu::op_CB_B1()
{
    RES_n3_r8(6, C);
}

void Cpu::op_CB_B2()
{
    RES_n3_r8(6, D);
}

void Cpu::op_CB_B3()
{
    RES_n3_r8(6, E);
}

void Cpu::op_CB_B4()
{
    RES_n3_r8(6, H);
}

void Cpu::op_CB_B5()
{
    RES_n3_r8(6, L);
}

void Cpu::op_CB_B6()
{
    RES_n3_HL(6);
}

void Cpu::op_CB_B7()
{
    RES_n3_r8(6, A);
}

void Cpu::op_CB_B8()
{
    RES_n3_r8(7, B);
}

void Cpu::op_CB_B9()
{
    RES_n3_r8(7, C);
}

void Cpu::op_CB_BA()
{
    RES_n3_r8(7, D);
}

void Cpu::op_CB_BB()
{
    RES_n3_r8(7, E);
}

void Cpu::op_CB_BC()
{
    RES_n3_r8(7, H);
}

void Cpu::op_CB_BD()
{
    RES_n3_r8(7, L);
}

void Cpu::op_CB_BE()
{
    RES_n3_HL(7);
}

void Cpu::op_CB_BF()
{
    RES_n3_r8(7, A);
}

void Cpu::op_CB_C0()
{
    SET_n3_r8(0, B);
}

void Cpu::op_CB_C1()
{
    SET_n3_r8(0, C);
}

void Cpu::op_CB_C2()
{
    SET_n3_r8(0, D);
}

void Cpu::op_CB_C3()
{
    SET_n3_r8(0, E);
}

void Cpu::op_CB_C4()
{
    SET_n3_r8(0, H);
}

void Cpu::op_CB_C5()
{
    SET_n3_r8(0, L);
}

void Cpu::op_CB_C6()
{
    SET_n3_HL(0);
}

void Cpu::op_CB_C7()
{
    SET_n3_r8(0, A);
}

void Cpu::op_CB_C8()
{
    SET_n3_r8(1, B);
}

void Cpu::op_CB_C9()
{
    SET_n3_r8(1, C);
}

void Cpu::op_CB_CA()
{
    SET_n3_r8(1, D);
}

void Cpu::op_CB_CB()
{
    SET_n3_r8(1, E);
}

void Cpu::op_CB_CC()
{
    SET_n3_r8(1, H);
}

void Cpu::op_CB_CD()
{
    SET_n3_r8(1, L);
}

void Cpu::op_CB_CE()
{
    SET_n3_HL(1);
}

void Cpu::op_CB_CF()
{
    SET_n3_r8(1, A);
}

void Cpu::op_CB_D0()
{
    SET_n3_r8(2, B);
}

void Cpu::op_CB_D1()
{
    SET_n3_r8(2, C);
}

void Cpu::op_CB_D2()
{
    SET_n3_r8(2, D);
}

void Cpu::op_CB_D3()
{
    SET_n3_r8(2, E);
}

void Cpu::op_CB_D4()
{
    SET_n3_r8(2, H);
}

void Cpu::op_CB_D5()
{
    SET_n3_r8(2, L);
}

void Cpu::op_CB_D6()
{
    SET_n3_HL(2);
}

void Cpu::op_CB_D7()
{
    SET_n3_r8(2, A);
}

void Cpu::op_CB_D8()
{
    SET_n3_r8(3, B);
}

void Cpu::op_CB_D9()
{
    SET_n3_r8(3, C);
}

void Cpu::op_CB_DA()
{
    SET_n3_r8(3, D);
}

void Cpu::op_CB_DB()
{
    SET_n3_r8(3, E);
}

void Cpu::op_CB_DC()
{
    SET_n3_r8(3, H);
}

void Cpu::op_CB_DD()
{
    SET_n3_r8(3, L);
}

void Cpu::op_CB_DE()
{
    SET_n3_HL(3);
}

void Cpu::op_CB_DF()
{
    SET_n3_r8(3, A);
}

void Cpu::op_CB_E0()
{
    SET_n3_r8(4, B);
}

void Cpu::op_CB_E1()
{
    SET_n3_r8(4, C);
}

void Cpu::op_CB_E2()
{
    SET_n3_r8(4, D);
}

void Cpu::op_CB_E3()
{
    SET_n3_r8(4, E);
}

void Cpu::op_CB_E4()
{
    SET_n3_r8(4, H);
}

void Cpu::op_CB_E5()
{
    SET_n3_r8(4, L);
}

void Cpu::op_CB_E6()
{
    SET_n3_HL(4);
}

void Cpu::op_CB_E7()
{
    SET_n3_r8(4, A);
}

void Cpu::op_CB_E8()
{
    SET_n3_r8(5, B);
}

void Cpu::op_CB_E9()
{
    SET_n3_r8(5, C);
}

void Cpu::op_CB_EA()
{
    SET_n3_r8(5, D);
}

void Cpu::op_CB_EB()
{
    SET_n3_r8(5, E);
}

void Cpu::op_CB_EC()
{
    SET_n3_r8(5, H);
}

void Cpu::op_CB_ED()
{
    SET_n3_r8(5, L);
}

void Cpu::op_CB_EE()
{
    SET_n3_HL(5);
}

void Cpu::op_CB_EF()
{
    SET_n3_r8(5, A);
}

void Cpu::op_CB_F0()
{
    SET_n3_r8(6, B);
}

void Cpu::op_CB_F1()
{
    SET_n3_r8(6, C);
}

void Cpu::op_CB_F2()
{
    SET_n3_r8(6, D);
}

void Cpu::op_CB_F3()
{
    SET_n3_r8(6, E);
}

void Cpu::op_CB_F4()
{
    SET_n3_r8(6, H);
}

void Cpu::op_CB_F5()
{
    SET_n3_r8(6, L);
}

void Cpu::op_CB_F6()
{
    SET_n3_HL(6);
}

void Cpu::op_CB_F7()
{
    SET_n3_r8(6, A);
}

void Cpu::op_CB_F8()
{
    SET_n3_r8(7, B);
}

void Cpu::op_CB_F9()
{
    SET_n3_r8(7, C);
}

void Cpu::op_CB_FA()
{
    SET_n3_r8(7, D);
}

void Cpu::op_CB_FB()
{
    SET_n3_r8(7, E);
}

void Cpu::op_CB_FC()
{
    SET_n3_r8(7, H);
}

void Cpu::op_CB_FD()
{
    SET_n3_r8(7, L);
}

void Cpu::op_CB_FE()
{
    SET_n3_HL(7);
}

void Cpu::op_CB_FF()
{
    SET_n3_r8(7, A);
}
