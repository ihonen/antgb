#pragma once

#include "instructionmodel.hh"
#include "../../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QLineEdit>
#include "instructiondelegate.hh"

class InstructionView : public QTableView, DebugObserver
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
