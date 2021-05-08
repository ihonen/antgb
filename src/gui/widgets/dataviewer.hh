#pragma once

#include "dataview.hh"
#include "debugger/debugcore.hh"
#include <QPushButton>
#include <QFrame>

class DataViewer : public QFrame
{
    Q_OBJECT
public slots:
    void on_search_text();
    void on_hover(const QModelIndex& index);
    void on_hover_stopped();
    void on_current_changed(const QModelIndex& current, const QModelIndex& previous);
public:
    DataViewer(DebugCore* debugger, QWidget* parent = nullptr);

    DataView* data_view = nullptr;
    QLineEdit* search_line = nullptr;
    QLineEdit* status_line = nullptr;
    DebugCore* debugger = nullptr;
};
