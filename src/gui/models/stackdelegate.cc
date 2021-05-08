#include "stackdelegate.hh"

#include "stackmodel.hh"

void StackDelegate::paint(QPainter* painter,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const
{
    auto my_option = option;
    if (my_option.state & QStyle::State_MouseOver)
        my_option.state &= ~QStyle::State_MouseOver;
    if (my_option.state & QStyle::State_HasFocus)
        my_option.state &= ~QStyle::State_HasFocus;
    if (index.column() == StackModel::BREAKPOINT_COLUMN
        || index.column() == StackModel::CURRENT_TOP_COLUMN)
        my_option.state &= ~QStyle::State_Selected;

    QStyledItemDelegate::paint(painter, my_option, index);
    return;
}
