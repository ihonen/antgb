#pragma once

#include "../../debugger/debugcore.hh"
#include <QAbstractTableModel>
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

class StackModel : public QAbstractTableModel, iDebugObserver
{
    Q_OBJECT
signals:
    void data_changed(const QModelIndex& index);
public:
    DebugCore* debugger = nullptr;

    static const int BREAKPOINT_COLUMN = 0;
    static const int CURRENT_TOP_COLUMN = 1;
    static const int ADDRESS_COLUMN = 2;
    static const int HEX_DATA_COLUMN = 3;

    static const int COLUMN_COUNT = 4;
    static const int ROW_COUNT = 0x10000 / 2;

    StackModel(DebugCore* debugger, QObject* parent = nullptr);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void update();

    virtual void debug_event(const DebugEvent* event) override;

    int address_to_row(regid_t address) const
    {
        auto row = (MAX_ADDRESS - 1 - address) / 2;
        cerr << "address: " << address << ", row: " << row << endl;
        return row;
    }

    int row_to_address(int row) const
    {
        auto address = (MAX_ADDRESS - 1) - (row * 2);
        cerr << "row: " << row << ", address: " << address << endl;
        return address;
    }
};
