#include "stdafx.h"
#include "UAV_Inertial.h"
#include "physics/constants.h"

UAV_Inertial::UAV_Inertial(QWidget *parent /* = 0 */)
: QWidget(parent)
, m_protocol(nullptr)
{
	m_ui.setupUi(this);

	m_ui.acceleration_plot->addGraph();
	m_ui.acceleration_plot->graph(0)->setPen(QPen(Qt::red));
	m_ui.acceleration_plot->addGraph();
	m_ui.acceleration_plot->graph(1)->setPen(QPen(Qt::green));
	m_ui.acceleration_plot->addGraph();
	m_ui.acceleration_plot->graph(2)->setPen(QPen(Qt::blue));

	m_ui.velocity_plot->addGraph();
	m_ui.velocity_plot->graph(0)->setPen(QPen(Qt::red));
	m_ui.velocity_plot->addGraph();
	m_ui.velocity_plot->graph(1)->setPen(QPen(Qt::green));
	m_ui.velocity_plot->addGraph();
	m_ui.velocity_plot->graph(2)->setPen(QPen(Qt::blue));

	m_ui.position_plot->addGraph();
	m_ui.position_plot->graph(0)->setPen(QPen(Qt::red));
	m_ui.position_plot->addGraph();
	m_ui.position_plot->graph(1)->setPen(QPen(Qt::green));
	m_ui.position_plot->addGraph();
	m_ui.position_plot->graph(2)->setPen(QPen(Qt::blue));

	m_last_time = std::chrono::high_resolution_clock::now();

	connect(m_ui.reset_inertial_frame, &QPushButton::released, [this]() 
	{
		if (m_protocol && m_protocol->is_connected())
		{ 
			m_protocol->reset_uav_inertial_frame();
		}
	});
}

UAV_Inertial::~UAV_Inertial()
{
}

void UAV_Inertial::init(SFull_Protocol* protocol)
{
	m_protocol = protocol;
}

void UAV_Inertial::set_stream_messages()
{
	m_protocol->set_stream_all_messages(false);
	m_protocol->set_stream_message(SFull_Protocol::RX_Message::PRINT, true);
	m_protocol->set_stream_message(SFull_Protocol::RX_Message::BOARD_CPU_USAGE, true);
	m_protocol->set_stream_message(SFull_Protocol::RX_Message::BOARD_TIME, true);
	m_protocol->set_stream_message(SFull_Protocol::RX_Message::UAV_ACCELERATION, true);
	m_protocol->set_stream_message(SFull_Protocol::RX_Message::UAV_VELOCITY, true);
	m_protocol->set_stream_message(SFull_Protocol::RX_Message::UAV_POSITION, true);
}


void UAV_Inertial::update()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto d = now - m_last_time;
	m_last_time = now;

	assert(m_protocol);

	m_ui.reset_inertial_frame->setEnabled(m_protocol->is_connected());

	if (isVisible() && m_protocol->is_connected())
	{
		if (!m_stream_messages_set)
		{
			m_stream_messages_set = true;
			set_stream_messages();
		}

		uint32_t time_us = m_protocol->data_board_time.value;
		if (time_us != m_last_time_us)
		{
			m_last_time_us = time_us;

			static const float graph_length_seconds = 3.f;

			double seconds = double(time_us) / 1000000.0;

			m_ui.acceleration_plot->graph(0)->addData(seconds, m_protocol->data_uav_acceleration.value.x);
			m_ui.acceleration_plot->graph(1)->addData(seconds, m_protocol->data_uav_acceleration.value.y);
			m_ui.acceleration_plot->graph(2)->addData(seconds, m_protocol->data_uav_acceleration.value.z);

			m_ui.velocity_plot->graph(0)->addData(seconds, m_protocol->data_uav_velocity.value.x);
			m_ui.velocity_plot->graph(1)->addData(seconds, m_protocol->data_uav_velocity.value.y);
			m_ui.velocity_plot->graph(2)->addData(seconds, m_protocol->data_uav_velocity.value.z);

			m_ui.position_plot->graph(0)->addData(seconds, m_protocol->data_uav_position.value.x);
			m_ui.position_plot->graph(1)->addData(seconds, m_protocol->data_uav_position.value.y);
			m_ui.position_plot->graph(2)->addData(seconds, m_protocol->data_uav_position.value.z);

			//////////////////////////////////////////////////////////////////////////

			if (seconds > graph_length_seconds)
			{
				m_ui.acceleration_plot->graph(0)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.acceleration_plot->graph(1)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.acceleration_plot->graph(2)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.velocity_plot->graph(0)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.velocity_plot->graph(1)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.velocity_plot->graph(2)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.position_plot->graph(0)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.position_plot->graph(1)->removeDataBefore(seconds - graph_length_seconds);
				m_ui.position_plot->graph(2)->removeDataBefore(seconds - graph_length_seconds);
			}

			m_ui.acceleration_plot->rescaleAxes(true);
			m_ui.acceleration_plot->replot();

			m_ui.velocity_plot->rescaleAxes(true);
			m_ui.velocity_plot->replot();

			m_ui.position_plot->rescaleAxes(true);
			m_ui.position_plot->replot();
		}
	}
	else
	{
		m_last_time_us = 0;
		m_stream_messages_set = false;
	}
}
