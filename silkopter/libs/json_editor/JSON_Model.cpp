#include "JSON_Model.h"

#include "QBase.h"

JSON_Model::Tree_Item::Tree_Item(JSON_Model* model)
    : m_model(model)
{
}
JSON_Model::Tree_Item::~Tree_Item()
{
}
void JSON_Model::Tree_Item::build(Tree_Item* parent, size_t row, std::string const& name, rapidjson::Value* json)
{
    m_model_index = m_model->createIndex(row, 0, this);
    m_name = name;
    m_parent = parent;
    m_json = json;
    m_children.clear();

    {
        if (json->GetType() == rapidjson::kObjectType)
        {
            m_children.reserve(json->MemberCount());
            for (auto it = json->MemberBegin(); it != json->MemberEnd(); ++it)
            {
                auto* c = new Tree_Item(m_model);
                m_children.push_back(std::unique_ptr<Tree_Item>(c));
                c->build(this, m_children.size() - 1, it->name.GetString(), &it->value);
            }
        }
        else if (json->GetType() == rapidjson::kArrayType)
        {
            m_children.reserve(json->Size());
            for (size_t i = 0; i < json->Size(); i++)
            {
                auto* c = new Tree_Item(m_model);
                m_children.push_back(std::unique_ptr<Tree_Item>(c));
                c->build(this, m_children.size() - 1, q::util::format2<std::string>("[{}]", i), &(*json)[i]);
            }
        }
    }
}
size_t JSON_Model::Tree_Item::get_row() const
{
    if (!m_parent)
    {
        return 0;
    }
    auto it = std::find_if(m_parent->m_children.begin(), m_parent->m_children.end(), [this](std::unique_ptr<Tree_Item> const& c)
    {
            return c.get() == this;
    });
    if (it != m_parent->m_children.end())
    {
        return std::distance(m_parent->m_children.begin(), it);
    }
    QASSERT(0);
    return 0;
}
void JSON_Model::Tree_Item::reset()
{
    //m_children.resize(0);
    m_children.clear();
    m_model_index = QModelIndex();
}

JSON_Model::Tree_Item* JSON_Model::Tree_Item::find(rapidjson::Value* json)
{
    std::vector<Tree_Item*> items;
    items.reserve(32);
    items.push_back((Tree_Item*)this);
    for (size_t i = 0; i < items.size(); i++)
    {
        Tree_Item* ti = items[i];
        if (ti->m_json == json)
        {
            return ti;
        }
        for (auto const& ch: ti->m_children)
        {
            items.push_back(ch.get());
        }
    }
    return nullptr;
}


JSON_Model::JSON_Model(QWidget* parent)
    : m_root(this)
{
}
JSON_Model::~JSON_Model()
{

}

QStringList JSON_Model::mimeTypes() const
{
    QStringList types;
    return types;
}

bool JSON_Model::dropMimeData(QMimeData const* data, Qt::DropAction action, int row, int column, QModelIndex const& parent)
{
    return true;
}

QModelIndex JSON_Model::index(int row, int column, QModelIndex const& parent) const
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);

    if (row < 0 || column < 0)
    {
        return QModelIndex();
    }
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    if (!parent.isValid())
    {
        return row == 0 ? createIndex(row, column, &m_root) : QModelIndex();
    }

    Tree_Item* ti = (Tree_Item*)parent.internalPointer();
    if (ti && row < (int)ti->m_children.size())
    {
        //QModelIndex const& idx = ti->m_children[row]->m_model_index;
        return createIndex(row, column, ti->m_children[row].get());
    }

    return QModelIndex();
}
QModelIndex JSON_Model::parent(QModelIndex const& index) const
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);

    if (!index.isValid())
    {
        return QModelIndex();
    }
    Tree_Item* ti = (Tree_Item*)index.internalPointer();
    if (!ti)
    {
        return QModelIndex();
    }

    Tree_Item* parent = ti->m_parent;
    if (!parent)
    {
        return QModelIndex();
    }

    return parent->m_model_index;
}

int JSON_Model::rowCount(QModelIndex const& index) const
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);

    if (!index.isValid())
    {
        return 1;
    }
    if (index.column() != 0)
    {
        return 0;
    }
    Tree_Item const* ti = (Tree_Item*)index.internalPointer();
    return ti ? ti->m_children.size() : 0;
}
int JSON_Model::columnCount(QModelIndex const& index) const
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);
    return 2;
}

