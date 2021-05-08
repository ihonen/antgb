#pragma once

#include <QStyledItemDelegate>

class DataDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public slots:
    void on_current_changed(const QModelIndex& current, const QModelIndex& previous);
public:
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    QModelIndex current;
    QModelIndex parallel;
};
