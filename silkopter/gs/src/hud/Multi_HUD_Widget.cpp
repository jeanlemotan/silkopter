#include "hud/Multi_HUD_Widget.h"

Multi_HUD_Widget::Multi_HUD_Widget(silk::HAL& hal, silk::Comms& comms, qinput::Input_Mgr& input_mgr, Render_Context& context, QWidget* parent)
    : m_hal(hal)
    , m_comms(comms)
    , m_input_mgr(input_mgr)
    , m_context(context)
{
    setParent(parent);
    setStyleSheet("background-color:black;");
}

void Multi_HUD_Widget::acquire_gamepad()
{
    if (!m_gamepad)
    {
        auto gamepads = m_input_mgr.get_all_gamepads();
        if (!gamepads.empty())
        {
            m_gamepad = gamepads.front();
        }
    }
}

void Multi_HUD_Widget::process_gamepad()
{
    acquire_gamepad();
    if (!m_gamepad)
    {
        return;
    }

    process_mode();

    m_comms.get_input_channel().pack_all(silk::comms::Input_Message::MULTI_INPUT, m_comms.get_new_req_id(), m_multi_input);
}

void Multi_HUD_Widget::process_vertical_thrust_rate()
{

}

void Multi_HUD_Widget::process_vertical_thrust_offset()
{

}

void Multi_HUD_Widget::process_vertical_climb_rate()
{

}

void Multi_HUD_Widget::process_vertical()
{
    switch (m_multi_state.last_input.vertical.mode.value)
    {
    case silk::node::stream::IMulti_Input::Vertical::THRUST_RATE:
        process_vertical_thrust_rate();
        break;
    case silk::node::stream::IMulti_Input::Vertical::THRUST_OFFSET:
        process_vertical_thrust_offset();
        break;
    case silk::node::stream::IMulti_Input::Vertical::CLIMB_RATE:
        process_vertical_climb_rate();
        break;
    }
}

void Multi_HUD_Widget::process_horizontal_angle_rate()
{

}

void Multi_HUD_Widget::process_horizontal_angle()
{

}

void Multi_HUD_Widget::process_horizontal_velocity()
{

}

void Multi_HUD_Widget::process_horizontal()
{
    switch (m_multi_state.last_input.horizontal.mode.value)
    {
    case silk::node::stream::IMulti_Input::Horizontal::ANGLE_RATE:
        process_horizontal_angle_rate();
        break;
    case silk::node::stream::IMulti_Input::Horizontal::ANGLE:
        process_horizontal_angle();
        break;
    case silk::node::stream::IMulti_Input::Horizontal::VELOCITY:
        process_horizontal_velocity();
        break;
    }
}

void Multi_HUD_Widget::process_yaw_angle_rate()
{
}

void Multi_HUD_Widget::process_yaw()
{
    switch (m_multi_state.last_input.yaw.mode.value)
    {
    case silk::node::stream::IMulti_Input::Yaw::ANGLE_RATE:
        process_yaw_angle_rate();
        break;
    }
}

void Multi_HUD_Widget::process_mode_idle()
{
    if (m_gamepad->is_button_released(qinput::Gamepad::Button::HOME))
    {
        set_input_value(m_multi_input.mode, silk::node::stream::IMulti_Input::Mode::ARMED);
    }
}

void Multi_HUD_Widget::process_mode_armed()
{
    if (m_gamepad->is_button_released(qinput::Gamepad::Button::HOME))
    {
        set_input_value(m_multi_input.mode, silk::node::stream::IMulti_Input::Mode::IDLE);
    }

    process_vertical();
    process_horizontal();
    process_yaw();
}

void Multi_HUD_Widget::process_mode()
{
    switch (m_multi_state.last_input.mode.value)
    {
    case silk::node::stream::IMulti_Input::Mode::IDLE:
        process_mode_idle();
        break;
    case silk::node::stream::IMulti_Input::Mode::ARMED:
        process_mode_armed();
        break;
    }
}

void Multi_HUD_Widget::process()
{
    m_input_version++;

    process_gamepad();
}


