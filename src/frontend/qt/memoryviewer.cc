#include "memoryviewer.hh"

#include "stackviewer.hh"

#include <QHeaderView>

void MemoryViewer::on_data_changed(const QModelIndex& top_left,
                                  const QModelIndex& bottom_right,
                                  const QVector<int>& roles)
{
    QTableView::update(top_left);
}

void MemoryViewer::on_clicked(const QModelIndex& index)
{
    debugger->toggle_data_breakpoint(model->get_row(index.row())->address);
}

MemoryViewer::MemoryViewer(DebugCore* debugger, MemoryModel* model, QWidget* parent) :
    QTableView(parent),
    debugger(debugger),
    model(model)
{
    setModel(model);

    horizontalHeader()->setVisible(false);
    horizontalHeader()->setMinimumSectionSize(1);
    horizontalHeader()->setDefaultSectionSize(18);
    horizontalHeader()->setMaximumSectionSize(100);
    horizontalHeader()->resizeSection(MemoryModel::ADDRESS_COLUMN, 100);
    horizontalHeader()->resizeSection(MemoryModel::HEX_DATA_COLUMN, 35);
    horizontalHeader()->resizeSection(MemoryModel::NAME_COLUMN, 80);

    verticalHeader()->setVisible(false);
    verticalHeader()->setMinimumSectionSize(1);
    verticalHeader()->setDefaultSectionSize(15);
    verticalHeader()->setMaximumSectionSize(100);

    setSortingEnabled(false);
    setDragEnabled(false);
    setCornerButtonEnabled(false);
    setShowGrid(false);

    auto font = QFont();
    font.setFamily("Courier");
    setFont(font);

    connect(model,
            &MemoryModel::dataChanged,
            this,
            &MemoryViewer::on_data_changed);

    connect(this,
            &QAbstractItemView::clicked,
            this,
            &MemoryViewer::on_clicked);
}
