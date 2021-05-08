#pragma once

#include "../../debugger/debugcore.hh"
#include <QAbstractItemModel>

class RegisterModel : public QAbstractItemModel, iDebugObserver
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
        regid_t register_id = RegNone;
/*        void* register_location = nullptr; */

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

    virtual void debug_event(const DebugEvent* event) override;
};
