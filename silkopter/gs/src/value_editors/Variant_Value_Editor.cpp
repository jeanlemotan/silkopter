#include "value_editors/Variant_Value_Editor.h"
#include "value_editors/Value_Editor_Factory.h"
#include "def_lang/Qualified_Type.h"

#include <QCheckBox>
#include <QHBoxLayout>

Variant_Value_Editor::Variant_Value_Editor(const Qualified_Value<ts::IVariant_Value>& qualified_value, std::string const& editor_str, std::string const& suffix_str, std::shared_ptr<Value_Editor_Factory> value_editor_factory)
    : m_value_editor_factory(value_editor_factory)
    , m_qualified_value(qualified_value)
    , m_editor_str(editor_str)
    , m_suffix_str(suffix_str)
{
    std::shared_ptr<const ts::IVariant_Value> value = m_qualified_value.get_const_value();
    std::shared_ptr<const ts::IVariant_Type> type = value->get_specialized_type();

	m_editor = new QWidget();
	m_editor->setAutoFillBackground(true);

	QHBoxLayout* layout = new QHBoxLayout(m_editor);
	layout->setMargin(0);
	layout->setSpacing(2);
	layout->setAlignment(Qt::AlignLeft);

	{
		m_combobox = new QComboBox();
		m_combobox->setMaxVisibleItems(40);
		m_combobox->setInsertPolicy(QComboBox::NoInsert);
		m_combobox->setMinimumWidth(200);
		m_combobox->setAutoFillBackground(true);

        for (size_t i = 0; i < type->get_inner_qualified_type_count(); i++)
		{
            m_combobox->addItem(type->get_inner_qualified_type(i)->get_type()->get_ui_name().c_str());
		}

        size_t index = value->get_value_type_index();
        m_combobox->setCurrentIndex(static_cast<int>(index));

		m_qt_connection = QObject::connect(m_combobox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index)
		{
            refresh_value();
		});

		layout->addWidget(m_combobox);
	}

    create_inner_editor();

    refresh_read_only_state();
}

Variant_Value_Editor::~Variant_Value_Editor()
{
	QObject::disconnect(m_qt_connection);
}

QWidget* Variant_Value_Editor::get_widget()
{
	return m_editor;
}

Qualified_Value<ts::IValue> Variant_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Variant_Value_Editor::refresh_editor()
{
    std::shared_ptr<const ts::IVariant_Value> value = m_qualified_value.get_const_value();
    size_t index = value->get_value_type_index();
    m_combobox->setCurrentIndex(static_cast<int>(index));

    create_inner_editor();
    if (m_inner_editor)
    {
        m_inner_editor->refresh_editor();
    }
}

void Variant_Value_Editor::refresh_value()
{
    if (!is_read_only())
    {
        int index = m_combobox->currentIndex();
        std::shared_ptr<ts::IVariant_Value> value = m_qualified_value.get_mutable_value();
        if (value)
        {
            ts::Result<void> result = value->set_value_type_index(static_cast<size_t>(index));
            QASSERT(result == ts::success);
        }

        create_inner_editor();
        if (m_inner_editor)
        {
            m_inner_editor->refresh_value();
        }
    }
}

void Variant_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
	if (m_inner_editor)
	{
        m_inner_editor->set_read_only_override(read_only);
	}
}

bool Variant_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Variant_Value_Editor::refresh_read_only_state()
{
    m_combobox->setDisabled(is_read_only());
}

void Variant_Value_Editor::create_inner_editor()
{
    std::shared_ptr<const ts::IVariant_Value> value = m_qualified_value.get_const_value();
    if (m_inner_editor && value->get_value_type_index() == m_inner_editor_type_index)
    {
        return;
    }

    if (m_inner_editor)
    {
        m_editor->layout()->removeWidget(m_inner_editor->get_widget());
		
        //TODO - Clarify why this is needed
        delete m_inner_editor->get_widget();
    }

    m_inner_editor.reset();

    if (value->is_set())
    {
        if (m_qualified_value.get_mutable_value())
        {
            m_inner_editor = m_value_editor_factory->create_editor(m_qualified_value.get_mutable_value()->get_value(), m_editor_str, m_suffix_str);
        }
        else
        {
            m_inner_editor = m_value_editor_factory->create_editor(m_qualified_value.get_const_value()->get_value(), m_editor_str, m_suffix_str);
        }
    }

    m_inner_editor_type_index = value->get_value_type_index();
    if (m_inner_editor)
    {
        m_editor->layout()->addWidget(m_inner_editor->get_widget());
    }
    refresh_read_only_state();
}
