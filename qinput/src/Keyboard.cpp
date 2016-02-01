#include "QInputStdAfx.h"
#include "QInput.h"

using namespace qinput;

Keyboard::Keyboard()
{
	;
}

bool Keyboard::is_key_inactive(int key) const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	return m_keys_released.find(key) == m_keys_released.end() &&
		m_keys_pressed.find(key) == m_keys_pressed.end();
}
bool Keyboard::is_key_pressed(int key) const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	return m_keys_pressed.find(key) != m_keys_pressed.end();
}
bool Keyboard::is_key_released(int key) const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	return m_keys_released.find(key) != m_keys_released.end();
}
std::vector<int> Keyboard::get_all_pressed_keys() const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	std::vector<int> res;
	res.reserve(16);
	for (auto k: m_keys_pressed)
	{
		res.push_back(k);
	}
	return res;
}
std::vector<int> Keyboard::get_all_released_keys() const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	std::vector<int> res;
	res.reserve(16);
	for (auto k: m_keys_released)
	{
		res.push_back(k);
	}
	return res;
}

void Keyboard::update(q::Clock::duration dt)
{
	{
		std::lock_guard<std::mutex> sm(m_mutex);
		m_keys_released.clear();
	}

	process_events(dt);
}

void Keyboard::process_events(q::Clock::duration /*dt*/)
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
		case Keyboard_Event::Type::PRESS:
			{
				m_keys_pressed.insert(event.key_code);
			}
			break;
		case Keyboard_Event::Type::RELEASE:
			{
				m_keys_pressed.erase(event.key_code);
				m_keys_released.insert(event.key_code);
			}
			break;
		default:
            //QLOGE("Ignored keyboard event - {}", (int)event.type);
			break;
		}
	}
	m_events.clear();
}

void Keyboard::add_event(Keyboard_Event const& event)
{
    std::lock_guard<std::mutex> sm(m_events_mutex);
    //QLOGI("Keyboard event {}, {}", event.key_code, event.timestamp);
	m_events.emplace_back(event);
}
