#pragma once

#include <vector>
#include <memory>
#include "utils/hw/II2C.h"


namespace silk
{

class HAL;
class IInput_Device;
class ISticks;
class IStick_Actuators;
class IRotary_Encoder;
class IButton;
class IButton_Matrix;
class IHaptic;

class Input
{
public:
    Input(HAL& hal, util::hw::II2C& i2c);
    ~Input();

    void init();

    //silk::stream::IMultirotor_Commands::Value const& get_commands() const;

    size_t get_input_devices_count() const;
    IInput_Device const& get_input_device(size_t index) const;

    ISticks const& get_sticks() const;
    ISticks& get_sticks();

    IStick_Actuators const& get_stick_actuators() const;
    IStick_Actuators& get_stick_actuators();

    IHaptic& get_haptic();

    IRotary_Encoder const& get_menu_encoder() const;
    IButton const& get_menu_switch() const;

    IRotary_Encoder const& get_param_encoder() const;
    IButton const& get_param_switch() const;

    IButton_Matrix const& get_button_matrix() const;

    IButton const& get_camera_quality_switch() const;
    IButton const& get_camera_recording_switch() const;

    IButton const& get_switch2_up() const;
    IButton const& get_switch2_down() const;

    IButton const& get_vertical_mode_switch() const;
    IButton const& get_yaw_mode_switch() const;
    IButton const& get_horizontal_mode_switch_up() const;
    IButton const& get_horizontal_mode_switch_down() const;

    IButton const& get_return_home_switch() const;
    IButton const& get_mode_switch() const;

    void process();

private:
    HAL& m_hal;
    util::hw::II2C& m_i2c;

    std::vector<IInput_Device*> m_input_devices;

    std::unique_ptr<ISticks> m_sticks;
    std::unique_ptr<IStick_Actuators> m_stick_actuators;

    std::unique_ptr<IRotary_Encoder> m_menu_encoder;
    std::unique_ptr<IRotary_Encoder> m_param_encoder;

    std::unique_ptr<IButton_Matrix> m_button_matrix;
};


}
