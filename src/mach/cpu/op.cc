#include "cpu.hh"

void Cpu::op_00()
{
    NOP();
}

void Cpu::op_01()
{

}

void Cpu::op_02()
{

}

void Cpu::op_03()
{

}

void Cpu::op_04()
{

}

void Cpu::op_05()
{

}

void Cpu::op_06()
{

}

void Cpu::op_07()
{

}

void Cpu::op_08()
{

}

void Cpu::op_09()
{
    ADD_HL_r16(BC);
}

void Cpu::op_0A()
{

}

void Cpu::op_0B()
{

}

void Cpu::op_0C()
{

}

void Cpu::op_0D()
{

}

void Cpu::op_0E()
{

}

void Cpu::op_0F()
{

}

void Cpu::op_10()
{

}

void Cpu::op_11()
{

}

void Cpu::op_12()
{

}

void Cpu::op_13()
{

}

void Cpu::op_14()
{

}

void Cpu::op_15()
{

}

void Cpu::op_16()
{

}

void Cpu::op_17()
{

}

void Cpu::op_18()
{

}

void Cpu::op_19()
{
    ADD_HL_r16(DE);
}

void Cpu::op_1A()
{

}

void Cpu::op_1B()
{

}

void Cpu::op_1C()
{

}

void Cpu::op_1D()
{

}

void Cpu::op_1E()
{

}

void Cpu::op_1F()
{

}

void Cpu::op_20()
{

}

void Cpu::op_21()
{

}

void Cpu::op_22()
{

}

void Cpu::op_23()
{

}

void Cpu::op_24()
{

}

void Cpu::op_25()
{

}

void Cpu::op_26()
{

}

void Cpu::op_27()
{

}

void Cpu::op_28()
{

}

void Cpu::op_29()
{
    ADD_HL_r16(HL);
}

void Cpu::op_2A()
{

}

void Cpu::op_2B()
{

}

void Cpu::op_2C()
{

}

void Cpu::op_2D()
{

}

void Cpu::op_2E()
{

}

void Cpu::op_2F()
{
    CPL();
}

void Cpu::op_30()
{

}

void Cpu::op_31()
{

}

void Cpu::op_32()
{

}

void Cpu::op_33()
{

}

void Cpu::op_34()
{

}

void Cpu::op_35()
{

}

void Cpu::op_36()
{

}

void Cpu::op_37()
{

}

void Cpu::op_38()
{

}

void Cpu::op_39()
{
    ADD_HL_r16(SP);
}

void Cpu::op_3A()
{

}

void Cpu::op_3B()
{

}

void Cpu::op_3C()
{

}

void Cpu::op_3D()
{

}

void Cpu::op_3E()
{

}

void Cpu::op_3F()
{
    CCF();
}

void Cpu::op_40()
{

}

void Cpu::op_41()
{

}

void Cpu::op_42()
{

}

void Cpu::op_43()
{

}

void Cpu::op_44()
{

}

void Cpu::op_45()
{

}

void Cpu::op_46()
{

}

void Cpu::op_47()
{

}

void Cpu::op_48()
{

}

void Cpu::op_49()
{

}

void Cpu::op_4A()
{

}

void Cpu::op_4B()
{

}

void Cpu::op_4C()
{

}

void Cpu::op_4D()
{

}

void Cpu::op_4E()
{

}

void Cpu::op_4F()
{

}

void Cpu::op_50()
{

}

void Cpu::op_51()
{

}

void Cpu::op_52()
{

}

void Cpu::op_53()
{

}

void Cpu::op_54()
{

}

void Cpu::op_55()
{

}

void Cpu::op_56()
{

}

void Cpu::op_57()
{

}

void Cpu::op_58()
{

}

void Cpu::op_59()
{

}

void Cpu::op_5A()
{

}

void Cpu::op_5B()
{

}

void Cpu::op_5C()
{

}

void Cpu::op_5D()
{

}

void Cpu::op_5E()
{

}

void Cpu::op_5F()
{

}

void Cpu::op_60()
{

}

void Cpu::op_61()
{

}

void Cpu::op_62()
{

}

void Cpu::op_63()
{

}

void Cpu::op_64()
{

}