QVariant JSON_Model::data(QModelIndex const& index, int role) const
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);

    if (!index.isValid())
    {
        return QVariant();
    }

    auto* ti = (Tree_Item*)index.internalPointer();
    if (!ti || !ti->m_json)
    {
        return QVariant();
    }

    auto* json = ti->m_json;

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == 0)
        {
            return QVariant(ti->m_name.c_str());
        }
        else
        {
            if (json->IsNull())
            {
                return QVariant("<null>");
            }
            if (json->IsBool())
            {
                return QVariant(json->GetBool());
            }
            if (json->IsInt())
            {
                return QVariant(json->GetInt());
            }
            if (json->IsUint())
            {
                return QVariant(json->GetUint());
            }
            if (json->IsInt64())
            {
                return QVariant((qlonglong)json->GetInt64());
            }
            if (json->IsUint64())
            {
                return QVariant((qulonglong)json->GetUint64());
            }
            if (json->IsDouble() || json->IsNumber())
            {
                return QVariant(json->GetDouble());
            }
            if (json->IsString())
            {
                return QVariant(json->GetString());
            }
        }
    }

    return QVariant();
}

Qt::ItemFlags JSON_Model::flags(QModelIndex const& index) const
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);


    if (!index.isValid())
    {
        return 0;
    }

    uint32_t default_flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    auto* ti = (Tree_Item*)index.internalPointer();
    if (!ti || !ti->m_json)
    {
        return Qt::ItemFlags(default_flags);
    }

    auto json = ti->m_json;
    if (json->IsObject() || json->IsArray())
    {
        return Qt::ItemFlags(default_flags);
    }

    return Qt::ItemFlags((index.column() == 0) ? default_flags : default_flags | Qt::ItemIsEditable);
}

void JSON_Model::set_document(std::string const& name, rapidjson::Document* document)
{
//	std::lock_guard<std::recursive_mutex> sm(mTreeMutex);

    beginResetModel();
    m_root.reset();
    endResetModel();

    m_document = document;
    if (!document)
    {
        return;
    }

    m_root.build(nullptr, 0, name, m_document);
}


//////////////////////////////////////////////////////////////////////////

bool JSON_Model::setData(QModelIndex const& index, QVariant const& value, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    auto* ti = (Tree_Item*)index.internalPointer();
    if (!ti || !ti->m_json)
    {
        return false;
    }

    bool modified = false;
    auto* json = ti->m_json;
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == 1)
        {
            if (json->IsBool())
            {
                json->SetBool(value.toBool());
                modified = true;
            }
            else if (json->IsInt())
            {
                json->SetInt(value.toInt());
                modified = true;
            }
            else if (json->IsUint())
            {
                json->SetUint(value.toUInt());
                modified = true;
            }
            else if (json->IsInt64())
            {
                json->SetInt64(value.toLongLong());
                modified = true;
            }
            else if (json->IsUint64())
            {
                json->SetUint64(value.toULongLong());
                modified = true;
            }
            else if (json->IsDouble() || json->IsNumber())
            {
                json->SetDouble(value.toDouble());
                modified = true;
            }
            else if (json->IsString())
            {
                json->SetString(value.toString().toLatin1().data(), m_document->GetAllocator());
                modified = true;
            }
        }
    }

    if (modified)
    {
        emit dataChanged(createIndex(index.row(), 0, index.internalPointer()), createIndex(index.row(), columnCount(), index.internalPointer()));
    }

    return modified;
}
bool JSON_Model::setHeaderData(int section, Qt::Orientation orientation, QVariant const& value, int role)
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);


    return false;
}
bool JSON_Model::insertColumns(int position, int columns, QModelIndex const& parent)
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);

    return false;
}
bool JSON_Model::removeColumns(int position, int columns, QModelIndex const& parent)
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);

    return false;
}
bool JSON_Model::insertRows(int position, int rows, QModelIndex const& parent)
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);

    return false;
}
bool JSON_Model::removeRows(int position, int rows, QModelIndex const& parent)
{
//    std::lock_guard<std::recursive_mutex> sm(mTreeMutex);
    return false;
}

