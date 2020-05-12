#include "instructiondelegate.hh"

#include "instructionmodel.hh"
#include "colors.hh"
#include <QPainter>

void InstructionDelegate::on_hover_index_changed(const QModelIndex& index)
{
    hover_row = index.row();
}

/*

QStyle::State_None	0x00000000	Indicates that the widget does not have a state.
QStyle::State_Active	0x00010000	Indicates that the widget is active.
QStyle::State_AutoRaise	0x00001000	Used to indicate if auto-raise appearance should be used on a tool button.
QStyle::State_Children	0x00080000	Used to indicate if an item view branch has children.
QStyle::State_DownArrow	0x00000040	Used to indicate if a down arrow should be visible on the widget.
QStyle::State_Editing	0x00400000	Used to indicate if an editor is opened on the widget.
QStyle::State_Enabled	0x00000001	Used to indicate if the widget is enabled.
QStyle::State_HasEditFocus	0x01000000	Used to indicate if the widget currently has edit focus.
QStyle::State_HasFocus	0x00000100	Used to indicate if the widget has focus.
QStyle::State_Horizontal	0x00000080	Used to indicate if the widget is laid out horizontally, for example. a tool bar.
QStyle::State_KeyboardFocusChange	0x00800000	Used to indicate if the focus was changed with the keyboard, e.g., tab, backtab or shortcut.
QStyle::State_MouseOver	0x00002000	Used to indicate if the widget is under the mouse.
QStyle::State_NoChange	0x00000010	Used to indicate a tri-state checkbox.
QStyle::State_Off	0x00000008	Used to indicate if the widget is not checked.
QStyle::State_On	0x00000020	Used to indicate if the widget is checked.
QStyle::State_Raised	0x00000002	Used to indicate if a button is raised.
QStyle::State_ReadOnly	0x02000000	Used to indicate if a widget is read-only.
QStyle::State_Selected	0x00008000	Used to indicate if a widget is selected.
QStyle::State_Item	0x00100000	Used by item views to indicate if a horizontal branch should be drawn.
QStyle::State_Open	0x00040000	Used by item views to indicate if the tree branch is open.
QStyle::State_Sibling	0x00200000	Used by item views to indicate if a vertical line needs to be drawn (for siblings).
QStyle::State_Sunken	0x00000004	Used to indicate if the widget is sunken or pressed.
QStyle::State_UpArrow	0x00004000	Used to indicate if an up arrow should be visible on the widget.
QStyle::State_Mini	0x08000000	Used to indicate a mini style Mac widget or button.
QStyle::State_Small	0x04000000	Used to indicate a small style Mac widget or button.

*/

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






    if (index.column() == InstructionModel::BREAKPOINT_COLUMN
        || index.column() == InstructionModel::CURRENT_INSTR_COLUMN)
    {
        auto my_option = option;
        my_option.state &= ~QStyle::State_Selected;
        my_option.state &= ~QStyle::State_MouseOver;
        my_option.state &= ~QStyle::State_HasFocus;
        my_option.state |= QStyle::State_None;
        QStyledItemDelegate::paint(painter, my_option, index);
    }
    else if (index.row() == hover_row)
    {
        auto my_option = option;
        my_option.state &= ~QStyle::State_MouseOver;
        my_option.state &= ~QStyle::State_HasFocus;
        my_option.state |= QStyle::State_None;
        QStyledItemDelegate::paint(painter, my_option, index);
    }
    else if (option.state & QStyle::State_HasFocus)
    {
        auto my_option = option;
        my_option.state &= ~QStyle::State_HasFocus;
        my_option.state |= QStyle::State_None;
        QStyledItemDelegate::paint(painter, my_option, index);
    }
    // Prevents mouseover when scrolling.
    else if (option.state & QStyle::State_MouseOver)
    {
        auto my_option = option;
        my_option.state &= ~QStyle::State_MouseOver;
        my_option.state |= QStyle::State_None;
        QStyledItemDelegate::paint(painter, my_option, index);
    }
    else QStyledItemDelegate::paint(painter, option, index);
}

QSize InstructionDelegate::sizeHint(const QStyleOptionViewItem& option,
                                    const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

