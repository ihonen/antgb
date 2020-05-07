#pragma once

#include "debugobserver.hh"
#include "disassembler.hh"
#include "../core/emulator.hh"
#include "../core/cpu.hh"
#include "../core/types.hh"
#include "../util/macros.hh"

#include <set>
#include <thread>

struct DebugCore
{    
    Emulator* emu;

    Disassembler disassembler;

    struct Snapshot
    {
        Cpu::Registers cpu;
        Irc::Registers irc;
    };

    Snapshot current_snap;
    Snapshot previous_snap;

    std::set<uint16_t> breakpoints;
    std::set<uint16_t> data_breakpoints;

    std::set<DebugObserver*> observers;

    std::thread* emulation_thread = nullptr;

    bool do_run = false;

    DebugCore(Emulator* emulator);
    ~DebugCore();
    void insert_cartridge(Cartridge* cartridge);
    void execute_next();
    void step_over();
    void step_into();
    void step_out();
    void keep_running();
    void run();
    void pause();
    void toggle_breakpoint(uint16_t address);
    void toggle_data_breakpoint(uint16_t address);
    void add_observer(DebugObserver* observer);
    void button_pressed(Joypad::Button button);
    void button_released(Joypad::Button button);
};
