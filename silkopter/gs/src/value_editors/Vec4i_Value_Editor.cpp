#include "value_editors/Vec4i_Value_Editor.h"
#include "value_editors/details/VecX_Value_Editor_Helper.h"

Vec4i_Value_Editor::Vec4i_Value_Editor(const Qualified_Value<ts::IVec4i_Value>& qualified_value, std::string const& editor_str, std::string const& suffix_str)
    : m_qualified_value(qualified_value)
{
    std::shared_ptr<const ts::IVec4i_Type> type = m_qualified_value.get_const_value()->get_specialized_type();
    ts::vec4i value = m_qualified_value.get_const_value()->get_value();
    ts::vec4i min_value = type->get_min_value();
    ts::vec4i max_value = type->get_max_value();
    ts::vec4i step = ts::vec4i(1);//type->GetStep();

    std::vector<VecX_Value_Editor_Helper::Component> components;
    VecX_Value_Editor_Helper::Component c;
	c.name = "x";
	c.value = value.x;
    c.minValue = min_value.x;
    c.maxValue = max_value.x;
	c.step = step.x;
	components.push_back(c);
	c.name = "y";
	c.value = value.y;
    c.minValue = min_value.y;
    c.maxValue = max_value.y;
	c.step = step.y;
	components.push_back(c);
	c.name = "z";
	c.value = value.z;
    c.minValue = min_value.z;
    c.maxValue = max_value.z;
	c.step = step.z;
	components.push_back(c);
	c.name = "w";
	c.value = value.w;
    c.minValue = min_value.w;
    c.maxValue = max_value.w;
	c.step = step.w;
	components.push_back(c);

    m_helper = std::make_shared<VecX_Value_Editor_Helper>(components, 0);

    m_connection = m_helper->sig_value_changed.connect([this](const std::vector<double>& values)
	{
        QASSERT(values.size() == 4);
        ts::vec4i value(values[0], values[1], values[2], values[3]);
        set_value(value);
	});

    refresh_read_only_state();
}

QWidget* Vec4i_Value_Editor::get_widget()
{
    return m_helper->get_widget();
}

Qualified_Value<ts::IValue> Vec4i_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Vec4i_Value_Editor::refresh_editor()
{
    ts::vec4i value = m_qualified_value.get_const_value()->get_value();
    m_helper->set_values({ static_cast<double>(value.x), static_cast<double>(value.y), static_cast<double>(value.z), static_cast<double>(value.w) });
}
void Vec4i_Value_Editor::refresh_value()
{
    std::vector<double> values = m_helper->get_values();
    QASSERT(values.size() == 4);
    set_value(ts::vec4i(values[0], values[1], values[2], values[3]));
}

void Vec4i_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
}

bool Vec4i_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Vec4i_Value_Editor::refresh_read_only_state()
{
    m_helper->set_read_only(is_read_only());
}

void Vec4i_Value_Editor::set_value(const ts::vec4i& value)
{
    if (!is_read_only())
	{
        if (std::shared_ptr<ts::IVec4i_Value> mutable_value = m_qualified_value.get_mutable_value())
		{
            auto result = mutable_value->set_value(value);
            QASSERT(result == ts::success);
        }
	}
}


