#include "memorymodel.hh"

#include "../util/helper.hh"
#include "../util/colors.hh"
#include "../util/pixmaps.hh"

MemoryModel::MemoryModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    debugger(debugger)
{
    debugger->add_observer(this);
}

Qt::ItemFlags MemoryModel::flags(const QModelIndex& index) const
{
    if (index.column() == 0 || index.column() == 1) return QAbstractTableModel::flags(index) & ~(Qt::ItemIsSelectable) & ~(Qt::ItemIsDragEnabled);
    else return QAbstractTableModel::flags(index);
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

QVariant MemoryModel::data(const QModelIndex& index, int role) const
{
    regid_t address = index.row();
    auto col = index.column();

    switch (role)
    {
        case Qt::DisplayRole:
            switch (col)
            {
                case ADDRESS_COLUMN:
                    return memory_abbreviation(address)
                           + ": "
                           + hexstr16(address).toUpper() + ":";
                case HEX_DATA_COLUMN:
                    return hexstr8(debugger->read(address));
                case NAME_COLUMN:
                    return register_name(address);
                default:
                    break;
            }
            break;
        case Qt::DecorationRole:
            switch (col)
            {
                case BREAKPOINT_COLUMN:
                    if (auto bp = debugger->breakpoint_at(address))
                    {
                        if (bp->has_mode(BpOnRead))
                            return QIcon(Pixmaps::DATA_BREAKPOINT->scaled(9, 9));
                        else if (bp->has_mode(BpOnWrite))
                            return QIcon(Pixmaps::DATA_BREAKPOINT2->scaled(9, 9));
                    }
                    return QVariant();
                case CURRENT_ITEM_COLUMN:
                    return QVariant();
                default:
                    break;
            }
            break;
        case Qt::ForegroundRole:
            if (debugger->is_running()) return *Colors::FG_LIGHT_GRAY;

            auto bp = debugger->breakpoint_at(address);
            if (bp && bp->has_mode(BpOnRead))
                return *Colors::FG_YELLOW;
            else if (bp && bp->has_mode(BpOnWrite))
                return *Colors::FG_ORANGE;
            else return *Colors::FG_WHITE;
    }

    return QVariant();
}

int MemoryModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return ROW_COUNT;
}

int MemoryModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

bool MemoryModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

void MemoryModel::debug_event(const DebugEvent* event)
{
    auto address = event->register_id();

    switch (event->type())
    {
        case DbContinued:
        case DbPaused:
            emit(dataChanged(index(0, BREAKPOINT_COLUMN),
                             index(ROW_COUNT - 1, HEX_DATA_COLUMN),
                             {Qt::DecorationRole, Qt::ForegroundRole}));
            break;
        case DbBreakpointAdded:
        case DbBreakpointModified:
        case DbBreakpointRemoved:
            emit(dataChanged(index(address, BREAKPOINT_COLUMN),
                             index(address, HEX_DATA_COLUMN),
                             {Qt::DecorationRole, Qt::ForegroundRole}));
            break;
        default:
            break;
    }
}
