#include "datamodel.hh"

#include "helper.hh"

DataModel::DataModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    debugger(debugger)
{
    row_count = 0x10000 / 16;
    column_count = 33;

    for (size_t row = 0; row < row_count; ++row)
    {
        items[row].address = row * 16;
    }

    update();
}

Qt::ItemFlags DataModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index);
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QVariant DataModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto item = items[index.row()];
        if (index.column() == 0)
        {
            return memory_abbreviation(item.address) + ": " + hexstr16(item.address).toUpper();
        }
        else if (index.column() < 17)
        {
            return hexstr8(item.data[index.column() - 1]);
        }
        else
        {
            char ret = (char)item.data[index.column() - 17];
            if (isprint(ret) && !isspace(ret))
                return QString(ret);
            else
                return QString(".");
        }
    }

    return QVariant();
}

int DataModel::rowCount(const QModelIndex& parent) const
{
    return row_count;
}

int DataModel::columnCount(const QModelIndex& parent) const
{
    return column_count;
}

bool DataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    return false;
}

void DataModel::update()
{
    for (size_t row = 0; row < row_count; ++row)
    {
        for (size_t offset = 0; offset < 16; ++offset)
        {
            items[row].data[offset] = debugger->emu->mem->read(row * 16 + offset);
        }
    }
}
