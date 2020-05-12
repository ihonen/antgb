#include "instructionviewer.hh"

#include <QHeaderView>

#include "helper.hh"
#include "../../debugger/disassembler.hh"

void InstructionViewer::search()
{
    auto text = search_line->text();
    search_line->setPlaceholderText("Jump to");
    if (text != "") instruction_view->search_text(text);
    else instruction_view->scroll_to_current();
}

InstructionViewer::InstructionViewer(DebugCore* debugger, InstructionModel* model, QWidget* parent) :
    QFrame(parent)
{
    instruction_view = new InstructionView(debugger, model, this);

    search_line = new QLineEdit(this);
    search_line->setPlaceholderText("Search");

    breakpoint_view = new BreakpointView(debugger, this);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(search_line, 0, 0);
    layout->addWidget(instruction_view, 1, 0, 1, 1);
    layout->addWidget(breakpoint_view, 0, 2, 2, 2);
    layout->setColumnMinimumWidth(0, 300);

    connect(search_line,
            &QLineEdit::textChanged,
            this,
            &InstructionViewer::search);
    connect(breakpoint_view,
            &BreakpointView::clicked,
            instruction_view,
            [&](const QModelIndex& index)
            {
                QString str = hexstr16(breakpoint_view->model->get_row(index.row())->address);
                instruction_view->search_text(str);
            });

    setFrameStyle(QFrame::StyledPanel);
}
