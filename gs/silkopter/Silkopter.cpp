#include "stdafx.h"
#include "Silkopter.h"


static const float k_calibration_sample_count = 100;

Silkopter::Silkopter(QWidget *parent)
	: QMainWindow(parent)
	, m_gyro_calibrarion_samples(k_calibration_sample_count)
{
	m_ui.setupUi(this);

	m_last_time = std::chrono::high_resolution_clock::now();

	auto* timer = new QTimer(this);
	timer->setSingleShot(false);
	timer->start(16);
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

	m_ui.sensors->update(m_protocol);
}

