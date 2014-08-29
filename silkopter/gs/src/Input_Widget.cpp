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
    if (v == "Stable")
    {
        m_new_uav_input.throttle_mode = silk::uav_input::Throttle_Mode::STABLE;
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
    if (v == "Stable")
    {
        m_new_uav_input.pitch_roll_mode = silk::uav_input::Pitch_Roll_Mode::STABLE;
    }
    if (v == "Assisted")
    {
        m_new_uav_input.pitch_roll_mode = silk::uav_input::Pitch_Roll_Mode::ASSISTED;
    }
}

void Input_Widget::set_reference_frame(QString const& v)
{

}

void Input_Widget::process(silk::Comms& comms)
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
        auto ls = m_gamepad->get_stick_data(qinput::Stick_Id::LEFT);
        auto rs = m_gamepad->get_stick_data(qinput::Stick_Id::RIGHT);

        float throttle = math::clamp(m_throttle - ls.value.y * 0.1f, 0.f, 1.f);
        float yaw = -ls.value.x;
        float pitch = rs.value.y;
        float roll = rs.value.x;

        m_ui.throttle_value->setValue(static_cast<int>(throttle * 100));
        m_ui.yaw_value->setValue(static_cast<int>(yaw * 100));
        m_ui.pitch_value->setValue(static_cast<int>(pitch * 100));
        m_ui.roll_value->setValue(static_cast<int>(roll * 100));

        m_new_uav_input.sticks.throttle = throttle;
        m_new_uav_input.sticks.yaw = yaw;
        m_new_uav_input.sticks.pitch = pitch;
        m_new_uav_input.sticks.roll = roll;

        if (m_gamepad->is_button_released(qinput::Button_Id::PAD_UP))
        {
            m_throttle += 0.05f;
        }
        if (m_gamepad->is_button_released(qinput::Button_Id::PAD_DOWN))
        {
            m_throttle -= 0.05f;
        }
        m_throttle = math::clamp(m_throttle, 0.f, 1.f);
    }

    if (m_uav_input.throttle_mode != m_new_uav_input.throttle_mode)
    {
        m_uav_input.throttle_mode = m_new_uav_input.throttle_mode;
        comms.send_uav_input(silk::uav_input::Input::THROTTLE_MODE, m_uav_input.throttle_mode);
    }
    if (m_uav_input.pitch_roll_mode != m_new_uav_input.pitch_roll_mode)
    {
        m_uav_input.pitch_roll_mode = m_new_uav_input.pitch_roll_mode;
        comms.send_uav_input(silk::uav_input::Input::PITCH_ROLL_MODE, m_uav_input.pitch_roll_mode);
    }
    if (m_uav_input.sticks.throttle != m_new_uav_input.sticks.throttle ||
        m_uav_input.sticks.yaw != m_new_uav_input.sticks.yaw ||
        m_uav_input.sticks.pitch != m_new_uav_input.sticks.pitch ||
        m_uav_input.sticks.roll != m_new_uav_input.sticks.roll)
    {
        m_uav_input.sticks = m_new_uav_input.sticks;
        comms.send_uav_input(silk::uav_input::Input::STICKS, m_uav_input.sticks);
    }

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


