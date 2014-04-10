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

	void set_stream_messages();

	void start_accelerometer_calibration();
	void start_gyroscope_calibration();

	void collect_vec3f_sample(std::vector<math::vec3f>& samples, SFull_Protocol::Data<math::vec3f>& data);

	//code copied from arducopter
	bool calibrate_accelerometer(math::vec3<double> samples[6], math::vec3<double>& bias, math::vec3<double>& scale);
	void calibrate_update_matrices(double dS[6], double JS[6][6], double beta[6], double data[3]);
	void calibrate_reset_matrices(double dS[6], double JS[6][6]);
	void calibrate_find_delta(double dS[6], double JS[6][6], double delta[6]);


	Ui::Sensors m_ui;

	uint32_t m_last_time_us = 0;
	bool m_stream_messages_set = false;
	std::chrono::high_resolution_clock::time_point m_last_time;
	SFull_Protocol* m_protocol;
};


