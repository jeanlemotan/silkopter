#pragma once

#include "ui_Sensors.h"
#include "SFull_Protocol.h"

class Sensors : public QWidget
{
public:
	Sensors(QWidget *parent = 0);
	~Sensors();

	void init(SFull_Protocol* protocol);

public:
	void update();

private:

	void start_accelerometer_calibration();
	void start_gyroscope_calibration();

	Ui::Sensors m_ui;

	uint32_t m_last_time_us;
	std::chrono::high_resolution_clock::time_point m_last_time;
	SFull_Protocol* m_protocol;
};


