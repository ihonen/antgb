#include "cpu.hh"

void Cpu::decode_and_dispatch(const uint8_t* instruction)
{
    switch (instruction[0])
    {
        case 0x00:
            NOP();
            break;
        case 0x01:
            LD_r16_n16(reg.get_BC(), extract_immediate16(current_instruction));
            break;
        case 0x02:
            LD_r16_A(reg.get_BC());
            break;
        case 0x03:
            INC_r16(reg.get_BC());
            break;
        case 0x04:
            INC_r8(reg.get_B());
            break;
        case 0x05:
            DEC_r8(reg.get_B());
            break;
        case 0x06:
            LD_r8_n8(reg.get_B(), extract_immediate8(current_instruction));
            break;
        case 0x07:
            RLCA();
            break;
        case 0x08:
            LD_n16_SP(extract_immediate16(current_instruction));
            break;
        case 0x09:
            ADD_HL_r16(reg.get_BC());
            break;
        case 0x0A:
            LD_A_r16(reg.get_BC());
            break;
        case 0x0B:
            DEC_r16(reg.get_BC());
            break;
        case 0x0C:
            INC_r8(reg.get_C());
            break;
        case 0x0D:
            DEC_r8(reg.get_C());
            break;
        case 0x0E:
            LD_r8_n8(reg.get_C(), extract_immediate8(current_instruction));
            break;
        case 0x0F:
            RRCA();
            break;
        case 0x10:
            STOP();
            break;
        case 0x11:
            LD_r16_n16(reg.get_DE(), extract_immediate16(current_instruction));
            break;
        case 0x12:
            LD_r16_A(reg.get_DE());
            break;
        case 0x13:
            INC_r16(reg.get_DE());
            break;
        case 0x14:
            INC_r8(reg.get_D());
            break;
        case 0x15:
            DEC_r8(reg.get_D());
            break;
        case 0x16:
            LD_r8_n8(reg.get_D(), extract_immediate8(current_instruction));
            break;
        case 0x17:
            RLA();
            break;
        case 0x18:
            JR_n8(static_cast<int8_t>(extract_immediate8(current_instruction)));
            break;
        case 0x19:
            ADD_HL_r16(reg.get_DE());
            break;
        case 0x1A:
            LD_A_r16(reg.get_DE());
            break;
        case 0x1B:
            DEC_r16(reg.get_DE());
            break;
        case 0x1C:
            INC_r8(reg.get_E());
            break;
        case 0x1D:
            DEC_r8(reg.get_E());
            break;
        case 0x1E:
            LD_r8_n8(reg.get_E(), extract_immediate8(current_instruction));
            break;
        case 0x1F:
            RRA();
            break;
        case 0x20:
            JR_cc_n8(!reg.read_Z_flag(), static_cast<int8_t>(extract_immediate8(current_instruction)));
            break;
        case 0x21:
            LD_r16_n16(reg.get_HL(), extract_immediate16(current_instruction));
            break;
        case 0x22:
            LDI_HL_A();
            break;
        case 0x23:
            INC_r16(reg.get_HL());
            break;
        case 0x24:
            INC_r8(reg.get_H());
            break;
        case 0x25:
            DEC_r8(reg.get_H());
            break;
        case 0x26:
            LD_r8_n8(reg.get_H(), extract_immediate8(current_instruction));
            break;
        case 0x27:
            DAA();
            break;
        case 0x28:
            JR_cc_n8(reg.read_Z_flag(), static_cast<int8_t>(extract_immediate8(current_instruction)));
            break;
        case 0x29:
            ADD_HL_r16(reg.get_HL());
            break;
        case 0x2A:
            LDI_A_HL();
            break;
        case 0x2B:
            DEC_r16(reg.get_HL());
            break;
        case 0x2C:
            INC_r8(reg.get_L());
            break;
        case 0x2D:
            DEC_r8(reg.get_L());
            break;
        case 0x2E:
            LD_r8_n8(reg.get_L(), extract_immediate8(current_instruction));
            break;
        case 0x2F:
            CPL();
            break;
        case 0x30:
            JR_cc_n8(!reg.read_C_flag(), static_cast<int8_t>(extract_immediate8(current_instruction)));
            break;
        case 0x31:
            LD_r16_n16(reg.get_SP(), extract_immediate16(current_instruction));
            break;
        case 0x32:
            LDD_HL_A();
            break;
        case 0x33:
            INC_r16(reg.get_SP());
            break;
        case 0x34:
            INC_HL();
            break;
        case 0x35:
            DEC_HL();
            break;
        case 0x36:
            LD_HL_n8(extract_immediate8(current_instruction));
            break;
        case 0x37:
            SCF();
            break;
        case 0x38:
            JR_cc_n8(reg.read_C_flag(), static_cast<int8_t>(extract_immediate8(current_instruction)));
            break;
        case 0x39:
            ADD_HL_r16(reg.get_SP());
            break;
        case 0x3A:
            LDD_A_HL();
            break;
        case 0x3B:
            DEC_r16(reg.get_SP());
            break;
        case 0x3C:
            INC_r8(reg.get_A());
            break;
        case 0x3D:
            DEC_r8(reg.get_A());
            break;
        case 0x3E:
            LD_r8_n8(reg.get_A(), extract_immediate8(current_instruction));
            break;
        case 0x3F:
            CCF();
            break;
        case 0x40:
            LD_r8_r8(reg.get_B(), reg.get_B());
            break;
        case 0x41:
            LD_r8_r8(reg.get_B(), reg.get_C());
            break;
        case 0x42:
            LD_r8_r8(reg.get_B(), reg.get_D());
            break;
        case 0x43:
            LD_r8_r8(reg.get_B(), reg.get_E());
            break;
        case 0x44:
            LD_r8_r8(reg.get_B(), reg.get_H());
            break;
        case 0x45:
            LD_r8_r8(reg.get_B(), reg.get_L());
            break;
        case 0x46:
            LD_r8_HL(reg.get_B());
            break;
        case 0x47:
            LD_r8_r8(reg.get_B(), reg.get_A());
            break;
        case 0x48:
            LD_r8_r8(reg.get_C(), reg.get_B());
            break;
        case 0x49:
            LD_r8_r8(reg.get_C(), reg.get_C());
            break;
        case 0x4A:
            LD_r8_r8(reg.get_C(), reg.get_D());
            break;
        case 0x4B:
            LD_r8_r8(reg.get_C(), reg.get_E());
            break;
        case 0x4C:
            LD_r8_r8(reg.get_C(), reg.get_H());
            break;
        case 0x4D:
            LD_r8_r8(reg.get_C(), reg.get_L());
            break;
        case 0x4E:
            LD_r8_HL(reg.get_C());
            break;
        case 0x4F:
            LD_r8_r8(reg.get_C(), reg.get_A());
            break;
        case 0x50:
            LD_r8_r8(reg.get_D(), reg.get_B());
            break;
        case 0x51:
            LD_r8_r8(reg.get_D(), reg.get_C());
            break;
        case 0x52:
            LD_r8_r8(reg.get_D(), reg.get_D());
            break;
        case 0x53:
            LD_r8_r8(reg.get_D(), reg.get_E());
            break;
        case 0x54:
            LD_r8_r8(reg.get_D(), reg.get_H());
            break;
        case 0x55:
            LD_r8_r8(reg.get_D(), reg.get_L());
            break;
        case 0x56:
            LD_r8_HL(reg.get_D());
            break;
        case 0x57:
            LD_r8_r8(reg.get_D(), reg.get_A());
            break;
        case 0x58:
            LD_r8_r8(reg.get_E(), reg.get_B());
            break;
        case 0x59:
            LD_r8_r8(reg.get_E(), reg.get_C());
            break;
        case 0x5A:
            LD_r8_r8(reg.get_E(), reg.get_D());
            break;
        case 0x5B:
            LD_r8_r8(reg.get_E(), reg.get_E());
            break;
        case 0x5C:
            LD_r8_r8(reg.get_E(), reg.get_H());
            break;
        case 0x5D:
            LD_r8_r8(reg.get_E(), reg.get_L());
            break;
        case 0x5E:
            LD_r8_HL(reg.get_E());
            break;
        case 0x5F:
            LD_r8_r8(reg.get_E(), reg.get_A());
            break;
        case 0x60:
            LD_r8_r8(reg.get_H(), reg.get_B());
            break;
        case 0x61:
            LD_r8_r8(reg.get_H(), reg.get_C());
            break;
        case 0x62:
            LD_r8_r8(reg.get_H(), reg.get_D());
            break;
        case 0x63:
            LD_r8_r8(reg.get_H(), reg.get_E());
            break;
        case 0x64:
            LD_r8_r8(reg.get_H(), reg.get_H());
            break;
        case 0x65:
            LD_r8_r8(reg.get_H(), reg.get_L());
            break;
        case 0x66:
            LD_r8_HL(reg.get_H());
            break;
        case 0x67:
            LD_r8_r8(reg.get_H(), reg.get_A());
            break;
        case 0x68:
            LD_r8_r8(reg.get_L(), reg.get_B());
            break;
        case 0x69:
            LD_r8_r8(reg.get_L(), reg.get_C());
            break;
        case 0x6A:
            LD_r8_r8(reg.get_L(), reg.get_D());
            break;
        case 0x6B:
            LD_r8_r8(reg.get_L(), reg.get_E());
            break;
        case 0x6C:
            LD_r8_r8(reg.get_L(), reg.get_H());
            break;
        case 0x6D:
            LD_r8_r8(reg.get_L(), reg.get_L());
            break;
        case 0x6E:
            LD_r8_HL(reg.get_L());
            break;
        case 0x6F:
            LD_r8_r8(reg.get_L(), reg.get_A());
            break;
        case 0x70:
            LD_HL_r8(reg.get_B());
            break;
        case 0x71:
            LD_HL_r8(reg.get_C());
            break;
        case 0x72:
            LD_HL_r8(reg.get_D());
            break;
        case 0x73:
            LD_HL_r8(reg.get_E());
            break;
        case 0x74:
            LD_HL_r8(reg.get_H());
            break;
        case 0x75:
            LD_HL_r8(reg.get_L());
            break;
        case 0x76:
            HALT();
            break;
        case 0x77:
            LD_HL_r8(reg.get_A());
            break;
        case 0x78:
            LD_r8_r8(reg.get_A(), reg.get_B());
            break;
        case 0x79:
            LD_r8_r8(reg.get_A(), reg.get_C());
            break;
        case 0x7A:
            LD_r8_r8(reg.get_A(), reg.get_D());
            break;
        case 0x7B:
            LD_r8_r8(reg.get_A(), reg.get_E());
            break;
        case 0x7C:
            LD_r8_r8(reg.get_A(), reg.get_H());
            break;
        case 0x7D:
            LD_r8_r8(reg.get_A(), reg.get_L());
            break;
        case 0x7E:
            LD_A_r16(reg.get_HL());
            break;
        case 0x7F:
            LD_r8_r8(reg.get_A(), reg.get_A());
            break;
        case 0x80:
            ADD_A_r8(reg.get_B());
            break;
        case 0x81:
            ADD_A_r8(reg.get_C());
            break;
        case 0x82:
            ADD_A_r8(reg.get_D());
            break;
        case 0x83:
            ADD_A_r8(reg.get_E());
            break;
        case 0x84:
            ADD_A_r8(reg.get_H());
            break;
        case 0x85:
            ADD_A_r8(reg.get_L());
            break;
        case 0x86:
            ADD_A_HL();
            break;
        case 0x87:
            ADD_A_r8(reg.get_A());
            break;
        case 0x88:
            ADC_A_r8(reg.get_B());
            break;
        case 0x89:
            ADC_A_r8(reg.get_C());
            break;
        case 0x8A:
            ADC_A_r8(reg.get_D());
            break;
        case 0x8B:
            ADC_A_r8(reg.get_E());
            break;
        case 0x8C:
            ADC_A_r8(reg.get_H());
            break;
        case 0x8D:
            ADC_A_r8(reg.get_L());
            break;
        case 0x8E:
            ADC_A_HL();
            break;
        case 0x8F:
            ADC_A_r8(reg.get_A());
            break;
        case 0x90:
            SUB_A_r8(reg.get_B());
            break;
        case 0x91:
            SUB_A_r8(reg.get_C());
            break;
        case 0x92:
            SUB_A_r8(reg.get_D());
            break;
        case 0x93:
            SUB_A_r8(reg.get_E());
            break;
        case 0x94:
            SUB_A_r8(reg.get_H());
            break;
        case 0x95:
            SUB_A_r8(reg.get_L());
            break;
        case 0x96:
            SUB_A_HL();
            break;
        case 0x97:
            SUB_A_r8(reg.get_A());
            break;
        case 0x98:
            SBC_A_r8(reg.get_B());
            break;
        case 0x99:
            SBC_A_r8(reg.get_C());
            break;
        case 0x9A:
            SBC_A_r8(reg.get_D());
            break;
        case 0x9B:
            SBC_A_r8(reg.get_E());
            break;
        case 0x9C:
            SBC_A_r8(reg.get_H());
            break;
        case 0x9D:
            SBC_A_r8(reg.get_L());
            break;
        case 0x9E:
            SBC_A_HL();
            break;
        case 0x9F:
            SBC_A_r8(reg.get_A());
            break;
        case 0xA0:
            AND_r8(reg.get_B());
            break;
        case 0xA1:
            AND_r8(reg.get_C());
            break;
        case 0xA2:
            AND_r8(reg.get_D());
            break;
        case 0xA3:
            AND_r8(reg.get_E());
            break;
        case 0xA4:
            AND_r8(reg.get_H());
            break;
        case 0xA5:
            AND_r8(reg.get_L());
            break;
        case 0xA6:
            AND_HL();
            break;
        case 0xA7:
            AND_r8(reg.get_A());
            break;
        case 0xA8:
            XOR_r8(reg.get_B());
            break;
        case 0xA9:
            XOR_r8(reg.get_C());
            break;
        case 0xAA:
            XOR_r8(reg.get_D());
            break;
        case 0xAB:
            XOR_r8(reg.get_E());
            break;
        case 0xAC:
            XOR_r8(reg.get_H());
            break;
        case 0xAD:
            XOR_r8(reg.get_L());
            break;
        case 0xAE:
            XOR_HL();
            break;
        case 0xAF:
            XOR_r8(reg.get_A());
            break;
        case 0xB0:
            OR_r8(reg.get_B());
            break;
        case 0xB1:
            OR_r8(reg.get_C());
            break;
        case 0xB2:
            OR_r8(reg.get_D());
            break;
        case 0xB3:
            OR_r8(reg.get_E());
            break;
        case 0xB4:
            OR_r8(reg.get_H());
            break;
        case 0xB5:
            OR_r8(reg.get_L());
            break;
        case 0xB6:
            OR_HL();
            break;
        case 0xB7:
            OR_r8(reg.get_A());
            break;
        case 0xB8:
            CP_r8(reg.get_B());
            break;
        case 0xB9:
            CP_r8(reg.get_C());
            break;
        case 0xBA:
            CP_r8(reg.get_D());
            break;
        case 0xBB:
            CP_r8(reg.get_E());
            break;
        case 0xBC:
            CP_r8(reg.get_H());
            break;
        case 0xBD:
            CP_r8(reg.get_L());
            break;
        case 0xBE:
            CP_HL();
            break;
        case 0xBF:
            CP_r8(reg.get_A());
            break;
        case 0xC0:
            RET_cc(!reg.read_Z_flag());
            break;
        case 0xC1:
            POP_r16(reg.get_BC());
            break;
        case 0xC2:
            JP_cc_n16(!reg.read_Z_flag(), extract_immediate16(current_instruction));
            break;
        case 0xC3:
            JP_n16(extract_immediate16(current_instruction));
            break;
        case 0xC4:
            CALL_cc_n16(!reg.read_Z_flag(), extract_immediate16(current_instruction));
            break;
        case 0xC5:
            PUSH_r16(reg.get_BC());
            break;
        case 0xC6:
            ADD_A_n8(extract_immediate8(current_instruction));
            break;
        case 0xC7:
            RST_f(0x00);
            break;
        case 0xC8:
            RET_cc(reg.read_Z_flag());
            break;
        case 0xC9:
            RET();
            break;
        case 0xCA:
            JP_cc_n16(reg.read_Z_flag(), extract_immediate16(current_instruction));
            break;
        case 0xCB:
            goto CB_OPCODES_BEGIN;
        case 0xCC:
            CALL_cc_n16(reg.read_Z_flag(), extract_immediate16(current_instruction));
            break;
        case 0xCD:
            CALL_n16(extract_immediate16(current_instruction));
            break;
        case 0xCE:
            ADC_A_n8(extract_immediate8(current_instruction));
            break;
        case 0xCF:
            RST_f(0x08);
            break;
        case 0xD0:
            RET_cc(!reg.read_C_flag());
            break;
        case 0xD1:
            POP_r16(reg.get_DE());
            break;
        case 0xD2:
            JP_cc_n16(!reg.read_C_flag(), extract_immediate16(current_instruction));
            break;
        case 0xD3:
            invalid_opcode();
            break;
        case 0xD4:
            CALL_cc_n16(!reg.read_C_flag(), extract_immediate16(current_instruction));
            break;
        case 0xD5:
            PUSH_r16(reg.get_DE());
            break;
        case 0xD6:
            SUB_A_n8(extract_immediate8(current_instruction));
            break;
        case 0xD7:
            RST_f(0x10);
            break;
        case 0xD8:
            RET_cc(reg.read_C_flag());
            break;
        case 0xD9:
            RETI();
            break;
        case 0xDA:
            JP_cc_n16(reg.read_C_flag(), extract_immediate16(current_instruction));
            break;
        case 0xDB:
            invalid_opcode();
            break;
        case 0xDC:
            CALL_cc_n16(reg.read_C_flag(), extract_immediate16(current_instruction));
            break;
        case 0xDD:
            invalid_opcode();
            break;
        case 0xDE:
            SBC_A_n8(extract_immediate8(current_instruction));
            break;
        case 0xDF:
            RST_f(0x18);
            break;
        case 0xE0:
            LDH_n8_A(extract_immediate8(current_instruction));
            break;
        case 0xE1:
            POP_r16(reg.get_HL());
            break;
        case 0xE2:
            LD_C_A();
            break;
        case 0xE3:
            invalid_opcode();
            break;
        case 0xE4:
            invalid_opcode();
            break;
        case 0xE5:
            PUSH_r16(reg.get_HL());
            break;
        case 0xE6:
            AND_n8(extract_immediate8(current_instruction));
            break;
        case 0xE7:
            RST_f(0x20);
            break;
        case 0xE8:
            ADD_SP_e8(static_cast<int8_t>(extract_immediate8(current_instruction)));
            break;
        case 0xE9:
            JP_n16(reg.get_HL());
            break;
        case 0xEA:
            LD_n16_A(extract_immediate16(current_instruction));
            break;
        case 0xEB:
            invalid_opcode();
            break;
        case 0xEC:
            invalid_opcode();
            break;
        case 0xED:
            invalid_opcode();
            break;
        case 0xEE:
            XOR_n8(extract_immediate8(current_instruction));
            break;
        case 0xEF:
            RST_f(0x28);
            break;
        case 0xF0:
            LDH_A_n8(extract_immediate8(current_instruction));
            break;
        case 0xF1:
            POP_r16(reg.get_AF());
            // Only the upper nibble of F can be written into, so make sure the lower
            // nibble is zero.
            reg.write_F(reg.read_F());
            break;
        case 0xF2:
            LD_A_C();
            break;
        case 0xF3:
            DI();
            break;
        case 0xF4:
            invalid_opcode();
            break;
        case 0xF5:
            PUSH_r16(reg.get_AF());
            break;
        case 0xF6:
            OR_n8(extract_immediate8(current_instruction));
            break;
        case 0xF7:
            RST_f(0x30);
            break;
        case 0xF8:
            LD_HL_SP_e8(static_cast<int8_t>(extract_immediate8(current_instruction)));
            break;
        case 0xF9:
            LD_SP_HL();
            break;
        case 0xFA:
            LD_A_n16(extract_immediate16(current_instruction));
            break;
        case 0xFB:
            EI();
            break;
        case 0xFC:
            invalid_opcode();
            break;
        case 0xFD:
            invalid_opcode();
            break;
        case 0xFE:
            CP_n8(extract_immediate8(current_instruction));
            break;
        case 0xFF:
            RST_f(0x38);
            break;
    }
    goto CB_OPCODES_END;

    CB_OPCODES_BEGIN:
    switch (instruction[1])
    {
        case 0x00:
            RLC_r8(reg.get_B());
            break;
        case 0x01:
            RLC_r8(reg.get_C());
            break;
        case 0x02:
            RLC_r8(reg.get_D());
            break;
        case 0x03:
            RLC_r8(reg.get_E());
            break;
        case 0x04:
            RLC_r8(reg.get_H());
            break;
        case 0x05:
            RLC_r8(reg.get_L());
            break;
        case 0x06:
            RLC_HL();
            break;
        case 0x07:
            RLC_r8(reg.get_A());
            break;
        case 0x08:
            RRC_r8(reg.get_B());
            break;
        case 0x09:
            RRC_r8(reg.get_C());
            break;
        case 0x0A:
            RRC_r8(reg.get_D());
            break;
        case 0x0B:
            RRC_r8(reg.get_E());
            break;
        case 0x0C:
            RRC_r8(reg.get_H());
            break;
        case 0x0D:
            RRC_r8(reg.get_L());
            break;
        case 0x0E:
            RRC_HL();
            break;
        case 0x0F:
            RRC_r8(reg.get_A());
            break;
        case 0x10:
            RL_r8(reg.get_B());
            break;
        case 0x11:
            RL_r8(reg.get_C());
            break;
        case 0x12:
            RL_r8(reg.get_D());
            break;
        case 0x13:
            RL_r8(reg.get_E());
            break;
        case 0x14:
            RL_r8(reg.get_H());
            break;
        case 0x15:
            RL_r8(reg.get_L());
            break;
        case 0x16:
            RL_HL();
            break;
        case 0x17:
            RL_r8(reg.get_A());
            break;
        case 0x18:
            RR_r8(reg.get_B());
            break;
        case 0x19:
            RR_r8(reg.get_C());
            break;
        case 0x1A:
            RR_r8(reg.get_D());
            break;
        case 0x1B:
            RR_r8(reg.get_E());
            break;
        case 0x1C:
            RR_r8(reg.get_H());
            break;
        case 0x1D:
            RR_r8(reg.get_L());
            break;
        case 0x1E:
            RR_HL();
            break;
        case 0x1F:
            RR_r8(reg.get_A());
            break;
        case 0x20:
            SLA_r8(reg.get_B());
            break;
        case 0x21:
            SLA_r8(reg.get_C());
            break;
        case 0x22:
            SLA_r8(reg.get_D());
            break;
        case 0x23:
            SLA_r8(reg.get_E());
            break;
        case 0x24:
            SLA_r8(reg.get_H());
            break;
        case 0x25:
            SLA_r8(reg.get_L());
            break;
        case 0x26:
            SLA_HL();
            break;
        case 0x27:
            SLA_r8(reg.get_A());
            break;
        case 0x28:
            SRA_r8(reg.get_B());
            break;
        case 0x29:
            SRA_r8(reg.get_C());
            break;
        case 0x2A:
            SRA_r8(reg.get_D());
            break;
        case 0x2B:
            SRA_r8(reg.get_E());
            break;
        case 0x2C:
            SRA_r8(reg.get_H());
            break;
        case 0x2D:
            SRA_r8(reg.get_L());
            break;
        case 0x2E:
            SRA_HL();
            break;
        case 0x2F:
            SRA_r8(reg.get_A());
            break;
        case 0x30:
            SWAP_r8(reg.get_B());
            break;
        case 0x31:
            SWAP_r8(reg.get_C());
            break;
        case 0x32:
            SWAP_r8(reg.get_D());
            break;
        case 0x33:
            SWAP_r8(reg.get_E());
            break;
        case 0x34:
            SWAP_r8(reg.get_H());
            break;
        case 0x35:
            SWAP_r8(reg.get_L());
            break;
        case 0x36:
            SWAP_HL();
            break;
        case 0x37:
            SWAP_r8(reg.get_A());
            break;
        case 0x38:
            SRL_r8(reg.get_B());
            break;
        case 0x39:
            SRL_r8(reg.get_C());
            break;
        case 0x3A:
            SRL_r8(reg.get_D());
            break;
        case 0x3B:
            SRL_r8(reg.get_E());
            break;
        case 0x3C:
            SRL_r8(reg.get_H());
            break;
        case 0x3D:
            SRL_r8(reg.get_L());
            break;
        case 0x3E:
            SRL_HL();
            break;
        case 0x3F:
            SRL_r8(reg.get_A());
            break;
        case 0x40:
            BIT_n3_r8(0, reg.get_B());
            break;
        case 0x41:
            BIT_n3_r8(0, reg.get_C());
            break;
        case 0x42:
            BIT_n3_r8(0, reg.get_D());
            break;
        case 0x43:
            BIT_n3_r8(0, reg.get_E());
            break;
        case 0x44:
            BIT_n3_r8(0, reg.get_H());
            break;
        case 0x45:
            BIT_n3_r8(0, reg.get_L());
            break;
        case 0x46:
            BIT_n3_HL(0);
            break;
        case 0x47:
            BIT_n3_r8(0, reg.get_A());
            break;
        case 0x48:
            BIT_n3_r8(1, reg.get_B());
            break;
        case 0x49:
            BIT_n3_r8(1, reg.get_C());
            break;
        case 0x4A:
            BIT_n3_r8(1, reg.get_D());
            break;
        case 0x4B:
            BIT_n3_r8(1, reg.get_E());
            break;
        case 0x4C:
            BIT_n3_r8(1, reg.get_H());
            break;
        case 0x4D:
            BIT_n3_r8(1, reg.get_L());
            break;
        case 0x4E:
            BIT_n3_HL(1);
            break;
        case 0x4F:
            BIT_n3_r8(1, reg.get_A());
            break;
        case 0x50:
            BIT_n3_r8(2, reg.get_B());
            break;
        case 0x51:
            BIT_n3_r8(2, reg.get_C());
            break;
        case 0x52:
            BIT_n3_r8(2, reg.get_D());
            break;
        case 0x53:
            BIT_n3_r8(2, reg.get_E());
            break;
        case 0x54:
            BIT_n3_r8(2, reg.get_H());
            break;
        case 0x55:
            BIT_n3_r8(2, reg.get_L());
            break;
        case 0x56:
            BIT_n3_HL(2);
            break;
        case 0x57:
            BIT_n3_r8(2, reg.get_A());
            break;
        case 0x58:
            BIT_n3_r8(3, reg.get_B());
            break;
        case 0x59:
            BIT_n3_r8(3, reg.get_C());
            break;
        case 0x5A:
            BIT_n3_r8(3, reg.get_D());
            break;
        case 0x5B:
            BIT_n3_r8(3, reg.get_E());
            break;
        case 0x5C:
            BIT_n3_r8(3, reg.get_H());
            break;
        case 0x5D:
            BIT_n3_r8(3, reg.get_L());
            break;
        case 0x5E:
            BIT_n3_HL(3);
            break;
        case 0x5F:
            BIT_n3_r8(3, reg.get_A());
            break;
        case 0x60:
            BIT_n3_r8(4, reg.get_B());
            break;
        case 0x61:
            BIT_n3_r8(4, reg.get_C());
            break;
        case 0x62:
            BIT_n3_r8(4, reg.get_D());
            break;
        case 0x63:
            BIT_n3_r8(4, reg.get_E());
            break;
        case 0x64:
            BIT_n3_r8(4, reg.get_H());
            break;
        case 0x65:
            BIT_n3_r8(4, reg.get_L());
            break;
        case 0x66:
            BIT_n3_HL(4);
            break;
        case 0x67:
            BIT_n3_r8(4, reg.get_A());
            break;
        case 0x68:
            BIT_n3_r8(5, reg.get_B());
            break;
        case 0x69:
            BIT_n3_r8(5, reg.get_C());
            break;
        case 0x6A:
            BIT_n3_r8(5, reg.get_D());
            break;
        case 0x6B:
            BIT_n3_r8(5, reg.get_E());
            break;
        case 0x6C:
            BIT_n3_r8(5, reg.get_H());
            break;
        case 0x6D:
            BIT_n3_r8(5, reg.get_L());
            break;
        case 0x6E:
            BIT_n3_HL(5);
            break;
        case 0x6F:
            BIT_n3_r8(5, reg.get_A());
            break;
        case 0x70:
            BIT_n3_r8(6, reg.get_B());
            break;
        case 0x71:
            BIT_n3_r8(6, reg.get_C());
            break;
        case 0x72:
            BIT_n3_r8(6, reg.get_D());
            break;
        case 0x73:
            BIT_n3_r8(6, reg.get_E());
            break;
        case 0x74:
            BIT_n3_r8(6, reg.get_H());
            break;
        case 0x75:
            BIT_n3_r8(6, reg.get_L());
            break;
        case 0x76:
            BIT_n3_HL(6);
            break;
        case 0x77:
            BIT_n3_r8(6, reg.get_A());
            break;
        case 0x78:
            BIT_n3_r8(7, reg.get_B());
            break;
        case 0x79:
            BIT_n3_r8(7, reg.get_C());
            break;
        case 0x7A:
            BIT_n3_r8(7, reg.get_D());
            break;
        case 0x7B:
            BIT_n3_r8(7, reg.get_E());
            break;
        case 0x7C:
            BIT_n3_r8(7, reg.get_H());
            break;
        case 0x7D:
            BIT_n3_r8(7, reg.get_L());
            break;
        case 0x7E:
            BIT_n3_HL(7);
            break;
        case 0x7F:
            BIT_n3_r8(7, reg.get_A());
            break;
        case 0x80:
            RES_n3_r8(0, reg.get_B());
            break;
        case 0x81:
            RES_n3_r8(0, reg.get_C());
            break;
        case 0x82:
            RES_n3_r8(0, reg.get_D());
            break;
        case 0x83:
            RES_n3_r8(0, reg.get_E());
            break;
        case 0x84:
            RES_n3_r8(0, reg.get_H());
            break;
        case 0x85:
            RES_n3_r8(0, reg.get_L());
            break;
        case 0x86:
            RES_n3_HL(0);
            break;
        case 0x87:
            RES_n3_r8(0, reg.get_A());
            break;
        case 0x88:
            RES_n3_r8(1, reg.get_B());
            break;
        case 0x89:
            RES_n3_r8(1, reg.get_C());
            break;
        case 0x8A:
            RES_n3_r8(1, reg.get_D());
            break;
        case 0x8B:
            RES_n3_r8(1, reg.get_E());
            break;
        case 0x8C:
            RES_n3_r8(1, reg.get_H());
            break;
        case 0x8D:
            RES_n3_r8(1, reg.get_L());
            break;
        case 0x8E:
            RES_n3_HL(1);
            break;
        case 0x8F:
            RES_n3_r8(1, reg.get_A());
            break;
        case 0x90:
            RES_n3_r8(2, reg.get_B());
            break;
        case 0x91:
            RES_n3_r8(2, reg.get_C());
            break;
        case 0x92:
            RES_n3_r8(2, reg.get_D());
            break;
        case 0x93:
            RES_n3_r8(2, reg.get_E());
            break;
        case 0x94:
            RES_n3_r8(2, reg.get_H());
            break;
        case 0x95:
            RES_n3_r8(2, reg.get_L());
            break;
        case 0x96:
            RES_n3_HL(2);
            break;
        case 0x97:
            RES_n3_r8(2, reg.get_A());
            break;
        case 0x98:
            RES_n3_r8(3, reg.get_B());
            break;
        case 0x99:
            RES_n3_r8(3, reg.get_C());
            break;
        case 0x9A:
            RES_n3_r8(3, reg.get_D());
            break;
        case 0x9B:
            RES_n3_r8(3, reg.get_E());
            break;
        case 0x9C:
            RES_n3_r8(3, reg.get_H());
            break;
        case 0x9D:
            RES_n3_r8(3, reg.get_L());
            break;
        case 0x9E:
            RES_n3_HL(3);
            break;
        case 0x9F:
            RES_n3_r8(3, reg.get_A());
            break;
        case 0xA0:
            RES_n3_r8(4, reg.get_B());
            break;
        case 0xA1:
            RES_n3_r8(4, reg.get_C());
            break;
        case 0xA2:
            RES_n3_r8(4, reg.get_D());
            break;
        case 0xA3:
            RES_n3_r8(4, reg.get_E());
            break;
        case 0xA4:
            RES_n3_r8(4, reg.get_H());
            break;
        case 0xA5:
            RES_n3_r8(4, reg.get_L());
            break;
        case 0xA6:
            RES_n3_HL(4);
            break;
        case 0xA7:
            RES_n3_r8(4, reg.get_A());
            break;
        case 0xA8:
            RES_n3_r8(5, reg.get_B());
            break;
        case 0xA9:
            RES_n3_r8(5, reg.get_C());
            break;
        case 0xAA:
            RES_n3_r8(5, reg.get_D());
            break;
        case 0xAB:
            RES_n3_r8(5, reg.get_E());
            break;
        case 0xAC:
            RES_n3_r8(5, reg.get_H());
            break;
        case 0xAD:
            RES_n3_r8(5, reg.get_L());
            break;
        case 0xAE:
            RES_n3_HL(5);
            break;
        case 0xAF:
            RES_n3_r8(5, reg.get_A());
            break;
        case 0xB0:
            RES_n3_r8(6, reg.get_B());
            break;
        case 0xB1:
            RES_n3_r8(6, reg.get_C());
            break;
        case 0xB2:
            RES_n3_r8(6, reg.get_D());
            break;
        case 0xB3:
            RES_n3_r8(6, reg.get_E());
            break;
        case 0xB4:
            RES_n3_r8(6, reg.get_H());
            break;
        case 0xB5:
            RES_n3_r8(6, reg.get_L());
            break;
        case 0xB6:
            RES_n3_HL(6);
            break;
        case 0xB7:
            RES_n3_r8(6, reg.get_A());
            break;
        case 0xB8:
            RES_n3_r8(7, reg.get_B());
            break;
        case 0xB9:
            RES_n3_r8(7, reg.get_C());
            break;
        case 0xBA:
            RES_n3_r8(7, reg.get_D());
            break;
        case 0xBB:
            RES_n3_r8(7, reg.get_E());
            break;
        case 0xBC:
            RES_n3_r8(7, reg.get_H());
            break;
        case 0xBD:
            RES_n3_r8(7, reg.get_L());
            break;
        case 0xBE:
            RES_n3_HL(7);
            break;
        case 0xBF:
            RES_n3_r8(7, reg.get_A());
            break;
        case 0xC0:
            SET_n3_r8(0, reg.get_B());
            break;
        case 0xC1:
            SET_n3_r8(0, reg.get_C());
            break;
        case 0xC2:
            SET_n3_r8(0, reg.get_D());
            break;
        case 0xC3:
            SET_n3_r8(0, reg.get_E());
            break;
        case 0xC4:
            SET_n3_r8(0, reg.get_H());
            break;
        case 0xC5:
            SET_n3_r8(0, reg.get_L());
            break;
        case 0xC6:
            SET_n3_HL(0);
            break;
        case 0xC7:
            SET_n3_r8(0, reg.get_A());
            break;
        case 0xC8:
            SET_n3_r8(1, reg.get_B());
            break;
        case 0xC9:
            SET_n3_r8(1, reg.get_C());
            break;
        case 0xCA:
            SET_n3_r8(1, reg.get_D());
            break;
        case 0xCB:
            SET_n3_r8(1, reg.get_E());
            break;
        case 0xCC:
            SET_n3_r8(1, reg.get_H());
            break;
        case 0xCD:
            SET_n3_r8(1, reg.get_L());
            break;
        case 0xCE:
            SET_n3_HL(1);
            break;
        case 0xCF:
            SET_n3_r8(1, reg.get_A());
            break;
        case 0xD0:
            SET_n3_r8(2, reg.get_B());
            break;
        case 0xD1:
            SET_n3_r8(2, reg.get_C());
            break;
        case 0xD2:
            SET_n3_r8(2, reg.get_D());
            break;
        case 0xD3:
            SET_n3_r8(2, reg.get_E());
            break;
        case 0xD4:
            SET_n3_r8(2, reg.get_H());
            break;
        case 0xD5:
            SET_n3_r8(2, reg.get_L());
            break;
        case 0xD6:
            SET_n3_HL(2);
            break;
        case 0xD7:
            SET_n3_r8(2, reg.get_A());
            break;
        case 0xD8:
            SET_n3_r8(3, reg.get_B());
            break;
        case 0xD9:
            SET_n3_r8(3, reg.get_C());
            break;
        case 0xDA:
            SET_n3_r8(3, reg.get_D());
            break;
        case 0xDB:
            SET_n3_r8(3, reg.get_E());
            break;
        case 0xDC:
            SET_n3_r8(3, reg.get_H());
            break;
        case 0xDD:
            SET_n3_r8(3, reg.get_L());
            break;
        case 0xDE:
            SET_n3_HL(3);
            break;
        case 0xDF:
            SET_n3_r8(3, reg.get_A());
            break;
        case 0xE0:
            SET_n3_r8(4, reg.get_B());
            break;
        case 0xE1:
            SET_n3_r8(4, reg.get_C());
            break;
        case 0xE2:
            SET_n3_r8(4, reg.get_D());
            break;
        case 0xE3:
            SET_n3_r8(4, reg.get_E());
            break;
        case 0xE4:
            SET_n3_r8(4, reg.get_H());
            break;
        case 0xE5:
            SET_n3_r8(4, reg.get_L());
            break;
        case 0xE6:
            SET_n3_HL(4);
            break;
        case 0xE7:
            SET_n3_r8(4, reg.get_A());
            break;
        case 0xE8:
            SET_n3_r8(5, reg.get_B());
            break;
        case 0xE9:
            SET_n3_r8(5, reg.get_C());
            break;
        case 0xEA:
            SET_n3_r8(5, reg.get_D());
            break;
        case 0xEB:
            SET_n3_r8(5, reg.get_E());
            break;
        case 0xEC:
            SET_n3_r8(5, reg.get_H());
            break;
        case 0xED:
            SET_n3_r8(5, reg.get_L());
            break;
        case 0xEE:
            SET_n3_HL(5);
            break;
        case 0xEF:
            SET_n3_r8(5, reg.get_A());
            break;
        case 0xF0:
            SET_n3_r8(6, reg.get_B());
            break;
        case 0xF1:
            SET_n3_r8(6, reg.get_C());
            break;
        case 0xF2:
            SET_n3_r8(6, reg.get_D());
            break;
        case 0xF3:
            SET_n3_r8(6, reg.get_E());
            break;
        case 0xF4:
            SET_n3_r8(6, reg.get_H());
            break;
        case 0xF5:
            SET_n3_r8(6, reg.get_L());
            break;
        case 0xF6:
            SET_n3_HL(6);
            break;
        case 0xF7:
            SET_n3_r8(6, reg.get_A());
            break;
        case 0xF8:
            SET_n3_r8(7, reg.get_B());
            break;
        case 0xF9:
            SET_n3_r8(7, reg.get_C());
            break;
        case 0xFA:
            SET_n3_r8(7, reg.get_D());
            break;
        case 0xFB:
            SET_n3_r8(7, reg.get_E());
            break;
        case 0xFC:
            SET_n3_r8(7, reg.get_H());
            break;
        case 0xFD:
            SET_n3_r8(7, reg.get_L());
            break;
        case 0xFE:
            SET_n3_HL(7);
            break;
        case 0xFF:
            SET_n3_r8(7, reg.get_A());
            break;
    }
CB_OPCODES_END:
    return;
}
