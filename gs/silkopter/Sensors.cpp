#include "stdafx.h"
#include "Sensors.h"
#include "physics/constants.h"

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

//  			m_ui.gyro_plot->graph(0)->addData(seconds, m_protocol->data_uav_attitude.value.x);
//  			m_ui.gyro_plot->graph(1)->addData(seconds, m_protocol->data_uav_attitude.value.y);
//  			m_ui.gyro_plot->graph(2)->addData(seconds, m_protocol->data_uav_attitude.value.z);

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
	else
	{
		m_last_time_us = 0;
	}
}

struct Scope_Exit
{
	Scope_Exit(std::function<void()> f) : f(f) {}
	~Scope_Exit() { f(); }
	std::function<void()> f;
};

void Sensors::start_accelerometer_calibration()
{
	Scope_Exit _helper([&]() { m_protocol->set_stream_all_messages(true); });

	//disable all messages except the ones we need to speed up data acquizition
	{
		bool ok = m_protocol->set_stream_all_messages(false);
		ok &= m_protocol->set_stream_message(SFull_Protocol::RX_Message::PRINT, true);
		ok &= m_protocol->set_stream_message(SFull_Protocol::RX_Message::BOARD_CPU_USAGE, true);
		ok &= m_protocol->set_stream_message(SFull_Protocol::RX_Message::BOARD_TIME, true);
		ok &= m_protocol->set_stream_message(SFull_Protocol::RX_Message::BOARD_ACCELEROMETER, true);
		if (!ok)
		{
			QMessageBox::critical(this, "Error", "Cannot start the calibration procedure.\nFailed to enable messages.");
			return;
		}
	}

	std::chrono::seconds duration(3);

	std::string msg;

	QMessageBox msgBox;
	msgBox.setWindowTitle("Accelerometer Calibration");

	//first reset the bias
	{
		int count = 10;
		do
		{
			count--;
		} while (!m_protocol->set_board_accelerometer_bias_scale(math::vec3f::zero, math::vec3f::one) && count > 0);
		if (count <= 0)
		{
			QMessageBox::critical(this, "Error", "Cannot start the calibration procedure.\nFailed to reset the sensors.");
			return;
		}
	}

	char const* direction_str[] =
	{
		"level",
		"on its LEFT side",
		"on its RIGHT side",
		"nose DOWN",
		"nose UP",
		"on its BACK",
	};

	math::vec3f avg[6];

	for (size_t step = 0; step < 6; step++)
	{
		std::vector<math::vec3f> samples;

		util::format(msg, "Place the board {} for {} seconds.\nPress Ok to continue.", direction_str[step], duration.count());
		msgBox.setText(msg.c_str());
		msgBox.setStandardButtons(QMessageBox::Button::Ok);
		msgBox.exec();

		msgBox.setStandardButtons(0);
		msgBox.setText(QString());
		msgBox.show();

		//collect data
		auto start = std::chrono::high_resolution_clock::now();
		do
		{
			collect_vec3f_sample(samples, m_protocol->data_board_accelerometer);

			auto left = std::chrono::duration_cast<std::chrono::seconds>(duration - (std::chrono::high_resolution_clock::now() - start)).count();
			left = std::max<int>(left, 0);

			util::format(msg, "Collected {} sample{}...\n{} second{} left.",
				samples.size(),
				samples.size() != 1 ? "s" : "",
				left,
				left != 1 ? "s" : "");

			msgBox.setText(msg.c_str());

			QApplication::processEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		} while (std::chrono::high_resolution_clock::now() - start < duration);

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

		avg[step] = std::accumulate(samples.begin(), samples.end(), math::vec3f());
		avg[step] /= static_cast<float>(samples.size());
	}

	math::vec3f bias, scale;
	calibrate_accelerometer(avg, bias, scale);

	util::format(msg, "New accelerometer data:\nBias: {.9} \nScale: {.9}.", bias, scale);
	msgBox.setText(msg.c_str());
	msgBox.setStandardButtons(QMessageBox::Button::Ok);
	msgBox.exec();

	//set the new bias/scale
	{
		int count = 10;
		do
		{
			count--;
		} while (!m_protocol->set_board_accelerometer_bias_scale(bias, scale) && count > 0);
		if (count <= 0)
		{
			QMessageBox::critical(this, "Error", "Cannot set the new calibration data.\nFailed to reset the sensors.");
			return;
		}
	}
}

