#pragma once

extern "C"
{
    struct js_event;
}


namespace qinput
{
	class Input_Mgr : q::util::Noncopyable
	{
	public:
        Input_Mgr(q::String const& window_handle);
        ~Input_Mgr();

        auto get_touchscreen() const -> Touchscreen const&;
        auto get_keybard() const -> Keyboard const&;
        auto get_sensors() const -> Sensors const&;

        auto get_all_gamepads() const -> std::vector<Gamepad_cptr>;
        auto find_gamepad_by_id(uint32_t id) const -> Gamepad_cptr;

		//////////////////////////////////////////////////////////////////////////
		// Methods called from the platform code

        void add_event(Touchscreen_Event const& event);
        void add_event(Keyboard_Event const& event);
        void add_event(Sensor_Event const& event);
//        void add_event(Gamepad_Event const& event);

        void update(q::Clock::duration dt);

	private:

        void enumerate_gamepads() const;

        void remove_disconnected_gamepads();
        void update_gamepads(q::Clock::duration dt);
        void connect_new_gamepads(q::Clock::duration dt);

		Touchscreen			m_touchscreen;
		Keyboard			m_keyboard;
		Sensors				m_sensors;

        struct Gamepad_Data
        {
            int id = 0;
            int fd = 0;
            Gamepad_ptr gamepad;
        };

        struct Mapping
        {
            std::map<int, Gamepad::Button> buttons;
            std::map<int, Gamepad::Axis> axes;
            std::map<int, Gamepad::Stick> sticks;
        } m_ouya_mapping, m_ps3_mapping, m_xbox_mapping;

        void process_ouya_gamepad_event(Gamepad_Data const& data, js_event const& ev);
        void process_ps3_gamepad_event(Gamepad_Data const& data, js_event const& ev);

        mutable std::mutex m_gamepad_mutex;
        mutable std::vector<Gamepad_Data>	m_gamepads;

//        uint32_t            m_last_gamepad_id;
//		std::mutex			m_new_gamepad_event_mutex;
//		std::deque<Gamepad_Event>		m_new_gamepad_events;
	};


	//////////////////////////////////////////////////////////////////////////

}
