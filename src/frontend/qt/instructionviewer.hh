#pragma once

#include "breakpointview.hh"
#include "instructionmodel.hh"
#include "instructionview.hh"
#include "../../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>

class InstructionViewer : public QFrame, public DebugObserver
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

    virtual void on_debugging_resumed() override;
    virtual void on_debugging_paused() override;
    virtual void on_breakpoint_added(uint16_t address) override;
    virtual void on_breakpoint_removed(uint16_t address) override;
    virtual void on_data_breakpoint_added(uint16_t address) override;
    virtual void on_data_breakpoint_removed(uint16_t address) override;
    virtual void on_memory_changed(uint16_t address) override;
    virtual void on_rom_loaded() override;
    virtual void on_special_register_changed() override;
};
