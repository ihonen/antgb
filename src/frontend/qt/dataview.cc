#include "dataview.hh"

#include <QDebug>
#include <QHeaderView>
#include <QMouseEvent>

void DataView::on_selection_changed(const QItemSelection& selected,
                                    const QItemSelection& deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)
}

void DataView::on_current_changed(const QModelIndex& current,
                                  const QModelIndex& previous)
{
    update(current);
    update(previous);
    if (previous.column() < DataModel::ASCII_COLUMN[0])
    {
        QModelIndex prev_parallel = model->index(previous.row(),
                                                 previous.column() + DataModel::BYTES_PER_ROW);
        update(prev_parallel);
    }
    else
    {
        QModelIndex prev_parallel = model->index(previous.row(),
                                                 previous.column() - DataModel::BYTES_PER_ROW);
        update(prev_parallel);
    }

    if (current.column() < DataModel::ASCII_COLUMN[0])
    {
        QModelIndex parallel = model->index(current.row(),
                                            current.column() + DataModel::BYTES_PER_ROW);
        update(parallel);
    }
    else
    {
        QModelIndex parallel = model->index(current.row(),
                                            current.column() - DataModel::BYTES_PER_ROW);
        update(parallel);
    }
}

DataView::DataView(DebugCore* debugger, QWidget* parent) :
    QTableView(parent),
    debugger(debugger)
{
    model = new DataModel(debugger, this);
    setModel(model);

    delegate = new DataDelegate;
    setItemDelegate(delegate);

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

    horizontalHeader()->resizeSection(DataModel::MEM_NAME_COLUMN, 55);
    horizontalHeader()->resizeSection(DataModel::ADDRESS_COLUMN, 45);
    for (size_t i = 2; i < 18; ++i)
        horizontalHeader()->resizeSection(i, 24);
    for (size_t i = 18; i < model->columnCount(); ++i)
        horizontalHeader()->resizeSection(i, 12);

    setSelectionMode(QAbstractItemView::SingleSelection);

    setEditTriggers(QAbstractItemView::DoubleClicked);

    connect(model,
            &DataModel::dataChanged,
            this,
            &DataView::on_data_changed);

    connect(selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &DataView::on_selection_changed);

    connect(selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &DataView::on_current_changed);

    connect(selectionModel(),
            &QItemSelectionModel::currentChanged,
            delegate,
            &DataDelegate::on_current_changed);

    auto font = QFont();
    font.setFamily("Courier");
    setFont(font);

    setMouseTracking(true);
}

DataView::~DataView()
{
    delete delegate;
}

void DataView::keyReleaseEvent(QKeyEvent* event)
{
    QTableView::keyReleaseEvent(event);
}

void DataView::mouseReleaseEvent(QMouseEvent* event)
{
    QTableView::mouseReleaseEvent(event);
}

void DataView::mouseMoveEvent(QMouseEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    emit(hover_index_changed(index));
}

void DataView::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    emit(hover_stopped());
}

void DataView::search_text(const QString& text)
{
    scrollTo(model->search_text(text), ScrollHint::PositionAtTop);
}
