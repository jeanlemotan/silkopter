#include "stdafx.h"
#include "UAV_Inertial.h"
#include "physics/constants.h"

UAV_Inertial::UAV_Inertial(QWidget *parent /* = 0 */)
: QWidget(parent)
, m_comms(nullptr)
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

    m_last_time = q::Clock::now();

//	connect(m_ui.reset_inertial_frame, &QPushButton::released, [this]()
//	{
//		if (m_protocol && m_protocol->is_connected())
//		{
//			m_protocol->reset_uav_inertial_frame();
//		}
//	});
}

UAV_Inertial::~UAV_Inertial()
{
}

void UAV_Inertial::init(silk::Comms* comms)
{
    m_comms = comms;
	m_ui.attitude->init();
}

void UAV_Inertial::process()
{
    auto now = q::Clock::now();
    //auto d = now - m_last_time;
	m_last_time = now;

    QASSERT(m_comms);

    m_ui.reset_inertial_frame->setEnabled(m_comms->is_connected());

    if (isVisible() && m_comms->is_connected())
	{
        m_ui.attitude->render(*m_comms);

        auto remote_now = m_comms->get_remote_clock().now();
        if (m_graph_remote_epoch.time_since_epoch().count() == 0)
        {
            m_graph_remote_epoch = remote_now;
        }

        if (remote_now != m_last_remote_time_stamp)
		{
            m_last_remote_time_stamp = remote_now;

			static const float graph_length_seconds = 3.f;

            double seconds = q::Seconds(remote_now - m_graph_remote_epoch).count();

            {
                auto data = m_comms->get_uav_linear_acceleration_sample().value;
                m_ui.acceleration_plot->graph(0)->addData(seconds, data.x);
                m_ui.acceleration_plot->graph(1)->addData(seconds, data.y);
                m_ui.acceleration_plot->graph(2)->addData(seconds, data.z);
            }

            {
                auto data = m_comms->get_uav_velocity_sample().value;
                m_ui.velocity_plot->graph(0)->addData(seconds, data.x);
                m_ui.velocity_plot->graph(1)->addData(seconds, data.y);
                m_ui.velocity_plot->graph(2)->addData(seconds, data.z);
            }

            {
                auto data = m_comms->get_uav_position_sample().value;
                m_ui.position_plot->graph(0)->addData(seconds, data.x);
                m_ui.position_plot->graph(1)->addData(seconds, data.y);
                m_ui.position_plot->graph(2)->addData(seconds, data.z);
            }

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
        m_graph_remote_epoch = silk::Remote_Clock::time_point(std::chrono::seconds(0));
        m_ui.acceleration_plot->graph(0)->clearData();
        m_ui.acceleration_plot->graph(1)->clearData();
        m_ui.acceleration_plot->graph(2)->clearData();
        m_ui.velocity_plot->graph(0)->clearData();
        m_ui.velocity_plot->graph(1)->clearData();
        m_ui.velocity_plot->graph(2)->clearData();
        m_ui.position_plot->graph(0)->clearData();
        m_ui.position_plot->graph(1)->clearData();
        m_ui.position_plot->graph(2)->clearData();
    }

}
