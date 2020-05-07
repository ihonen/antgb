#include "stackviewer.hh"

#include <QHeaderView>

void StackViewer::on_data_changed(const QModelIndex& top_left,
                                  const QModelIndex& bottom_right,
                                  const QVector<int>& roles)
{
    Q_UNUSED(roles)
    Q_UNUSED(bottom_right)
    scrollTo(model->index(model->SP->row, 0));
    QTableView::update(top_left);
}

StackViewer::StackViewer(DebugCore* debugger, StackModel* model, QWidget* parent) :
    QTableView(parent),
    debugger(debugger),
    model(model)
{
    setModel(model);

    horizontalHeader()->setVisible(false);
    horizontalHeader()->setMinimumSectionSize(1);
    horizontalHeader()->setDefaultSectionSize(18);
    horizontalHeader()->setMaximumSectionSize(100);
    horizontalHeader()->resizeSection(StackModel::ADDRESS_COLUMN, 100);
    horizontalHeader()->resizeSection(StackModel::HEX_DATA_COLUMN, 55);

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
            &StackModel::dataChanged,
            this,
            &StackViewer::on_data_changed);

    scrollTo(model->index(model->SP->row, 0));
}
