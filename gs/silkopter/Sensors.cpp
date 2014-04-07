#include "stdafx.h"
#include "Sensors.h"


Sensors::Sensors(QWidget *parent /* = 0 */)
	: QWidget(parent)
	, m_protocol(nullptr)
{
	m_ui.setupUi(this);

	m_ui.accel_plot->addGraph();
	m_ui.accel_plot->graph(0)->setPen(QPen(Qt::red));
	m_ui.accel_plot->addGraph();
	m_ui.accel_plot->graph(1)->setPen(QPen(Qt::green));
	m_ui.accel_plot->addGraph();
	m_ui.accel_plot->graph(2)->setPen(QPen(Qt::blue));

	m_ui.gyro_plot->addGraph();
	m_ui.gyro_plot->graph(0)->setPen(QPen(Qt::red));
	m_ui.gyro_plot->addGraph();
	m_ui.gyro_plot->graph(1)->setPen(QPen(Qt::green));
	m_ui.gyro_plot->addGraph();
	m_ui.gyro_plot->graph(2)->setPen(QPen(Qt::blue));

	m_ui.baro_plot->addGraph();
	m_ui.baro_plot->graph(0)->setPen(QPen(Qt::red));

	m_ui.sonar_plot->addGraph();
	m_ui.sonar_plot->graph(0)->setPen(QPen(Qt::red));

	connect(m_ui.accel_calibrate, &QPushButton::released, this, &Sensors::calibrate_accelerometer);

	m_last_time = std::chrono::high_resolution_clock::now();
}

Sensors::~Sensors()
{
}

void Sensors::init(SFull_Protocol* protocol)
{
	m_protocol = protocol;
}


void Sensors::update()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto d = now - m_last_time;
	m_last_time = now;

	assert(m_protocol);

	if (m_protocol->is_connected())
	{
		uint32_t time_us = m_protocol->data_board_time.value;
		if (time_us != m_last_time_us)
		{
			m_last_time_us = time_us;

			static const float graph_length_seconds = 3.f;

			double seconds = double(time_us) / 1000000.0;
			//static double seconds = 0;// double(time_ms) / 1000.0;
			//seconds += 0.01f;

			m_ui.gyro_plot->graph(0)->addData(seconds, m_protocol->data_board_gyroscope.value.x);
			m_ui.gyro_plot->graph(1)->addData(seconds, m_protocol->data_board_gyroscope.value.y);
			m_ui.gyro_plot->graph(2)->addData(seconds, m_protocol->data_board_gyroscope.value.z);

// 			m_ui.gyro_plot->graph(0)->addData(seconds, m_protocol->data_uav_attitude.value.x);
// 			m_ui.gyro_plot->graph(1)->addData(seconds, m_protocol->data_uav_attitude.value.y);
// 			m_ui.gyro_plot->graph(2)->addData(seconds, m_protocol->data_uav_attitude.value.z);

			m_ui.accel_plot->graph(0)->addData(seconds, m_protocol->data_board_accelerometer.value.x);
			m_ui.accel_plot->graph(1)->addData(seconds, m_protocol->data_board_accelerometer.value.y);
			m_ui.accel_plot->graph(2)->addData(seconds, m_protocol->data_board_accelerometer.value.z);

			m_ui.sonar_plot->graph(0)->addData(seconds, m_protocol->data_board_sonar_distance.value);
			m_ui.baro_plot->graph(0)->addData(seconds, m_protocol->data_board_baro_pressure.value);

			//printf("\n%f, %f, %f", m_protocol->data_board_accelerometer.value.x, m_protocol->data_board_accelerometer.value.y, m_protocol->data_board_accelerometer.value.z);

			//////////////////////////////////////////////////////////////////////////

			if (seconds > graph_length_seconds)
			{
				m_ui.gyro_plot->graph(0)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.gyro_plot->graph(1)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.gyro_plot->graph(2)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.accel_plot->graph(0)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.accel_plot->graph(1)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.accel_plot->graph(2)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.baro_plot->graph(0)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.sonar_plot->graph(0)->removeDataBefore(seconds - graph_length_seconds);
			}


			m_ui.gyro_plot->rescaleAxes();
			m_ui.gyro_plot->replot();

			m_ui.accel_plot->rescaleAxes();
			m_ui.accel_plot->replot();

			m_ui.baro_plot->rescaleAxes();
			m_ui.baro_plot->replot();

			m_ui.sonar_plot->rescaleAxes();
			m_ui.sonar_plot->replot();
		}
	}
}

void Sensors::calibrate_accelerometer()
{
	m_protocol->set_board_gyroscope_bias(math::vec3f(1, 2, 3));
}
