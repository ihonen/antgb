#include "breakpointdelegate.hh"

void BreakpointDelegate::paint(QPainter* painter,
                               const QStyleOptionViewItem& option,
                               const QModelIndex& index) const
{
    auto my_option = option;
    if (my_option.state & QStyle::State_MouseOver)
        my_option.state &= ~QStyle::State_MouseOver;
    if (my_option.state & QStyle::State_HasFocus)
        my_option.state &= ~QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, my_option, index);
    return;
}

QSize BreakpointDelegate::sizeHint(const QStyleOptionViewItem& option,
                                   const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
