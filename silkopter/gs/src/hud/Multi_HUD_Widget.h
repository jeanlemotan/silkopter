#pragma once

#include "Render_Widget.h"
#include "HAL.h"
#include "Comms.h"
#include "IHUD_Widget.h"

class Multi_HUD_Widget : public IHUD_Widget
{
public:
    Multi_HUD_Widget(silk::HAL& hal, silk::Comms& comms, qinput::Input_Mgr& input_mgr, Render_Context& context, QWidget* parent);

    virtual void process() override;

private:
    silk::HAL& m_hal;
    silk::Comms& m_comms;
    qinput::Input_Mgr& m_input_mgr;
    Render_Context& m_context;

    qinput::Gamepad_cptr m_gamepad;

    uint32_t m_input_version = 0;

    template<class T> void set_input_value(T& input, typename T::Value const& value)
    {
        input.value = value;
        input.version = m_input_version;
    }

    silk::node::stream::IMulti_Input::Value m_multi_input;
    silk::node::stream::IMulti_State::Value m_multi_state;

    void process_vertical_thrust_rate();
    void process_vertical_thrust_offset();
    void process_vertical_climb_rate();
    void process_vertical();

    void process_horizontal_angle_rate();
    void process_horizontal_angle();
    void process_horizontal_velocity();
    void process_horizontal();

    void process_yaw_angle_rate();
    void process_yaw();

    void process_mode_idle();
    void process_mode_armed();
    void process_mode();

    void acquire_gamepad();
    void process_gamepad();
};

