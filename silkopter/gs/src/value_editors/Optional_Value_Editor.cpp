#include "value_editors/Optional_Value_Editor.h"
#include "value_editors/Value_Editor_Factory.h"

#include <QCheckBox>
#include <QHBoxLayout>


Optional_Value_Editor::Optional_Value_Editor(const Qualified_Value<ts::IOptional_Value>& qualified_value, std::shared_ptr<Value_Editor_Factory> value_editor_factory)
    : m_value_editor_factory(value_editor_factory)
    , m_qualified_value(qualified_value)
{
    std::shared_ptr<const ts::IOptional_Value> value = m_qualified_value.get_const_value();

	m_editor = new QWidget();
	m_editor->setAutoFillBackground(true);
	QHBoxLayout* layout = new QHBoxLayout(m_editor);
	layout->setMargin(0);
	layout->setSpacing(2);
	layout->setAlignment(Qt::AlignLeft);

	m_checkbox = new QCheckBox();
    m_checkbox->setChecked(value->is_set());
    m_checkbox->setText(value->is_set() ? "" : "<none>");
	m_checkbox->setAutoFillBackground(true);

	layout->addWidget(m_checkbox);
	create_inner_editor();

    m_toggled_connection = QObject::connect(m_checkbox, &QCheckBox::toggled, [=](bool v)
	{
        m_checkbox->setText(v ? "" : "<none>");
		if (m_inner_editor)
		{
            m_inner_editor->get_widget()->setVisible(v);
		}
	});

    m_changed_connection = QObject::connect(m_checkbox, &QCheckBox::stateChanged, [this](int)
	{
        refresh_value();
	});

    refresh_read_only_state();
}

Optional_Value_Editor::~Optional_Value_Editor()
{
    QObject::disconnect(m_changed_connection);
    QObject::disconnect(m_toggled_connection);
}

QWidget* Optional_Value_Editor::get_widget()
{
	return m_editor;
}

Qualified_Value<ts::IValue> Optional_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Optional_Value_Editor::refresh_editor()
{
    bool is_set = m_qualified_value.get_const_value()->is_set();
    m_checkbox->setChecked(is_set);
	if (m_inner_editor)
	{
        m_inner_editor->get_widget()->setVisible(is_set);
        m_inner_editor->refresh_editor();
	}
}

void Optional_Value_Editor::refresh_value()
{
    set_value(m_checkbox->isChecked());

	if (m_inner_editor)
	{
        m_inner_editor->refresh_value();
	}
}

void Optional_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
	if (m_inner_editor)
	{
        m_inner_editor->set_read_only_override(read_only);
	}
}

bool Optional_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Optional_Value_Editor::refresh_read_only_state()
{
    m_checkbox->setDisabled(is_read_only());
}

void Optional_Value_Editor::set_value(bool value)
{
    if (!is_read_only())
	{
        if (std::shared_ptr<ts::IOptional_Value> mutable_value = m_qualified_value.get_mutable_value())
		{
//            tts::ExecutionResult<bool> result = mutable_value->MarkAsSet(value);
//			QASSERT(result);

//			create_inner_editor();
		}
	}
}

void Optional_Value_Editor::create_inner_editor()
{
	if (m_inner_editor)
	{
		return;
	}
//    jtl::lent_ref<const ts::IOptional_Value> value = m_qualified_value.get_const_value();
//	if (value->IsSet())
//	{
//        if (m_qualified_value.get_mutable_value())
//		{
//            m_inner_editor = m_value_editor_factory->CreateEditor(jtl::promote_to_ref(m_qualified_value.get_mutable_value()->get_value()));
//		}
//		else
//		{
//            m_inner_editor = m_value_editor_factory->CreateEditor(jtl::promote_to_ref(m_qualified_value.get_const_value()->get_value()));
//		}

//		if (m_inner_editor)
//		{
//            m_editor->layout()->addWidget(m_inner_editor->get_widget());
//            m_inner_editor->get_widget()->setVisible(value->IsSet());
//		}
//        refresh_read_only_state();
//	}
}

