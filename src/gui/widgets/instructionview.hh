#pragma once

#include "gui/models/instructionmodel.hh"
#include "debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QLineEdit>
#include "gui/models/instructiondelegate.hh"

class InstructionView : public QTableView, iDebugObserver
{
    Q_OBJECT
signals:
    void hover_index_changed(const QModelIndex& index);
public slots:
    void on_data_changed(const QModelIndex& top_left,
                         const QModelIndex& bottom_right,
                         const QVector<int>& roles = {});
    void on_clicked(const QModelIndex& index);
    void on_double_clicked(const QModelIndex& index);
    void scroll_to_current();
    void search_text(QString& text);
public:
    DebugCore* debugger = nullptr;
    InstructionModel* model = nullptr;
    InstructionDelegate* delegate = nullptr;

    InstructionView(DebugCore* debugger, InstructionModel* model, QWidget* parent = nullptr);
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void debug_event(const DebugEvent* event) override;
};
