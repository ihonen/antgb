#pragma once


#include "debugger/debugcore.hh"
#include <QAbstractTableModel>
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

class MemoryModel : public QAbstractTableModel, iDebugObserver
{
    Q_OBJECT
public:
    DebugCore* debugger = nullptr;

    static constexpr int BREAKPOINT_COLUMN = 0;
    static constexpr int CURRENT_ITEM_COLUMN = 1;
    static constexpr int ADDRESS_COLUMN = 2;
    static constexpr int HEX_DATA_COLUMN = 3;
    static constexpr int NAME_COLUMN = 4;

    static constexpr int COLUMN_COUNT = 5;
    static constexpr int ROW_COUNT = MAX_ADDRESS + 1;

    MemoryModel(DebugCore* debugger, QObject* parent = nullptr);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void update();

    virtual void debug_event(const DebugEvent* event) override;
};
