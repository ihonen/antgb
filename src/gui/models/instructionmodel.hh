#pragma once

#include "debugger/idebugobserver.hh"
#include "debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

#include <QAbstractTableModel>

class InstructionModel : public QAbstractTableModel, iDebugObserver
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

    // More than 0xFFFF items are never needed, because every instruction
    // occupies at least one byte. Statically reserve memory for that many
    // items.
    std::array<InstructionItem, 0xFFFF> item_pool;
    // Actual items currently being shown in the view.
    QVector<InstructionItem*> items;
    // NOTE: Multiple addresses will be referring to the same item if the item
    // consists of multiple bytes.
    QMap<int, InstructionItem*> items_by_address;
    // Points to the item that has the current address of the program counter.
    InstructionItem* PC = nullptr;

    DebugCore* debugger = nullptr;
    bool is_debugger_running = false;

    bool is_updating = false;

    InstructionModel(DebugCore* debugger, QObject* parent = nullptr);
    virtual ~InstructionModel() override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    InstructionItem* get_row(int row_index);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void update_all_from_row(int row);
    void clear_all();
    QModelIndex search_text(const QString& text, int starting_from = 0);

    virtual void debug_event(const DebugEvent* event) override;

    void on_breakpoint_added(uint16_t address);
    void on_breakpoint_removed(uint16_t address);
    void on_debugging_resumed();
    void on_debugging_paused();
    void on_memory_changed(uint16_t address);
    void on_rom_loaded();
};
