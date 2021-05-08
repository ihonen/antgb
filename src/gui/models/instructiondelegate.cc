#include "instructiondelegate.hh"

#include "instructionmodel.hh"
#include "../util/colors.hh"
#include <QPainter>

void InstructionDelegate::on_hover_index_changed(const QModelIndex& index)
{
    hover_row = index.row();
}

void InstructionDelegate::paint(QPainter* painter,
                                const QStyleOptionViewItem& option,
                                const QModelIndex& index) const
{
    auto my_option = option;
    if (my_option.state & QStyle::State_MouseOver)
        my_option.state &= ~QStyle::State_MouseOver;
    if (my_option.state & QStyle::State_HasFocus)
        my_option.state &= ~QStyle::State_HasFocus;
    if (index.column() == InstructionModel::BREAKPOINT_COLUMN
        || index.column() == InstructionModel::CURRENT_INSTR_COLUMN)
        my_option.state &= ~QStyle::State_Selected;

    QStyledItemDelegate::paint(painter, my_option, index);
    return;
}

QSize InstructionDelegate::sizeHint(const QStyleOptionViewItem& option,
                                    const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
