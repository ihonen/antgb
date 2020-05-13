#pragma once

#include "datadelegate.hh"
#include "datamodel.hh"
#include "../../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

class DataViewer : public QTableView
{
    Q_OBJECT
public slots:
    Q_ALWAYS_INLINE void on_data_changed(const QModelIndex& top_left,
                                         const QModelIndex& bottom_right,
                                         const QVector<int>& roles);
public:

    DebugCore* debugger = nullptr;
    DataModel* model = nullptr;

    DataViewer(DebugCore* debugger, DataModel* model, QWidget* parent = nullptr);
};

Q_ALWAYS_INLINE void DataViewer::on_data_changed(const QModelIndex& top_left,
                                                 const QModelIndex& bottom_right,
                                                 const QVector<int>& roles)
{
    Q_UNUSED(bottom_right)
    Q_UNUSED(roles)
    QTableView::update(top_left);
}
