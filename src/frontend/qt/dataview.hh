#pragma once

#include "datadelegate.hh"
#include "datamodel.hh"
#include "../../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

class DataView : public QTableView
{
    Q_OBJECT
signals:
    void hover_stopped();
    void hover_index_changed(const QModelIndex& index);
public slots:
    Q_ALWAYS_INLINE void on_data_changed(const QModelIndex& top_left,
                                         const QModelIndex& bottom_right,
                                         const QVector<int>& roles);
    void on_selection_changed(const QItemSelection& selected,
                              const QItemSelection& deselected);
    void on_current_changed(const QModelIndex& current, const QModelIndex& previous);
public:
    DebugCore* debugger = nullptr;
    DataModel* model = nullptr;
    DataDelegate* delegate = nullptr;

    DataView(DebugCore* debugger, QWidget* parent = nullptr);
    virtual ~DataView() override;

    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

    void search_text(const QString& text);
};

Q_ALWAYS_INLINE void DataView::on_data_changed(const QModelIndex& top_left,
                                                 const QModelIndex& bottom_right,
                                                 const QVector<int>& roles)
{
    Q_UNUSED(bottom_right)
    Q_UNUSED(roles)
    QTableView::update(top_left);
}
