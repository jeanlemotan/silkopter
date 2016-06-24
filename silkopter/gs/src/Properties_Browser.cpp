#include "Properties_Browser.h"
#include "Properties_Model.h"
#include "Properties_Delegate.h"
#include "value_editors/Value_Editor_Factory.h"

#include <QtEvents>
#include <QSettings>
#include <QModelIndex>
#include <QStack>

//////////////////////////////////////////////////////////////////////////

Properties_Browser::Properties_Browser(QWidget* parent /* = nullptr */)
	: QTreeView(parent)
{
	setSelectionBehavior(QAbstractItemView::SelectItems);
}

//////////////////////////////////////////////////////////////////////////

Properties_Browser::~Properties_Browser()
{
}

//////////////////////////////////////////////////////////////////////////

void Properties_Browser::init(std::shared_ptr<Value_Editor_Factory> editor_factory)
{
    m_model = std::make_shared<Properties_Model>(this);
    m_delegate = std::make_shared<Properties_Delegate>(this, editor_factory);
    QTreeView::setItemDelegate(m_delegate.get());
    QTreeView::setModel(m_model.get());

	// setup vertical scrollbar
	setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
	setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);

	setAlternatingRowColors(true);
}

//////////////////////////////////////////////////////////////////////////

void Properties_Browser::shutdown()
{
    QTreeView::setModel(nullptr);
	m_model.reset();

	setItemDelegate(nullptr);
	m_delegate.reset();
}

//////////////////////////////////////////////////////////////////////////

void Properties_Browser::reset()
{
    set_value(nullptr);
	if (m_delegate)
	{
        m_delegate->reset();
	}
}

//////////////////////////////////////////////////////////////////////////

void Properties_Browser::set_value(std::shared_ptr<ts::IStruct_Value> value)
{
    save_collapse_status();

	//to make sure that any editing in progress gets saved
    m_delegate->finish_editing();

    m_model->set_value(value);

    load_collapse_status();
}

///////////////////////////////////////////////////////////////////////////////

void Properties_Browser::setModel(QAbstractItemModel* model)
{
    QASSERT_MSG(false, "Don't call Properties_Browser::setModel!");
}

//////////////////////////////////////////////////////////////////////////

void Properties_Browser::defaultExpand()
{
	expandToDepth(2);
}

//////////////////////////////////////////////////////////////////////////

void Properties_Browser::load_collapse_status()
{
    QSettings settings;
    settings.beginGroup("Properties_Browser");
    QSet<QString> collapsed_nodes = settings.value("collapsed_nodes").toStringList().toSet();
	settings.endGroup();

	QStack<QModelIndex> indexes;
	indexes.push(QModelIndex());
	auto m = QTreeView::model();
	while (!indexes.empty())
	{
		auto current = indexes.pop();
        if (collapsed_nodes.contains(current.data().toString()))
		{
			collapse(current);
		}
		else
		{
			expand(current);
		}
		for (int i = 0; i < m->rowCount(current); ++i)
		{
			auto index = m->index(i, 0, current);
			if (m->hasChildren(index))
			{
				indexes.push(index);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void Properties_Browser::save_collapse_status()
{
    QSettings settings;
    settings.beginGroup("Properties_Browser");
    QSet<QString> collapsed_nodes = settings.value("collapsed_nodes").toStringList().toSet();

	QStack<QModelIndex> indexes;
	indexes.push(QModelIndex());
	auto m = QTreeView::model();
	while (!indexes.empty())
	{
		auto current = indexes.pop();
		if (current.isValid() && !isExpanded(current))
		{
            collapsed_nodes.insert(current.data().toString());
		}
		else
		{
            collapsed_nodes.remove(current.data().toString());
		}

		for (int i = 0; i < m->rowCount(current); ++i)
		{
			auto index = m->index(i, 0, current);
			if (m->hasChildren(index))
			{
				indexes.push(index);
			}
		}
	}

    settings.setValue("collapsed_nodes", QVariant(collapsed_nodes.toList()));
    settings.endGroup(); // Properties_Browser
}

//////////////////////////////////////////////////////////////////////////
