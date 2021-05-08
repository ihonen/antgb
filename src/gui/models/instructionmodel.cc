#include "instructionmodel.hh"

#include "debugger/disassembler.hh"
#include "gui/util/colors.hh"
#include "gui/util/pixmaps.hh"
#include "gui/util/helper.hh"

#include <QHeaderView>
#include <QDebug>


static const QVector<int> ALL_ROLES = {Qt::DecorationRole, Qt::DisplayRole, Qt::ForegroundRole};

InstructionModel::InstructionModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    debugger(debugger)
{
    qRegisterMetaType<QVector<int> >("QVector<int>");
    debugger->add_observer(this);

    visible_row_count = 0;

    // Populate the item pool.
    // We need at most 0xFFFF items, so reserve that many for the duration
    // of the program.
    for (size_t address = 0; address < 0xFFFF; ++address)
    {
        auto item = &item_pool[address];
        item->address = address;
        item->row = address;

        items_by_address[item->address] = item;

        items.push_back(item);
    }

    update_all_from_row(0);
}

InstructionModel::~InstructionModel()
{
    for (auto& item : items)
        delete item;
}

Qt::ItemFlags InstructionModel::flags(const QModelIndex& index) const
{
    if (index.column() == 0 || index.column() == 1) return QAbstractTableModel::flags(index) & ~(Qt::ItemIsSelectable);
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
                    return debugger->disasm()->disassemble(item->data).c_str();
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
                    else
                    {
                        if (item->is_current) return *Colors::FG_BLUE;
                        else if (item->has_breakpoint) return *Colors::FG_RED;
                        else return QVariant();
                    }
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
}

void InstructionModel::update_all_from_row(int row)
{
    is_updating = true;

    int current_row = row;
    size_t current_address = items.size() > row ? items[row]->address : 0x0000;

    while (current_address <= 0xFFFF)
    {
        InstructionItem* item = &item_pool[current_row];
        item->row = current_row;
        if (current_row < visible_row_count)
        {
            emit(dataChanged(index(current_row, 0),
                             index(current_row, LENGTH_COLUMN),
                             ALL_ROLES));
        }
        else
        {
            beginInsertRows(QModelIndex(), current_row, current_row);
            endInsertRows();
        }

        item->address = current_address;

        // TODO: Only disassemble one byte if address == 0xFFFF.
        uint8_t instruction[3] = {0xFF};
        size_t offset = 0;
        for (; offset < 3 && item->address + offset <= 0xFFFF; ++offset)
            instruction[offset] = debugger->read(item->address + offset);

        item->has_breakpoint = debugger->breakpoints().count(item->address) != 0;

        if (item->address == debugger->read(RegPC))
        {

            if (PC) PC->is_current = false;
            item->is_current = true;
            PC = item;
        }
        else item->is_current = false;

        //item->row = current_row;
        item->len = std::max(debugger->disasm()->instr_len(instruction), (size_t)1);

        memcpy(item->data, instruction, item->len);

        // Update containers
        for (size_t i = 0; i < item->len; ++i)
            items_by_address[item->address + i] = item;

        current_address += item->len;
        ++current_row;
    }

    // Remove rows that are now invisible.
    if (current_row < visible_row_count)
    {
        beginRemoveRows(QModelIndex(), current_row, visible_row_count - 1);
        endRemoveRows();
    }

    visible_row_count = current_row;

    is_updating = false;
}

QModelIndex InstructionModel::search_text(const QString& anycase_text, int starting_from)
{
    auto lowercase_text = anycase_text.toLower();

    if (lowercase_text == "pc") return index(PC->row, 0);

    bool is_hex = false;
    auto value = lowercase_text.toUInt(&is_hex, 16);
    if (is_hex && value <= 0xFFFF)
        for (size_t i = 0; i < items.size(); ++i)
            if (items[i]->address >= value)
                return index(i, 0);

    QRegularExpression regex(".*" + escape_regex_special_chars(lowercase_text) + ".*");
    regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    for (size_t i = starting_from; i < visible_row_count; ++i)
    {
        auto match = regex.match(data(index(i, DISASSEMBLY_COLUMN), Qt::DisplayRole).toString());
        if (match.hasMatch()) return index(i, 0);
    }

    return QModelIndex();
}

void InstructionModel::debug_event(const DebugEvent* event)
{
    switch (event->type())
    {
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
        case DbContinued:
            on_debugging_resumed();
            break;
        case DbPaused:
            on_debugging_paused();
            break;
        case DbDataChanged:
            on_memory_changed(event->register_id());
            break;
        case DbRomLoaded:
            on_rom_loaded();
            break;
        default:
            break;
    }
}

void InstructionModel::on_breakpoint_added(uint16_t address)
{
    auto item = items_by_address[address];
    item->has_breakpoint = true;
    emit(dataChanged(index(item->row, BREAKPOINT_COLUMN),
                     index(item->row, LENGTH_COLUMN),
                     {Qt::DecorationRole}));
}

void InstructionModel::on_breakpoint_removed(uint16_t address)
{
    auto item = items_by_address[address];
    item->has_breakpoint = false;
    emit(dataChanged(index(item->row, BREAKPOINT_COLUMN),
                     index(item->row, LENGTH_COLUMN),
                     {Qt::DecorationRole}));
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
    InstructionItem* first_to_change = items_by_address[address];
    assert(first_to_change);
    update_all_from_row(first_to_change->row);
}

void InstructionModel::on_rom_loaded()
{
    update_all_from_row(0);
}
