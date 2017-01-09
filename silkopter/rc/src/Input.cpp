#include "Input.h"

#include "Sticks_ADS1115.h"
#include "Stick_Actuators_Throttle_DRV883x.h"
#include "Button_PIGPIO.h"
#include "Rotary_Encoder_PIGPIO.h"
#include "Button_Matrix_PIGPIO.h"

#include "settings.def.h"

namespace silk
{

extern settings::Settings s_settings;

Input::Input()
{

}

Input::~Input()
{
    m_input_devices.clear();
}

void Input::init()
{
    Sticks_ADS1115* sticks = new Sticks_ADS1115();
    auto result = sticks->init();
    QASSERT(result == ts::success);
    m_sticks.reset(sticks);
    m_input_devices.push_back(sticks);

    Stick_Actuators_Throttle_DRV883x* actuators = new Stick_Actuators_Throttle_DRV883x(*m_sticks);
    result = actuators->init();
    QASSERT(result == ts::success);
    m_stick_actuators.reset(actuators);
    m_input_devices.push_back(actuators);

    Rotary_Encoder_PIGPIO* encoder = new Rotary_Encoder_PIGPIO();
    result = encoder->init(12, 13);
    QASSERT(result == ts::success);
    m_menu_encoder.reset(encoder);
    m_input_devices.push_back(encoder);

    encoder = new Rotary_Encoder_PIGPIO();
    result = encoder->init(24, 25);
    QASSERT(result == ts::success);
    m_param_encoder.reset(encoder);
    m_input_devices.push_back(encoder);

    Button_Matrix_PIGPIO* button_matrix = new Button_Matrix_PIGPIO();
    result = button_matrix->init({16, 19, 20, 21}, {17, 18, 22, 23});
    //result = button_matrix->init({16, 19, 20, 21}, {23, 18, 22, 23});
    QASSERT(result == ts::success);
    m_button_matrix.reset(button_matrix);
    m_input_devices.push_back(button_matrix);

    silk::settings::Settings::Input::Sticks_Calibration const& sc = s_settings.get_input().get_sticks_calibration();
    m_sticks->set_yaw_calibration(sc.get_yaw_center(), sc.get_yaw_min(), sc.get_yaw_max());
    m_sticks->set_pitch_calibration(sc.get_pitch_center(), sc.get_pitch_min(), sc.get_pitch_max());
    m_sticks->set_roll_calibration(sc.get_roll_center(), sc.get_roll_min(), sc.get_roll_max());
    m_sticks->set_throttle_calibration(sc.get_throttle_center(), sc.get_throttle_min(), sc.get_throttle_max());
}

//silk::stream::IMultirotor_Commands::Value const& Input::get_commands() const
//{
//    return m_commands;
//}

size_t Input::get_input_devices_count() const
{
    return m_input_devices.size();
}
IInput_Device const& Input::get_input_device(size_t index) const
{
    return *m_input_devices[index];
}

ISticks const& Input::get_sticks() const
{
    return *m_sticks;
}
ISticks& Input::get_sticks()
{
    return *m_sticks;
}
IStick_Actuators const& Input::get_stick_actuators() const
{
    return *m_stick_actuators;
}
IStick_Actuators& Input::get_stick_actuators()
{
    return *m_stick_actuators;
}

IRotary_Encoder const& Input::get_menu_encoder() const
{
    return *m_menu_encoder;
}
IButton const& Input::get_menu_switch() const
{
    return m_button_matrix->get_button(0, 2);
}
IRotary_Encoder const& Input::get_param_encoder() const
{
    return *m_param_encoder;
}
IButton const& Input::get_param_switch() const
{
    return m_button_matrix->get_button(1, 2);
}
IButton_Matrix const& Input::get_button_matrix() const
{
    return *m_button_matrix;
}

IButton const& Input::get_switch1_up() const
{
    return m_button_matrix->get_button(2, 0);
}
IButton const& Input::get_switch1_down() const
{
    return m_button_matrix->get_button(3, 0);
}

IButton const& Input::get_switch2_up() const
{
    return m_button_matrix->get_button(0, 0);
}
IButton const& Input::get_switch2_down() const
{
    return m_button_matrix->get_button(1, 0);
}

IButton const& Input::get_vertical_mode_switch() const
{
    return m_button_matrix->get_button(0, 3);
}
IButton const& Input::get_horizontal_mode_switch_down() const
{
    return m_button_matrix->get_button(1, 3);
}
IButton const& Input::get_horizontal_mode_switch_up() const
{
    return m_button_matrix->get_button(2, 3);
}
IButton const& Input::get_yaw_mode_switch() const
{
    return m_button_matrix->get_button(3, 3);
}

IButton const& Input::get_return_home_switch() const
{
    return m_button_matrix->get_button(1, 1);
}
IButton const& Input::get_mode_switch() const
{
    return m_button_matrix->get_button(2, 1);
}


void Input::process()
{
    for (IInput_Device* device: m_input_devices)
    {
        device->process();
    }


//    m_stick_actuators->set_target_throttle(m_sticks->get_yaw());

//    QLOGI("Y:{}, P:{}, R:{}, T:{}, R1:{}, R2:{}", m_sticks->get_yaw(), m_sticks->get_pitch(), m_sticks->get_roll(), m_sticks->get_throttle(),
//          m_param_encoder1->get_clicks(), m_param_encoder2->get_clicks());
}


}
