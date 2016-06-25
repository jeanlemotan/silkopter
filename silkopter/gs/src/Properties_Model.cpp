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

Properties_Model::Tree_Item::Tree_Item(Properties_Model* model, const std::string& name)
	: m_model(model)
	, m_name(name)
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

        std::shared_ptr<Tree_Item> item = std::make_shared<Tree_Item>(m_model, ui_name);

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
        //m_connections.push_back(optional_value->sig_wasSet.connect(&Properties_Model::on_optional_was_set, m_model, jtl::promote_to_ref(optional_value)));
        //m_connections.push_back(optional_value->sig_willBeUnset.connect(&Properties_Model::on_optional_will_be_unset, m_model, jtl::promote_to_ref(optional_value)));

        if (optional_value->is_set())
		{
            std::shared_ptr<ts::IValue> inner_value = optional_value->get_value();
            m_secondary_value = inner_value; //as it is wrapped
            build_children(inner_value);
		}
	}
    else if (std::shared_ptr<ts::IVariant_Value> variant_value = std::dynamic_pointer_cast<ts::IVariant_Value>(value))
	{
        //m_connections.push_back(variant_value->sig_typeIndexWillBeChanged.connect(&Properties_Model::on_variant_type_index_will_be_changed, m_model, jtl::promote_to_ref(variant_value)));
        //m_connections.push_back(variant_value->sig_typeIndexWasChanged.connect(&Properties_Model::on_variant_type_index_was_changed, m_model, jtl::promote_to_ref(variant_value)));

        if (variant_value->is_set())
		{
            std::shared_ptr<ts::IValue> inner_value = variant_value->get_value();
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

            std::shared_ptr<Tree_Item> item = std::make_shared<Tree_Item>(m_model, member->get_member_def()->get_ui_name());
            item->build(member->get_value());
            add_child(std::move(item));
		}
	}
    else if (std::shared_ptr<ts::IVector_Value> vector_value = std::dynamic_pointer_cast<ts::IVector_Value>(value))
	{
        //m_connections.push_back(vectorValue->sig_elementsAdded.connect(&Properties_Model::on_elements_added, m_model, jtl::promote_to_ref(vectorValue), std::placeholders::_1, std::placeholders::_2));
        //m_connections.push_back(vectorValue->sig_elementsWillBeRemoved.connect(&Properties_Model::on_elements_will_be_removed, m_model, jtl::promote_to_ref(vectorValue), std::placeholders::_1, std::placeholders::_2));

        m_children.reserve(vector_value->get_value_count());
        for (size_t i = 0; i < vector_value->get_value_count(); i++)
		{
            std::shared_ptr<Tree_Item> item = std::make_shared<Tree_Item>(m_model, std::to_string(i));
            item->build(value);
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
    , m_root(std::make_shared<Tree_Item>(this, "root"))
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
                return QVariant(result.payload().c_str());
			}

			size_t count = ti->m_children.size();
			if (count == 0)
			{
				return QVariant();
			}

			//don't use value->GetAsString() for structs because it returns the real member count, but the tree item might be hiding some members
            return QVariant(/*jtl::format<std::string>("{} member{}", count, count == 1 ? "" : "s").c_str()*/);
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
	endResetModel();

    m_rootValue = value;
    if (!m_rootValue)
	{
        //sig_handleChanged.emit();
		return;
	}

    m_root->build_root(*m_rootValue);
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
		// 		auto cmd = std::make_shared<SetItemNameCmd>(*item, value.toString().toUtf8().data());
		// 		Commander::inst().execute(cmd);
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

// void Properties_Model::PropertyEnabledChanged(Property& prop)
// {
// 	std::vector<TreeItem*> allTis = mRoot.findAll(prop);
// 	QASSERT(!allTis.empty());
// 
// 	for (size_t i = 0; i < allTis.size(); i++)
// 	{
// 		allTis[i]->refreshCachedIcon();
// 		Q_EMIT dataChanged(allTis[i]->mModelIndex, allTis[i]->mModelIndex);
// 	}
// }

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

//void Properties_Model::on_elements_added(jtl::lent_ref<ts::IVectorValue> parent, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> beginIt, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> endIt)
//{
//	const size_t start = std::distance(parent->get_values().begin(), beginIt);
//	const size_t count = std::distance(beginIt, endIt);

//    std::vector<jtl::lent_ref<ts::IValue>> elements;
//	elements.reserve(count);

//	std::copy(beginIt, endIt, std::back_inserter(elements));

//    _added(*parent, elements, start, count);

//	//update the names
//    jtl::lent_ptr<Tree_Item> parentTi = m_root.find_by_primary_or_secondary(*parent);
//	if (parentTi)
//	{
//		for (size_t i = 0; i < parentTi->m_children.size(); i++)
//		{
//            Tree_Item& child = *parentTi->m_children[i];
//			child.m_name = jtl::format("{}", i);
//            const QModelIndex& topLeft = child.m_model_index;
//			QModelIndex bottomRight = createIndex(static_cast<int>(topLeft.row()), columnCount(topLeft) - 1, topLeft.internalPointer());
//			Q_EMIT dataChanged(topLeft, bottomRight, QVector<int>() << Qt::DisplayRole << Qt::EditRole);
//		}
//	}
//}

///////////////////////////////////////////////////////////////////////////////

//void Properties_Model::on_elements_will_be_removed(jtl::lent_ref<ts::IVectorValue> parent, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> beginIt, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> endIt)
//{
//	const size_t start = std::distance(parent->get_values().begin(), beginIt);
//	const size_t count = std::distance(beginIt, endIt);

//    std::vector<jtl::lent_ref<ts::IValue>> elements;
//	elements.reserve(count);

//	std::copy(beginIt, endIt, std::back_inserter(elements));

//    _will_be_removed(*parent, elements, start, count);

//	//update the names
//    jtl::lent_ptr<Tree_Item> parentTi = m_root.find_by_primary_or_secondary(*parent);
//	if (parentTi)
//	{
//		for (size_t i = 0; i < parentTi->m_children.size(); i++)
//		{
//            Tree_Item& child = *parentTi->m_children[i];
//			child.m_name = jtl::format("{}", i);
//            const QModelIndex& topLeft = child.m_model_index;
//			QModelIndex bottomRight = createIndex(static_cast<int>(topLeft.row()), columnCount(topLeft) - 1, topLeft.internalPointer());
//			Q_EMIT dataChanged(topLeft, bottomRight, QVector<int>() << Qt::DisplayRole << Qt::EditRole);
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////

//void Properties_Model::on_members_added(jtl::lent_ref<ts::IDynamicStructValue> parent, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> beginIt, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> endIt)
//{
//	const size_t start = std::distance(parent->GetMembers().begin(), beginIt);
//	const size_t count = std::distance(beginIt, endIt);

//    std::vector<jtl::lent_ref<ts::IValue>> members;
//	members.reserve(count);

//	std::copy(beginIt, endIt, std::back_inserter(members));

//    _added(*parent, members, start, count);

//	for (auto it = beginIt; it != endIt; ++it)
//	{
//        const jtl::lent_ptr<ts::IValue> value = *it;
//        const jtl::lent_ptr<Tree_Item> item = m_root.find_by_primary_or_secondary(*value);
//		item->m_name = parent->GetMemberName(it);
//	}
//}

///////////////////////////////////////////////////////////////////////////////

//void Properties_Model::on_members_will_be_removed(jtl::lent_ref<ts::IDynamicStructValue> parent, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> beginIt, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> endIt)
//{
//	const size_t start = std::distance(parent->GetMembers().begin(), beginIt);
//	const size_t count = std::distance(beginIt, endIt);

//    std::vector<jtl::lent_ref<ts::IValue>> elements;
//	elements.reserve(count);

//	std::copy(beginIt, endIt, std::back_inserter(elements));

//    _will_be_removed(*parent, elements, start, count);
//}

//////////////////////////////////////////////////////////////////////////

//void Properties_Model::on_optional_was_set(jtl::lent_ref<ts::IOptionalValue> parent)
//{
//	QASSERT(parent->IsSet()); //already set

//    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
//    jtl::lent_ptr<Tree_Item> parentTi = m_root.find_by_primary_or_secondary(*parent);
//	if (parentTi)
//	{
//		//assign the wrapped value now, as it changed. This makes the TreeItem findable using both values (the Optional and the Inner value).
//		parentTi->m_secondaryValue = parent->get_value();

//		//first we build in a temp tree item to be able to count the children. Qt demands we know the count before actually adding them!
//        jtl::unique_ptr<Tree_Item> temp = jtl::make_unique<Tree_Item>(this, std::string());
//		temp->BuildChildren(jtl::promote_to_ref(parent->get_value()));

//		size_t count = temp->GetChildCount();
//		if (count > 0)
//		{
//			beginInsertRows(parentTi->m_modelIndex, static_cast<int>(0), static_cast<int>(count - 1));
//			for (size_t c = 0; c < count; c++)
//			{
//                jtl::lent_ref<Tree_Item> ti = parentTi->AddChild(std::move(temp->m_children[c]));
//			}
//			endInsertRows();
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////

//void Properties_Model::on_optional_will_be_unset(jtl::lent_ref<ts::IOptionalValue> parent)
//{
//	QASSERT(parent->IsSet()); //still set, will be unset after this callback

//    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
//    jtl::lent_ptr<Tree_Item> parentTi = m_root.find_by_primary_or_secondary(*parent);
//	if (parentTi)
//	{
//		//assign the wrapped value to null as the optinal doesn't have an inner value anymore
//		parentTi->m_secondaryValue = nullptr;

//		size_t count = parentTi->GetChildCount();
//		if (count > 0)
//		{
//			beginRemoveRows(parentTi->m_modelIndex, static_cast<int>(0), static_cast<int>(count - 1));
//			parentTi->m_children.clear();
//			endRemoveRows();
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////

//void Properties_Model::on_variant_type_index_was_changed(jtl::lent_ref<ts::IVariantValue> parent)
//{
//    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);

//	if (parent->IsSet())
//	{
//        jtl::lent_ptr<Tree_Item> parentTi = m_root.find_by_primary_or_secondary(*parent);
//		if (parentTi)
//		{
//			//assign the wrapped value now, as it changed. This makes the TreeItem findable using both values (the Variant and the Inner value).
//			parentTi->m_secondaryValue = parent->get_value();

//			//first we build in a temp tree item to be able to count the children. Qt demands we know the count before actually adding them!
//            jtl::unique_ptr<Tree_Item> temp = jtl::make_unique<Tree_Item>(this, std::string());
//			temp->BuildChildren(jtl::promote_to_ref(parent->get_value()));

//			size_t count = temp->GetChildCount();
//			if (count > 0)
//			{
//				beginInsertRows(parentTi->m_modelIndex, static_cast<int>(0), static_cast<int>(count - 1));
//				for (size_t c = 0; c < count; c++)
//				{
//                    jtl::lent_ref<Tree_Item> ti = parentTi->AddChild(std::move(temp->m_children[c]));
//				}
//				endInsertRows();
//			}
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////

//void Properties_Model::on_variant_type_index_will_be_changed(jtl::lent_ref<ts::IVariantValue> parent)
//{
//    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
//    jtl::lent_ptr<Tree_Item> parentTi = m_root.find_by_primary_or_secondary(*parent);
//	if (parentTi)
//	{
//		size_t count = parentTi->GetChildCount();
//		if (count > 0)
//		{
//			beginRemoveRows(parentTi->m_modelIndex, static_cast<int>(0), static_cast<int>(count - 1));
//			parentTi->m_children.clear();
//			endRemoveRows();
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////

//void Properties_Model::OnDictionaryElementsAdded(jtl::lent_ref<ts::IDictionaryValue> parent, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> beginIt, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> endIt)
//{
//	const size_t start = std::distance(parent->GetElements().begin(), beginIt);
//	const size_t count = std::distance(beginIt, endIt);

//    std::vector<jtl::lent_ref<ts::IValue>> elements;
//	elements.reserve(count);

//	std::copy(beginIt, endIt, std::back_inserter(elements));

//    _added(*parent, elements, start, count);

//	for (auto it = beginIt; it != endIt; ++it)
//	{
//        const jtl::lent_ptr<ts::IValue> value = *it;
//        const jtl::lent_ptr<Tree_Item> item = m_root.find_by_primary_or_secondary(*value);
//		item->m_name = parent->GetElementName(it);
//	}
//}

///////////////////////////////////////////////////////////////////////////////

//void Properties_Model::OnDictionaryElementsWillBeRemoved(jtl::lent_ref<ts::IDictionaryValue> parent, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> beginIt, jtl::const_random_iterator<jtl::lent_ref<ts::IValue>> endIt)
//{
//	const size_t start = std::distance(parent->GetElements().begin(), beginIt);
//	const size_t count = std::distance(beginIt, endIt);

//    std::vector<jtl::lent_ref<ts::IValue>> elements;
//	elements.reserve(count);

//	std::copy(beginIt, endIt, std::back_inserter(elements));

//    _will_be_removed(*parent, elements, start, count);
//}

//////////////////////////////////////////////////////////////////////////

//void Properties_Model::_added(const ts::IValue& parent, const std::vector<jtl::lent_ref<ts::IValue>>& elements, size_t start, size_t count)
//{
//    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
//    jtl::lent_ptr<Tree_Item> parentTi = m_root.find_by_primary_or_secondary(parent);
//	if (parentTi)
//	{
//		beginInsertRows(parentTi->m_modelIndex, static_cast<int>(start), static_cast<int>(start + count - 1));
//		for (size_t c = 0; c < count; c++)
//		{
//            jtl::unique_ref<Tree_Item> ti = jtl::make_unique<Tree_Item>(this, std::string());
//			ti->Build(elements[c]);
//			ti->RefreshModelIndex(c + start);
//			parentTi->InsertChild(start + c, std::move(ti));
//		}
//		for (size_t j = 0; j < parentTi->m_children.size(); j++)
//		{
//			parentTi->m_children[j]->RefreshModelIndex(j);
//		}
//		endInsertRows();
//	}
//}

///////////////////////////////////////////////////////////////////////////////

//void Properties_Model::_will_be_removed(const ts::IValue& parent, const std::vector<jtl::lent_ref<ts::IValue>>& elements, size_t start, size_t count)
//{
//	JTL_TODO("beginRemoveRows should be called once, not in every iteration to improve performance");

//    std::lock_guard<std::recursive_mutex> sm(m_tree_mutex);
//	for (size_t c = 0; c < count; c++)
//	{
//        jtl::lent_ptr<Tree_Item> ti = m_root.find_by_primary_or_secondary(*elements[c]);
//		QASSERT(ti);
//		if (ti)
//		{
//			size_t row = ti->GetRow();
//            jtl::lent_ptr<Tree_Item> parentTi = ti->m_parent;
//			QASSERT(parentTi);
//			if (parentTi)
//			{
//				ti.reset();
//				beginRemoveRows(parentTi->m_modelIndex, static_cast<int>(row), static_cast<int>(row));//check todo at the beginning of the function
//				parentTi->m_children.erase(parentTi->m_children.begin() + row);
//				for (size_t j = 0; j < parentTi->m_children.size(); j++)
//				{
//					parentTi->m_children[j]->RefreshModelIndex(j);
//				}
//				endRemoveRows();
//			}
//		}
//	}
//}
