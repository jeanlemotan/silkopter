#include "QInputStdAfx.h"
#include "QInput.h"

using namespace qinput;

static const size_t k_history_size = 1024;

Sensors::Sensors()
{

}

Sensors::Acceleration_Data Sensors::get_acceleration_data() const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	if (m_acceleration_data.empty())
	{
		return Acceleration_Data();
	}
	auto val = m_acceleration_data.front();
	return val;
}

void Sensors::process_events(q::Clock::duration /*dt*/)
{
	std::lock_guard<std::mutex> sm(m_events_mutex);
	if (m_events.empty())
	{
		return;
	}

	std::lock_guard<std::mutex> sm2(m_mutex);

	for (auto const& event: m_events)
	{
		switch (event.type)
		{
		case Sensor_Event::Type::ACCELEROMETER_CHANGE:
			{
				Acceleration_Data data;
				data.timestamp = event.timestamp;
				data.value = event.accelerometer_value;
				m_acceleration_data.push_front(data);
			}
			break;
		default:
			QLOG_ERR("Q", "Ignored sensor event - {0}", (int)event.type);
			break;
		}
	}
	m_events.clear();

	//keep history under control :P
	while (m_acceleration_data.size() > k_history_size)
	{
		m_acceleration_data.pop_back();
	}
}

void Sensors::update(q::Clock::duration dt)
{
	process_events(dt);
}

void Sensors::add_event(Sensor_Event const& event)
{
	std::lock_guard<std::mutex> sm(m_events_mutex);
	m_events.emplace_back(event);
}
