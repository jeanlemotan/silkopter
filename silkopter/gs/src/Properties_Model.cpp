#include "Properties_Model.h"

#include <QWidget>
#include <QIcon>

#include <unordered_map>

#include "def_lang/IVector_Value.h"
#include "def_lang/IStruct_Type.h"
#include "def_lang/IMember_Def.h"
#include "def_lang/IMember.h"
#include "def_lang/IMember_Container.h"

//////////////////////////////////////////////////////////////////////////

Properties_Model::Tree_Item::Tree_Item(Properties_Model* model, const std::string& name, const std::string& suffix)
	: m_model(model)
	, m_name(name)
    , m_suffix(suffix)
{
}

//////////////////////////////////////////////////////////////////////////

Properties_Model::Tree_Item::~Tree_Item()
{
	m_connections.clear();
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::Tree_Item::build_root(ts::IStruct_Value& root)
{
	m_children.clear();
	m_connections.clear();

    std::unordered_map<std::string, std::shared_ptr<Tree_Item>> sections;
    m_children.reserve(root.get_member_count());

    for (size_t i = 0; i < root.get_member_count(); i++)
	{
        std::shared_ptr<ts::IMember> member = root.get_member(i);
        std::shared_ptr<const ts::IMember_Def> member_def = root.get_specialized_type()->get_member_def(i);

        const std::string& ui_name = member_def->get_ui_name();
        const std::string& ui_suffix = member_def->get_ui_suffix();

        std::shared_ptr<Tree_Item> item = std::make_shared<Tree_Item>(m_model, ui_name, ui_suffix);

        item->build(member->get_value());
        add_child(std::move(item));
	}
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::Tree_Item::build(std::shared_ptr<ts::IValue> value)
{
	m_value = value;
	m_children.clear();
	m_connections.clear();

    m_connections.push_back(value->sig_value_changed.connect(std::bind(&Properties_Model::on_value_changed, m_model, value)));

    if (std::shared_ptr<ts::IOptional_Value> optional_value = std::dynamic_pointer_cast<ts::IOptional_Value>(value))
	{
        m_connections.push_back(optional_value->sig_will_be_unset.connect(std::bind(&Properties_Model::on_optional_will_be_unset, m_model, optional_value)));
        m_connections.push_back(optional_value->sig_was_set.connect(std::bind(&Properties_Model::on_optional_was_set, m_model, optional_value)));

        if (optional_value->is_set())
		{
            std::shared_ptr<ts::IValue> inner_value = optional_value->get_value();
            m_secondary_value = inner_value; //as it is wrapped
            build_children(inner_value);
		}
	}
    else if (std::shared_ptr<ts::IVariant_Value> variant_value = std::dynamic_pointer_cast<ts::IVariant_Value>(value))
	{
        m_connections.push_back(variant_value->sig_type_index_will_change.connect(std::bind(&Properties_Model::on_variant_type_index_will_change, m_model, variant_value)));
        m_connections.push_back(variant_value->sig_type_index_has_changed.connect(std::bind(&Properties_Model::on_variant_type_index_has_changed, m_model, variant_value)));

        if (variant_value->is_set())
		{
            std::shared_ptr<ts::IValue> inner_value = variant_value->get_value();
            m_secondary_value = inner_value; //as it is wrapped
            build_children(inner_value);
		}
	}
    else if (std::shared_ptr<ts::IPoly_Value> poly_value = std::dynamic_pointer_cast<ts::IPoly_Value>(value))
    {
        m_connections.push_back(poly_value->sig_type_will_change.connect(std::bind(&Properties_Model::on_poly_type_will_change, m_model, poly_value)));
        m_connections.push_back(poly_value->sig_type_has_changed.connect(std::bind(&Properties_Model::on_poly_type_has_changed, m_model, poly_value)));

        std::shared_ptr<ts::IValue> inner_value = poly_value->get_value();
        if (inner_value)
        {
            m_secondary_value = inner_value; //as it is wrapped
            build_children(inner_value);
        }
    }
    else
	{
        build_children(value);
	}
}

void Properties_Model::Tree_Item::build_children(std::shared_ptr<ts::IValue> value)
{
    if (std::shared_ptr<ts::IMember_Container> member_container = std::dynamic_pointer_cast<ts::IMember_Container>(value))
	{
        const size_t member_count = member_container->get_member_count();
        m_children.reserve(member_count);

        for (size_t i = 0; i < member_count; ++i)
		{
            const std::shared_ptr<ts::IMember> member = member_container->get_member(i);

            std::shared_ptr<Tree_Item> item = std::make_shared<Tree_Item>(m_model, member->get_member_def()->get_ui_name(), member->get_member_def()->get_ui_suffix());
            item->build(member->get_value());
            add_child(std::move(item));
		}
	}
    else if (std::shared_ptr<ts::IVector_Value> vector_value = std::dynamic_pointer_cast<ts::IVector_Value>(value))
	{
        //m_connections.push_back(vectorValue->sig_elementsAdded.connect(&Properties_Model::on_elements_added, m_model, vectorValue, std::placeholders::_1, std::placeholders::_2));
        //m_connections.push_back(vectorValue->sig_elementsWillBeRemoved.connect(&Properties_Model::on_elements_will_be_removed, m_model, vectorValue, std::placeholders::_1, std::placeholders::_2));

        m_children.reserve(vector_value->get_value_count());
        for (size_t i = 0; i < vector_value->get_value_count(); i++)
		{
            std::shared_ptr<Tree_Item> item = std::make_shared<Tree_Item>(m_model, std::to_string(i), std::string());
            item->build(vector_value->get_value(i));
            add_child(std::move(item));
		}
	}
	else
	{
		//QFAIL();
	}
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::Tree_Item::refresh_model_index(size_t row)
{
    if (m_parent.lock())
	{
        m_model_index = m_model->createIndex(static_cast<int>(row), 0, this);
	}
	else
	{
        m_model_index = QModelIndex();
	}
}

//////////////////////////////////////////////////////////////////////////

size_t Properties_Model::Tree_Item::get_row() const
{
    std::shared_ptr<Tree_Item> parent = m_parent.lock();
    if (!parent)
	{
		return 0;
	}
    auto it = std::find_if(parent->m_children.begin(), parent->m_children.end(), [this](std::shared_ptr<Tree_Item> const& c) { return c.get() == this; });
    if (it != parent->m_children.end())
	{
        return std::distance(parent->m_children.begin(), it);
	}
    QASSERT(false);
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::Tree_Item::reset()
{
	m_children.clear();
    m_model_index = QModelIndex();
}

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<Properties_Model::Tree_Item> Properties_Model::Tree_Item::find_by_primary(ts::IValue const& value) const
{
    std::vector<std::shared_ptr<Tree_Item>> items;
	items.reserve(32);
    items.push_back((const_cast<Tree_Item&>(*this)).shared_from_this());
	for (size_t i = 0; i < items.size(); i++)
	{
        std::shared_ptr<Tree_Item> ti = items[i];
        if (ti->m_value.get() == &value)
		{
			return ti;
		}
		for (auto const& ch : ti->m_children)
		{
			items.push_back(ch);
		}
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<Properties_Model::Tree_Item> Properties_Model::Tree_Item::find_by_primary_or_secondary(ts::IValue const& value) const
{
    std::vector<std::shared_ptr<Tree_Item>> items;
	items.reserve(32);
    items.push_back((const_cast<Tree_Item&>(*this)).shared_from_this());
    for (size_t i = 0; i < items.size(); i++)
	{
        std::shared_ptr<Tree_Item> ti = items[i];
        if (ti->m_value.get() == &value || ti->m_secondary_value.get() == &value)
		{
			return ti;
		}
		for (auto const& ch : ti->m_children)
		{
			items.push_back(ch);
		}
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////

size_t Properties_Model::Tree_Item::deep_get_child_count() const
{
	size_t count = m_children.size();
	for (auto const& c : m_children)
	{
        count += c->deep_get_child_count();
	}
	return count;
}

//////////////////////////////////////////////////////////////////////////

size_t Properties_Model::Tree_Item::get_child_count() const
{
	return m_children.size();
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::Tree_Item::add_child(std::shared_ptr<Properties_Model::Tree_Item> child)
{
    insert_child(m_children.size(), std::move(child));
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::Tree_Item::insert_child(size_t off, std::shared_ptr<Tree_Item> child)
{
    child->m_parent = this->shared_from_this();
    child->refresh_model_index(m_children.size());

	m_children.insert(m_children.begin() + off, std::move(child));
}

//////////////////////////////////////////////////////////////////////////

Properties_Model::Properties_Model(QWidget* parent)
	: QAbstractItemModel(parent)
    , m_root(std::make_shared<Tree_Item>(this, "root", std::string()))
{
}

//////////////////////////////////////////////////////////////////////////

Properties_Model::~Properties_Model()
{

}

//////////////////////////////////////////////////////////////////////////

QModelIndex Properties_Model::index(int row, int column, QModelIndex const& parent) const
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

	if (row < 0 || column < 0)
	{
		return QModelIndex();
	}
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}
	// 	if (parent.isValid() && parent.column() != 0)
	// 	{
	// 		return QModelIndex();
	// 	}

    Tree_Item const* ti = nullptr;
	if (!parent.isValid())
	{
        ti = m_root.get();
	}
	else
	{
        ti = (Tree_Item*)parent.internalPointer();
	}

	if (ti && row < (int)ti->m_children.size())
	{
        QModelIndex const& idx = ti->m_children[row]->m_model_index;
		return createIndex(row, column, idx.internalPointer());
	}

	return QModelIndex();
}

//////////////////////////////////////////////////////////////////////////

QModelIndex Properties_Model::parent(QModelIndex const& index) const
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

	if (!index.isValid())
	{
		return QModelIndex();
	}
    Tree_Item* ti = (Tree_Item*)index.internalPointer();
	if (!ti)
	{
		return QModelIndex();
	}

    std::shared_ptr<Tree_Item> parent = ti->m_parent.lock();
    if (!parent || parent == m_root)
	{
		return QModelIndex();
	}

    return parent->m_model_index;
}

//////////////////////////////////////////////////////////////////////////

int Properties_Model::rowCount(QModelIndex const& index) const
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

    Tree_Item const* ti = nullptr;
	if (!index.isValid())
	{
        ti = m_root.get();
	}
	else
	{
        ti = (Tree_Item*)index.internalPointer();
	}

	return static_cast<int>(ti->m_children.size());
}

//////////////////////////////////////////////////////////////////////////

int Properties_Model::columnCount(QModelIndex const& index) const
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
	return 2;
}

//////////////////////////////////////////////////////////////////////////

QVariant Properties_Model::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
			return "Property";
		case 1:
			return "Value";
		}
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

//////////////////////////////////////////////////////////////////////////

QVariant Properties_Model::data(QModelIndex const& index, int role) const
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

    Tree_Item* ti = nullptr;
	if (!index.isValid())
	{
		return QVariant();
	}

    ti = (Tree_Item*)index.internalPointer();
	if (!ti)
	{
		if (index.parent().isValid())
		{
            Tree_Item* parent = (Tree_Item*)index.parent().internalPointer();
		}
		if (role == Qt::DisplayRole && (index.column() == 0) && ti)
		{
			return QVariant(ti->m_name.c_str());//sections
		}
		return QVariant();
	}

    const std::shared_ptr<ts::IValue> value = ti->m_value;

	if (role == Qt::SizeHintRole)
	{
		QFont font = data(index, Qt::FontRole).value<QFont>();
		QFontMetrics fm(font);
		QSize size(fm.width(data(index, Qt::DisplayRole).toString()),
			fm.height());
		QVariant retval;
		retval.setValue(size);
		return retval;
	}
	else if (role == EditorNameRole)
	{
		return QVariant();
	}
	else if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		if (index.column() == 0)
		{
			return QVariant(ti->m_name.c_str());
		}
		else
		{
			if (!value)
			{
				return QVariant();
			}

            ts::Result<std::string> result = value->get_ui_string();
            if (result == ts::success)
			{
                return QVariant((result.payload() + " " + ti->m_suffix).c_str());
			}

			size_t count = ti->m_children.size();
			if (count == 0)
			{
				return QVariant();
			}

			//don't use value->GetAsString() for structs because it returns the real member count, but the tree item might be hiding some members
            return QVariant();
		}
	}
	else if (role == Qt::DecorationRole && index.column() == 0)
	{
        return QIcon();//tui::icons::LoadIcon("param");
	}
	else if (role == TypeRole && index.column() != 0)
	{
        QASSERT(ti);
		if (value)
		{
            return value->get_type()->get_ui_name().c_str();
		}

		return QVariant();
	}

	return QVariant();
}

//////////////////////////////////////////////////////////////////////////

Qt::ItemFlags Properties_Model::flags(QModelIndex const& index) const
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

	auto defaultFlags = Qt::ItemIsEnabled;

	if (!index.isValid())
	{
		return defaultFlags;
	}

    Tree_Item* ti = (Tree_Item*)index.internalPointer();

	if (ti && ti->m_value)
	{
//        const std::shared_ptr<const ts::IVector_Type> vector_type = std::dynamic_pointer_cast<const ts::IVector_Type>(ti->m_value->get_type());
//        if((vector_type && vector_type->get_min_size() == vector_type->get_max_size())
//            || std::dynamic_pointer_cast<ts::IArrayValue>(ti->m_value)
//            || std::dynamic_pointer_cast<ts::IStructValue>(ti->m_value))
//		{
//			return defaultFlags;
//		}
	}

	auto value = ti->m_value;
	return (index.column() == 0 || !value) ? defaultFlags : (defaultFlags | Qt::ItemIsEditable);
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::set_value(std::shared_ptr<ts::IStruct_Value> value)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

    if (m_rootValue == value)
	{
		return;
	}

	beginResetModel();
    m_rootValue.reset();
    m_root = std::make_shared<Tree_Item>(this, "root", std::string());
	endResetModel();

    m_rootValue = value;
    if (m_rootValue)
	{
        m_root->build_root(*m_rootValue);
    }
    Q_EMIT layoutChanged();

    //sig_handleChanged.emit();
}

//////////////////////////////////////////////////////////////////////////

bool Properties_Model::setData(QModelIndex const& index, QVariant const& value, int role)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

	if (role == ReferenceClickedRole)
	{
        //m_pressed = value.toBool() ? index : QModelIndex();
		return true;
	}
	else if (!index.isValid() || role == Qt::EditRole)
	{
		return false;
	}
    Tree_Item* ti = (Tree_Item*)index.internalPointer();
	if (!ti)
	{
		return false;
	}

	if (index.column() == 0)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

bool Properties_Model::setHeaderData(int section, Qt::Orientation orientation, QVariant const& value, int role)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);


	return false;
}

