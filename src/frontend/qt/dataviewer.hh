#pragma once

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
    void data_changed(const QModelIndex& index);
public:

    DebugCore* debugger = nullptr;
    DataModel* model = nullptr;

    DataViewer(DebugCore* debugger, DataModel* model, QWidget* parent = nullptr);
    void update();
};
