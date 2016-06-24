#include "value_editors/Vec3f_Value_Editor.h"
#include "value_editors/details/VecX_Value_Editor_Helper.h"

Vec3f_Value_Editor::Vec3f_Value_Editor(const Qualified_Value<ts::IVec3f_Value>& qualified_value)
    : m_qualified_value(qualified_value)
{
    std::shared_ptr<const ts::IVec3f_Type> type = m_qualified_value.get_const_value()->get_specialized_type();
    ts::vec3f value = m_qualified_value.get_const_value()->get_value();
    ts::vec3f minValue = type->get_min_value();
    ts::vec3f maxValue = type->get_max_value();
    ts::vec3f step = ts::vec3f(1.f);//type->GetStep();

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

    m_helper = std::make_shared<VecX_Value_Editor_Helper>(components, type->get_decimals());

    m_connection = m_helper->sig_value_changed.connect([this](const std::vector<double>& values)
	{
        QASSERT(values.size() == 3);
        ts::vec3f value(static_cast<float>(values[0]), static_cast<float>(values[1]), static_cast<float>(values[2]));
        set_value(value);
	});

    refresh_read_only_state();
}

QWidget* Vec3f_Value_Editor::get_widget()
{
    return m_helper->get_widget();
}

Qualified_Value<ts::IValue> Vec3f_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Vec3f_Value_Editor::refresh_editor()
{
    ts::vec3f value = m_qualified_value.get_const_value()->Get();
	m_helper->SetValues({ value.x, value.y, value.z });
}
void Vec3f_Value_Editor::refresh_value()
{
    std::vector<double> values = m_helper->get_values();
    QASSERT(values.size() == 3);
    set_value(ts::vec3f(values[0], values[1], values[2]));
}

void Vec3f_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
}

bool Vec3f_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Vec3f_Value_Editor::refresh_read_only_state()
{
    m_helper->set_read_only(is_read_only());
}

void Vec3f_Value_Editor::set_value(const math::vec3f& value)
{
    if (!is_read_only())
	{
        if (std::shared_ptr<ts::IVec3f_Value> mutable_value = m_qualified_value.get_mutable_value())
		{
            mutable_value->Set(value);
		}
	}
}

