#pragma once

#include "registermodel.hh"
#include "../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTreeView>

class RegisterViewer : public QTreeView
{
    Q_OBJECT
public slots:
    void on_data_changed(const QModelIndex& index);
public:
    DebugCore* debugger = nullptr;
    RegisterModel* model = nullptr;

    RegisterViewer(DebugCore* debugger, RegisterModel* model, QWidget* parent = nullptr);
};
