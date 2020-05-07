#pragma once

#include "../../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

#include <QAbstractTableModel>

class DataModel : public QAbstractTableModel
{
    Q_OBJECT
 signals:
     void data_changed(const QModelIndex& index);
public:
    struct DataItem
    {
        uint16_t address = 0x0000;
        uint8_t data[16] = {0};
    };

    std::array<DataItem, 0x10000 / 16> items;

    DebugCore* debugger = nullptr;

    int row_count = 0;
    int column_count = 0;

    DataModel(DebugCore* debugger, QObject* parent = nullptr);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void update();
};
