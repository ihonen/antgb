#include "instructionviewer.hh"

#include <QHeaderView>

#include "helper.hh"
#include "../../debugger/disassembler.hh"

void InstructionViewer::search()
{
    auto text = search_line->text();
    search_line->setPlaceholderText("Jump to");
    if (text != "") view->search(text);
    else view->scroll_to_current();
}

InstructionViewer::InstructionViewer(DebugCore* debugger, InstructionModel* model, QWidget* parent) :
    QFrame(parent)
{
    view = new InstructionView(debugger, model, this);
    search_line = new QLineEdit(this);
    settings_button = new QPushButton(this);
    settings_button->setIcon(QIcon(QPixmap::fromImage(QImage(":/img/gear.png"))));
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(search_line, 0, 0);
    layout->addWidget(settings_button, 0, 1);
    layout->addWidget(view, 1, 0, 1, 2);

    connect(search_line,
            &QLineEdit::textChanged,
            this,
            &InstructionViewer::search);

    setFrameStyle(QFrame::StyledPanel);

    search_line->setPlaceholderText("Jump to");
}