//////////////////////////////////////////////////////////////////////////

bool Properties_Model::insertColumns(int position, int columns, QModelIndex const& parent)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

	return false;
}

//////////////////////////////////////////////////////////////////////////

bool Properties_Model::removeColumns(int position, int columns, QModelIndex const& parent)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

	return false;
}

//////////////////////////////////////////////////////////////////////////

bool Properties_Model::insertRows(int position, int rows, QModelIndex const& parent)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

	return false;
}

//////////////////////////////////////////////////////////////////////////

bool Properties_Model::removeRows(int position, int rows, QModelIndex const& parent)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
	return false;
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::on_value_changed(std::shared_ptr<const ts::IValue> value)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

    const std::shared_ptr<Tree_Item> item = m_root->find_by_primary_or_secondary(*value);
	if (item)
	{
        QModelIndex& topLeft = item->m_model_index;
		QModelIndex bottomRight = createIndex(static_cast<int>(topLeft.row()), columnCount(topLeft) - 1, topLeft.internalPointer());

		Q_EMIT dataChanged(topLeft, bottomRight, QVector<int>() << Qt::DisplayRole << Qt::EditRole);
	}
}

//////////////////////////////////////////////////////////////////////////

Properties_Model::Editor_Data Properties_Model::get_editor_data_from_index(QModelIndex index) const
{
    Editor_Data ed;
    if (!index.isValid())
    {
        return ed;
    }
    Tree_Item* ti = (Tree_Item*)index.internalPointer();
    if (!ti)
    {
        return ed;
    }
    ed.value = ti->m_value;
    ed.suffix = ti->m_suffix;
    return ed;
}

