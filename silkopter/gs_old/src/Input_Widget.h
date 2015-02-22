#pragma once

#include "ui_Input_Widget.h"
#include "Comms.h"

class Input_Widget : public QWidget
{
public:
    Input_Widget(QWidget *parent = 0);

public:
    void init(qinput::Input_Mgr* input, silk::Comms& comms);
    void process(q::Duration dt, silk::Comms& comms);

private:
    Ui::Input_Widget m_ui;
    qinput::Input_Mgr* m_input_mgr;
    qinput::Gamepad_cptr m_gamepad;

    void set_throttle_mode(QString const& v);
    void set_pitch_roll_mode(QString const& v);
    void set_reference_frame(QString const& v);

    float filter_stick_value(float v);

    void process_uav_input(q::Duration dt, silk::Comms& comms, qinput::Gamepad const& gamepad);
    void process_camera_mount_input(q::Duration dt, silk::Comms& comms, qinput::Gamepad const& gamepad);
    void process_camera_input(q::Duration dt, silk::Comms& comms, qinput::Gamepad const& gamepad);


    struct UAV_Input
    {
        boost::optional<silk::comms::Operation_Mode> operation_mode;
        silk::comms::UAV_Input input;

        struct Base_Throttle
        {
            float value = 0.f;
            bool wait_for_zero = false;
        } base_throttle;
    } m_uav_input;

    struct Camera_Mount_Input
    {
        silk::comms::Camera_Mount_Input input;

        math::vec3f base_rotation;
        math::vec3f delta_rotation;
    } m_camera_mount_input;
};


