#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Silkopter.h"
#include "qcustomplot.h"
#include "SFull_Protocol.h"


class Silkopter : public QMainWindow
{
public:
	Silkopter(QWidget *parent = 0);
	~Silkopter();

private:
	void update();

	Ui::SilkopterClass m_ui;

	SFull_Protocol m_protocol;

	std::chrono::high_resolution_clock::time_point m_last_time;
	std::chrono::high_resolution_clock::duration m_duration;

	math::vec3f m_accel;
	math::vec3f m_accel_rotation;

	math::vec3f m_gyro;
	math::vec3f m_gyro_offset;
	math::vec3f m_gyro_vector;
	int m_gyro_calibrarion_samples;

	math::vec3f m_rotation;

	uint32_t m_last_time_ms;
};


