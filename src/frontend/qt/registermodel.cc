#include "registermodel.hh"

#include "helper.hh"

#include "colors.hh"

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
        new RegisterItem{apu_item, {}, "NR10", 8, &debugger->emu->mem->hff10_nr10, {}},
        new RegisterItem{apu_item, {}, "NR11", 8, &debugger->emu->mem->hff11_nr11, {}},
        new RegisterItem{apu_item, {}, "NR12", 8, &debugger->emu->mem->hff12_nr12, {}},
        new RegisterItem{apu_item, {}, "NR13", 8, &debugger->emu->mem->hff13_nr13, {}},
        new RegisterItem{apu_item, {}, "NR14", 8, &debugger->emu->mem->hff14_nr14, {}},
        new RegisterItem{apu_item, {}, "NR21", 8, &debugger->emu->mem->hff16_nr21, {}},
        new RegisterItem{apu_item, {}, "NR22", 8, &debugger->emu->mem->hff17_nr22, {}},
        new RegisterItem{apu_item, {}, "NR23", 8, &debugger->emu->mem->hff18_nr23, {}},
        new RegisterItem{apu_item, {}, "NR24", 8, &debugger->emu->mem->hff19_nr24, {}},
        new RegisterItem{apu_item, {}, "NR30", 8, &debugger->emu->mem->hff1a_nr30, {}},
        new RegisterItem{apu_item, {}, "NR31", 8, &debugger->emu->mem->hff1b_nr31, {}},
        new RegisterItem{apu_item, {}, "NR32", 8, &debugger->emu->mem->hff1c_nr32, {}},
        new RegisterItem{apu_item, {}, "NR33", 8, &debugger->emu->mem->hff1d_nr33, {}},
        new RegisterItem{apu_item, {}, "NR34", 8, &debugger->emu->mem->hff1e_nr34, {}},
        new RegisterItem{apu_item, {}, "NR41", 8, &debugger->emu->mem->hff20_nr_41, {}},
        new RegisterItem{apu_item, {}, "NR42", 8, &debugger->emu->mem->hff21_nr_42, {}},
        new RegisterItem{apu_item, {}, "NR43", 8, &debugger->emu->mem->hff22_nr_43, {}},
        new RegisterItem{apu_item, {}, "NR44", 8, &debugger->emu->mem->hff23_nr_44, {}},
        new RegisterItem{apu_item, {}, "NR50", 8, &debugger->emu->mem->hff24_nr_50, {}},
        new RegisterItem{apu_item, {}, "NR51", 8, &debugger->emu->mem->hff25_nr_51, {}},
        new RegisterItem{apu_item, {}, "NR52", 8, &debugger->emu->mem->hff26_nr_52, {}}
    };

    auto cpu_item = new RegisterItem;
    cpu_item->parent = root;
    cpu_item->name = "CPU";
    cpu_item->children =
    {
        new RegisterItem{cpu_item, {}, "AF", 16, &debugger->emu->cpu->AF, {}},
        new RegisterItem{cpu_item, {}, "BC", 16, &debugger->emu->cpu->BC, {}},
        new RegisterItem{cpu_item, {}, "DE", 16, &debugger->emu->cpu->DE, {}},
        new RegisterItem{cpu_item, {}, "HL", 16, &debugger->emu->cpu->HL, {}},
        new RegisterItem{cpu_item, {}, "SP", 16, &debugger->emu->cpu->SP, {}},
        new RegisterItem{cpu_item, {}, "PC", 16, &debugger->emu->cpu->PC, {}}
    };

    auto interrupts_item = new RegisterItem;
    interrupts_item->parent = root;
    interrupts_item->name = "Interrupts";
    interrupts_item->children =
    {
        new RegisterItem{interrupts_item, {}, "IME", 8, &debugger->emu->irc->IME, {}},
        new RegisterItem{interrupts_item, {}, "IE", 8, &debugger->emu->mem->hffff_ie, {}},
        new RegisterItem{interrupts_item, {}, "IF", 8, &debugger->emu->mem->hff0f_if, {}}
    };

    auto joypad_item = new RegisterItem;
    joypad_item->parent = root;
    joypad_item->name = "Joypad";
    joypad_item->children =
    {
        new RegisterItem{joypad_item, {}, "JOYP", 8, &debugger->emu->mem->hff00_joyp, {}}
    };

    auto lcd_item = new RegisterItem;
    lcd_item->parent = root;
    lcd_item->name = "LCD";
    lcd_item->children =
    {
        new RegisterItem{lcd_item, {}, "LCDC", 8, &debugger->emu->mem->hff40_lcdc, {}},
        new RegisterItem{lcd_item, {}, "STAT", 8, &debugger->emu->mem->hff41_stat, {}},
        new RegisterItem{lcd_item, {}, "SCY", 8, &debugger->emu->mem->hff42_scy, {}},
        new RegisterItem{lcd_item, {}, "SCX", 8, &debugger->emu->mem->hff43_scx, {}},
        new RegisterItem{lcd_item, {}, "LY", 8, &debugger->emu->mem->hff44_ly, {}},
        new RegisterItem{lcd_item, {}, "LYC", 8, &debugger->emu->mem->hff45_lyc, {}},
        new RegisterItem{lcd_item, {}, "DMA", 8, &debugger->emu->mem->hff46_dma, {}},
        new RegisterItem{lcd_item, {}, "BGP", 8, &debugger->emu->mem->hff47_bgp, {}},
        new RegisterItem{lcd_item, {}, "OBP0", 8, &debugger->emu->mem->hff48_obp0, {}},
        new RegisterItem{lcd_item, {}, "OBP1", 8, &debugger->emu->mem->hff49_obp1, {}},
        new RegisterItem{lcd_item, {}, "WY", 8, &debugger->emu->mem->hff4a_wy, {}},
        new RegisterItem{lcd_item, {}, "WX", 8, &debugger->emu->mem->hff4b_wx, {}}
    };

    auto serial_item = new RegisterItem;
    serial_item->parent = root;
    serial_item->name = "Serial";
    serial_item->children =
    {
        new RegisterItem{serial_item, {}, "SB", 8, &debugger->emu->mem->hff01_sb, {}},
        new RegisterItem{serial_item, {}, "SC", 8, &debugger->emu->mem->hff02_sc, {}},
    };

    auto timer_item = new RegisterItem;
    timer_item->parent = root;
    timer_item->name = "Timer";
    timer_item->children =
    {
        new RegisterItem{timer_item, {}, "DIV", 8, &debugger->emu->mem->hff04_div, {}},
        new RegisterItem{timer_item, {}, "TIMA", 8, &debugger->emu->mem->hff05_tima, {}},
        new RegisterItem{timer_item, {}, "TMA", 8, &debugger->emu->mem->hff06_tma, {}},
        new RegisterItem{timer_item, {}, "TAC", 8, &debugger->emu->mem->hff07_tac, {}}
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

            if (child_item->size == 8 && child_item->register_location)
                setData(child_value_index, hexstr8(*static_cast<uint8_t*>(child_item->register_location)), Qt::DisplayRole);
            else if (child_item->size == 16 && child_item->register_location)
                setData(child_value_index, hexstr16(*static_cast<uint16_t*>(child_item->register_location)), Qt::DisplayRole);
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





void RegisterModel::on_debugging_resumed()
{
    is_debugger_running = true;
    update_all();
}

void RegisterModel::on_debugging_paused()
{
    is_debugger_running = false;
    update_all();
}

void RegisterModel::on_breakpoint_added(uint16_t address)
{

}

void RegisterModel::on_breakpoint_removed(uint16_t address)
{

}

void RegisterModel::on_data_breakpoint_added(uint16_t address)
{

}

void RegisterModel::on_data_breakpoint_removed(uint16_t address)
{

}

void RegisterModel::on_memory_changed(uint16_t address)
{

}

void RegisterModel::on_rom_loaded()
{

}

void RegisterModel::on_special_register_changed()
{

}
