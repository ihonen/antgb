#include "breakpointview.hh"

#include <QHeaderView>
#include "fonts.hh"

void BreakpointView::on_data_changed(const QModelIndex& top_left, const QModelIndex& bottom_right, QVector<int> roles)
{
    QTableView::update(top_left);
}

BreakpointView::BreakpointView(DebugCore* debugger, QWidget* parent) :
    QTableView(parent),
    debugger(debugger)
{
    model = new BreakpointModel(debugger, this);
    setModel(model);

    delegate = new BreakpointDelegate(this);
    setItemDelegate(delegate);


    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);
    setShowGrid(false);

    verticalHeader()->setMinimumSectionSize(1);
    verticalHeader()->setDefaultSectionSize(15);
    verticalHeader()->setMaximumSectionSize(1000);

    horizontalHeader()->setMinimumSectionSize(1);
    horizontalHeader()->setDefaultSectionSize(30);
    horizontalHeader()->setMaximumSectionSize(1000);

    horizontalHeader()->resizeSection(BreakpointModel::BREAKPOINT_COLUMN, 15);
    horizontalHeader()->resizeSection(BreakpointModel::CURRENT_INSTR_COLUMN, 15);
    horizontalHeader()->resizeSection(BreakpointModel::ADDRESS_COLUMN, 50);
    horizontalHeader()->resizeSection(BreakpointModel::DISASSEMBLY_COLUMN, 140);

    setFont(*Fonts::COURIER);

    connect(model,
            &BreakpointModel::dataChanged,
            this,
            &BreakpointView::on_data_changed);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setFixedWidth(225);
}
