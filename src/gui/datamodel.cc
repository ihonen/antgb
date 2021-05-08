#include "datamodel.hh"

#include "helper.hh"

DataModel::DataModel(DebugCore* debugger, QObject* parent) :
    QAbstractTableModel(parent),
    debugger(debugger)
{
    debugger->add_observer(this);
    items.reserve(ROW_COUNT);

    for (size_t row = 0; row < ROW_COUNT; ++row)
    {
        uint16_t address = row * BYTES_PER_ROW;
        auto item = new DataItem;
        std::string memory_str = memory_abbreviation(address).toStdString() + ":";
        std::string address_str = hexstr16(address).toUpper().toStdString();
        assert(strlen(memory_str.c_str()) < 7);
        assert(strlen(address_str.c_str()) < 5);
        strcpy(item->mem_str, memory_str.c_str());
        strcpy(item->address_str, address_str.c_str());
        items.push_back(item);
    }

    update();
}

bool DataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

void DataModel::update()
{
    for (size_t row = 0; row < ROW_COUNT; ++row)
    {
        for (size_t offset = 0; offset < BYTES_PER_ROW; ++offset)
        {
            auto item = items[row];
            auto new_data = debugger->read(row * BYTES_PER_ROW + offset);
            if (new_data != item->data[offset] || !has_been_initialized)
            {
                item->data[offset] = new_data;
                std::string hex_str = hexstr8(item->data[offset]).toStdString();
                strcpy(item->hex[offset], hex_str.c_str());
                if (isprint((char)item->data[offset]) && !isspace((char)item->data[offset]))
                    item->ascii[offset][0] = (char)item->data[offset];
                else item->ascii[offset][0] = '.';
                item->ascii[offset][1] = '\0';
            }
        }
    }

    has_been_initialized = true;

    emit(dataChanged(index(0, 0),
                     index(LAST_ROW, LAST_COLUMN),
    {Qt::DisplayRole}));
}

QModelIndex DataModel::search_text(const QString& anycase_text, int starting_from)
{
    auto lowercase_text = anycase_text.toLower();

    bool is_hex = false;
    auto value = lowercase_text.toUInt(&is_hex, 16);
    if (is_hex && value <= 0xFFFF)
        for (size_t i = 0; i < items.size(); ++i)
            if (i / BYTES_PER_ROW >= value)
                return index(i, 0);

    return QModelIndex();
}

uint16_t DataModel::index_to_address(const QModelIndex& index)
{
    if (index.column() < HEX_COLUMN[0]) return 0xFFFF;
    else if (index.column() <= HEX_COLUMN[BYTES_PER_ROW - 1])
        return index.row() * BYTES_PER_ROW + index.column() - HEX_COLUMN[0];
    else return index.row() * BYTES_PER_ROW + index.column() - ASCII_COLUMN[0];
}

void DataModel::debug_event(const DebugEvent* event)
{
    switch (event->type())
    {
        case DbPaused:
        case DbRomLoaded:
            update();
            break;
        default:
            break;
    }
}

bool DataModel::index_has_address(const QModelIndex& index)
{
    return index.isValid() && index.column() >= HEX_COLUMN[0];
}