//////////////////////////////////////////////////////////////////////////

QModelIndex Properties_Model::get_root_index() const
{
    return m_root->m_model_index;
}

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<ts::IValue> Properties_Model::get_value_from_index(QModelIndex index) const
{
	if (!index.isValid())
	{
		return nullptr;
	}
    Tree_Item* ti = (Tree_Item*)index.internalPointer();
	if (!ti)
	{
		return nullptr;
	}
	return ti->m_value;
}

//////////////////////////////////////////////////////////////////////////

bool Properties_Model::is_index_read_only(QModelIndex index) const
{
	if (!index.isValid())
	{
        return false;
	}
    Tree_Item* ti = (Tree_Item*)index.internalPointer();
	if (!ti)
	{
        return false;
	}
	return ti->m_isReadOnly;
}

//////////////////////////////////////////////////////////////////////////

QModelIndex Properties_Model::get_index_from_value(const ts::IValue& value) const
{
    auto ti = m_root->find_by_primary_or_secondary(value);
	if (!ti)
	{
		return QModelIndex();
	}
    return ti->m_model_index;
}

//////////////////////////////////////////////////////////////////////////

//void Properties_Model::on_elements_added(std::shared_ptr<ts::IVectorValue> parent, size_t idx, size_t count)
//{
//	const size_t start = std::distance(parent->get_values().begin(), beginIt);
//	const size_t count = std::distance(beginIt, endIt);

