#include "datadelegate.hh"

#include "../util/colors.hh"
#include "datamodel.hh"
#include <QPainter>

void DataDelegate::on_current_changed(const QModelIndex& curr,
                                      const QModelIndex& prev)
{
    Q_UNUSED(prev)
    current = curr;
    if (curr.column() < DataModel::HEX_COLUMN[0])
    {
            current = QModelIndex();
            parallel = QModelIndex();
    }
    else if (curr.column() < DataModel::ASCII_COLUMN[0])
        parallel = current.model()->index(current.row(),
                                          current.column() + DataModel::BYTES_PER_ROW);
    else
        parallel = current.model()->index(current.row(),
                                          current.column() - DataModel::BYTES_PER_ROW);
}

void DataDelegate::paint(QPainter* painter,
                         const QStyleOptionViewItem& option,
                         const QModelIndex& index) const
{
    painter->save();

    auto my_option = option;
    if (my_option.state & QStyle::State_MouseOver)
        my_option.state &= ~QStyle::State_MouseOver;
    if (my_option.state & QStyle::State_HasFocus)
        my_option.state &= ~QStyle::State_HasFocus;
    if (index.column() < DataModel::HEX_COLUMN[0])
        my_option.state |= QStyle::State_Enabled;
    if (index == parallel)
        my_option.state |= QStyle::State_Selected;

    // This seems to help with performance somewhat.
    painter->setRenderHint(QPainter::TextAntialiasing, false);
    painter->setRenderHint(QPainter::Antialiasing, false);

    QStyledItemDelegate::paint(painter, my_option, index);

    painter->restore();
}

QSize DataDelegate::sizeHint(const QStyleOptionViewItem& option,
                             const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
