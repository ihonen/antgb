#pragma once

#include "../../debugger/debugcore.hh"
#include <QAbstractItemModel>

class RegisterModel : public QAbstractItemModel, DebugObserver
{
    Q_OBJECT
signals:
    void data_changed(const QModelIndex& index);
public:
    struct RegisterItem
    {
        RegisterItem* parent = nullptr;
        QVector<RegisterItem*> children = {};
        QString name = "";
        size_t size = 0;
        void* register_location = nullptr;

        QMap<int, QMap<Qt::ItemDataRole, QVariant>> data_by_column;

        int row_in_parent()
        {
            if (!parent) return 0;
            return parent->children.indexOf(this);
        }
    };

    RegisterItem* root = nullptr;
    DebugCore* debugger;

    bool is_debugger_running = false;

    RegisterModel(DebugCore* debugger, QObject* parent = nullptr);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    virtual QModelIndex parent(const QModelIndex& index) const override;
    virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    void update_all();

    virtual void on_debugging_resumed() override;
    virtual void on_debugging_paused() override;
    virtual void on_breakpoint_added(uint16_t address) override;
    virtual void on_breakpoint_removed(uint16_t address) override;
    virtual void on_data_breakpoint_added(uint16_t address) override;
    virtual void on_data_breakpoint_removed(uint16_t address) override;
    virtual void on_memory_changed(uint16_t address) override;
    virtual void on_whole_memory_changed() override;
    virtual void on_special_register_changed() override;
};
