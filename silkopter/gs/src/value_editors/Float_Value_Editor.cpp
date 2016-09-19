#include "value_editors/Float_Value_Editor.h"
#include "value_editors/details/Numeric_Value_Editor_Helper.h"


Float_Value_Editor::Float_Value_Editor(const Qualified_Value<ts::IFloat_Value>& qualified_value, std::string const& editor_str, std::string const& suffix_str)
    : m_qualified_value(qualified_value)
{
    std::shared_ptr<const ts::IFloat_Type> type = m_qualified_value.get_const_value()->get_specialized_type();

    m_helper = std::make_shared<Numeric_Value_Editor_Helper>(m_qualified_value.get_const_value()->get_value(),
                                                             type->get_min_value(), type->get_max_value(),
                                                             1.f,
                                                             type->get_decimals());

    m_connection = m_helper->sig_value_changed.connect([this](double value)
	{
        set_value(static_cast<float>(value));
	});

    refresh_read_only_state();
}

QWidget* Float_Value_Editor::get_widget()
{
    return m_helper->get_widget();
}

Qualified_Value<ts::IValue> Float_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Float_Value_Editor::refresh_editor()
{
    m_helper->set_value(m_qualified_value.get_const_value()->get_value());
}
void Float_Value_Editor::refresh_value()
{
    set_value(m_helper->get_value());
}

void Float_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
}

bool Float_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Float_Value_Editor::refresh_read_only_state()
{
    m_helper->set_read_only(is_read_only());
}

void Float_Value_Editor::set_value(float value)
{
    if (!is_read_only())
	{
        if (std::shared_ptr<ts::IFloat_Value> mutable_value = m_qualified_value.get_mutable_value())
		{
            auto result = mutable_value->set_value(value);
            QASSERT(result == ts::success);
		}
	}
}

