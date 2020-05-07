#include "dataviewer.hh"

#include <QHeaderView>

void DataViewer::data_changed(const QModelIndex& index)
{
    QAbstractItemView::update(index);
}

DataViewer::DataViewer(DebugCore* debugger, DataModel* model, QWidget* parent) :
    QTableView(parent),
    debugger(debugger),
    model(model)
{
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

    update();

    horizontalHeader()->resizeSection(0, 100);
    for (size_t i = 1; i < 17; ++i)
        horizontalHeader()->resizeSection(i, 24);
    for (size_t i = 17; i < model->columnCount(); ++i)
        horizontalHeader()->resizeSection(i, 11);

    setSelectionMode(QAbstractItemView::SingleSelection);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto font = QFont();
    font.setFamily("Courier");
    setFont(font);
}

void DataViewer::update()
{
    model->update();
}
