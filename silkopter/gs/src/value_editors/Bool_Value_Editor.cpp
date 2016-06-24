#include "value_editors/Bool_Value_Editor.h"

#include <QCheckBox>

Bool_Value_Editor::Bool_Value_Editor(const Qualified_Value<ts::IBool_Value>& qualified_value)
    : m_qualified_value(qualified_value)
{
	m_editor = new QCheckBox();
    m_editor->setChecked(m_qualified_value.get_const_value()->get_value());
    m_editor->setText(m_qualified_value.get_const_value()->get_value() ? "true" : "false");
	m_editor->setAutoFillBackground(true);

    m_toggled_connection = QObject::connect(m_editor, &QCheckBox::toggled, [=](bool v)
	{
		m_editor->setText(v ? "true" : "false");
	});

    m_changed_connection = QObject::connect(m_editor, &QCheckBox::stateChanged, [this](int)
	{
        refresh_value();
        sig_editing_finished.execute();
	});

    refresh_read_only_state();
}

Bool_Value_Editor::~Bool_Value_Editor()
{
    QObject::disconnect(m_changed_connection);
    QObject::disconnect(m_toggled_connection);
}

QWidget* Bool_Value_Editor::get_widget()
{
	return m_editor;
}

Qualified_Value<ts::IValue> Bool_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Bool_Value_Editor::refresh_editor()
{
    m_editor->setChecked(m_qualified_value.get_const_value()->get_value());
}

void Bool_Value_Editor::refresh_value()
{
    set_value(m_editor->isChecked());
}

void Bool_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
}

bool Bool_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Bool_Value_Editor::refresh_read_only_state()
{
    m_editor->setDisabled(is_read_only());
}

void Bool_Value_Editor::set_value(bool value)
{
    if (!is_read_only())
	{
        if (std::shared_ptr<ts::IBool_Value> mutable_value = m_qualified_value.get_mutable_value())
		{
            auto result = mutable_value->set_value(value);
            QASSERT(result == ts::success);
		}
	}
}

