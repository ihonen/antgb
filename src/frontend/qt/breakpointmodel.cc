#include "breakpointmodel.hh"

#include "helper.hh"
#include "pixmaps.hh"

BreakpointModel::BreakpointModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    DebugObserver(),
    debugger(debugger)
{
    debugger->add_observer(this);
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
    auto item = get_row(index.row());

    switch (role)
    {
        case Qt::DisplayRole:
            switch (index.column())
            {
                case ADDRESS_COLUMN:
                    return hexstr16(item->address);
                case DISASSEMBLY_COLUMN:
                {
                    uint8_t instruction[3];
                    size_t offset = 0;
                    for (; offset < 3 && item->address + offset <= 0xFFFF; ++offset)
                        instruction[offset] = debugger->emu->mem->read(item->address + offset);
                    return debugger->disassembler.disassemble(instruction).c_str();
                }
                default: break;
            }
            break;

        case Qt::DecorationRole:
            switch (index.column())
            {
                case BREAKPOINT_COLUMN:
                    return Pixmaps::BREAKPOINT->scaled(9, 9);
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
    return static_cast<int>(items_by_address.size());
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

BreakpointModel::BreakpointItem* BreakpointModel::get_row(int row) const
{
    int current_row = 0;
    for (const auto& item : items_by_address)
    {
        if (current_row == row)
            return item.second;
        ++current_row;
    }
    assert(false);
    return nullptr;
}

void BreakpointModel::on_debugging_resumed()
{

}

void BreakpointModel::on_debugging_paused()
{

}

void BreakpointModel::on_breakpoint_added(uint16_t address)
{
    if (items_by_address.count(address) == 0)
    {
        items_by_address[address] = new BreakpointItem{address};

        // Figure out the row.
        int row = 0;
        for (auto& item : items_by_address)
        {
            if (item.second->address == address)
            {
                beginInsertRows(QModelIndex(), row, row);
                endInsertRows();
                emit(dataChanged(index(0, 0), index(items_by_address.size() - 1, 2), {Qt::DisplayRole, Qt::DecorationRole}));
                break;
            }
            ++row;
        }
    }
}

void BreakpointModel::on_breakpoint_removed(uint16_t address)
{
    if (items_by_address.count(address))
    {
        // Figure out the row.
        int row = 0;
        for (auto it = items_by_address.begin(); it != items_by_address.end(); ++it)
        {
            if (it->second->address == address)
            {
                beginRemoveRows(QModelIndex(), row, row);
                items_by_address.erase(it);
                delete it->second;
                endRemoveRows();
                emit(dataChanged(index(0, 0), index(items_by_address.size() - 1, 2), {Qt::DisplayRole, Qt::DecorationRole}));
                break;
            }
            ++row;
        }
    }
}

void BreakpointModel::on_data_breakpoint_added(uint16_t address)
{

}

void BreakpointModel::on_data_breakpoint_removed(uint16_t address)
{

}

void BreakpointModel::on_memory_changed(uint16_t address)
{

}

void BreakpointModel::on_rom_loaded()
{
    for (size_t i = 0; i < items_by_address.size(); ++i)
    {
        emit(dataChanged(index(i, 0),
                         index(i, COLUMN_COUNT - 1),
                         {Qt::DecorationRole,
                          Qt::DisplayRole}));
    }
}

void BreakpointModel::on_special_register_changed()
{

}
