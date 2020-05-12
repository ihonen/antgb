#include "stackmodel.hh"

#include "helper.hh"
#include "colors.hh"
#include "pixmaps.hh"

StackModel::StackModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    debugger(debugger)
{
    debugger->add_observer(this);

    for (uint16_t row = 0; row < ROW_COUNT; ++row)
    {
        auto item = new StackItem;
        item->row = row;
        item->address = 0xFFFE - row * 2;
        items[row] = item;
    }

    update();
}

Qt::ItemFlags StackModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index);
}

QVariant StackModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QVariant StackModel::data(const QModelIndex& index, int role) const
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
                    return hexstr8(item->value[0]) + " " + hexstr8(item->value[1]);
                default:
                    break;
            }
            break;
        case Qt::DecorationRole:
            switch (col)
            {
                case BREAKPOINT_COLUMN:
                    return item->has_breakpoint ? QIcon(Pixmaps::DATA_BREAKPOINT->scaled(9, 9)) : QVariant();
                case CURRENT_TOP_COLUMN:
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

int StackModel::rowCount(const QModelIndex& parent) const
{
    return ROW_COUNT;
}

int StackModel::columnCount(const QModelIndex& parent) const
{
    return COLUMN_COUNT;
}

bool StackModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    return false;
}

void StackModel::update()
{
    for (size_t row = 0; row < ROW_COUNT; ++row)
    {
        auto item = items[row];
        uint16_t low_address = item->address;
        item->value[0] = debugger->emu->mem->read(low_address + 0);
        item->value[1] = debugger->emu->mem->read(low_address + 1);
        if (low_address == debugger->emu->cpu->SP
            || low_address + 1 == debugger->emu->cpu->SP)
        {
            if (SP) SP->is_current = false;
            item->is_current = true;
            SP = item;
        }
    }

    emit(dataChanged(index(0, BREAKPOINT_COLUMN),
                     index(ROW_COUNT - 1, HEX_DATA_COLUMN),
                     {Qt::DisplayRole, Qt::DecorationRole, Qt::ForegroundRole}));
}

StackModel::StackItem* StackModel::get_row(int row)
{
    return items[row];
}

void StackModel::on_debugging_resumed()
{
    is_debugger_running = true;
    update();
}

void StackModel::on_debugging_paused()
{
    is_debugger_running = false;
    update();
}

void StackModel::on_breakpoint_added(uint16_t address)
{

}

void StackModel::on_breakpoint_removed(uint16_t address)
{

}

void StackModel::on_data_breakpoint_added(uint16_t address)
{

}

void StackModel::on_data_breakpoint_removed(uint16_t address)
{

}

void StackModel::on_memory_changed(uint16_t address)
{

}

void StackModel::on_rom_loaded()
{

}

void StackModel::on_special_register_changed()
{

}