void Cpu::op_65()
{

}

void Cpu::op_66()
{

}

void Cpu::op_67()
{

}

void Cpu::op_68()
{

}

void Cpu::op_69()
{

}

void Cpu::op_6A()
{

}

void Cpu::op_6B()
{

}

void Cpu::op_6C()
{

}

void Cpu::op_6D()
{

}

void Cpu::op_6E()
{

}

void Cpu::op_6F()
{

}

void Cpu::op_70()
{

}

void Cpu::op_71()
{

}

void Cpu::op_72()
{

}

void Cpu::op_73()
{

}

void Cpu::op_74()
{

}

void Cpu::op_75()
{

}

void Cpu::op_76()
{

}

void Cpu::op_77()
{

}

void Cpu::op_78()
{

}

void Cpu::op_79()
{

}

void Cpu::op_7A()
{

}

void Cpu::op_7B()
{

}

void Cpu::op_7C()
{

}

void Cpu::op_7D()
{

}

void Cpu::op_7E()
{

}

void Cpu::op_7F()
{

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

}

void Cpu::op_91()
{

}

void Cpu::op_92()
{

}

void Cpu::op_93()
{

}

void Cpu::op_94()
{

}

void Cpu::op_95()
{

}

void Cpu::op_96()
{

}

void Cpu::op_97()
{

}

void Cpu::op_98()
{

}

void Cpu::op_99()
{

}

void Cpu::op_9A()
{

}

void Cpu::op_9B()
{

}

void Cpu::op_9C()
{

}

void Cpu::op_9D()
{

}

void Cpu::op_9E()
{

}

void Cpu::op_9F()
{

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

}

void Cpu::op_A9()
{

}

void Cpu::op_AA()
{

}

void Cpu::op_AB()
{

}

void Cpu::op_AC()
{

}

void Cpu::op_AD()
{

}

void Cpu::op_AE()
{

}

void Cpu::op_AF()
{

}

void Cpu::op_B0()
{

}

void Cpu::op_B1()
{

}

void Cpu::op_B2()
{

}

void Cpu::op_B3()
{

}

void Cpu::op_B4()
{

}

void Cpu::op_B5()
{

}

void Cpu::op_B6()
{

}

void Cpu::op_B7()
{

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

}

void Cpu::op_C1()
{

}

void Cpu::op_C2()
{

}

void Cpu::op_C3()
{

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

}

void Cpu::op_C9()
{

}

void Cpu::op_CA()
{

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

}

void Cpu::op_D1()
{

}

void Cpu::op_D2()
{

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

}

void Cpu::op_D6()
{

}

void Cpu::op_D7()
{

}

void Cpu::op_D8()
{

}

void Cpu::op_D9()
{

}

void Cpu::op_DA()
{

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

}

void Cpu::op_DF()
{

}

void Cpu::op_E0()
{

}

void Cpu::op_E1()
{

}

void Cpu::op_E2()
{

}

void Cpu::op_E3()
{

}

void Cpu::op_E4()
{

}

void Cpu::op_E5()
{

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

}

void Cpu::op_EA()
{

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

}

void Cpu::op_EF()
{

}

void Cpu::op_F0()
{

}

void Cpu::op_F1()
{

}

void Cpu::op_F2()
{

}

void Cpu::op_F3()
{

}

void Cpu::op_F4()
{

}

void Cpu::op_F5()
{

}

void Cpu::op_F6()
{

}

void Cpu::op_F7()
{

}

void Cpu::op_F8()
{

}

void Cpu::op_F9()
{

}

void Cpu::op_FA()
{

}

void Cpu::op_FB()
{

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

}

void Cpu::op_CB_01()
{

}

void Cpu::op_CB_02()
{

}

void Cpu::op_CB_03()
{

}

void Cpu::op_CB_04()
{

}

void Cpu::op_CB_05()
{

}

void Cpu::op_CB_06()
{

}

void Cpu::op_CB_07()
{

}

void Cpu::op_CB_08()
{

}

void Cpu::op_CB_09()
{

}

void Cpu::op_CB_0A()
{

}

void Cpu::op_CB_0B()
{

}

void Cpu::op_CB_0C()
{

}

void Cpu::op_CB_0D()
{

}

