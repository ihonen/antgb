#pragma once

#include "../../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

#include <QAbstractTableModel>

class DataModel : public QAbstractTableModel, DebugObserver
{
    Q_OBJECT
 signals:
     void data_changed(const QModelIndex& index);
public:
    static constexpr int BYTES_PER_ROW = 16;

    static constexpr int MEM_NAME_COLUMN = 0;
    static constexpr int ADDRESS_COLUMN = 1;
    static constexpr int HEX_COLUMN[BYTES_PER_ROW] =
        {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    static constexpr int ASCII_COLUMN[BYTES_PER_ROW] =
        {18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};

    static constexpr int COLUMN_COUNT = 34;
    static constexpr int LAST_COLUMN = COLUMN_COUNT - 1;
    static constexpr int ROW_COUNT = 0x10000 / BYTES_PER_ROW;
    static constexpr int LAST_ROW = ROW_COUNT - 1;

    struct DataItem
    {
        char mem_str[6];
        char address_str[5];
        uint8_t data[BYTES_PER_ROW] = {0};
        char hex[BYTES_PER_ROW][3];
        char ascii[BYTES_PER_ROW][2];
    } Q_PACKED;

    bool has_been_initialized = false;

    std::vector<DataItem*> items;
    DebugCore* debugger = nullptr;

    DataModel(DebugCore* debugger, QObject* parent = nullptr);

    virtual Q_ALWAYS_INLINE Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual Q_ALWAYS_INLINE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual Q_ALWAYS_INLINE QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual Q_ALWAYS_INLINE int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual Q_ALWAYS_INLINE int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void update();

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

Q_ALWAYS_INLINE Qt::ItemFlags DataModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index);
}

Q_ALWAYS_INLINE QVariant DataModel::headerData(int section,
                                                   Qt::Orientation orientation,
                                                   int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

Q_ALWAYS_INLINE int DataModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return ROW_COUNT;
}

Q_ALWAYS_INLINE int DataModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

Q_ALWAYS_INLINE QVariant DataModel::data(const QModelIndex& index,
                                             int role) const
{
    auto item(items[index.row()]);
    const auto col = index.column();

    switch (role)
    {
        case Qt::DisplayRole:
            switch (col)
            {
                case MEM_NAME_COLUMN:
                    return item->mem_str;
                case ADDRESS_COLUMN:
                    return item->address_str;
                default:
                    if (col <= HEX_COLUMN[BYTES_PER_ROW - 1])
                        return item->hex[col - HEX_COLUMN[0]];
                    return item->ascii[col - ASCII_COLUMN[0]];
            }

        default:
            break;
    }

    return QVariant();
}
