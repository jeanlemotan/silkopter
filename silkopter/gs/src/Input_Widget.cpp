#include "stdafx.h"
#include "Input_Widget.h"

Input_Widget::Input_Widget(QWidget *parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);

    QObject::connect(m_ui.throttle_mode, &QComboBox::currentTextChanged, [this](const QString& v) { set_throttle_mode(v); });
    QObject::connect(m_ui.pitch_roll_mode, &QComboBox::currentTextChanged, [this](const QString& v) { set_pitch_roll_mode(v); });
    QObject::connect(m_ui.reference_frame, &QComboBox::currentTextChanged, [this](const QString& v) { set_reference_frame(v); });

    QObject::connect(m_ui.arm, &QPushButton::released, [this]() { m_new_uav_input.arm = true; });
    QObject::connect(m_ui.disarm, &QPushButton::released, [this]() { m_new_uav_input.disarm = true; });
    QObject::connect(m_ui.land, &QPushButton::released, [this]() { m_new_uav_input.land = true; });
    QObject::connect(m_ui.take_off, &QPushButton::released, [this]() { m_new_uav_input.take_off = true; });
}

void Input_Widget::init(qinput::Input_Mgr *input)
{
    QASSERT(input);
    m_input_mgr = input;
}

void Input_Widget::set_throttle_mode(QString const& v)
{
    if (v == "Rate")
    {
        m_new_uav_input.throttle_mode = silk::uav_input::Throttle_Mode::RATE;
    }
    if (v == "Offset")
    {
        m_new_uav_input.throttle_mode = silk::uav_input::Throttle_Mode::OFFSET;
    }
    if (v == "Assisted")
    {
        m_new_uav_input.throttle_mode = silk::uav_input::Throttle_Mode::ASSISTED;
    }
}

void Input_Widget::set_pitch_roll_mode(QString const& v)
{
    if (v == "Rate")
    {
        m_new_uav_input.pitch_roll_mode = silk::uav_input::Pitch_Roll_Mode::RATE;
    }
    if (v == "Horizontal")
    {
        m_new_uav_input.pitch_roll_mode = silk::uav_input::Pitch_Roll_Mode::HORIZONTAL;
    }
    if (v == "Assisted")
    {
        m_new_uav_input.pitch_roll_mode = silk::uav_input::Pitch_Roll_Mode::ASSISTED;
    }
}

void Input_Widget::set_reference_frame(QString const& v)
{

}

float Input_Widget::filter_stick_value(float v)
{
    float av = math::abs(v);
    float sv = math::sgn(v);

    constexpr float k_min = 0.15f;

    av = math::max(av - k_min, 0.f) / (1 - k_min);
    return sv * av;
}