void Cpu::op_CB_0E()
{

}

void Cpu::op_CB_0F()
{

}

void Cpu::op_CB_10()
{

}

void Cpu::op_CB_11()
{

}

void Cpu::op_CB_12()
{

}

void Cpu::op_CB_13()
{

}

void Cpu::op_CB_14()
{

}

void Cpu::op_CB_15()
{

}

void Cpu::op_CB_16()
{

}

void Cpu::op_CB_17()
{

}

void Cpu::op_CB_18()
{

}

void Cpu::op_CB_19()
{

}

void Cpu::op_CB_1A()
{

}

void Cpu::op_CB_1B()
{

}

void Cpu::op_CB_1C()
{

}

void Cpu::op_CB_1D()
{

}

void Cpu::op_CB_1E()
{

}

void Cpu::op_CB_1F()
{

}

void Cpu::op_CB_20()
{

}

void Cpu::op_CB_21()
{

}

void Cpu::op_CB_22()
{

}

void Cpu::op_CB_23()
{

}

void Cpu::op_CB_24()
{

}

void Cpu::op_CB_25()
{

}

void Cpu::op_CB_26()
{

}

void Cpu::op_CB_27()
{

}

void Cpu::op_CB_28()
{

}

void Cpu::op_CB_29()
{

}

void Cpu::op_CB_2A()
{

}

void Cpu::op_CB_2B()
{

}

void Cpu::op_CB_2C()
{

}

void Cpu::op_CB_2D()
{

}

void Cpu::op_CB_2E()
{

}

void Cpu::op_CB_2F()
{

}

void Cpu::op_CB_30()
{

}

void Cpu::op_CB_31()
{

}

void Cpu::op_CB_32()
{

}

void Cpu::op_CB_33()
{

}

void Cpu::op_CB_34()
{

}

void Cpu::op_CB_35()
{

}

void Cpu::op_CB_36()
{

}

void Cpu::op_CB_37()
{

}

void Cpu::op_CB_38()
{

}

void Cpu::op_CB_39()
{

}

void Cpu::op_CB_3A()
{

}

void Cpu::op_CB_3B()
{

}

void Cpu::op_CB_3C()
{

}

void Cpu::op_CB_3D()
{

}

void Cpu::op_CB_3E()
{

}

void Cpu::op_CB_3F()
{

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

}

void Cpu::op_CB_81()
{

}

void Cpu::op_CB_82()
{

}

void Cpu::op_CB_83()
{

}

void Cpu::op_CB_84()
{

}

void Cpu::op_CB_85()
{

}

void Cpu::op_CB_86()
{

}

void Cpu::op_CB_87()
{

}

void Cpu::op_CB_88()
{

}

void Cpu::op_CB_89()
{

}

void Cpu::op_CB_8A()
{

}

void Cpu::op_CB_8B()
{

}

void Cpu::op_CB_8C()
{

}

void Cpu::op_CB_8D()
{

}

void Cpu::op_CB_8E()
{

}

void Cpu::op_CB_8F()
{

}

void Cpu::op_CB_90()
{

}

void Cpu::op_CB_91()
{

}

void Cpu::op_CB_92()
{

}

void Cpu::op_CB_93()
{

}

void Cpu::op_CB_94()
{

}

void Cpu::op_CB_95()
{

}

void Cpu::op_CB_96()
{

}

void Cpu::op_CB_97()
{

}

void Cpu::op_CB_98()
{

}

void Cpu::op_CB_99()
{

}

void Cpu::op_CB_9A()
{

}

void Cpu::op_CB_9B()
{

}

void Cpu::op_CB_9C()
{

}

void Cpu::op_CB_9D()
{

}

void Cpu::op_CB_9E()
{

}

void Cpu::op_CB_9F()
{

}

void Cpu::op_CB_A0()
{

}

void Cpu::op_CB_A1()
{

}

void Cpu::op_CB_A2()
{

}

void Cpu::op_CB_A3()
{

}

void Cpu::op_CB_A4()
{

}

void Cpu::op_CB_A5()
{

}

void Cpu::op_CB_A6()
{

}

void Cpu::op_CB_A7()
{

}

void Cpu::op_CB_A8()
{

}

