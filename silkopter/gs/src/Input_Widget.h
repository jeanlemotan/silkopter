#pragma once

#include "ui_Input_Widget.h"
#include "common/input/Camera_Input.h"
#include "common/input/UAV_Input.h"
#include "Comms.h"

class Input_Widget : public QWidget
{
public:
    Input_Widget(QWidget *parent = 0);

public:
    void init(qinput::Input_Mgr* input);
    void process(silk::Comms& comms);

private:
    Ui::Input_Widget m_ui;
    qinput::Input_Mgr* m_input_mgr;
    qinput::Gamepad_cptr m_gamepad;

    void set_throttle_mode(QString const& v);
    void set_pitch_roll_mode(QString const& v);
    void set_reference_frame(QString const& v);

    struct UAV_Input
    {
        silk::uav_input::Throttle_Mode throttle_mode = silk::uav_input::Throttle_Mode::STABLE;
        silk::uav_input::Pitch_Roll_Mode pitch_roll_mode = silk::uav_input::Pitch_Roll_Mode::STABLE;
        silk::uav_input::Assists assists;
        silk::uav_input::Sticks sticks;

        bool arm = false;
        bool disarm = false;
        bool land = false;
        bool take_off = false;
    } m_uav_input, m_new_uav_input;

    float m_throttle = 0.f;
};


