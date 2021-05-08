#include "registermodel.hh"

#include "gui/util/helper.hh"
#include "gui/util/colors.hh"

#include "debugger/addresses.hh"

RegisterModel::RegisterModel(DebugCore* debugger, QObject* parent) :
    QAbstractItemModel(parent),
    debugger(debugger)
{
    debugger->add_observer(this);

    root = new RegisterItem;

    auto apu_item = new RegisterItem;
    apu_item->parent = root;
    apu_item->name = "APU";
    apu_item->children =
    {
        new RegisterItem{apu_item, {}, "NR10", 8, NR10_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR11", 8, NR11_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR12", 8, NR12_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR13", 8, NR13_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR14", 8, NR14_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR21", 8, NR21_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR22", 8, NR22_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR23", 8, NR23_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR24", 8, NR24_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR30", 8, NR30_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR31", 8, NR31_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR32", 8, NR32_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR33", 8, NR33_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR34", 8, NR34_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR41", 8, NR41_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR42", 8, NR42_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR43", 8, NR43_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR44", 8, NR44_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR50", 8, NR50_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR51", 8, NR51_ADDR, {}},
        new RegisterItem{apu_item, {}, "NR52", 8, NR52_ADDR, {}}
    };

    auto cpu_item = new RegisterItem;
    cpu_item->parent = root;
    cpu_item->name = "CPU";
    cpu_item->children =
    {
        new RegisterItem{cpu_item, {}, "AF", 16, RegAF, {}},
        new RegisterItem{cpu_item, {}, "BC", 16, RegBC, {}},
        new RegisterItem{cpu_item, {}, "DE", 16, RegDE, {}},
        new RegisterItem{cpu_item, {}, "HL", 16, RegHL, {}},
        new RegisterItem{cpu_item, {}, "SP", 16, RegSP, {}},
        new RegisterItem{cpu_item, {}, "PC", 16, RegPC, {}}
    };

    auto interrupts_item = new RegisterItem;
    interrupts_item->parent = root;
    interrupts_item->name = "Interrupts";
    interrupts_item->children =
    {
        new RegisterItem{interrupts_item, {}, "IME", 8, RegIME, {}},
        new RegisterItem{interrupts_item, {}, "IE", 8, IE_ADDR, {}},
        new RegisterItem{interrupts_item, {}, "IF", 8, IF_ADDR, {}}
    };

    auto joypad_item = new RegisterItem;
    joypad_item->parent = root;
    joypad_item->name = "Joypad";
    joypad_item->children =
    {
        new RegisterItem{joypad_item, {}, "JOYP", 8, JOYP_ADDR, {}}
    };

    auto lcd_item = new RegisterItem;
    lcd_item->parent = root;
    lcd_item->name = "LCD";
    lcd_item->children =
    {
        new RegisterItem{lcd_item, {}, "LCDC", 8, LCDC_ADDR, {}},
        new RegisterItem{lcd_item, {}, "STAT", 8, STAT_ADDR, {}},
        new RegisterItem{lcd_item, {}, "SCY", 8, SCY_ADDR, {}},
        new RegisterItem{lcd_item, {}, "SCX", 8, SCX_ADDR, {}},
        new RegisterItem{lcd_item, {}, "LY", 8, LY_ADDR, {}},
        new RegisterItem{lcd_item, {}, "LYC", 8, LYC_ADDR, {}},
        new RegisterItem{lcd_item, {}, "DMA", 8, DMA_ADDR, {}},
        new RegisterItem{lcd_item, {}, "BGP", 8, BGP_ADDR, {}},
        new RegisterItem{lcd_item, {}, "OBP0", 8, OBP0_ADDR, {}},
        new RegisterItem{lcd_item, {}, "OBP1", 8, OBP1_ADDR, {}},
        new RegisterItem{lcd_item, {}, "WY", 8, WY_ADDR, {}},
        new RegisterItem{lcd_item, {}, "WX", 8, WX_ADDR, {}}
    };

    auto serial_item = new RegisterItem;
    serial_item->parent = root;
    serial_item->name = "Serial";
    serial_item->children =
    {
        new RegisterItem{serial_item, {}, "SB", 8, SB_ADDR, {}},
        new RegisterItem{serial_item, {}, "SC", 8, SC_ADDR, {}},
    };

    auto timer_item = new RegisterItem;
    timer_item->parent = root;
    timer_item->name = "Timer";
    timer_item->children =
    {
        new RegisterItem{timer_item, {}, "DIV", 8, DIV_ADDR, {}},
        new RegisterItem{timer_item, {}, "TIMA", 8, TIMA_ADDR, {}},
        new RegisterItem{timer_item, {}, "TMA", 8, TMA_ADDR, {}},
        new RegisterItem{timer_item, {}, "TAC", 8, TAC_ADDR, {}}
    };

    root->children =
    {
        apu_item,
        cpu_item,
        interrupts_item,
        joypad_item,
        lcd_item,
        serial_item,
        timer_item
    };

    for (size_t i = 0; i < rowCount(); ++i)
    {
        auto parent_index = index(i, 0, QModelIndex());
        auto parent_item = root->children[i];
        setData(parent_index, parent_item->name, Qt::DisplayRole);

        for (size_t j = 0; j < rowCount(parent_index); ++j)
        {
            auto child_index = index(j, 0, parent_index);
            auto child_item = parent_item->children[j];
            setData(child_index, child_item->name, Qt::DisplayRole);
        }
    }

    update_all();
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex& index) const
{
    return QAbstractItemModel::flags(index);
}

QVariant RegisterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return "";
}

