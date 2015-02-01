#include "QInputStdAfx.h"
#include "QInput.h"

using namespace qinput;

Touchscreen::Touchscreen()
{
	;
}

std::vector<Touch_cptr> Touchscreen::get_all_touches() const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	std::vector<Touch_cptr> res;
	res.reserve(m_touches.size());

	for (auto const& t: m_touches)
	{
		res.emplace_back(t);
	}

	return res;
}

Touch_cptr Touchscreen::find_touch_by_area(math::aabb2f const& area, Touch::States states /* = Touch::States */) const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	if (states.value())
	{
		for (auto const& t: m_touches)
		{
			if (states[t->get_state()])
			{
				math::vec2f pos = t->get_position();
				if (area.is_point_inside(pos))
				{
					return t;
				}
			}
		}
	}
	return Touch_cptr();
}
Touch_cptr Touchscreen::find_touch_by_id(uint32_t id, Touch::States states /* = Touch::States */) const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	if (states.value())
	{
		for (auto const& t: m_touches)
		{
			if (states[t->get_state()])
			{
				if (t->get_id() == id)
				{
					return t;
				}
			}
		}
	}
	return Touch_cptr();
}
Touch_cptr Touchscreen::find_any_touch(Touch::States states /* = Touch::States */) const
{
	std::lock_guard<std::mutex> sm(m_mutex);
	if (states.value())
	{
		for (auto const& t: m_touches)
		{
			if (states[t->get_state()])
			{
				return t;
			}
		}
	}
	return Touch_cptr();
}


Touch_ptr Touchscreen::find_touch_by_id_locked(uint32_t id)
{
	auto it = std::find_if(m_touches.begin(), m_touches.end(), [&](Touch_ptr const& t) { return t->get_id() == id; });
	if (it != m_touches.end())
	{
		return *it;
	}
	return Touch_ptr();
}
void Touchscreen::update(q::Clock::duration dt)
{
	std::lock_guard<std::mutex> sm(m_mutex);
	for (auto const& t: m_touches)
	{
		t->update_delta();
		if (t->get_state() == Touch::State::PRESSED)
		{
			t->set_state(Touch::State::MOVED);
		}
	}

	m_touches.erase(std::remove_if(m_touches.begin(), m_touches.end(), [](Touch_ptr const& t)
	{ 
		return t->get_state() == Touch::State::RELEASED || t->get_state() == Touch::State::CANCELED; 
	}), m_touches.end());

	process_events(dt);
}

void Touchscreen::process_events(q::Clock::duration /*dt*/)
{
	std::lock_guard<std::mutex> sm(m_events_mutex);
	if (m_events.empty())
	{
		return;
	}

	for (auto const& event: m_events)
	{
		switch (event.type)
		{
		case Touchscreen_Event::Type::PRESS:
			{
				if (find_touch_by_id_locked(event.touch_id))
				{
					break;
				}
				Touch_ptr touch(std::make_shared<Touch>(event.touch_id));
				touch->set_state(Touch::State::PRESSED);
				touch->set_position(Touch::State::PRESSED, event.position);
				touch->set_position(Touch::State::MOVED, event.position);
				m_touches.emplace_back(touch);
			}
			break;
		case Touchscreen_Event::Type::MOVE:
			{
				Touch_ptr touch = find_touch_by_id_locked(event.touch_id);
				if (touch && touch->get_state() != Touch::State::CANCELED && touch->get_state() != Touch::State::RELEASED)
				{
					if (touch->get_position() != event.position)
					{
						touch->set_state(Touch::State::MOVED);
						touch->set_position(Touch::State::MOVED, event.position);
					}
				}
			}
			break;
		case Touchscreen_Event::Type::RELEASE:
			{
				Touch_ptr touch = find_touch_by_id_locked(event.touch_id);
				if (touch)
				{
					touch->set_position(Touch::State::RELEASED, event.position);
					touch->set_state(Touch::State::RELEASED);
				}
			}
			break;
		case Touchscreen_Event::Type::CANCEL:
			{
				Touch_ptr touch = find_touch_by_id_locked(event.touch_id);
				if (touch)
				{
					touch->set_position(Touch::State::CANCELED, touch->get_position());
					touch->set_position(Touch::State::RELEASED, touch->get_position());
					touch->set_state(Touch::State::CANCELED);
				}
			}
			break;
		default:
            QLOGE("Ignored gamepad event - {0}", (int)event.type);
			break;
		}
	}
	m_events.clear();
}

void Touchscreen::add_event(Touchscreen_Event const& event)
{
	std::lock_guard<std::mutex> sm(m_events_mutex);
	m_events.emplace_back(event);
}


