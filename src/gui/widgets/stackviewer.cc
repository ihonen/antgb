#include "stackviewer.hh"

#include <QHeaderView>

void StackViewer::on_data_changed(const QModelIndex& top_left,
                                  const QModelIndex& bottom_right,
                                  const QVector<int>& roles)
{
    Q_UNUSED(roles)
    Q_UNUSED(bottom_right)
    scrollTo(model->index(model->address_to_row(debugger->read(RegSP)), 0),
             ScrollHint::PositionAtTop);
    QTableView::update(top_left);
}

StackViewer::StackViewer(DebugCore* debugger, QWidget* parent) :
    QTableView(parent),
    debugger(debugger)
{
    model = new StackModel(debugger, this);
    setModel(model);

    delegate = new StackDelegate(this);
    setItemDelegate(delegate);

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
    setSelectionBehavior(QAbstractItemView::SelectRows);

    auto font = QFont();
    font.setFamily("Courier");
    setFont(font);

    connect(model,
            &StackModel::dataChanged,
            this,
            &StackViewer::on_data_changed);

    scrollTo(model->index(model->address_to_row(debugger->read(RegSP)), 0),
             ScrollHint::PositionAtTop);
}