void Cpu::op_CB_A9()
{

}

void Cpu::op_CB_AA()
{

}

void Cpu::op_CB_AB()
{

}

void Cpu::op_CB_AC()
{

}

void Cpu::op_CB_AD()
{

}

void Cpu::op_CB_AE()
{

}

void Cpu::op_CB_AF()
{

}

void Cpu::op_CB_B0()
{

}

void Cpu::op_CB_B1()
{

}

void Cpu::op_CB_B2()
{

}

void Cpu::op_CB_B3()
{

}

void Cpu::op_CB_B4()
{

}

void Cpu::op_CB_B5()
{

}

void Cpu::op_CB_B6()
{

}

void Cpu::op_CB_B7()
{

}

void Cpu::op_CB_B8()
{

}

void Cpu::op_CB_B9()
{

}

void Cpu::op_CB_BA()
{

}

void Cpu::op_CB_BB()
{

}

void Cpu::op_CB_BC()
{

}

void Cpu::op_CB_BD()
{

}

void Cpu::op_CB_BE()
{

}

void Cpu::op_CB_BF()
{

}

void Cpu::op_CB_C0()
{

}

void Cpu::op_CB_C1()
{

}

void Cpu::op_CB_C2()
{

}

void Cpu::op_CB_C3()
{

}

void Cpu::op_CB_C4()
{

}

void Cpu::op_CB_C5()
{

}

void Cpu::op_CB_C6()
{

}

void Cpu::op_CB_C7()
{

}

void Cpu::op_CB_C8()
{

}

void Cpu::op_CB_C9()
{

}

void Cpu::op_CB_CA()
{

}

void Cpu::op_CB_CB()
{

}

void Cpu::op_CB_CC()
{

}

void Cpu::op_CB_CD()
{

}

void Cpu::op_CB_CE()
{

}

void Cpu::op_CB_CF()
{

}

void Cpu::op_CB_D0()
{

}

void Cpu::op_CB_D1()
{

}

void Cpu::op_CB_D2()
{

}

void Cpu::op_CB_D3()
{

}

void Cpu::op_CB_D4()
{

}

void Cpu::op_CB_D5()
{

}

void Cpu::op_CB_D6()
{

}

void Cpu::op_CB_D7()
{

}

void Cpu::op_CB_D8()
{

}

void Cpu::op_CB_D9()
{

}

void Cpu::op_CB_DA()
{

}

void Cpu::op_CB_DB()
{

}

void Cpu::op_CB_DC()
{

}

void Cpu::op_CB_DD()
{

}

void Cpu::op_CB_DE()
{

}

void Cpu::op_CB_DF()
{

}

void Cpu::op_CB_E0()
{

}

void Cpu::op_CB_E1()
{

}

void Cpu::op_CB_E2()
{

}

void Cpu::op_CB_E3()
{

}

void Cpu::op_CB_E4()
{

}

void Cpu::op_CB_E5()
{

}

void Cpu::op_CB_E6()
{

}

void Cpu::op_CB_E7()
{

}

void Cpu::op_CB_E8()
{

}

void Cpu::op_CB_E9()
{

}

void Cpu::op_CB_EA()
{

}

void Cpu::op_CB_EB()
{

}

void Cpu::op_CB_EC()
{

}

void Cpu::op_CB_ED()
{

}

void Cpu::op_CB_EE()
{

}

void Cpu::op_CB_EF()
{

}

void Cpu::op_CB_F0()
{

}

void Cpu::op_CB_F1()
{

}

void Cpu::op_CB_F2()
{

}

void Cpu::op_CB_F3()
{

}

void Cpu::op_CB_F4()
{

}

void Cpu::op_CB_F5()
{

}

void Cpu::op_CB_F6()
{

}

void Cpu::op_CB_F7()
{

}

void Cpu::op_CB_F8()
{

}

void Cpu::op_CB_F9()
{

}

void Cpu::op_CB_FA()
{

}

void Cpu::op_CB_FB()
{

}

void Cpu::op_CB_FC()
{

}

void Cpu::op_CB_FD()
{

}

void Cpu::op_CB_FE()
{

}

void Cpu::op_CB_FF()
{

}
