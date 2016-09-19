#include "value_editors/Empty_Value_Editor.h"


Empty_Value_Editor::Empty_Value_Editor(const Qualified_Value<ts::IValue>& qualified_value, std::string const& suffix_str)
    : m_qualified_value(qualified_value)
{
	m_editor = new QWidget();
	m_editor->setAutoFillBackground(false);

    refresh_read_only_state();
}

QWidget* Empty_Value_Editor::get_widget()
{
	return m_editor;
}

Qualified_Value<ts::IValue> Empty_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Empty_Value_Editor::refresh_editor()
{
}

void Empty_Value_Editor::refresh_value()
{
}

void Empty_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
}

bool Empty_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Empty_Value_Editor::refresh_read_only_state()
{
    m_editor->setDisabled(is_read_only());
}

