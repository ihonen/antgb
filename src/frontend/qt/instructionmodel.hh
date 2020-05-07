#pragma once

#include "../../debugger/debugobserver.hh"
#include "../../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

#include <QAbstractTableModel>

class InstructionModel : public QAbstractTableModel, DebugObserver
{
    Q_OBJECT
public:
    // Updating this model is a heavy operation, so don't store
    // the representations of the values here - only the raw values.
    struct InstructionItem
    {
        int row = 0;
        bool has_breakpoint = false;
        bool is_current = false;
        uint16_t address = 0x0000;
        uint8_t data[8] = {0};
        uint8_t len = 0;
    };

    static const int BREAKPOINT_COLUMN = 0;
    static const int CURRENT_INSTR_COLUMN = 1;
    static const int MEM_REGION_COLUMN = 2;
    static const int ADDRESS_COLUMN = 3;
    static const int HEX_DATA_COLUMN = 4;
    static const int DISASSEMBLY_COLUMN = 5;
    static const int LENGTH_COLUMN = 6;

    static const int COLUMN_COUNT = 7;
    int visible_row_count = 0;
    int total_row_count = 0;

    QVector<InstructionItem*> items;
    QMap<int, InstructionItem*> items_by_address;
    InstructionItem* PC = nullptr;

    DebugCore* debugger = nullptr;

    bool is_debugger_running = false;

    InstructionModel(DebugCore* debugger, QObject* parent = nullptr);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    InstructionItem* get_row(int row_index);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void update_all_from_row(int row);
    void clear_all();
    QModelIndex search(const QString& text);

    virtual void on_breakpoint_added(uint16_t address) override;
    virtual void on_breakpoint_removed(uint16_t address) override;
    virtual void on_data_breakpoint_added(uint16_t address) override;
    virtual void on_data_breakpoint_removed(uint16_t address) override;
    virtual void on_debugging_resumed() override;
    virtual void on_debugging_paused() override;
    virtual void on_memory_changed(uint16_t address) override;
    virtual void on_whole_memory_changed() override;
    virtual void on_special_register_changed() override;
};