QVariant RegisterModel::data(const QModelIndex& index, int role) const
{
    RegisterItem* item = static_cast<RegisterItem*>(index.internalPointer());
    return item->data_by_column[index.column()][static_cast<Qt::ItemDataRole>(role)];
}

int RegisterModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid()) return root->children.size();
    return static_cast<RegisterItem*>(parent.internalPointer())->children.size();
}

int RegisterModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

bool RegisterModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    RegisterItem* item = static_cast<RegisterItem*>(index.internalPointer());

    item->data_by_column[index.column()][static_cast<Qt::ItemDataRole>(role)] = value;
    emit(data_changed(index));
    return true;
}

QModelIndex RegisterModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();

    RegisterItem *parent_item = parent.isValid()
           ? static_cast<RegisterItem*>(parent.internalPointer())
           : root;

    RegisterItem* child_item = parent_item->children.at(row);

    return createIndex(row, column, child_item);
}

QModelIndex RegisterModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    RegisterItem* child_item = static_cast<RegisterItem*>(index.internalPointer());
    RegisterItem* parent_item = child_item->parent;

    if (parent_item == root) return QModelIndex();
    else return createIndex(parent_item->row_in_parent(), 0, parent_item);
}

bool RegisterModel::hasChildren(const QModelIndex& parent) const
{
    if (!parent.isValid()) return true;
    return static_cast<RegisterItem*>(parent.internalPointer())->children.size() != 0;
}

void RegisterModel::update_all()
{    
    for (int i = 0; i < rowCount(); ++i)
    {
        auto parent_index = index(i, 0, QModelIndex());
        auto parent_item = root->children[i];

        for (int j = 0; j < rowCount(parent_index); ++j)
        {
            auto child_item = parent_item->children[j];
            auto child_name_index = index(j, 0, parent_index);
            auto child_value_index = index(j, 1, parent_index);

            if (child_item->size == 8)
                setData(child_value_index, hexstr8(debugger->read(child_item->register_id)), Qt::DisplayRole);
            else if (child_item->size == 16)
                setData(child_value_index, hexstr16(debugger->read(child_item->register_id)), Qt::DisplayRole);
            if (is_debugger_running)
            {
                setData(parent_index, *Colors::FG_LIGHT_GRAY, Qt::ForegroundRole);
                setData(child_name_index, *Colors::FG_LIGHT_GRAY, Qt::ForegroundRole);
                setData(child_value_index, *Colors::FG_LIGHT_GRAY, Qt::ForegroundRole);
            }
            else
            {
                setData(parent_index, *Colors::FG_WHITE, Qt::ForegroundRole);
                setData(child_name_index, *Colors::FG_WHITE, Qt::ForegroundRole);
                setData(child_value_index, *Colors::FG_WHITE, Qt::ForegroundRole);
            }
        }
    }
}

void RegisterModel::debug_event(const DebugEvent* event)
{
    switch (event->type())
    {
        case DbContinued:
            is_debugger_running = true;
            update_all();
            break;
        case DbPaused:
            is_debugger_running = false;
            update_all();
            break;
        default:
            break;
    }
}
