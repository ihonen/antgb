#include "memorydelegate.hh"

#include "memorymodel.hh"
#include <QPainter>

void MemoryDelegate::paint(QPainter* painter,
                           const QStyleOptionViewItem& option,
                           const QModelIndex& index) const
{
    auto my_option = option;
    if (my_option.state & QStyle::State_MouseOver)
        my_option.state &= ~QStyle::State_MouseOver;
    if (my_option.state & QStyle::State_HasFocus)
        my_option.state &= ~QStyle::State_HasFocus;
    if (index.column() == MemoryModel::BREAKPOINT_COLUMN
        || index.column() == MemoryModel::CURRENT_ITEM_COLUMN)
        my_option.state &= ~QStyle::State_Selected;

    QStyledItemDelegate::paint(painter, my_option, index);
    return;
}
