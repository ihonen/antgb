#include "memoryviewer.hh"

#include "stackviewer.hh"

#include <QHeaderView>

void MemoryViewer::on_data_changed(const QModelIndex& top_left,
                                  const QModelIndex& bottom_right,
                                  const QVector<int>& roles)
{
    Q_UNUSED(bottom_right)
    Q_UNUSED(roles)
    QTableView::update(top_left);
}

void MemoryViewer::on_clicked(const QModelIndex& index)
{
    if (index.column() == MemoryModel::BREAKPOINT_COLUMN
        || index.column() == MemoryModel::CURRENT_ITEM_COLUMN)
    {
        // The row is equal to the address in this view.
        auto address = index.row();

        auto bp = debugger->breakpoint_at(address);
        if (!bp)
        {
            debugger->toggle_breakpoint(address, BpOnRead);
        }
        else if (bp->has_mode(BpOnRead))
        {
            debugger->toggle_breakpoint(address, BpOnRead);
            debugger->toggle_breakpoint(address, BpOnWrite);
        }
        else if (bp->has_mode(BpOnWrite))
        {
            debugger->toggle_breakpoint(address, BpOnWrite);
        }
        else
        {
            debugger->toggle_breakpoint(address, BpOnRead);
        }
    }
}

void MemoryViewer::on_double_clicked(const QModelIndex& index)
{
    if (index.column() != MemoryModel::BREAKPOINT_COLUMN
        && index.column() != MemoryModel::CURRENT_ITEM_COLUMN)
    {
        // The row is equal to the address in this view.
        auto address = index.row();

        auto bp = debugger->breakpoint_at(address);
        if (!bp)
        {
            debugger->toggle_breakpoint(address, BpOnRead);
        }
        else if (bp->has_mode(BpOnRead))
        {
            debugger->toggle_breakpoint(address, BpOnRead);
            debugger->toggle_breakpoint(address, BpOnWrite);
        }
        else if (bp->has_mode(BpOnWrite))
        {
            debugger->toggle_breakpoint(address, BpOnWrite);
        }
        else
        {
            debugger->toggle_breakpoint(address, BpOnRead);
        }
    }
}

MemoryViewer::MemoryViewer(DebugCore* debugger, QWidget* parent) :
    QTableView(parent),
    debugger(debugger)
{
    model = new MemoryModel(debugger, this);
    setModel(model);

    delegate = new MemoryDelegate(this);
    setItemDelegate(delegate);

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
    setSelectionBehavior(QAbstractItemView::SelectRows);

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

    connect(this,
            &QAbstractItemView::doubleClicked,
            this,
            &MemoryViewer::on_double_clicked);

}
