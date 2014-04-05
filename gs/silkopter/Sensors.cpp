#include "stdafx.h"
#include "Sensors.h"


Sensors::Sensors(QWidget *parent /* = 0 */)
	: QWidget(parent)
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

	m_last_time = std::chrono::high_resolution_clock::now();
}

Sensors::~Sensors()
{
}


void Sensors::update(SFull_Protocol& protocol)
{
	auto now = std::chrono::high_resolution_clock::now();
	auto d = now - m_last_time;
	m_last_time = now;

	if (protocol.is_connected())
	{
		uint32_t time_ms = protocol.data_board_time_ms.value;
		if (time_ms != m_last_time_ms)
		{
			m_last_time_ms = time_ms;

			static const float graph_length_seconds = 3.f;

			double seconds = double(time_ms) / 1000.0;
			//static double seconds = 0;// double(time_ms) / 1000.0;
			//seconds += 0.01f;

// 			m_ui.gyro_plot->graph(0)->addData(seconds, protocol.data_board_gyroscope.value.x);
// 			m_ui.gyro_plot->graph(1)->addData(seconds, protocol.data_board_gyroscope.value.y);
// 			m_ui.gyro_plot->graph(2)->addData(seconds, protocol.data_board_gyroscope.value.z);

			m_ui.gyro_plot->graph(0)->addData(seconds, protocol.data_uav_attitude.value.x);
			m_ui.gyro_plot->graph(1)->addData(seconds, protocol.data_uav_attitude.value.y);
			m_ui.gyro_plot->graph(2)->addData(seconds, protocol.data_uav_attitude.value.z);

			m_ui.accel_plot->graph(0)->addData(seconds, protocol.data_board_accelerometer.value.x);
			m_ui.accel_plot->graph(1)->addData(seconds, protocol.data_board_accelerometer.value.y);
			m_ui.accel_plot->graph(2)->addData(seconds, protocol.data_board_accelerometer.value.z);

			m_ui.sonar_plot->graph(0)->addData(seconds, protocol.data_board_sonar_distance.value);
			m_ui.baro_plot->graph(0)->addData(seconds, protocol.data_board_baro_pressure.value);

			//printf("\n%f, %f, %f", protocol.data_board_accelerometer.value.x, protocol.data_board_accelerometer.value.y, protocol.data_board_accelerometer.value.z);

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

