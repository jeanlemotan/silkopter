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



void Multi_HUD_Widget::process_vertical_thrust_rate()
{
    float v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;
    m_multi_state.last_input.vertical.thrust_rate.set(v);
}

void Multi_HUD_Widget::process_vertical_thrust_offset()
{
    float v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;
    m_multi_state.last_input.vertical.thrust_offset.set(v);
}

void Multi_HUD_Widget::process_vertical_climb_rate()
{
    float v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value.y;
    v *= 5.f;
    m_multi_state.last_input.vertical.climb_rate.set(v);
}

void Multi_HUD_Widget::process_vertical()
{
    switch (m_multi_state.last_input.vertical.mode.value)
    {
    case silk::node::stream::IMulti_Input::Vertical::Mode::THRUST_RATE:
        process_vertical_thrust_rate();
        break;
    case silk::node::stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET:
        process_vertical_thrust_offset();
        break;
    case silk::node::stream::IMulti_Input::Vertical::Mode::CLIMB_RATE:
        process_vertical_climb_rate();
        break;
    }

    if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_UP))
    {
        int v = math::clamp(static_cast<int>(m_multi_state.last_input.vertical.mode.get()) - 1,
                            static_cast<int>(silk::node::stream::IMulti_Input::Vertical::Mode::THRUST_RATE),
                            static_cast<int>(silk::node::stream::IMulti_Input::Vertical::Mode::CLIMB_RATE));

        m_multi_state.last_input.vertical.mode.set(static_cast<silk::node::stream::IMulti_Input::Vertical::Mode>(v));
    }
    else if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_DOWN))
    {
        int v = math::clamp(static_cast<int>(m_multi_state.last_input.vertical.mode.get()) + 1,
                            static_cast<int>(silk::node::stream::IMulti_Input::Vertical::Mode::THRUST_RATE),
                            static_cast<int>(silk::node::stream::IMulti_Input::Vertical::Mode::CLIMB_RATE));

        m_multi_state.last_input.vertical.mode.set(static_cast<silk::node::stream::IMulti_Input::Vertical::Mode>(v));
    }

}

void Multi_HUD_Widget::process_horizontal_angle_rate()
{
    math::vec2f v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;
    m_multi_state.last_input.horizontal.angle_rate.set(v);
}

void Multi_HUD_Widget::process_horizontal_angle()
{
    math::vec2f v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;
    v *= math::vec2f(math::anglef::pi / 4.f);
    m_multi_state.last_input.horizontal.angle_rate.set(v);
}

void Multi_HUD_Widget::process_horizontal_velocity()
{
    math::vec2f v = m_gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;
    v *= 5.f;
    m_multi_state.last_input.horizontal.angle_rate.set(v);
}

void Multi_HUD_Widget::process_horizontal()
{
    switch (m_multi_state.last_input.horizontal.mode.value)
    {
    case silk::node::stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE:
        process_horizontal_angle_rate();
        break;
    case silk::node::stream::IMulti_Input::Horizontal::Mode::ANGLE:
        process_horizontal_angle();
        break;
    case silk::node::stream::IMulti_Input::Horizontal::Mode::VELOCITY:
        process_horizontal_velocity();
        break;
    }

    if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_LEFT))
    {
        int v = math::clamp(static_cast<int>(m_multi_state.last_input.horizontal.mode.get()) - 1,
                            static_cast<int>(silk::node::stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE),
                            static_cast<int>(silk::node::stream::IMulti_Input::Horizontal::Mode::VELOCITY));

        m_multi_state.last_input.horizontal.mode.set(static_cast<silk::node::stream::IMulti_Input::Horizontal::Mode>(v));
    }
    else if (m_gamepad->is_button_released(qinput::Gamepad::Button::LPAD_RIGHT))
    {
        int v = math::clamp(static_cast<int>(m_multi_state.last_input.horizontal.mode.get()) + 1,
                            static_cast<int>(silk::node::stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE),
                            static_cast<int>(silk::node::stream::IMulti_Input::Horizontal::Mode::VELOCITY));

        m_multi_state.last_input.horizontal.mode.set(static_cast<silk::node::stream::IMulti_Input::Horizontal::Mode>(v));
    }
}

void Multi_HUD_Widget::process_yaw_angle_rate()
{
}

void Multi_HUD_Widget::process_yaw()
{
    switch (m_multi_state.last_input.yaw.mode.value)
    {
    case silk::node::stream::IMulti_Input::Yaw::Mode::ANGLE_RATE:
        process_yaw_angle_rate();
        break;
    }
}

void Multi_HUD_Widget::process_mode_idle()
{
    if (m_gamepad->is_button_held(qinput::Gamepad::Button::HOME) &&
        m_gamepad->is_button_released(qinput::Gamepad::Button::PS_X))
    {
        m_multi_input.mode.set(silk::node::stream::IMulti_Input::Mode::ARMED);
        QLOGI("Trying to ARM");
    }
}

void Multi_HUD_Widget::process_mode_armed()
{
    if (m_gamepad->is_button_held(qinput::Gamepad::Button::HOME) &&
        m_gamepad->is_button_released(qinput::Gamepad::Button::PS_X))
    {
        m_multi_input.mode.set(silk::node::stream::IMulti_Input::Mode::IDLE);
        QLOGI("Trying to DISARM");
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

    m_comms.send_multi_input_value(m_multi_input);
}

#define SYNC(x) \
{\
    auto& remote = m_multi_state.last_input.x;\
    auto& local = m_multi_input.x;\
    if (remote.version >= local.version)\
    {\
        if (local.value == remote.value)\
        {\
            QLOGI("Confirmed input {}: value {}@{}", #x, remote.get(), static_cast<uint32_t>(remote.version));\
        }\
        else\
        {\
            QLOGI("Overriden input {}: value {}@{}, req {}@{}", #x, remote.get(), static_cast<uint32_t>(remote.version), local.get(), static_cast<uint32_t>(local.version));\
        }\
        local.value = remote.value;\
        local.version = remote.version;\
    }\
}

void Multi_HUD_Widget::sync_input()
{
    SYNC(toggles.land);
    SYNC(toggles.take_off);
    SYNC(toggles.return_home);

    SYNC(vertical.mode);
    SYNC(vertical.thrust_rate);
    SYNC(vertical.thrust_offset);
    SYNC(vertical.climb_rate);

    SYNC(horizontal.mode);
    SYNC(horizontal.angle_rate);
    SYNC(horizontal.angle);
    SYNC(horizontal.velocity);

    SYNC(yaw.mode);
    SYNC(yaw.angle_rate);

    SYNC(mode);
    SYNC(reference_frame);

    SYNC(assists.stay_in_range);
    SYNC(assists.stay_in_battery_range);
    SYNC(assists.stay_in_perimeter);
    SYNC(assists.avoid_altitude_drop);
    SYNC(assists.avoid_the_user);
    SYNC(assists.avoid_proximity);
}
#undef SYNC


void Multi_HUD_Widget::process()
{
    QLOG_TOPIC("input");

    auto const& samples = m_comms.get_multi_state_samples();
    if (!samples.empty())
    {
        m_multi_state = samples.back().value;
    }

    process_gamepad();

    sync_input();
}


