#pragma once

#include "instructionmodel.hh"
#include "../../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QLineEdit>

class InstructionView : public QTableView
{
public slots:
    void on_data_changed(const QModelIndex& top_left,
                         const QModelIndex& bottom_right,
                         const QVector<int>& roles = {});
    void on_clicked(const QModelIndex& index);
public:
    DebugCore* debugger = nullptr;
    InstructionModel* model = nullptr;

    InstructionView(DebugCore* debugger, InstructionModel* model, QWidget* parent = nullptr);
    void update();
    void search(QString& text);
};
