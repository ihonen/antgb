#include "instructionmodel.hh"

#include "../../debugger/disassembler.hh"
#include "helper.hh"
#include <QHeaderView>

#include "helper.hh"
#include <QDebug>

#include "colors.hh"
#include "pixmaps.hh"

InstructionModel::InstructionModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    debugger(debugger)
{
    qRegisterMetaType<QVector<int> >("QVector<int>");
    debugger->add_observer(this);

    for (size_t i = 0; i < 0x10000; ++i)
        items.push_back(new InstructionItem);
    total_row_count = 0x10000;
    visible_row_count = 0x1000;

    update_all_from_row(0);
}

Qt::ItemFlags InstructionModel::flags(const QModelIndex& index) const
{
    if (index.column() == 0 || index.column() == 1) return QAbstractTableModel::flags(index) & ~(Qt::ItemIsSelectable) & ~(Qt::ItemIsDragEnabled);
    else return QAbstractTableModel::flags(index);
}

QVariant InstructionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant("");
}

InstructionModel::InstructionItem* InstructionModel::get_row(int row_index)
{
    return items[row_index];
}

QVariant InstructionModel::data(const QModelIndex& index, int role) const
{
    int col = index.column();
    auto item = items[index.row()];

    switch (role)
    {
        case Qt::DisplayRole:
            switch (col)
            {
                case MEM_REGION_COLUMN:
                    return memory_abbreviation(item->address) + ":";
                case ADDRESS_COLUMN:
                    return hexstr16(item->address).toUpper();
                case HEX_DATA_COLUMN:
                    return hexstr(item->data, std::max(item->len, (uint8_t)1));
                case DISASSEMBLY_COLUMN:
                    return debugger->disassembler.disassemble(item->data).c_str();
                case LENGTH_COLUMN:
                    return std::max(item->len, (uint8_t)1);
                default:
                    break;
            }
            break;
        case Qt::DecorationRole:
            switch (col)
            {
                case BREAKPOINT_COLUMN:
                    return item->has_breakpoint ? QIcon(Pixmaps::BREAKPOINT->scaled(9, 9)) : QVariant();
                case CURRENT_INSTR_COLUMN:
                    return item->is_current ? QIcon(Pixmaps::CURRENT_ARROW->scaled(11, 11)) : QVariant();
                default:
                    break;
            }
            break;
        case Qt::ForegroundRole:
            switch (col)
            {
                default:
                    if (is_debugger_running) return *Colors::FG_LIGHT_GRAY;
                    return item->is_current ? *Colors::FG_BLUE : QVariant();
            }
        default:
            break;
    }

    return QVariant();
}

int InstructionModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return visible_row_count;
}

int InstructionModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

bool InstructionModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
    assert(index.isValid());
//    items[index.row()]->data_by_column[index.column()][role] = value;
    emit(dataChanged(index, index, {role}));
    return true;
}

void InstructionModel::update_all_from_row(int row)
{
    size_t address = items[row]->address;
    int current_row = row;

    while (address < total_row_count)
    {
        // Maximum instruction length is 3 bytes, so read that many.
        // TODO: Don't overindex memory.
        uint8_t instruction[3];
        for (size_t i = 0; i < 3; ++i)
            instruction[i] = debugger->emu->mem->read(address + i);

        InstructionItem* item(items[current_row]);
        // TODO: Actually check if there's a breakpoint here.
        item->row = current_row;
        item->has_breakpoint = debugger->breakpoints.count(address) != 0;
        item->address = address;
        if (item->address == debugger->emu->cpu->PC)
        {
            if (PC) PC->is_current = false;
            item->is_current = true;
            PC = item;
        }
        item->len = debugger->disassembler.instr_len(instruction);
        auto effective_len = std::max(item->len, (uint8_t)1);
        memcpy(item->data, instruction, effective_len);

        for (size_t i = 0; i < effective_len; ++i)
            items_by_address[address + i] = item;

        emit(dataChanged(index(current_row, BREAKPOINT_COLUMN),
                         index(current_row, LENGTH_COLUMN),
                         {Qt::DisplayRole, Qt::DecorationRole, Qt::ForegroundRole}));

        address += effective_len;
        ++current_row;
    }

    visible_row_count = current_row;

}

