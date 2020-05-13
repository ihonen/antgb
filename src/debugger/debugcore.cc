#include "debugcore.hh"

DebugCore::DebugCore(Emulator* emulator)
{
    emu = emulator;
    emu->hard_reset();
}

DebugCore::~DebugCore()
{
    pause();
}

void DebugCore::insert_cartridge(Cartridge* cartridge)
{
    emu->insert_cartridge(cartridge);

    for (auto& observer : observers)
        observer->on_rom_loaded();
}

void DebugCore::execute_next()
{
    emu->tick();
}

void DebugCore::step_over()
{

}

void DebugCore::step_into()
{
    for (auto& observer : observers)
        observer->on_debugging_resumed();

    execute_next();

    for (auto& observer : observers)
        observer->on_debugging_paused();
}

void DebugCore::step_out()
{

}

void DebugCore::keep_running()
{
    // To prevent being stuck in the current breakpoint.
    bool is_first_instruction = true;
    while ((do_run && !breakpoints.count(emu->cpu->PC))
           || (do_run && is_first_instruction))
    {
        is_first_instruction = false;
        execute_next();
    }

    for (auto& observer : observers)
        observer->on_debugging_paused();
}

void DebugCore::run()
{
    for (auto& observer : observers)
        observer->on_debugging_resumed();

    if (emulation_thread && emulation_thread->joinable())
        emulation_thread->join();
    delete emulation_thread;
    emulation_thread = nullptr;
    do_run = true;
    emulation_thread = new std::thread([&](){ keep_running(); });
}

void DebugCore::pause()
{
    do_run = false;
    if (emulation_thread)
    {
        if (emulation_thread->joinable()) emulation_thread->join();
        delete emulation_thread;
        emulation_thread = nullptr;
    }

    for (auto& observer : observers)
        observer->on_debugging_paused();
}

void DebugCore::toggle_breakpoint(uint16_t address)
{
    if (breakpoints.count(address))
    {
        breakpoints.erase(breakpoints.find(address));
        for (auto& observer : observers)
            observer->on_breakpoint_removed(address);
    }
    else
    {
        breakpoints.insert(address);
        for (auto& observer : observers)
            observer->on_breakpoint_added(address);
    }
}

void DebugCore::toggle_data_breakpoint(uint16_t address)
{
    if (data_breakpoints.count(address))
    {
        data_breakpoints.erase(data_breakpoints.find(address));
        for (auto& observer : observers)
            observer->on_data_breakpoint_removed(address);
    }
    else
    {
        data_breakpoints.insert(address);
        for (auto& observer : observers)
            observer->on_data_breakpoint_added(address);
    }
}

void DebugCore::add_observer(DebugObserver* observer)
{
    observers.push_back(observer);
}

void DebugCore::button_pressed(Joypad::Button button)
{
    emu->button_pressed(button);
//    cerr << std::hex << (size_t)emu->mem->hff00_joyp << endl;
}

void DebugCore::button_released(Joypad::Button button)
{
    emu->button_released(button);
}
