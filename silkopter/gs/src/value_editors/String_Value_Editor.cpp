#include "value_editors/String_Value_Editor.h"

#include <QLineEdit>

String_Value_Editor::String_Value_Editor(const Qualified_Value<ts::IString_Value>& qualified_value)
    : m_qualified_value(qualified_value)
{
	m_editor = new QLineEdit();
    m_editor->setPlaceholderText(m_qualified_value.get_const_value()->get_value().c_str());
	m_editor->setAutoFillBackground(true);

	m_editingFinishedConnection = QObject::connect(m_editor, &QLineEdit::editingFinished, [this]()
	{
        refresh_value();
	});

    refresh_read_only_state();
}

String_Value_Editor::~String_Value_Editor()
{
	QObject::disconnect(m_editingFinishedConnection);
}

QWidget* String_Value_Editor::get_widget()
{
	return m_editor;
}

Qualified_Value<ts::IValue> String_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void String_Value_Editor::refresh_editor()
{
    m_editor->setText(m_qualified_value.get_const_value()->get_value().c_str());
}

void String_Value_Editor::refresh_value()
{
    set_value(m_editor->text().toLatin1().data());
}

void String_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
}

bool String_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void String_Value_Editor::refresh_read_only_state()
{
    m_editor->setDisabled(is_read_only());
}

void String_Value_Editor::set_value(std::string const& value)
{
    if (!is_read_only())
	{
        if (std::shared_ptr<ts::IString_Value> mutable_value = m_qualified_value.get_mutable_value())
		{
            mutable_value->set_value(value);
		}
	}
}
