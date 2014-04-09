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

	connect(m_ui.accel_calibrate, &QPushButton::released, this, &Sensors::start_accelerometer_calibration);
	connect(m_ui.gyro_calibrate, &QPushButton::released, this, &Sensors::start_gyroscope_calibration);

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

	m_ui.accel_calibrate->setEnabled(m_protocol->is_connected());
	m_ui.gyro_calibrate->setEnabled(m_protocol->is_connected());

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

void Sensors::start_accelerometer_calibration()
{
	//m_protocol->set_board_gyroscope_bias(math::vec3f(1, 2, 3));
}

void Sensors::start_gyroscope_calibration()
{
	std::chrono::seconds duration(10);

	std::string msg;
	util::format(msg, "Place the board level and completely still for {} seconds.", duration.count());

	QMessageBox msgBox;
	msgBox.setWindowTitle("Gyroscope Calibration");
	msgBox.setText(msg.c_str());
	msgBox.exec();

	//first reset the bias
// 	{
// 		int count = 10;
// 		do
// 		{
// 			count--;
// 		} while (!m_protocol->set_board_gyroscope_bias(math::vec3f::zero) && count > 0);
// 		if (count <= 0)
// 		{
// 			QMessageBox::critical(this, "Error", "Cannot start the calibration procedure.\nFailed to reset the sensors.");
// 			return;
// 		}
// 	}

	//wait a bit to counter any movement the user might do after clicking ok
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	//start calibration
	msgBox.setStandardButtons(0);
	msgBox.setText(QString());
	msgBox.show();

	std::vector<math::vec3<double>> samples;

	//collect data
	uint32_t last_board_time_ms = 0;
	auto start = std::chrono::high_resolution_clock::now();
	do
	{
		m_protocol->process();
		if (m_protocol->data_board_gyroscope.is_valid && 
			m_protocol->data_board_time.is_valid && 
			m_protocol->data_board_time.value > last_board_time_ms)
		{
			last_board_time_ms = m_protocol->data_board_time.value;
			samples.push_back(math::vec3<double>(m_protocol->data_board_gyroscope.value));
		}

		auto left = std::chrono::duration_cast<std::chrono::seconds>(duration - (std::chrono::high_resolution_clock::now() - start)).count();
		left = std::max<int>(left, 0);

		auto bias = std::accumulate(samples.begin(), samples.end(), math::vec3<double>());
		if (!samples.empty())
		{
			bias /= static_cast<float>(samples.size());
		}

		util::format(msg, "Collected {} sample{}...\n{} second{} left.\nApproximated bias: {.9}", 
			samples.size(), 
			samples.size() != 1 ? "s" : "", 
			left, 
			left != 1 ? "s" : "", 
			bias);

		msgBox.setText(msg.c_str());

		QApplication::processEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (std::chrono::high_resolution_clock::now() - start < std::chrono::seconds(10));

	//make sure we have enough data
	if (samples.size() < 30)
	{
		msgBox.setText("Calibration failed - not enough samples collected!");
		msgBox.setStandardButtons(QMessageBox::Button::Close);
		msgBox.exec();
		return;
	}

	//remove 10 samples to avoid any garbage from the beginning
	samples.erase(samples.begin(), samples.begin() + 10);

	auto bias = std::accumulate(samples.begin(), samples.end(), math::vec3<double>());
	bias /= static_cast<float>(samples.size());


	util::format(msg, "New gyro bias: {.9}.", bias);
	msgBox.setText(msg.c_str());
	msgBox.setStandardButtons(QMessageBox::Button::Ok);
	msgBox.exec();

	//set the new bias
	{
		int count = 10;
		do
		{
			count--;
		} while (!m_protocol->set_board_gyroscope_bias(math::vec3f(bias)) && count > 0);
		if (count <= 0)
		{
			QMessageBox::critical(this, "Error", "Cannot set the new calibration data.\nFailed to reset the sensors.");
			return;
		}
	}
}

