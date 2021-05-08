#include "breakpoint.hh"


Breakpoint::Breakpoint(regid_t register_id, const std::set<BpMode>& modes) :
    m_register_id(register_id)
{
    for (auto& mode : modes)
        set_mode(mode);
}

void Breakpoint::set_mode(BpMode mode)
{
    switch (mode)
    {
        case BpOnRead:
            m_on_read = true;
            break;
        case BpOnWrite:
            m_on_write = true;
            break;
        case BpOnExec:
            m_on_exec = true;
            break;
        default:
            break;
    }
}

void Breakpoint::clear_mode(BpMode mode)
{
    switch (mode)
    {
        case BpOnRead:
            m_on_read = false;
            break;
        case BpOnWrite:
            m_on_write = false;
            break;
        case BpOnExec:
            m_on_exec = false;
            break;
        default:
            break;
    }
}

void Breakpoint::toggle_mode(BpMode mode)
{
    switch (mode)
    {
        case BpOnRead:
            m_on_read = !m_on_read;
            break;
        case BpOnWrite:
            m_on_write = !m_on_write;
            break;
        case BpOnExec:
            m_on_exec = !m_on_exec;
            break;
        default:
            break;
    }
}

bool Breakpoint::has_mode(BpMode mode)
{
    switch (mode)
    {
        case BpOnRead:
            return m_on_read;
        case BpOnWrite:
            return m_on_write;
        case BpOnExec:
            return m_on_exec;
        case BpNone:
            return !m_on_read && !m_on_write && !m_on_exec;
        default:
            assert(false);
            break;
    }
    return false;
}

const std::set<BpMode>& Breakpoint::modes()
{
    if (m_on_read) m_modes.insert(BpOnRead);
    if (m_on_write) m_modes.insert(BpOnWrite);
    if (m_on_exec) m_modes.insert(BpOnExec);
    return m_modes;
}

regid_t Breakpoint::register_id() const
{
    return m_register_id;
}
