#include "memorymodel.hh"

#include "helper.hh"
#include "colors.hh"
#include "pixmaps.hh"

MemoryModel::MemoryModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    debugger(debugger)
{
    debugger->add_observer(this);

    for (size_t row = 0; row < ROW_COUNT; ++row)
    {
        auto item = new MemoryItem;
        item->address = row;
        items[row] = item;
    }

    update();
}

Qt::ItemFlags MemoryModel::flags(const QModelIndex& index) const
{
    if (index.column() == 0 || index.column() == 1) return QAbstractTableModel::flags(index) & ~(Qt::ItemIsSelectable) & ~(Qt::ItemIsDragEnabled);
    else return QAbstractTableModel::flags(index);
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QVariant MemoryModel::data(const QModelIndex& index, int role) const
{
    auto item = items[index.row()];
    int col = index.column();

    switch (role)
    {
        case Qt::DisplayRole:
            switch (col)
            {
                case ADDRESS_COLUMN:
                    return memory_abbreviation(item->address) + ": " + hexstr16(item->address).toUpper() + ":";
                case HEX_DATA_COLUMN:
                    return hexstr8(item->value);
                case NAME_COLUMN:
                    return register_name(item->address);
                default:
                    break;
            }
            break;
        case Qt::DecorationRole:
            switch (col)
            {
                case BREAKPOINT_COLUMN:
                    return item->has_breakpoint ? QIcon(Pixmaps::DATA_BREAKPOINT->scaled(9, 9)) : QVariant();
                case CURRENT_ITEM_COLUMN:
                    return item->is_current ? QIcon(Pixmaps::CURRENT_ARROW->scaled(11, 11)) : QVariant();
                default:
                    break;
            }
            break;
        case Qt::ForegroundRole:
            if (is_debugger_running) return *Colors::FG_LIGHT_GRAY;
            return item->is_current ? *Colors::FG_BLUE : *Colors::FG_WHITE;
    }

    return QVariant();
}

int MemoryModel::rowCount(const QModelIndex& parent) const
{
    return ROW_COUNT;
}

int MemoryModel::columnCount(const QModelIndex& parent) const
{
    return COLUMN_COUNT;
}

bool MemoryModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    return false;
}

void MemoryModel::update()
{
    for (size_t row = 0; row < ROW_COUNT; ++row)
    {
        auto item = items[row];
        item->value = debugger->emu->mem->read(item->address);
    }

    emit(dataChanged(index(0, BREAKPOINT_COLUMN),
                     index(ROW_COUNT - 1, HEX_DATA_COLUMN),
                     {Qt::DisplayRole, Qt::DecorationRole, Qt::ForegroundRole}));
}

MemoryModel::MemoryItem* MemoryModel::get_row(int row)
{
    return items[row];
}

void MemoryModel::on_debugging_resumed()
{
    is_debugger_running = true;
    update();
}

void MemoryModel::on_debugging_paused()
{
    is_debugger_running = false;
    update();
}

void MemoryModel::on_breakpoint_added(uint16_t address)
{

}

void MemoryModel::on_breakpoint_removed(uint16_t address)
{

}

void MemoryModel::on_data_breakpoint_added(uint16_t address)
{
    auto item = items[address];
    item->has_breakpoint = true;
    emit(dataChanged(index(item->address, BREAKPOINT_COLUMN),
                     index(item->address, BREAKPOINT_COLUMN),
                     {Qt::DecorationRole}));
}

void MemoryModel::on_data_breakpoint_removed(uint16_t address)
{
    auto item = items[address];
    item->has_breakpoint = false;
    emit(dataChanged(index(item->address, BREAKPOINT_COLUMN),
                     index(item->address, BREAKPOINT_COLUMN),
                     {Qt::DecorationRole}));
}

void MemoryModel::on_memory_changed(uint16_t address)
{

}

void MemoryModel::on_rom_loaded()
{

}

void MemoryModel::on_special_register_changed()
{

}
