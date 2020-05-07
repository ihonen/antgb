#include "instructionview.hh"

#include <QHeaderView>

void InstructionView::on_data_changed(const QModelIndex& top_left, const QModelIndex& bottom_right, const QVector<int>& roles)
{
    scrollTo(model->index(model->PC->row, 0), ScrollHint::PositionAtCenter);
    QTableView::update(top_left);
}

void InstructionView::on_clicked(const QModelIndex& index)
{
    debugger->toggle_breakpoint(model->get_row(index.row())->address);
}

InstructionView::InstructionView(DebugCore* debugger, InstructionModel* model, QWidget* parent) :
    QTableView(parent),
    debugger(debugger),
    model(model)
{
    setModel(model);

    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);
    setShowGrid(false);

    verticalHeader()->setMinimumSectionSize(1);
    verticalHeader()->setDefaultSectionSize(15);
    verticalHeader()->setMaximumSectionSize(1000);

    horizontalHeader()->setMinimumSectionSize(1);
    horizontalHeader()->setDefaultSectionSize(1);
    horizontalHeader()->setMaximumSectionSize(1000);

    horizontalHeader()->resizeSection(InstructionModel::BREAKPOINT_COLUMN, 18);
    horizontalHeader()->resizeSection(InstructionModel::CURRENT_INSTR_COLUMN, 18);
    horizontalHeader()->resizeSection(InstructionModel::MEM_REGION_COLUMN, 50);
    horizontalHeader()->resizeSection(InstructionModel::ADDRESS_COLUMN, 50);
    horizontalHeader()->resizeSection(InstructionModel::HEX_DATA_COLUMN, 80);
    horizontalHeader()->resizeSection(InstructionModel::DISASSEMBLY_COLUMN, 140);
    horizontalHeader()->resizeSection(InstructionModel::LENGTH_COLUMN, 15);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::NoSelection);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto font = QFont();
    font.setFamily("Courier");
    setFont(font);

    connect(model,
            &InstructionModel::dataChanged,
            this,
            &InstructionView::on_data_changed);

    connect(this,
            &QAbstractItemView::clicked,
            this,
            &InstructionView::on_clicked);

    scrollTo(model->index(model->PC->row, 0), ScrollHint::PositionAtCenter);
}

void InstructionView::search(QString& text)
{
    auto index = model->search(text);
    if (index.isValid()) scrollTo(index, ScrollHint::PositionAtCenter);
}
