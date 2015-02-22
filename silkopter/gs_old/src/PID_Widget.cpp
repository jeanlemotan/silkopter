#include "stdafx.h"
#include "PID_Widget.h"

PID_Widget::PID_Widget(QWidget *parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);
}

void PID_Widget::init(silk::Comms* comms)
{
    using namespace silk;

    m_comms = comms;

    QObject::connect(m_ui.load, &QPushButton::released, [this]() { request_pids(); });
    QObject::connect(m_ui.save, &QPushButton::released, [this]() { send_pids(); });

    m_comms->yaw_rate_pid_params_received.connect([this](Comms::Yaw_Rate_PID::Params const& params)
    {
        m_ui.yaw_rate_p->setValue(params.kp);
        m_ui.yaw_rate_i->setValue(params.ki);
        m_ui.yaw_rate_d->setValue(params.kd);
        m_ui.yaw_rate_max->setValue(params.max);
    });
    m_comms->pitch_rate_pid_params_received.connect([this](Comms::Pitch_Rate_PID::Params const& params)
    {
        m_ui.pitch_rate_p->setValue(params.kp);
        m_ui.pitch_rate_i->setValue(params.ki);
        m_ui.pitch_rate_d->setValue(params.kd);
        m_ui.pitch_rate_max->setValue(params.max);
    });
    m_comms->roll_rate_pid_params_received.connect([this](Comms::Roll_Rate_PID::Params const& params)
    {
        m_ui.roll_rate_p->setValue(params.kp);
        m_ui.roll_rate_i->setValue(params.ki);
        m_ui.roll_rate_d->setValue(params.kd);
        m_ui.roll_rate_max->setValue(params.max);
    });
    m_comms->altitude_rate_pid_params_received.connect([this](Comms::Altitude_Rate_PID::Params const& params)
    {
        m_ui.altitude_rate_p->setValue(params.kp);
        m_ui.altitude_rate_i->setValue(params.ki);
        m_ui.altitude_rate_d->setValue(params.kd);
        m_ui.altitude_rate_max->setValue(params.max);
    });

    m_comms->yaw_pid_params_received.connect([this](Comms::Yaw_PID::Params const& params)
    {
        m_ui.yaw_p->setValue(params.kp);
        m_ui.yaw_i->setValue(params.ki);
        m_ui.yaw_d->setValue(params.kd);
        m_ui.yaw_max->setValue(params.max);
    });
    m_comms->pitch_pid_params_received.connect([this](Comms::Pitch_PID::Params const& params)
    {
        m_ui.pitch_p->setValue(params.kp);
        m_ui.pitch_i->setValue(params.ki);
        m_ui.pitch_d->setValue(params.kd);
        m_ui.pitch_max->setValue(params.max);
    });
    m_comms->roll_pid_params_received.connect([this](Comms::Roll_PID::Params const& params)
    {
        m_ui.roll_p->setValue(params.kp);
        m_ui.roll_i->setValue(params.ki);
        m_ui.roll_d->setValue(params.kd);
        m_ui.roll_max->setValue(params.max);
    });
    m_comms->altitude_pid_params_received.connect([this](Comms::Altitude_PID::Params const& params)
    {
        m_ui.altitude_p->setValue(params.kp);
        m_ui.altitude_i->setValue(params.ki);
        m_ui.altitude_d->setValue(params.kd);
        m_ui.altitude_max->setValue(params.max);
    });

    request_pids();
}


void PID_Widget::request_pids()
{
    m_comms->request_yaw_rate_pid_params();
    m_comms->request_pitch_rate_pid_params();
    m_comms->request_roll_rate_pid_params();
    m_comms->request_altitude_rate_pid_params();

    m_comms->request_yaw_pid_params();
    m_comms->request_pitch_pid_params();
    m_comms->request_roll_pid_params();
    m_comms->request_altitude_pid_params();
}

void PID_Widget::send_pids()
{
    using namespace silk;
    m_comms->set_yaw_rate_pid_params(Comms::Yaw_Rate_PID::Params(
                                         m_ui.yaw_rate_p->value(),
                                         m_ui.yaw_rate_i->value(),
                                         m_ui.yaw_rate_d->value(),
                                         m_ui.yaw_rate_max->value()));
    m_comms->set_pitch_rate_pid_params(Comms::Pitch_Rate_PID::Params(
                                           m_ui.pitch_rate_p->value(),
                                           m_ui.pitch_rate_i->value(),
                                           m_ui.pitch_rate_d->value(),
                                           m_ui.pitch_rate_max->value()));
    if (m_ui.roll_rate_group->isChecked())
    {
        m_comms->set_roll_rate_pid_params(Comms::Roll_Rate_PID::Params(
                                              m_ui.roll_rate_p->value(),
                                              m_ui.roll_rate_i->value(),
                                              m_ui.roll_rate_d->value(),
                                              m_ui.roll_rate_max->value()));
    }
    else
    {
        //copy from pitch
        m_comms->set_roll_rate_pid_params(Comms::Pitch_Rate_PID::Params(
                                               m_ui.pitch_rate_p->value(),
                                               m_ui.pitch_rate_i->value(),
                                               m_ui.pitch_rate_d->value(),
                                               m_ui.pitch_rate_max->value()));
    }
    m_comms->set_altitude_rate_pid_params(Comms::Altitude_Rate_PID::Params(
                                              m_ui.altitude_rate_p->value(),
                                              m_ui.altitude_rate_i->value(),
                                              m_ui.altitude_rate_d->value(),
                                              m_ui.altitude_rate_max->value()));

    m_comms->set_yaw_pid_params(Comms::Yaw_PID::Params(
                                    m_ui.yaw_p->value(),
                                    m_ui.yaw_i->value(),
                                    m_ui.yaw_d->value(),
                                    m_ui.yaw_max->value()));
    m_comms->set_pitch_pid_params(Comms::Pitch_PID::Params(
                                      m_ui.pitch_p->value(),
                                      m_ui.pitch_i->value(),
                                      m_ui.pitch_d->value(),
                                      m_ui.pitch_max->value()));
    if (m_ui.roll_group->isChecked())
    {
        m_comms->set_roll_pid_params(Comms::Roll_PID::Params(
                                         m_ui.roll_p->value(),
                                         m_ui.roll_i->value(),
                                         m_ui.roll_d->value(),
                                         m_ui.roll_max->value()));
    }
    else
    {
        //copy from pitch
        m_comms->set_roll_pid_params(Comms::Pitch_PID::Params(
                                          m_ui.pitch_p->value(),
                                          m_ui.pitch_i->value(),
                                          m_ui.pitch_d->value(),
                                          m_ui.pitch_max->value()));
    }
    m_comms->set_altitude_pid_params(Comms::Altitude_PID::Params(
                                         m_ui.altitude_p->value(),
                                         m_ui.altitude_i->value(),
                                         m_ui.altitude_d->value(),
                                         m_ui.altitude_max->value()));
}
