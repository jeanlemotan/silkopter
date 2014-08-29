#pragma once

namespace qinput
{
	class Input_Mgr : q::util::Noncopyable
	{
	public:
        Input_Mgr(q::String const& window_handle);

		Touchscreen const&	get_touchscreen() const;
		Keyboard const&		get_keybard() const;
		Sensors const&		get_sensors() const;

		std::vector<Gamepad_cptr> get_all_gamepads() const;
		Gamepad_cptr		find_gamepad_by_id(uint32_t id) const;

		//////////////////////////////////////////////////////////////////////////
		// Methods called from the platform code

		void				add_event(Touchscreen_Event const& event);
		void				add_event(Keyboard_Event const& event);
		void				add_event(Sensor_Event const& event);
		void				add_event(Gamepad_Event const& event);

		void				update(q::Clock::duration dt);

	private:

        void                poll_sdl_events();

		void				remove_disconnected_gamepads();
		void				update_gamepads(q::Clock::duration dt);
		void				connect_new_gamepads(q::Clock::duration dt);

		Touchscreen			m_touchscreen;
		Keyboard			m_keyboard;
		Sensors				m_sensors;

		std::vector<Gamepad_ptr>	m_gamepads;

        uint32_t            m_last_gamepad_id;
		std::mutex			m_new_gamepad_event_mutex;
		std::deque<Gamepad_Event>		m_new_gamepad_events;

        class Event_Handler;

        std::shared_ptr<Event_Handler> m_event_handler;
	};


	//////////////////////////////////////////////////////////////////////////

}
