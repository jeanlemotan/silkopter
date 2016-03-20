#pragma once

#include <QAbstractItemModel>
#include "Comms.h"

class NodeDefModel : public QAbstractItemModel
{
public:
    typedef silk::Comms::Node_Def Node_Def;

    NodeDefModel(QObject* parent = 0);

    void init(std::vector<Node_Def> const& defs);

private:
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    enum Roles
    {
        NameRole = Qt::UserRole + 10,
        DescriptionRole = Qt::UserRole + 11
    };

    class TreeItem
    {
    public:
        explicit TreeItem(const Node_Def& def, bool isCategory, TreeItem* parent = 0);
        void appendChild(std::unique_ptr<TreeItem>&& child);

        TreeItem& getChild(size_t row);
        size_t getChildCount() const;
        size_t getColumnCount() const;
        QVariant data(size_t column) const;
        size_t getRow() const;
        TreeItem* getParent();

    private:
        std::vector<std::unique_ptr<TreeItem>> m_children;
        Node_Def m_def;
        bool m_isCategory = false;
        TreeItem* m_parent;
    };

    mutable TreeItem m_root;
};
