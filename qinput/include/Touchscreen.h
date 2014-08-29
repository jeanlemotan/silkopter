#pragma once

namespace qinput
{
	class Input_Mgr;

	struct Touchscreen_Event
	{
		enum class Type
		{
			PRESS,
			MOVE,
			RELEASE,
			CANCEL
		};

		Type type;
		q::Clock::time_point timestamp;

		uint32_t touch_id;
		math::vec2f position;
	};


	class Touchscreen
	{
		friend class Input_Mgr;
	public:
		Touchscreen();

		std::vector<Touch_cptr> get_all_touches() const;

        Touch_cptr			find_any_touch(Touch::States states = Touch::States(-1)) const;
        Touch_cptr			find_touch_by_area(math::aabb2f const& area, Touch::States states = Touch::States(-1)) const;
        Touch_cptr			find_touch_by_id(uint32_t id, Touch::States states = Touch::States(-1)) const;

	protected:
		void				update(q::Clock::duration dt);
		void				add_event(Touchscreen_Event const& event);

	private:
		void				process_events(q::Clock::duration dt);
		Touch_ptr			find_touch_by_id_locked(uint32_t id);

		std::vector<Touch_ptr> m_touches;
		mutable std::mutex	m_mutex;

		mutable std::mutex	m_events_mutex;
		std::deque<Touchscreen_Event> m_events;
	};


}
