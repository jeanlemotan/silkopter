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

	void collect_vec3f_sample(std::vector<math::vec3f>& samples, SFull_Protocol::Data<math::vec3f>& data);

	bool calibrate_accelerometer(math::vec3f samples[6], math::vec3f& bias, math::vec3f& scale);
	void calibrate_update_matrices(float dS[6], float JS[6][6], float beta[6], float data[3]);
	void calibrate_reset_matrices(float dS[6], float JS[6][6]);
	void calibrate_find_delta(float dS[6], float JS[6][6], float delta[6]);


	Ui::Sensors m_ui;

	uint32_t m_last_time_us = 0;
	std::chrono::high_resolution_clock::time_point m_last_time;
	SFull_Protocol* m_protocol;
};


