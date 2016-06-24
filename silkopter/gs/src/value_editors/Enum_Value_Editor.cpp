#include "value_editors/Enum_Value_Editor.h"

#include <QComboBox>


Enum_Value_Editor::Enum_Value_Editor(const Qualified_Value<ts::IEnum_Value>& qualified_value)
    : m_qualified_value(qualified_value)
{
	m_editor = new QComboBox();
	m_editor->setMaxVisibleItems(40);
	m_editor->setInsertPolicy(QComboBox::NoInsert);
	m_editor->setMinimumWidth(200);
	m_editor->setAutoFillBackground(true);

    std::shared_ptr<const ts::IEnum_Value> value = m_qualified_value.get_const_value();
    std::shared_ptr<const ts::IEnum_Type> type = value->get_specialized_type();

    size_t idx = 0;
    for (size_t i = 0; i < type->get_item_count(); i++)
	{
        m_editor->addItem(type->get_item(i)->get_ui_name().c_str());
        if (type->get_item(i) == value->get_value())
        {
            idx = i;
        }
	}

    m_editor->setCurrentIndex(idx);

    m_changed_connection = QObject::connect(m_editor, &QComboBox::currentTextChanged, [=](const QString& val)
	{
        refresh_value();
	});

    refresh_read_only_state();
}

Enum_Value_Editor::~Enum_Value_Editor()
{
    QObject::disconnect(m_changed_connection);
}

QWidget* Enum_Value_Editor::get_widget()
{
	return m_editor;
}

Qualified_Value<ts::IValue> Enum_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Enum_Value_Editor::refresh_editor()
{
    std::shared_ptr<const ts::IEnum_Value> value = m_qualified_value.get_const_value();
    std::shared_ptr<const ts::IEnum_Type> type = value->get_specialized_type();

    size_t idx = 0;
    for (size_t i = 0; i < type->get_item_count(); i++)
    {
        if (type->get_item(i) == value->get_value())
        {
            idx = i;
            break;
        }
    }

    m_editor->setCurrentIndex(idx);
}

void Enum_Value_Editor::refresh_value()
{
    std::shared_ptr<const ts::IEnum_Value> value = m_qualified_value.get_const_value();
    std::shared_ptr<const ts::IEnum_Type> type = value->get_specialized_type();

    set_value(type->get_item(m_editor->currentIndex()));
}

void Enum_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
}

bool Enum_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Enum_Value_Editor::refresh_read_only_state()
{
    m_editor->setDisabled(is_read_only());
}

void Enum_Value_Editor::set_value(std::shared_ptr<const ts::IEnum_Item> element)
{
    if (!is_read_only())
	{
        if (std::shared_ptr<ts::IEnum_Value> mutable_value = m_qualified_value.get_mutable_value())
		{
            mutable_value->set_value(element);
		}
	}
}

