#pragma once

#include "ui_UAV_Inertial.h"
#include "Comms.h"

class UAV_Inertial : public QWidget
{
public:
	UAV_Inertial(QWidget *parent = 0);
	~UAV_Inertial();

    void init(silk::Comms* comms);

public:
	void process();

private:

	void set_stream_messages();

	Ui::UAV_Inertial m_ui;

    q::Clock::duration m_last_uav_alive_duration;
    q::Clock::time_point m_last_time;
    silk::Comms* m_comms;
};


