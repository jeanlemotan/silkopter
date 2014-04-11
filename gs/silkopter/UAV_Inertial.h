#pragma once

#include "ui_UAV_Inertial.h"
#include "SFull_Protocol.h"

class UAV_Inertial : public QWidget
{
public:
	UAV_Inertial(QWidget *parent = 0);
	~UAV_Inertial();

	void init(SFull_Protocol* protocol);

public:
	void update();

private:

	void set_stream_messages();

	Ui::UAV_Inertial m_ui;

	uint32_t m_last_time_us = 0;
	bool m_stream_messages_set = false;
	std::chrono::high_resolution_clock::time_point m_last_time;
	SFull_Protocol* m_protocol;
};


