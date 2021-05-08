#pragma once

#include "../debugger/debugcore.hh"
#include "memorydelegate.hh"
#include "memorymodel.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

class MemoryViewer : public QTableView
{
    Q_OBJECT
public slots:
    void on_data_changed(const QModelIndex& top_left,
                         const QModelIndex& bottom_right,
                         const QVector<int>& roles = {});
    void on_clicked(const QModelIndex& index);
    void on_double_clicked(const QModelIndex& index);
public:
    DebugCore* debugger = nullptr;
    MemoryModel* model = nullptr;
    MemoryDelegate* delegate = nullptr;

    MemoryViewer(DebugCore* debugger, QWidget* parent = nullptr);
};