void Input_Widget::process_uav_input(q::Duration dt, silk::Comms& comms, qinput::Gamepad const& gamepad)
{
    using namespace qinput;

    auto dts = q::Seconds(dt).count();

    //sticks
    {
        auto ls = gamepad.get_stick_data(Gamepad::Stick::LEFT);
        auto rs = gamepad.get_stick_data(Gamepad::Stick::RIGHT);

        float throttle = m_base_throttle.value;

        if (m_base_throttle.wait_for_zero)
        {
            m_base_throttle.wait_for_zero = !math::is_zero(ls.value.y, 0.05f);
        }
        else
        {
            throttle = math::clamp(throttle + ls.value.y, 0.f, 1.f);
        }

        float yaw = -filter_stick_value(ls.value.x);
        float pitch = filter_stick_value(rs.value.y);
        float roll = filter_stick_value(rs.value.x);

        m_ui.throttle_value->setValue(static_cast<int>(throttle * 100));
        m_ui.yaw_value->setValue(static_cast<int>(yaw * 100));
        m_ui.pitch_value->setValue(static_cast<int>(pitch * 100));
        m_ui.roll_value->setValue(static_cast<int>(roll * 100));

        m_new_uav_input.sticks.throttle = throttle;
        m_new_uav_input.sticks.yaw = yaw;
        m_new_uav_input.sticks.pitch = pitch;
        m_new_uav_input.sticks.roll = roll;

        if (gamepad.is_button_released(Gamepad::Button::LEFT_BUMPER))
        {
            m_base_throttle.value = throttle;
            m_base_throttle.wait_for_zero = true;
        }
    }

    if (gamepad.is_button_released(Gamepad::Button::OUYA_O))
    {
        m_new_uav_input.arm = true;
    }
    if (gamepad.is_button_released(Gamepad::Button::OUYA_A))
    {
        m_new_uav_input.disarm = true;
    }

    //camera rotation
    {
        auto lt = gamepad.get_axis_data(Gamepad::Axis::LEFT_TRIGGER);
        auto rt = gamepad.get_axis_data(Gamepad::Axis::RIGHT_TRIGGER);

        if (m_gamepad->is_button_pressed(Gamepad::Button::LPAD_UP))
        {
            m_new_cm_input.base_rotation.x += math::radians(90.f) * dts;
        }
        if (m_gamepad->is_button_pressed(Gamepad::Button::LPAD_DOWN))
        {
            m_new_cm_input.base_rotation.x -= math::radians(90.f) * dts;
        }

        m_new_cm_input.delta_rotation.x = (lt.value - rt.value) * math::anglef::pi;

        m_new_cm_input.final_rotation = math::clamp(m_new_cm_input.base_rotation + m_new_cm_input.delta_rotation, math::vec3f(-math::anglef::pi), math::vec3f(math::anglef::pi));
    }


    // send
    if (m_cm_input.final_rotation != m_new_cm_input.final_rotation)
    {
        comms.send_uav_input(silk::uav_input::Input::CAMERA_MOUNT_ROTATION, m_new_cm_input.final_rotation);
    }

    if (m_uav_input.throttle_mode != m_new_uav_input.throttle_mode)
    {
        comms.send_uav_input(silk::uav_input::Input::THROTTLE_MODE, m_new_uav_input.throttle_mode);
    }
    if (m_uav_input.pitch_roll_mode != m_new_uav_input.pitch_roll_mode)
    {
        comms.send_uav_input(silk::uav_input::Input::PITCH_ROLL_MODE, m_new_uav_input.pitch_roll_mode);
    }
    if (m_uav_input.sticks.throttle != m_new_uav_input.sticks.throttle ||
        m_uav_input.sticks.yaw != m_new_uav_input.sticks.yaw ||
        m_uav_input.sticks.pitch != m_new_uav_input.sticks.pitch ||
        m_uav_input.sticks.roll != m_new_uav_input.sticks.roll)
    {
        comms.send_uav_input(silk::uav_input::Input::STICKS, m_new_uav_input.sticks);
    }

    //store the change
    m_uav_input = m_new_uav_input;
    m_cm_input = m_new_cm_input;

    //toggles
    if (m_new_uav_input.arm)
    {
        m_new_uav_input.arm = false;
        comms.send_uav_input(silk::uav_input::Input::ARM);
    }
    if (m_new_uav_input.disarm)
    {
        m_new_uav_input.disarm = false;
        comms.send_uav_input(silk::uav_input::Input::DISARM);
    }
    if (m_new_uav_input.land)
    {
        m_new_uav_input.land = false;
        comms.send_uav_input(silk::uav_input::Input::LAND);
    }
    if (m_new_uav_input.take_off)
    {
        m_new_uav_input.take_off = false;
        comms.send_uav_input(silk::uav_input::Input::TAKE_OFF);
    }
}

void Input_Widget::process_camera_input(q::Duration dt, silk::Comms& comms, qinput::Gamepad const& gamepad)
{
    m_camera_input = m_new_camera_input;
}


void Input_Widget::process(q::Duration dt, silk::Comms& comms)
{
    if (!m_gamepad)
    {
        auto gamepads = m_input_mgr->get_all_gamepads();
        if (!gamepads.empty())
        {
            m_gamepad = gamepads[0];
        }
    }

    if (m_gamepad)
    {
        process_uav_input(dt, comms, *m_gamepad);
        process_camera_input(dt, comms, *m_gamepad);
    }
}


