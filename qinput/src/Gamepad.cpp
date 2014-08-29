#include "QInputStdAfx.h"
#include "QInput.h"

using namespace qinput;


Gamepad::Gamepad(uint32_t id)
	: m_id(id)
	, m_is_connected(false)
{
}

uint32_t Gamepad::get_id() const
{
	return m_id;
}

bool Gamepad::is_connected() const
{
	return m_is_connected;
}

Gamepad::Stick_Data Gamepad::get_stick_data(Stick_Id id) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if ((size_t)id < m_sticks.size())
	{
		return m_sticks[(size_t)id];
	}
	return Stick_Data();
}
Gamepad::Trigger_Data Gamepad::get_trigger_data(Trigger_Id id) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if ((size_t)id < m_triggers.size())
	{
		return m_triggers[(size_t)id];
	}
	return Trigger_Data();
}

bool Gamepad::is_button_inactive(Button_Id button) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_buttons_pressed.find(button) == m_buttons_pressed.end() && m_buttons_released.find(button) == m_buttons_released.end();
}
bool Gamepad::is_button_pressed(Button_Id button) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_buttons_pressed.find(button) != m_buttons_pressed.end();
}
bool Gamepad::is_button_released(Button_Id button) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_buttons_released.find(button) != m_buttons_released.end();
}
std::vector<Button_Id> Gamepad::get_all_pressed_buttons() const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	std::vector<Button_Id> res;
	res.reserve(16);
	for (auto k: m_buttons_pressed)
	{
		res.push_back(k);
	}
	return res;
}
std::vector<Button_Id> Gamepad::get_all_released_buttons() const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	std::vector<Button_Id> res;
	res.reserve(16);
	for (auto k: m_buttons_released)
	{
		res.push_back(k);
	}
	return res;
}
Sensors const& Gamepad::get_sensors() const
{
	return m_sensors;
}

void Gamepad::update(q::Clock::duration dt)
{
	{
		std::lock_guard<std::mutex> sm(m_mutex);
		m_buttons_released.clear();
	}

	process_events(dt);
}

void Gamepad::process_events(q::Clock::duration /*dt*/)
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
		case Gamepad_Event::Type::CONNECT:
			{
				QASSERT(!m_is_connected);
				m_is_connected = true;
			}
			break;
		case Gamepad_Event::Type::DISCONNECT:
			{
				QASSERT(m_is_connected);
				m_is_connected = false;
			}
			break;
		case Gamepad_Event::Type::STICK_CHANGE:
			{
				size_t id = (size_t)event.stick_id;
				if (id < m_sticks.size())
				{
					m_sticks[id].value = event.stick_value;
				}
			}
			break;
		case Gamepad_Event::Type::TRIGGER_CHANGE:
			{
				size_t id = (size_t)event.trigger_id;
				if (id < m_triggers.size())
				{
					m_triggers[id].value = event.trigger_value;
				}
			}
			break;
		case Gamepad_Event::Type::SENSOR_EVENT:
			{
				//mSensors.addEvent(event);
			}
			break;
		case Gamepad_Event::Type::BUTTON_PRESS:
			{
				m_buttons_pressed.insert(event.button_id);
			}
			break;
		case Gamepad_Event::Type::BUTTON_RELEASE:
			{
				m_buttons_pressed.erase(event.button_id);
				m_buttons_released.insert(event.button_id);
			}
			break;
		default:
			QLOG_ERR("Q", "Ignored gamepad event - {0}", (int)event.type);
			break;
		}
	}
	m_events.clear();
}

void Gamepad::add_event(Gamepad_Event const& event)
{
	std::lock_guard<std::mutex> sm(m_events_mutex);
	m_events.emplace_back(event);
}

