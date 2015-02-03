#include "stdafx.h"
#include "Input_Widget.h"

Input_Widget::Input_Widget(QWidget *parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);

    QObject::connect(m_ui.throttle_mode, &QComboBox::currentTextChanged, [this](const QString& v) { set_throttle_mode(v); });
    QObject::connect(m_ui.pitch_roll_mode, &QComboBox::currentTextChanged, [this](const QString& v) { set_pitch_roll_mode(v); });
    QObject::connect(m_ui.reference_frame, &QComboBox::currentTextChanged, [this](const QString& v) { set_reference_frame(v); });

//    QObject::connect(m_ui.land, &QPushButton::toggled, [this](bool v) { m_uav_input.input.toggles.land = v; });
//    QObject::connect(m_ui.take_off, &QPushButton::toggled, [this](bool v) { m_uav_input.input.toggles.take_off = v; });
}

void Input_Widget::init(qinput::Input_Mgr* input, silk::Comms& comms)
{
    QASSERT(input);
    m_input_mgr = input;

    auto original_ss = m_ui.arm->styleSheet();

    QObject::connect(m_ui.arm, &QPushButton::released, [this, &comms]()
    {
        bool armed = m_uav_input.operation_mode ? ((*m_uav_input.operation_mode) == silk::comms::Operation_Mode::ARMED) : false;
        comms.set_operation_mode(armed ? silk::comms::Operation_Mode::IDLE : silk::comms::Operation_Mode::ARMED);
        comms.request_operation_mode();
    });

    comms.operation_mode_received.connect([this, &comms, original_ss](silk::comms::Operation_Mode m)
    {
        m_uav_input.operation_mode = m;
        m_ui.arm->setStyleSheet(m == silk::comms::Operation_Mode::ARMED ? "background-color: red" : original_ss);
        m_ui.arm->setText(m == silk::comms::Operation_Mode::ARMED ? "Disarm" : "Arm");
    });
}

void Input_Widget::set_throttle_mode(QString const& v)
{
    if (v == "Rate")
    {
        m_uav_input.input.throttle_mode = silk::comms::UAV_Input::Throttle_Mode::RATE;
    }
    if (v == "Offset")
    {
        m_uav_input.input.throttle_mode = silk::comms::UAV_Input::Throttle_Mode::OFFSET;
    }
    if (v == "Assisted")
    {
        m_uav_input.input.throttle_mode = silk::comms::UAV_Input::Throttle_Mode::ASSISTED;
    }
}

void Input_Widget::set_pitch_roll_mode(QString const& v)
{
    if (v == "Rate")
    {
        m_uav_input.input.pitch_roll_mode = silk::comms::UAV_Input::Pitch_Roll_Mode::RATE;
    }
    if (v == "Horizontal")
    {
        m_uav_input.input.pitch_roll_mode = silk::comms::UAV_Input::Pitch_Roll_Mode::HORIZONTAL;
    }
    if (v == "Assisted")
    {
        m_uav_input.input.pitch_roll_mode = silk::comms::UAV_Input::Pitch_Roll_Mode::ASSISTED;
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

    //sticks
    {
        auto ls = gamepad.get_stick_data(Gamepad::Stick::LEFT);
        auto rs = gamepad.get_stick_data(Gamepad::Stick::RIGHT);

        float throttle = m_uav_input.base_throttle.value;

        if (m_uav_input.base_throttle.wait_for_zero)
        {
            m_uav_input.base_throttle.wait_for_zero = !math::is_zero(ls.value.y, 0.05f);
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

        m_uav_input.input.sticks.throttle = throttle;
        m_uav_input.input.sticks.yaw = yaw;
        m_uav_input.input.sticks.pitch = pitch;
        m_uav_input.input.sticks.roll = roll;

        if (gamepad.is_button_released(Gamepad::Button::LEFT_BUMPER))
        {
            m_uav_input.base_throttle.value = throttle;
            m_uav_input.base_throttle.wait_for_zero = true;
        }
    }

    if (gamepad.is_button_released(Gamepad::Button::OUYA_O))
    {
        comms.set_operation_mode(silk::comms::Operation_Mode::ARMED);
        comms.request_operation_mode();
    }
    if (gamepad.is_button_released(Gamepad::Button::OUYA_A))
    {
        comms.set_operation_mode(silk::comms::Operation_Mode::IDLE);
        comms.request_operation_mode();
    }


    // send
    comms.send_uav_input(m_uav_input.input);
}

void Input_Widget::process_camera_mount_input(q::Duration dt, silk::Comms& comms, qinput::Gamepad const& gamepad)
{
    using namespace qinput;

    auto dts = q::Seconds(dt).count();

    //camera rotation
    {
        auto lt = gamepad.get_axis_data(Gamepad::Axis::LEFT_TRIGGER);
        auto rt = gamepad.get_axis_data(Gamepad::Axis::RIGHT_TRIGGER);

        if (m_gamepad->is_button_pressed(Gamepad::Button::LPAD_UP))
        {
            m_camera_mount_input.base_rotation.x += math::radians(90.f) * dts;
        }
        if (m_gamepad->is_button_pressed(Gamepad::Button::LPAD_DOWN))
        {
            m_camera_mount_input.base_rotation.x -= math::radians(90.f) * dts;
        }

        m_camera_mount_input.delta_rotation.x = (lt.value - rt.value) * math::anglef::pi;

        m_camera_mount_input.input.rotation =
                math::clamp(m_camera_mount_input.base_rotation + m_camera_mount_input.delta_rotation,
                            math::vec3f(-math::anglef::pi), math::vec3f(math::anglef::pi));
    }


    // send
    comms.send_camera_mount_input(m_camera_mount_input.input);
}

void Input_Widget::process_camera_input(q::Duration dt, silk::Comms& comms, qinput::Gamepad const& gamepad)
{
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
        process_camera_mount_input(dt, comms, *m_gamepad);
//        process_camera_input(dt, comms, *m_gamepad);
    }
}


