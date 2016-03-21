#include "NodeDefModel.h"


NodeDefModel::TreeItem::TreeItem(const Node_Def& def, bool isCategory, TreeItem* parent)
    : m_def(def)
    , m_isCategory(isCategory)
    , m_parent(parent)
{
    if (m_isCategory)
    {
        m_text = silk::node::get_as_string(m_def.type).c_str();
        m_iconSource = QString("qrc:/icons/nodes/") + m_text + ".png";
    }
    else
    {
        m_text = def.name.c_str();
    }
}

void NodeDefModel::TreeItem::appendChild(std::unique_ptr<TreeItem>&& child)
{
    m_children.push_back(std::move(child));
}

NodeDefModel::TreeItem& NodeDefModel::TreeItem::getChild(size_t row)
{
    return *m_children[row];
}

size_t NodeDefModel::TreeItem::getChildCount() const
{
    return m_children.size();
}
size_t NodeDefModel::TreeItem::getColumnCount() const
{
    return 1;
}
QVariant NodeDefModel::TreeItem::getText(size_t column) const
{
    return m_text;
}
QVariant NodeDefModel::TreeItem::getIconSource() const
{
    return m_iconSource;
}
size_t NodeDefModel::TreeItem::getRow() const
{
    if (m_parent)
    {
        return std::distance(m_parent->m_children.begin(),
                             std::find_if(m_parent->m_children.begin(), m_parent->m_children.end(), [this](const std::unique_ptr<TreeItem>& c) { return c.get() == this; }));
    }
    return 0;
}
NodeDefModel::TreeItem* NodeDefModel::TreeItem::getParent()
{
    return m_parent;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

NodeDefModel::NodeDefModel(QObject* parent)
    : QAbstractItemModel(parent)
    , m_root(Node_Def(), true, nullptr)
{

}

void NodeDefModel::init(const std::vector<Node_Def>& defs)
{
    std::map<silk::node::Type, TreeItem*> typeItems;

    for (const Node_Def& def: defs)
    {
        silk::node::Type type = def.type;

        TreeItem* parent = nullptr;

        auto it = typeItems.find(type);
        if (it != typeItems.end())
        {
            parent = it->second;
        }
        else
        {
            parent = new TreeItem(def, true, &m_root);
            typeItems[type] = parent;
            m_root.appendChild(std::unique_ptr<TreeItem>(parent));
        }
        QASSERT(parent);

        TreeItem* item = new TreeItem(def, false, parent);
        parent->appendChild(std::unique_ptr<TreeItem>(item));
    }
}

QModelIndex NodeDefModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    TreeItem* parentItem = nullptr;
    if (!parent.isValid())
    {
        parentItem = &m_root;
    }
    else
    {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem* childItem = &parentItem->getChild(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex NodeDefModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->getParent();
    if (parentItem == &m_root)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->getRow(), 0, parentItem);
}

int NodeDefModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = &m_root;
    }
    else
    {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->getChildCount();
}

int NodeDefModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<TreeItem*>(parent.internalPointer())->getColumnCount();
    }
    else
    {
        return m_root.getColumnCount();
    }
}

QVariant NodeDefModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (role == NameRole)
    {
        return item->getText(index.column());
    }
    else if (role == IconRole)
    {
        return item->getIconSource();
    }

    return QVariant();
}

QVariant NodeDefModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return m_root.getText(section);
    }

    return QVariant();
}

Qt::ItemFlags NodeDefModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    return QAbstractItemModel::flags(index);
}

QHash<int, QByteArray> NodeDefModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IconRole] = "Icon";
    roles[NameRole] = "Name";
    return roles;
}
