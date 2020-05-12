#include "instructionview.hh"

#include <QHeaderView>
#include <QMouseEvent>

void InstructionView::on_data_changed(const QModelIndex& top_left,
                                      const QModelIndex& bottom_right,
                                      const QVector<int>& roles)
{
    Q_UNUSED(bottom_right)
    Q_UNUSED(roles)
    QTableView::update(top_left);
}

void InstructionView::on_clicked(const QModelIndex& index)
{
    if (index.column() == InstructionModel::BREAKPOINT_COLUMN
        || index.column() == InstructionModel::CURRENT_INSTR_COLUMN)
    {
        debugger->toggle_breakpoint(model->get_row(index.row())->address);
    }
}

void InstructionView::on_double_clicked(const QModelIndex& index)
{
    if (index.column() != InstructionModel::BREAKPOINT_COLUMN
        && index.column() != InstructionModel::CURRENT_INSTR_COLUMN)
    {
        debugger->toggle_breakpoint(model->get_row(index.row())->address);
    }
}

InstructionView::InstructionView(DebugCore* debugger, InstructionModel* model, QWidget* parent) :
    QTableView(parent),
    debugger(debugger),
    model(model)
{
    debugger->add_observer(this);

    setModel(model);
    delegate = new InstructionDelegate;
    setItemDelegate(delegate);

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
    horizontalHeader()->resizeSection(InstructionModel::ADDRESS_COLUMN, 40);
    horizontalHeader()->resizeSection(InstructionModel::HEX_DATA_COLUMN, 80);
    horizontalHeader()->resizeSection(InstructionModel::DISASSEMBLY_COLUMN, 140);
    horizontalHeader()->resizeSection(InstructionModel::LENGTH_COLUMN, 15);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setTabKeyNavigation(false);

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

    connect(this,
            &QAbstractItemView::doubleClicked,
            this,
            &InstructionView::on_double_clicked);

    connect(this,
            &InstructionView::hover_index_changed,
            delegate,
            &InstructionDelegate::on_hover_index_changed);

    setMouseTracking(true);
    scroll_to_current();
}

void InstructionView::search(QString& text)
{
    auto index = model->search(text);
    if (index.isValid()) scrollTo(index, ScrollHint::PositionAtTop);
}

void InstructionView::scroll_to_current()
{
    // NOTE: Make sure that model->PC has been updated by this point!
    scrollTo(model->index(model->PC->row, 0), ScrollHint::PositionAtTop);
}

void InstructionView::mouseMoveEvent(QMouseEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    emit(hover_index_changed(index));
}

void InstructionView::on_debugging_resumed()
{

}

void InstructionView::on_debugging_paused()
{
    scroll_to_current();
}

void InstructionView::on_breakpoint_added(uint16_t address)
{
    Q_UNUSED(address)
}

void InstructionView::on_breakpoint_removed(uint16_t address)
{
    Q_UNUSED(address)
}

void InstructionView::on_data_breakpoint_added(uint16_t address)
{
    Q_UNUSED(address)
}

void InstructionView::on_data_breakpoint_removed(uint16_t address)
{
    Q_UNUSED(address)
}

void InstructionView::on_memory_changed(uint16_t address)
{
    Q_UNUSED(address)
}

void InstructionView::on_rom_loaded()
{
    scroll_to_current();
}

void InstructionView::on_special_register_changed()
{

}
