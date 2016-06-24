#include "value_editors/Vec3i_Value_Editor.h"
#include "value_editors/details/VecX_Value_Editor_Helper.h"

Vec3i_Value_Editor::Vec3i_Value_Editor(const Qualified_Value<ts::IVec3i_Value>& qualified_value)
    : m_qualified_value(qualified_value)
{
    std::shared_ptr<const ts::IVec3i_Type> type = m_qualified_value.get_const_value()->get_specialized_type();
    ts::vec3i value = m_qualified_value.get_const_value()->get_value();
    ts::vec3i minValue = type->get_min_value();
    ts::vec3i maxValue = type->get_max_value();
    ts::vec3i step = ts::vec3i(1);//type->GetStep();

    std::vector<VecX_Value_Editor_Helper::Component> components;
    VecX_Value_Editor_Helper::Component c;
	c.name = "x";
	c.value = value.x;
	c.minValue = minValue.x;
	c.maxValue = maxValue.x;
	c.step = step.x;
	components.push_back(c);
	c.name = "y";
	c.value = value.y;
	c.minValue = minValue.y;
	c.maxValue = maxValue.y;
	c.step = step.y;
	components.push_back(c);
	c.name = "z";
	c.value = value.z;
	c.minValue = minValue.z;
	c.maxValue = maxValue.z;
	c.step = step.z;
	components.push_back(c);

    m_helper = std::make_shared<VecX_Value_Editor_Helper>(components, 0);

    m_connection = m_helper->sig_value_changed.connect([this](const std::vector<double>& values)
	{
        QASSERT(values.size() == 3);
        ts::vec3i value(static_cast<int64_t>(values[0]), static_cast<int64_t>(values[1]), static_cast<int64_t>(values[2]));
        set_value(value);
	});

    refresh_read_only_state();
}

QWidget* Vec3i_Value_Editor::get_widget()
{
    return m_helper->get_widget();
}

Qualified_Value<ts::IValue> Vec3i_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Vec3i_Value_Editor::refresh_editor()
{
    ts::vec3i value = m_qualified_value.get_const_value()->get_value();
    m_helper->set_values({ static_cast<double>(value.x), static_cast<double>(value.y), static_cast<double>(value.z) });
}
void Vec3i_Value_Editor::refresh_value()
{
    std::vector<double> values = m_helper->get_values();
    QASSERT(values.size() == 3);
    ts::vec3i value(static_cast<int64_t>(values[0]), static_cast<int64_t>(values[1]), static_cast<int64_t>(values[2]));
    set_value(value);
}

void Vec3i_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
}

bool Vec3i_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Vec3i_Value_Editor::refresh_read_only_state()
{
    m_helper->set_read_only(is_read_only());
}

void Vec3i_Value_Editor::set_value(const ts::vec3i& value)
{
    if (!is_read_only())
	{
        if (std::shared_ptr<ts::IVec3i_Value> mutable_value = m_qualified_value.get_mutable_value())
		{
            auto result = mutable_value->set_value(value);
            QASSERT(result == ts::success);
        }
	}
}


