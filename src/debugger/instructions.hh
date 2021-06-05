#pragma once

#include "types.hh"
#include <cstdint>
#include <unordered_set>

class DebugCore;

struct Opcode
{
    int len_bytes = -1;
    int cycles_on_action = -1;
    int cycles = -1;
    uint8_t index = 0xFF;
    // NOTE: The following two don't include the F register.
    std::unordered_set<CpuReg> write_regs = {RegNone};
    std::unordered_set<CpuReg> read_regs = {RegNone};
    // These include the F register.
    bool write_flags = false;
    bool read_flags = false;
};

extern const std::array<Opcode, 256> OPCODES;
extern const std::array<Opcode, 256> CB_OPCODES;

class Instruction
{
public:
    Instruction(const uint8_t* bytes, size_t max_len = 3);
    const std::unordered_set<regid_t>& write_addresses(DebugCore* debugger);
    const std::unordered_set<regid_t>& read_addresses(DebugCore* debugger);
    const std::string& disassembly();
    const uint8_t* bytes();
    const Opcode* opcode();
    bool is_valid();
private:
    uint8_t m_bytes[3];
    const Opcode* m_opcode = nullptr;
    // Evaluated when requested.
    std::unordered_set<regid_t> m_write_addresses;
    std::unordered_set<regid_t> m_read_addresses;
    std::string m_disasm;
};
