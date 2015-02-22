#pragma once

#include "ui_Setup_Motors_Widget.h"
#include "common/Comm_Data.h"
#include "Comms.h"

class Setup_Motors_Widget : public QWidget
{
public:
    Setup_Motors_Widget(qinput::Input_Mgr& input, silk::Comms& comms, QWidget* parent);

private:
    void process();

    Ui::Setup_Motors_Widget m_ui;
    qinput::Input_Mgr& m_input_mgr;
    silk::Comms& m_comms;
    qinput::Gamepad_cptr m_gamepad;

    boost::optional<silk::comms::Operation_Mode> m_operation_mode;

    silk::comms::Motor_Test_Input m_input;
};


