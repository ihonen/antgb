#pragma once

#include <QStyledItemDelegate>

class InstructionDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public slots:
    void on_hover_index_changed(const QModelIndex& index);
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
private:
    int hover_row = -1;
};
