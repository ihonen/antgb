#include "dataviewer.hh"

#include <QHeaderView>

DataViewer::DataViewer(DebugCore* debugger, DataModel* model, QWidget* parent) :
    QTableView(parent),
    debugger(debugger),
    model(model)
{
    setItemDelegate(new DataDelegate);

    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);
    setShowGrid(false);

    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setMinimumSectionSize(15);
    verticalHeader()->setDefaultSectionSize(15);
    verticalHeader()->setMaximumSectionSize(15);

    horizontalHeader()->setMinimumSectionSize(10);
    horizontalHeader()->setDefaultSectionSize(22);
    horizontalHeader()->setMaximumSectionSize(100);

    setModel(model);

    horizontalHeader()->resizeSection(DataModel::MEM_NAME_COLUMN, 55);
    horizontalHeader()->resizeSection(DataModel::ADDRESS_COLUMN, 45);
    for (size_t i = 2; i < 18; ++i)
        horizontalHeader()->resizeSection(i, 24);
    for (size_t i = 18; i < model->columnCount(); ++i)
        horizontalHeader()->resizeSection(i, 11);

    setSelectionMode(QAbstractItemView::SingleSelection);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(model,
            &DataModel::dataChanged,
            this,
            &DataViewer::on_data_changed);

    auto font = QFont();
    font.setFamily("Courier");
    setFont(font);
}
