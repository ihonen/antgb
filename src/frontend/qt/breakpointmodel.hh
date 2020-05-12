#pragma once

#include "../../debugger/debugcore.hh"
#include <QAbstractTableModel>
#include <QSet>

class BreakpointModel : public QAbstractTableModel, DebugObserver
{
public:
    struct BreakpointItem
    {
        uint16_t address;
    };

    std::map<uint16_t, BreakpointItem*> items_by_address;

//    std::set<uint16_t> addresses;

    DebugCore* debugger = nullptr;

    // TODO: Use in actual modeling.

    static const int BREAKPOINT_COLUMN = 0;
    static const int ADDRESS_COLUMN = 1;
    static const int DISASSEMBLY_COLUMN = 2;

    static const int COLUMN_COUNT = 3;

    BreakpointModel(DebugCore* debugger, QObject* parent = nullptr);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole) override;

    BreakpointItem* get_row(int row) const;

    /* Observer interface */

    virtual void on_debugging_resumed() override;
    virtual void on_debugging_paused() override;
    virtual void on_breakpoint_added(uint16_t address) override;
    virtual void on_breakpoint_removed(uint16_t address) override;
    virtual void on_data_breakpoint_added(uint16_t address) override;
    virtual void on_data_breakpoint_removed(uint16_t address) override;
    virtual void on_memory_changed(uint16_t address) override;
    virtual void on_rom_loaded() override;
    virtual void on_special_register_changed() override;
};
