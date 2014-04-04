#include "stdafx.h"
#include "Silkopter.h"


static const float k_calibration_sample_count = 100;

Silkopter::Silkopter(QWidget *parent)
	: QMainWindow(parent)
	, m_gyro_calibrarion_samples(k_calibration_sample_count)
{
	m_ui.setupUi(this);

// 	QCPScatterStyle scatter;
// 	scatter.setShape(QCPScatterStyle::ssCircle);
// 	scatter.setPen(QPen(Qt::blue));
// 	scatter.setBrush(QBrush(Qt::black));
// 	scatter.setSize(5);

	m_ui.plot->addGraph();
	m_ui.plot->graph(0)->setPen(QPen(Qt::red));
//	m_ui.plot->graph(0)->setScatterStyle(scatter);
	m_ui.plot->addGraph();
	m_ui.plot->graph(1)->setPen(QPen(Qt::green));
//	m_ui.plot->graph(1)->setScatterStyle(scatter);
	m_ui.plot->addGraph();
	m_ui.plot->graph(2)->setPen(QPen(Qt::blue));
//	m_ui.plot->graph(2)->setScatterStyle(scatter);

// 	m_ui.plot->addGraph();
// 	m_ui.plot->graph(3)->setPen(QPen(Qt::darkRed));
// 	m_ui.plot->addGraph();
// 	m_ui.plot->graph(4)->setPen(QPen(Qt::darkGreen));
// 	m_ui.plot->addGraph();
// 	m_ui.plot->graph(5)->setPen(QPen(Qt::darkBlue));

	m_ui.plot2->addGraph();
	m_ui.plot2->graph(0)->setPen(QPen(Qt::red));
	m_ui.plot2->addGraph();
	m_ui.plot2->graph(1)->setPen(QPen(Qt::green));
	m_ui.plot2->addGraph();
	m_ui.plot2->graph(2)->setPen(QPen(Qt::blue));

	m_last_time = std::chrono::high_resolution_clock::now();

	auto* timer = new QTimer(this);
	timer->setSingleShot(false);
	timer->start(1);
	connect(timer, &QTimer::timeout, this, &Silkopter::update);
}

Silkopter::~Silkopter()
{
	m_protocol.stop();
}


void Silkopter::update()
{
	if (!m_protocol.is_listening())
	{
		m_protocol.listen_for_connection("com3", 115200);
	}
	m_protocol.process();

	auto now = std::chrono::high_resolution_clock::now();
	auto d = now - m_last_time;
	m_last_time = now;

	if (m_protocol.is_connected())
	{
		uint32_t time_ms = m_protocol.data_board_time_ms.value;
		if (time_ms != m_last_time_ms)
		{
			m_last_time_ms = time_ms;

			float altitude = m_protocol.data_board_sonar_altitude.value;
			float pressure = m_protocol.data_board_baro_pressure.value;
			float temperature = m_protocol.data_board_temperature.value;
			math::vec3f gyro = m_protocol.data_board_gyroscope.value;
			math::vec3f accel = m_protocol.data_board_accelerometer.value;

			m_accel = m_protocol.data_board_accelerometer.is_valid ? math::normalized(accel) : math::vec3f(0, 0, 1);
			if (!m_protocol.data_board_accelerometer.is_valid)
			{
				int a = 0;
			}

			math::vec3f accel_euler;
			accel_euler.x = math::atan2(m_accel.y, m_accel.z);// - math::anglef::pi.radians;
			accel_euler.y = -math::atan2(m_accel.x, sqrt(m_accel.y*m_accel.y + m_accel.z*m_accel.z));

			m_gyro = gyro;

			m_ui.orientation->m_euler = m_protocol.data_uav_attitude.value;
			m_ui.orientation->update();

			m_ui.orientation2->m_euler = accel_euler;
			m_ui.orientation2->update();


			//m_gyro_vector = xxx;
			//m_gyro_vector = m_gyro;
			//m_gyro_vector = math::lerp(m_gyro_vector, m_accel, 0.01f);

			static const float graph_length_seconds = 3.f;

			double seconds = double(time_ms) / 1000.0;
// 			m_ui.plot->graph(0)->addData(seconds, m_protocol.data_board_gyroscope.value.x);
// 			m_ui.plot->graph(1)->addData(seconds, m_protocol.data_board_gyroscope.value.y);
// 			m_ui.plot->graph(2)->addData(seconds, m_protocol.data_board_gyroscope.value.z);
			m_ui.plot->graph(0)->addData(seconds, m_protocol.data_uav_attitude.value.x);
			m_ui.plot->graph(1)->addData(seconds, m_protocol.data_uav_attitude.value.y);
			m_ui.plot->graph(2)->addData(seconds, m_protocol.data_uav_attitude.value.z);

			m_ui.plot2->graph(0)->addData(seconds, m_protocol.data_board_accelerometer.value.x);
			m_ui.plot2->graph(1)->addData(seconds, m_protocol.data_board_accelerometer.value.y);
			m_ui.plot2->graph(2)->addData(seconds, m_protocol.data_board_accelerometer.value.z);

			printf("\n%f, %f, %f", m_protocol.data_board_accelerometer.value.x, m_protocol.data_board_accelerometer.value.y, m_protocol.data_board_accelerometer.value.z);

			//////////////////////////////////////////////////////////////////////////

			m_ui.plot->graph(0)->removeDataBefore(seconds - graph_length_seconds);
			m_ui.plot->graph(1)->removeDataBefore(seconds - graph_length_seconds);
			m_ui.plot->graph(2)->removeDataBefore(seconds - graph_length_seconds);
			m_ui.plot2->graph(0)->removeDataBefore(seconds - graph_length_seconds);
			m_ui.plot2->graph(1)->removeDataBefore(seconds - graph_length_seconds);
			m_ui.plot2->graph(2)->removeDataBefore(seconds - graph_length_seconds);
		}
	}


 	m_ui.plot->rescaleAxes();
 	m_ui.plot->replot();
 
 	m_ui.plot2->rescaleAxes();
 	m_ui.plot2->replot();
}

