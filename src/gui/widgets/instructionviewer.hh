#pragma once

#include "breakpointview.hh"
#include "gui/models/instructionmodel.hh"
#include "instructionview.hh"
#include "debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>

class InstructionViewer : public QFrame, public iDebugObserver
{
    Q_OBJECT
public slots:
    void on_search();
public:
    QLineEdit* search_line = nullptr;
    InstructionView* instruction_view = nullptr;
    BreakpointView* breakpoint_view = nullptr;
    QLineEdit* status_line = nullptr;

    DebugCore* debugger = nullptr;

    InstructionViewer(DebugCore* debugger, InstructionModel* model, QWidget* parent = nullptr);
    void update_status_line();

    virtual void debug_event(const DebugEvent* event) override;
};
