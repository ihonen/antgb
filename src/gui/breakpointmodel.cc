#include "breakpointmodel.hh"

#include "helper.hh"
#include "pixmaps.hh"

static const QVector<int> ALL_ROLES = {Qt::DecorationRole, Qt::DisplayRole};

BreakpointModel::BreakpointModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    iDebugObserver(),
    debugger(debugger)
{
    debugger->add_observer(this);

    stored_pc = debugger->read(RegPC);
    addresses.insert(stored_pc);
    pc_is_also_breakpoint = false;
}

Qt::ItemFlags BreakpointModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index) & ~(Qt::ItemIsSelectable);
}

QVariant BreakpointModel::headerData(int section,
                                     Qt::Orientation orientation,
                                     int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

QVariant BreakpointModel::data(const QModelIndex& index, int role) const
{
    auto address = get_row(index.row());

    switch (role)
    {
        case Qt::DisplayRole:
            switch (index.column())
            {
                case ADDRESS_COLUMN:
                    return hexstr16(address).toUpper();
                case DISASSEMBLY_COLUMN:
                {
                    uint8_t instruction[3];
                    size_t offset = 0;
                    for (; offset < 3 && address + offset <= 0xFFFF; ++offset)
                        instruction[offset] = debugger->read(address + offset);
                    return debugger->disasm()->disassemble(instruction).c_str();
                }
                default: break;
            }
            break;

        case Qt::DecorationRole:
            switch (index.column())
            {
                case BREAKPOINT_COLUMN:
                    if (address != stored_pc || pc_is_also_breakpoint)
                        return Pixmaps::BREAKPOINT->scaled(9, 9);
                    break;
                case CURRENT_INSTR_COLUMN:
                    if (address == stored_pc)
                        return Pixmaps::CURRENT_ARROW->scaled(11, 11);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return QVariant();
}

int BreakpointModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(addresses.size());
}

int BreakpointModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

bool BreakpointModel::setData(const QModelIndex& index,
                              const QVariant& value,
                              int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

uint16_t BreakpointModel::get_row(int row) const
{
    int current_row = 0;
    for (const auto& address : addresses)
    {
        if (current_row == row)
            return address;
        ++current_row;
    }
    return 0xFFFF;
}

void BreakpointModel::debug_event(const DebugEvent* event)
{
    switch (event->type())
    {
        case DbPaused:
            on_debugging_paused();
            break;
        case DbBreakpointAdded:
            if (debugger->breakpoint_at(event->register_id())->has_mode(BpOnExec))
                on_breakpoint_added(event->register_id());
            break;
        case DbBreakpointModified:
            if (debugger->breakpoint_at(event->register_id())->has_mode(BpOnExec))
                on_breakpoint_added(event->register_id());
            else
                on_breakpoint_removed(event->register_id());
            break;
        case DbBreakpointRemoved:
            on_breakpoint_removed(event->register_id());
            break;
        case DbRomLoaded:
            on_rom_loaded();
            break;
        default:
            break;
    }
}

void BreakpointModel::on_debugging_paused()
{
    // Erase old PC if it's not an actual breakpoint.
    if (!pc_is_also_breakpoint) addresses.erase(stored_pc);

    // Check if the new PC is an actual breakpoint.
    stored_pc = debugger->read(RegPC);
    if (debugger->breakpoints().count(stored_pc))
    {
        pc_is_also_breakpoint = true;
    }
    else
    {
        addresses.insert(stored_pc);
        pc_is_also_breakpoint = false;
    }

    emit(dataChanged(index(0, 0),
                     index(addresses.size(), COLUMN_COUNT - 1),
                     ALL_ROLES));
}

void BreakpointModel::on_breakpoint_added(uint16_t address)
{
    if (address == stored_pc)
    {
        pc_is_also_breakpoint = true;
    }
    else
    {
        addresses.insert(address);

        int row = 0;
        for (auto item : addresses)
        {
            if (item == address)
            {
                beginInsertRows(QModelIndex(), row, row);
                endInsertRows();
            }
            ++row;
        }
    }

    emit(dataChanged(index(0, 0),
                     index(addresses.size(), COLUMN_COUNT - 1),
                     ALL_ROLES));
}

void BreakpointModel::on_breakpoint_removed(uint16_t address)
{
    int row = 0;
    for (auto item : addresses)
    {
        if (item == address) break;
        ++row;
    }

    if (address == stored_pc && pc_is_also_breakpoint)
        pc_is_also_breakpoint = false;
    else
    {
        addresses.erase(address);
        beginRemoveRows(QModelIndex(), row, row);
        endRemoveRows();
    }

    emit(dataChanged(index(0, 0),
                     index(addresses.size(), COLUMN_COUNT - 1),
                     ALL_ROLES));
}

void BreakpointModel::on_rom_loaded()
{
    if (!pc_is_also_breakpoint)
        addresses.erase(stored_pc);

    stored_pc = debugger->read(RegPC);
    if (addresses.count(stored_pc))
        pc_is_also_breakpoint = true;
    else
    {
        addresses.insert(stored_pc);
        pc_is_also_breakpoint = false;
    }

    emit(dataChanged(index(0, 0),
                     index(addresses.size(), COLUMN_COUNT - 1),
                     ALL_ROLES));
}
