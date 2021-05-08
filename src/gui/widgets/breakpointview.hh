#pragma once

#include "../models/breakpointmodel.hh"
#include "../models/breakpointdelegate.hh"
#include "../../debugger/debugcore.hh"
#include <QTableView>

class BreakpointView : public QTableView
{
public slots:
    void on_data_changed(const QModelIndex& top_left,
                         const QModelIndex& bottom_right,
                         QVector<int> roles);
public:
    DebugCore* debugger = nullptr;
    BreakpointModel* model = nullptr;
    BreakpointDelegate* delegate = nullptr;

    BreakpointView(DebugCore* debugger, QWidget* parent = nullptr);
};
