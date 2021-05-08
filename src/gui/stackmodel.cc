#include "stackmodel.hh"

#include "helper.hh"
#include "colors.hh"
#include "pixmaps.hh"

StackModel::StackModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    debugger(debugger)
{
    debugger->add_observer(this);
}

Qt::ItemFlags StackModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index);
}

QVariant StackModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

QVariant StackModel::data(const QModelIndex& index, int role) const
{
    int col = index.column();
    regid_t lower_address = row_to_address(index.row());

    switch (role)
    {
        case Qt::DisplayRole:
            switch (col)
            {
                case ADDRESS_COLUMN:
                    return memory_abbreviation(lower_address)
                           + ": "
                           + hexstr16(lower_address).toUpper()
                           + ":";
                case HEX_DATA_COLUMN:
                    return hexstr8(debugger->read(lower_address))
                           + " "
                           + hexstr8(debugger->read(lower_address + 1));
                default:
                    break;
            }
            break;
        case Qt::DecorationRole:
            switch (col)
            {
                case BREAKPOINT_COLUMN:
                    if (debugger->breakpoint_at(lower_address)
                        || debugger->breakpoint_at(lower_address + 1))
                        return QIcon(Pixmaps::DATA_BREAKPOINT->scaled(9, 9));
                    else return QVariant();
                case CURRENT_TOP_COLUMN:
                    if (debugger->read(RegSP) == lower_address
                        || debugger->read(RegSP) == lower_address + 1)
                        return QIcon(Pixmaps::CURRENT_ARROW->scaled(11, 11));
                    else return QVariant();
                default:
                    break;
            }
            break;
        case Qt::ForegroundRole:
            if (debugger->is_running()) return *Colors::FG_LIGHT_GRAY;
            else if (debugger->read(RegSP) == lower_address
                     || debugger->read(RegSP) == lower_address + 1)
                return *Colors::FG_BLUE;
            else return *Colors::FG_WHITE;
    }

    return QVariant();
}

int StackModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return ROW_COUNT;
}

int StackModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

bool StackModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

void StackModel::debug_event(const DebugEvent* event)
{

    switch (event->type())
    {
        case DbContinued:
        case DbPaused:
            emit(dataChanged(index(0, BREAKPOINT_COLUMN),
                             index(ROW_COUNT - 1, HEX_DATA_COLUMN),
                             {Qt::DecorationRole, Qt::ForegroundRole}));
            break;
        case DbDataChanged:
            emit(dataChanged(index(address_to_row(event->register_id()), BREAKPOINT_COLUMN),
                             index(address_to_row(event->register_id()), HEX_DATA_COLUMN),
                             {Qt::DecorationRole, Qt::DisplayRole, Qt::ForegroundRole}));
            break;
        default:
            break;
    }
}
