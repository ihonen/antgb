#include "registerviewer.hh"

#include <QScrollBar>

void RegisterViewer::on_data_changed(const QModelIndex& index)
{
    QAbstractItemView::update(index);
}

RegisterViewer::RegisterViewer(DebugCore* debugger, RegisterModel* model, QWidget* parent) :
    QTreeView(parent),
    debugger(debugger),
    model(model)
{
    setModel(model);

    setSortingEnabled(false);
    setDragEnabled(false);

    auto font = QFont();
    font.setFamily("Courier");
    setFont(font);

    setColumnWidth(0, 106);
    setColumnWidth(1, 44);
    setFixedWidth(170);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    for (size_t i = 0; i < model->rowCount(); ++i)
    {
        auto reg_set_index = model->index(i, 0, QModelIndex());
        for (size_t j = 0; j < model->rowCount(reg_set_index); ++j)
        {
            auto reg_value_index = model->index(j, 1, reg_set_index);
            model->setData(reg_value_index, Qt::AlignRight, Qt::TextAlignmentRole);
        }

        if (i != 0) setExpanded(reg_set_index, true);
    }

    connect(model,
            &RegisterModel::data_changed,
            this,
            &RegisterViewer::on_data_changed);
}
