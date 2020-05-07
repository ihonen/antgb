#pragma once

#include "../../debugger/debugcore.hh"
#include "stackmodel.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

class StackViewer : public QTableView
{
    Q_OBJECT
public slots:
    void on_data_changed(const QModelIndex& top_left,
                         const QModelIndex& bottom_right,
                         const QVector<int>& roles = {});
public:
    DebugCore* debugger = nullptr;
    StackModel* model = nullptr;

    StackViewer(DebugCore* debugger, StackModel* model, QWidget* parent = nullptr);
};
