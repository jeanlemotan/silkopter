#pragma once

#include "ui_Sensors.h"
#include "SFull_Protocol.h"

class Sensors : public QWidget
{
public:
	Sensors(QWidget *parent = 0);
	~Sensors();

public:
	void update(SFull_Protocol& protocol);

private:

	Ui::Sensors m_ui;

	uint32_t m_last_time_ms;
	std::chrono::high_resolution_clock::time_point m_last_time;
};