void Sensors::start_gyroscope_calibration()
{
	Scope_Exit _helper([&]() { m_protocol->set_stream_all_messages(true); });

	std::chrono::seconds duration(10);

	//disable all messages except the ones we need to speed up data acquizition
	{
		bool ok = m_protocol->set_stream_all_messages(false);
		ok &= m_protocol->set_stream_message(SFull_Protocol::RX_Message::PRINT, true);
 		ok &= m_protocol->set_stream_message(SFull_Protocol::RX_Message::BOARD_CPU_USAGE, true);
 		ok &= m_protocol->set_stream_message(SFull_Protocol::RX_Message::BOARD_TIME, true);
 		ok &= m_protocol->set_stream_message(SFull_Protocol::RX_Message::BOARD_GYROSCOPE, true);
		if (!ok)
		{
			QMessageBox::critical(this, "Error", "Cannot start the calibration procedure.\nFailed to enable messages.");
			return;
		}
	}

	std::string msg;
	util::format(msg, "Place the board level and completely still for {} seconds.", duration.count());

	QMessageBox msgBox;
	msgBox.setWindowTitle("Gyroscope Calibration");
	msgBox.setText(msg.c_str());
	msgBox.exec();

	//first reset the bias
	{
		int count = 10;
		do
		{
			count--;
		} while (!m_protocol->set_board_gyroscope_bias(math::vec3f::zero) && count > 0);
		if (count <= 0)
		{
			QMessageBox::critical(this, "Error", "Cannot start the calibration procedure.\nFailed to reset the sensors.");
			return;
		}
	}

	//start calibration
	msgBox.setStandardButtons(0);
	msgBox.setText(QString());
	msgBox.show();

	std::vector<math::vec3f> samples;

	//collect data
	auto start = std::chrono::high_resolution_clock::now();
	do
	{
		collect_vec3f_sample(samples, m_protocol->data_board_gyroscope);

		auto left = std::chrono::duration_cast<std::chrono::seconds>(duration - (std::chrono::high_resolution_clock::now() - start)).count();
		left = std::max<int>(left, 0);

		auto bias = std::accumulate(samples.begin(), samples.end(), math::vec3f());
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
	} while (std::chrono::high_resolution_clock::now() - start < duration);

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

	auto bias = std::accumulate(samples.begin(), samples.end(), math::vec3f());
	bias /= static_cast<float>(samples.size());


	util::format(msg, "New gyroscope bias: {.9}.", bias);
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

void Sensors::collect_vec3f_sample(std::vector<math::vec3f>& samples, SFull_Protocol::Data<math::vec3f>& data)
{
	m_protocol->process();

	//collect data
	if (data.is_valid &&
		m_protocol->data_board_time.is_valid &&
		m_protocol->data_board_time.value > m_last_time_us)
	{
		m_last_time_us = m_protocol->data_board_time.value;
		samples.push_back(data.value);
	}
}

bool Sensors::calibrate_accelerometer(math::vec3f samples[6], math::vec3f& bias, math::vec3f& scale)
{
	int16_t i;
	int16_t num_iterations = 0;
	float eps = 0.000000001;
	float change = 100.0;
	float data[3];
	float beta[6];
	float delta[6];
	float ds[6];
	float JS[6][6];
	bool success = true;

	// reset
	beta[0] = beta[1] = beta[2] = 0;
	beta[3] = beta[4] = beta[5] = 1.0f / physics::constants::g;

	while (num_iterations < 20 && change > eps) 
	{
		num_iterations++;

		calibrate_reset_matrices(ds, JS);

		for (i = 0; i < 6; i++) 
		{
			data[0] = samples[i].x;
			data[1] = samples[i].y;
			data[2] = samples[i].z;
			calibrate_update_matrices(ds, JS, beta, data);
		}

		calibrate_find_delta(ds, JS, delta);

		change = delta[0] * delta[0] +
			delta[0] * delta[0] +
			delta[1] * delta[1] +
			delta[2] * delta[2] +
			delta[3] * delta[3] / (beta[3] * beta[3]) +
			delta[4] * delta[4] / (beta[4] * beta[4]) +
			delta[5] * delta[5] / (beta[5] * beta[5]);

		for (i = 0; i < 6; i++) 
		{
			beta[i] -= delta[i];
		}
	}

	// copy results out
	scale.x = beta[3] * physics::constants::g;
	scale.y = beta[4] * physics::constants::g;
	scale.z = beta[5] * physics::constants::g;
	bias.x = beta[0] * scale.x;
	bias.y = beta[1] * scale.y;
	bias.z = beta[2] * scale.z;

	// sanity check scale
// 	if (scale.is_nan() || fabsf(scale.x - 1.0f) > 0.1f || fabsf(scale.y - 1.0f) > 0.1f || fabsf(scale.z - 1.0f) > 0.1f) 
// 	{
// 		success = false;
// 	}
// 	// sanity check offsets (3.5 is roughly 3/10th of a G, 5.0 is roughly half a G)
// 	if (bias.is_nan() || fabsf(bias.x) > 3.5f || fabsf(bias.y) > 3.5f || fabsf(bias.z) > 3.5f) 
// 	{
// 		success = false;
// 	}

	// return success or failure
	return success;
}

void Sensors::calibrate_update_matrices(float dS[6], float JS[6][6], float beta[6], float data[3])
{
	int16_t j, k;
	float dx, b;
	float residual = 1.0;
	float jacobian[6];

	for (j = 0; j < 3; j++) {
		b = beta[3 + j];
		dx = (float)data[j] - beta[j];
		residual -= b*b*dx*dx;
		jacobian[j] = 2.0f*b*b*dx;
		jacobian[3 + j] = -2.0f*b*dx*dx;
	}

	for (j = 0; j < 6; j++) {
		dS[j] += jacobian[j] * residual;
		for (k = 0; k < 6; k++) {
			JS[j][k] += jacobian[j] * jacobian[k];
		}
	}
}


// _calibrate_reset_matrices - clears matrices
void Sensors::calibrate_reset_matrices(float dS[6], float JS[6][6])
{
	int16_t j, k;
	for (j = 0; j < 6; j++) {
		dS[j] = 0.0f;
		for (k = 0; k < 6; k++) {
			JS[j][k] = 0.0f;
		}
	}
}

void Sensors::calibrate_find_delta(float dS[6], float JS[6][6], float delta[6])
{
	//Solve 6-d matrix equation JS*x = dS
	//first put in upper triangular form
	int16_t i, j, k;
	float mu;

	//make upper triangular
	for (i = 0; i < 6; i++) {
		//eliminate all nonzero entries below JS[i][i]
		for (j = i + 1; j < 6; j++) {
			mu = JS[i][j] / JS[i][i];
			if (mu != 0.0f) {
				dS[j] -= mu*dS[i];
				for (k = j; k < 6; k++) {
					JS[k][j] -= mu*JS[k][i];
				}
			}
		}
	}

	//back-substitute
	for (i = 5; i >= 0; i--) {
		dS[i] /= JS[i][i];
		JS[i][i] = 1.0f;

		for (j = 0; j < i; j++) {
			mu = JS[i][j];
			dS[j] -= mu*dS[i];
			JS[i][j] = 0.0f;
		}
	}

	for (i = 0; i < 6; i++) {
		delta[i] = dS[i];
	}
}

