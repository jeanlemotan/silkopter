#include "stdafx.h"
#include "Status_Widget.h"

Status_Widget::Status_Widget(QWidget* parent)
: QWidget(parent)
{
    m_ui.setupUi(this);
}

void Status_Widget::process(UAV const& uav)
{
    std::string str;

    {
        auto v = math::positive_zero(math::round(uav.get_position(), 3));
        q::util::format(str, "{.3}", v.x);
        m_ui.position_x->setText(str.c_str());
        q::util::format(str, "{.3}", v.y);
        m_ui.position_y->setText(str.c_str());
        q::util::format(str, "{.3}", v.z);
        m_ui.position_z->setText(str.c_str());
    }

    {
        auto v = math::positive_zero(math::round(uav.get_velocity(), 3));
        q::util::format(str, "{.3}", v.x);
        m_ui.velocity_x->setText(str.c_str());
        q::util::format(str, "{.3}", v.y);
        m_ui.velocity_y->setText(str.c_str());
        q::util::format(str, "{.3}", v.z);
        m_ui.velocity_z->setText(str.c_str());
    }
    {
        auto s = math::length(uav.get_velocity());
        auto ms = math::positive_zero(math::round(s, 3));
        q::util::format(str, "{.3} m/s", ms);
        m_ui.speed_ms_lbl->setText(str.c_str());

        auto kmh = math::positive_zero(math::round(s * 3.6f, 3));
        q::util::format(str, "{.3} km/h", kmh);
        m_ui.speed_kmh_lbl->setText(str.c_str());
    }

    {
        math::vec3f v;
        uav.get_rotation().get_as_euler_xyz(v);
        v = math::positive_zero(math::round(math::degrees(v), 3));
        q::util::format(str, "{.3}", v.x);
        m_ui.rotation_x->setText(str.c_str());
        q::util::format(str, "{.3}", v.y);
        m_ui.rotation_y->setText(str.c_str());
        q::util::format(str, "{.3}", v.z);
        m_ui.rotation_z->setText(str.c_str());
    }
    {
        auto v = math::positive_zero(math::round(math::degrees(uav.get_angular_velocity()), 3));
        q::util::format(str, "{.3}", v.x);
        m_ui.angular_velocity_x->setText(str.c_str());
        q::util::format(str, "{.3}", v.y);
        m_ui.angular_velocity_y->setText(str.c_str());
        q::util::format(str, "{.3}", v.z);
        m_ui.angular_velocity_z->setText(str.c_str());
    }
    {
        auto v = math::positive_zero(math::round(math::degrees(math::length(uav.get_angular_velocity())), 3));
        q::util::format(str, "{.3} °/s", v);
        m_ui.angular_speed->setText(str.c_str());
    }
}