//    std::vector<std::shared_ptr<ts::IValue>> elements;
//	elements.reserve(count);

//	std::copy(beginIt, endIt, std::back_inserter(elements));

//    _added(*parent, elements, start, count);

//	//update the names
//    std::shared_ptr<Tree_Item> parent_ti = m_root.find_by_primary_or_secondary(*parent);
//	if (parent_ti)
//	{
//		for (size_t i = 0; i < parent_ti->m_children.size(); i++)
//		{
//            Tree_Item& child = *parent_ti->m_children[i];
//			child.m_name = format("{}", i);
//            const QModelIndex& topLeft = child.m_model_index;
//			QModelIndex bottomRight = createIndex(static_cast<int>(topLeft.row()), columnCount(topLeft) - 1, topLeft.internalPointer());
//			Q_EMIT dataChanged(topLeft, bottomRight, QVector<int>() << Qt::DisplayRole << Qt::EditRole);
//		}
//	}
//}

///////////////////////////////////////////////////////////////////////////////

//void Properties_Model::on_elements_will_be_removed(std::shared_ptr<ts::IVectorValue> parent, size_t idx, size_t count)
//{
//	const size_t start = std::distance(parent->get_values().begin(), beginIt);
//	const size_t count = std::distance(beginIt, endIt);

