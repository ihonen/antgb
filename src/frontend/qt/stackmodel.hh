#pragma once

#include "../../debugger/debugcore.hh"
#include <QAbstractTableModel>
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

class StackModel : public QAbstractTableModel, DebugObserver
{
    Q_OBJECT
signals:
    void data_changed(const QModelIndex& index);
public:
    DebugCore* debugger = nullptr;

    struct StackItem
    {
        int row = 0;
        bool has_breakpoint = false;
        bool is_current = false;
        uint16_t address = 0x0000;
        uint8_t value[2] = {0x00};
    };

    static const int BREAKPOINT_COLUMN = 0;
    static const int CURRENT_TOP_COLUMN = 1;
    static const int ADDRESS_COLUMN = 2;
    static const int HEX_DATA_COLUMN = 3;

    static const int COLUMN_COUNT = 4;
    static const int ROW_COUNT = 0x10000 / 2;

    StackItem* SP = nullptr;
    std::array<StackItem*, ROW_COUNT> items;

    bool is_debugger_running = false;

    StackModel(DebugCore* debugger, QObject* parent = nullptr);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void update();
    StackItem* get_row(int row);

    virtual void on_debugging_resumed() override;
    virtual void on_debugging_paused() override;
    virtual void on_breakpoint_added(uint16_t address) override;
    virtual void on_breakpoint_removed(uint16_t address) override;
    virtual void on_data_breakpoint_added(uint16_t address) override;
    virtual void on_data_breakpoint_removed(uint16_t address) override;
    virtual void on_memory_changed(uint16_t address) override;
    virtual void on_whole_memory_changed() override;
    virtual void on_special_register_changed() override;
};