QModelIndex InstructionModel::search(const QString& text)
{
    bool is_hex = false;
    auto value = text.toUInt(&is_hex, 16);
    if (is_hex && value <= 0xFFFF)
        for (size_t i = 0; i < items.size(); ++i)
            if (items[i]->address >= value)
                return index(i, 0);

    QRegularExpression regex(".*" + escape_regex_special_chars(text) + ".*");
    regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    for (size_t i = 0; i < items.size(); ++i)
    {
        auto match = regex.match(data(index(i, DISASSEMBLY_COLUMN), Qt::DisplayRole).toString());
        if (match.hasMatch()) return index(i, 0);
    }

    return QModelIndex();
}

void InstructionModel::on_breakpoint_added(uint16_t address)
{
    auto item = items_by_address[address];
    item->has_breakpoint = true;
    emit(dataChanged(index(item->row, BREAKPOINT_COLUMN),
                     index(item->row, BREAKPOINT_COLUMN),
                     {Qt::DecorationRole}));
}

void InstructionModel::on_breakpoint_removed(uint16_t address)
{
    auto item = items_by_address[address];
    item->has_breakpoint = false;
    emit(dataChanged(index(item->row, BREAKPOINT_COLUMN),
                     index(item->row, BREAKPOINT_COLUMN),
                     {Qt::DecorationRole}));
}

void InstructionModel::on_data_breakpoint_added(uint16_t address)
{

}

void InstructionModel::on_data_breakpoint_removed(uint16_t address)
{

}

void InstructionModel::on_debugging_resumed()
{
    is_debugger_running = true;
    PC->is_current = false;
    emit(dataChanged(index(PC->row, CURRENT_INSTR_COLUMN),
                     index(PC->row, CURRENT_INSTR_COLUMN),
                     {Qt::DecorationRole}));
    emit(dataChanged(index(0, BREAKPOINT_COLUMN),
                     index(visible_row_count - 1, CURRENT_INSTR_COLUMN),
                     {Qt::ForegroundRole}));
}

void InstructionModel::on_debugging_paused()
{
    is_debugger_running = false;
    update_all_from_row(0);
}

void InstructionModel::on_memory_changed(uint16_t address)
{
    // TODO: Do this more efficiently.
    InstructionItem* first_to_change = nullptr;
    for (size_t i = 0; !first_to_change; ++i)
    {
        assert(i < 5);
        first_to_change = items_by_address[address - i];
    }
    assert(first_to_change);
    update_all_from_row(0);
}

void InstructionModel::on_whole_memory_changed()
{
    update_all_from_row(0);
}

void InstructionModel::on_special_register_changed()
{
    /*
    PC->is_current = false;
    emit(dataChanged(index(PC->row, BREAKPOINT_COLUMN),
                     index(PC->row, LENGTH_COLUMN),
                     {Qt::DisplayRole, Qt::DecorationRole, Qt::ForegroundRole}));

    for (int row = 0; row < items.size(); ++row)
    {
        auto item = items[row];
        if (item->address == debugger->emu->cpu->PC)
        {
            item->is_current = true;
            PC = item;
            emit(dataChanged(index(PC->row, 0), index(PC->row, 0), {Qt::DisplayRole, Qt::DecorationRole, Qt::ForegroundRole}));
            emit(dataChanged(index(PC->row, BREAKPOINT_COLUMN),
                             index(PC->row, LENGTH_COLUMN),
                             {Qt::DisplayRole, Qt::DecorationRole, Qt::ForegroundRole}));
            break;
        }
    }
    */
}
