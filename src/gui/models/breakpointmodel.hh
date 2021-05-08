#pragma once

#include "../../debugger/debugcore.hh"
#include <QAbstractTableModel>
#include <QSet>

class BreakpointModel : public QAbstractTableModel, iDebugObserver
{
public:
    std::set<uint16_t> addresses;
    DebugCore* debugger = nullptr;
    uint16_t stored_pc = 0x0000;
    bool pc_is_also_breakpoint = false;

    static const int BREAKPOINT_COLUMN = 0;
    static const int CURRENT_INSTR_COLUMN = 1;
    static const int ADDRESS_COLUMN = 2;
    static const int DISASSEMBLY_COLUMN = 3;

    static const int COLUMN_COUNT = 4;

    BreakpointModel(DebugCore* debugger, QObject* parent = nullptr);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole) override;

    uint16_t get_row(int row) const;

    /* Observer interface */

    virtual void debug_event(const DebugEvent* event) override;

    void on_debugging_resumed();
    void on_debugging_paused();
    void on_breakpoint_added(uint16_t address);
    void on_breakpoint_removed(uint16_t address);
    void on_data_breakpoint_added(uint16_t address);
    void on_data_breakpoint_removed(uint16_t address);
    void on_memory_changed(uint16_t address);
    void on_rom_loaded();
    void on_special_register_changed();
};