//    std::vector<std::shared_ptr<ts::IValue>> elements;
//	elements.reserve(count);

//	std::copy(beginIt, endIt, std::back_inserter(elements));

//    _will_be_removed(*parent, elements, start, count);

//	//update the names
//    std::shared_ptr<Tree_Item> parent_ti = m_root.find_by_primary_or_secondary(*parent);
//	if (parent_ti)
//	{
//		for (size_t i = 0; i < parent_ti->m_children.size(); i++)
//		{
//            Tree_Item& child = *parent_ti->m_children[i];
//			child.m_name = format("{}", i);
//            const QModelIndex& topLeft = child.m_model_index;
//			QModelIndex bottomRight = createIndex(static_cast<int>(topLeft.row()), columnCount(topLeft) - 1, topLeft.internalPointer());
//			Q_EMIT dataChanged(topLeft, bottomRight, QVector<int>() << Qt::DisplayRole << Qt::EditRole);
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::on_optional_was_set(std::shared_ptr<ts::IOptional_Value> parent)
{
    QASSERT(parent->is_set()); //already set

    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
    std::shared_ptr<Tree_Item> parent_ti = m_root->find_by_primary_or_secondary(*parent);
    if (parent_ti)
    {
        //assign the wrapped value now, as it changed. This makes the TreeItem findable using both values (the Optional and the Inner value).
        parent_ti->m_secondary_value = parent->get_value();

        //first we build in a temp tree item to be able to count the children. Qt demands we know the count before actually adding them!
        std::shared_ptr<Tree_Item> temp = std::make_shared<Tree_Item>(this, std::string(), std::string());
        temp->build_children(parent->get_value());

        size_t count = temp->get_child_count();
        if (count > 0)
        {
            beginInsertRows(parent_ti->m_model_index, static_cast<int>(0), static_cast<int>(count - 1));
            for (size_t c = 0; c < count; c++)
            {
                parent_ti->add_child(std::move(temp->m_children[c]));
            }
            endInsertRows();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::on_optional_will_be_unset(std::shared_ptr<ts::IOptional_Value> parent)
{
    QASSERT(parent->is_set()); //still set, will be unset after this callback

    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
    std::shared_ptr<Tree_Item> parent_ti = m_root->find_by_primary_or_secondary(*parent);
    if (parent_ti)
    {
        //assign the wrapped value to null as the optinal doesn't have an inner value anymore
        parent_ti->m_secondary_value = nullptr;

        size_t count = parent_ti->get_child_count();
        if (count > 0)
        {
            beginRemoveRows(parent_ti->m_model_index, static_cast<int>(0), static_cast<int>(count - 1));
            parent_ti->m_children.clear();
            endRemoveRows();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::on_variant_type_index_has_changed(std::shared_ptr<ts::IVariant_Value> parent)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

    if (parent->is_set())
    {
        std::shared_ptr<Tree_Item> parent_ti = m_root->find_by_primary_or_secondary(*parent);
        if (parent_ti)
        {
            //assign the wrapped value now, as it changed. This makes the TreeItem findable using both values (the Variant and the Inner value).
            parent_ti->m_secondary_value = parent->get_value();

            //first we build in a temp tree item to be able to count the children. Qt demands we know the count before actually adding them!
            std::shared_ptr<Tree_Item> temp = std::make_shared<Tree_Item>(this, std::string(), std::string());
            temp->build_children(parent->get_value());

            size_t count = temp->get_child_count();
            if (count > 0)
            {
                beginInsertRows(parent_ti->m_model_index, static_cast<int>(0), static_cast<int>(count - 1));
                for (size_t c = 0; c < count; c++)
                {
                    parent_ti->add_child(std::move(temp->m_children[c]));
                }
                endInsertRows();
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::on_variant_type_index_will_change(std::shared_ptr<ts::IVariant_Value> parent)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
    std::shared_ptr<Tree_Item> parent_ti = m_root->find_by_primary_or_secondary(*parent);
    if (parent_ti)
    {
        size_t count = parent_ti->get_child_count();
        if (count > 0)
        {
            beginRemoveRows(parent_ti->m_model_index, static_cast<int>(0), static_cast<int>(count - 1));
            parent_ti->m_children.clear();
            endRemoveRows();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::on_poly_type_has_changed(std::shared_ptr<ts::IPoly_Value> parent)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

    if (parent->get_value())
    {
        std::shared_ptr<Tree_Item> parent_ti = m_root->find_by_primary_or_secondary(*parent);
        if (parent_ti)
        {
            //assign the wrapped value now, as it changed. This makes the TreeItem findable using both values (the Variant and the Inner value).
            parent_ti->m_secondary_value = parent->get_value();

            //first we build in a temp tree item to be able to count the children. Qt demands we know the count before actually adding them!
            std::shared_ptr<Tree_Item> temp = std::make_shared<Tree_Item>(this, std::string(), std::string());
            temp->build_children(parent->get_value());

            size_t count = temp->get_child_count();
            if (count > 0)
            {
                beginInsertRows(parent_ti->m_model_index, static_cast<int>(0), static_cast<int>(count - 1));
                for (size_t c = 0; c < count; c++)
                {
                    parent_ti->add_child(std::move(temp->m_children[c]));
                }
                endInsertRows();
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Model::on_poly_type_will_change(std::shared_ptr<ts::IPoly_Value> parent)
{
    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
    std::shared_ptr<Tree_Item> parent_ti = m_root->find_by_primary_or_secondary(*parent);
    if (parent_ti)
    {
        size_t count = parent_ti->get_child_count();
        if (count > 0)
        {
            beginRemoveRows(parent_ti->m_model_index, static_cast<int>(0), static_cast<int>(count - 1));
            parent_ti->m_children.clear();
            endRemoveRows();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

//void Properties_Model::_added(const ts::IValue& parent, const std::vector<std::shared_ptr<ts::IValue>>& elements, size_t start, size_t count)
//{
//    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
//    std::shared_ptr<Tree_Item> parent_ti = m_root.find_by_primary_or_secondary(parent);
//	if (parent_ti)
//	{
//		beginInsertRows(parent_ti->m_model_index, static_cast<int>(start), static_cast<int>(start + count - 1));
//		for (size_t c = 0; c < count; c++)
//		{
//            std::shared_ptr<Tree_Item> ti = std::make_shared<Tree_Item>(this, std::string());
//			ti->Build(elements[c]);
//			ti->RefreshModelIndex(c + start);
//			parent_ti->InsertChild(start + c, std::move(ti));
//		}
//		for (size_t j = 0; j < parent_ti->m_children.size(); j++)
//		{
//			parent_ti->m_children[j]->RefreshModelIndex(j);
//		}
//		endInsertRows();
//	}
//}

///////////////////////////////////////////////////////////////////////////////

//void Properties_Model::_will_be_removed(const ts::IValue& parent, const std::vector<std::shared_ptr<ts::IValue>>& elements, size_t start, size_t count)
//{
//	JTL_TODO("beginRemoveRows should be called once, not in every iteration to improve performance");

//    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
//	for (size_t c = 0; c < count; c++)
//	{
//        std::shared_ptr<Tree_Item> ti = m_root.find_by_primary_or_secondary(*elements[c]);
//		QASSERT(ti);
//		if (ti)
//		{
//			size_t row = ti->GetRow();
//            std::shared_ptr<Tree_Item> parent_ti = ti->m_parent;
//			QASSERT(parent_ti);
//			if (parent_ti)
//			{
//				ti.reset();
//				beginRemoveRows(parent_ti->m_model_index, static_cast<int>(row), static_cast<int>(row));//check todo at the beginning of the function
//				parent_ti->m_children.erase(parent_ti->m_children.begin() + row);
//				for (size_t j = 0; j < parent_ti->m_children.size(); j++)
//				{
//					parent_ti->m_children[j]->RefreshModelIndex(j);
//				}
//				endRemoveRows();
//			}
//		}
//	}
//}
