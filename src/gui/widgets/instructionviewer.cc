#include "instructionviewer.hh"

#include <QHeaderView>

#include "../util/fonts.hh"
#include "../util/helper.hh"
#include "../../debugger/disassembler.hh"

void InstructionViewer::on_search()
{
    auto text = search_line->text();
    search_line->setPlaceholderText("Jump to");
    if (text != "") instruction_view->search_text(text);
    else instruction_view->scroll_to_current();
}

InstructionViewer::InstructionViewer(DebugCore* debugger, InstructionModel* model, QWidget* parent) :
    QFrame(parent),
    debugger(debugger)
{
    debugger->add_observer(this);

    instruction_view = new InstructionView(debugger, model, this);

    search_line = new QLineEdit(this);
    search_line->setFont(*Fonts::COURIER);
    search_line->setPlaceholderText("Search");

    breakpoint_view = new BreakpointView(debugger, this);

    status_line = new QLineEdit(this);
    status_line->setFont(*Fonts::COURIER);
    status_line->setReadOnly(true);
    update_status_line();

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(search_line, 0, 0);
    layout->addWidget(instruction_view, 1, 0, 1, 1);
    layout->addWidget(breakpoint_view, 0, 2, 3, 1);
    layout->addWidget(status_line, 2, 0, 1, 1);
    layout->setColumnMinimumWidth(0, 300);

    connect(search_line,
            &QLineEdit::textChanged,
            this,
            &InstructionViewer::on_search);
    connect(breakpoint_view,
            &BreakpointView::clicked,
            instruction_view,
            [&](const QModelIndex& index)
            {
                QString str = hexstr16(breakpoint_view->model->get_row(index.row()));
                instruction_view->search_text(str);
            });

    setFrameStyle(QFrame::StyledPanel);
}

void InstructionViewer::update_status_line()
{
    uint16_t PC = debugger->read(RegPC);
    uint8_t current[3];
    for (size_t i = 0; i < 3; ++i)
        current[i] = debugger->read(debugger->read(RegPC) + i);
    QString cpu_status = hexstr16(PC).toUpper() + ": "
                         + debugger->disasm()->disassemble(current).c_str();
    status_line->setText(cpu_status);
}

void InstructionViewer::debug_event(const DebugEvent* event)
{
    switch (event->type())
    {
        case DbContinued:
            status_line->setStyleSheet("color: #afafaf");
            break;
        case DbPaused:
            status_line->setStyleSheet("color: #ffffff");
            update_status_line();
            break;
        case DbRomLoaded:
            update_status_line();
            break;
        default:
            break;
    }
}
