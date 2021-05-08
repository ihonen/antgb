#include "dataviewer.hh"

#include "fonts.hh"
#include "helper.hh"
#include <QVBoxLayout>
#include <QLineEdit>


void DataViewer::on_search_text()
{
    QString text = search_line->text();
    data_view->search_text(text);
}

void DataViewer::on_hover(const QModelIndex& index)
{
    if (!data_view->hasFocus())
    {
        if (data_view->model->index_has_address(index))
        {
            auto address = data_view->model->index_to_address(index);
            status_line->setText(hexstr16(address).toUpper() + " " + register_name(address));
        }
        else status_line->setText("");
    }
}

void DataViewer::on_hover_stopped()
{
    if (!data_view->hasFocus())
        status_line->setText("");
}

void DataViewer::on_current_changed(const QModelIndex& current,
                                    const QModelIndex& previous)
{
    Q_UNUSED(previous)
    auto address = data_view->model->index_to_address(current);
    status_line->setText(hexstr16(address).toUpper() + " " + register_name(address));
}

DataViewer::DataViewer(DebugCore* debugger, QWidget* parent) :
    QFrame(parent),
    debugger(debugger)
{
    data_view = new DataView(debugger, this);

    search_line = new QLineEdit(this);
    search_line->setFont(*Fonts::COURIER);
    search_line->setPlaceholderText("Search");

    status_line = new QLineEdit(this);
    status_line->setFont(*Fonts::COURIER);
    status_line->setReadOnly(true);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(search_line, 0, 0);
    layout->addWidget(data_view, 1, 0);
    layout->addWidget(status_line, 2, 0);

    setFrameStyle(QFrame::StyledPanel);

    connect(search_line,
            &QLineEdit::textChanged,
            this,
            &DataViewer::on_search_text);

    connect(data_view,
            &DataView::hover_index_changed,
            this,
            &DataViewer::on_hover);

    connect(data_view,
            &DataView::hover_stopped,
            this,
            &DataViewer::on_hover_stopped);

    connect(data_view->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &DataViewer::on_current_changed);
}
