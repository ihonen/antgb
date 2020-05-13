#include "datadelegate.hh"

#include <QPainter>

void DataDelegate::paint(QPainter* painter,
                         const QStyleOptionViewItem& option,
                         const QModelIndex& index) const
{
    auto my_option = option;
    if (my_option.state & QStyle::State_MouseOver)
        my_option.state &= ~QStyle::State_MouseOver;
    if (my_option.state & QStyle::State_HasFocus)
        my_option.state &= ~QStyle::State_HasFocus;

    painter->setRenderHint(QPainter::TextAntialiasing, false);
    painter->setRenderHint(QPainter::Antialiasing, false);

    QStyledItemDelegate::paint(painter, my_option, index);
}

QSize DataDelegate::sizeHint(const QStyleOptionViewItem& option,
                             const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
