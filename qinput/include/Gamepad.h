#pragma once

namespace qinput
{
	class Input_Mgr;
	struct Gamepad_Event;

	enum class Button_Id
	{
		//generic semantics
		CONFIRM		= 0,
		OPTION1		= 1,
		OPTION2		= 2,
		BACK		= 3,

		OUYA_O		= 0,
		OUYA_U		= 1,
		OUYA_Y		= 2,
		OUYA_A		= 3,

		PS_X		= 0,
		PS_SQUARE	= 1,
		PS_TRIANGLE	= 2,
		PS_CIRCLE	= 3,

		XBOX_A		= 0,
		XBOX_X		= 1,
		XBOX_Y		= 2,
		XBOX_B		= 3,

		//////////////////////////////////////////////////////////////////////////

		RIGHT_BUMPER= 100,
		LEFT_BUMPER	= 101,
		RIGHT_STICK	= 102,
		LEFT_STICK	= 103,

		OUYA_RIGHT_BUMPER	= 100,
		OUYA_LEFT_BUMPER	= 101,
		OUYA_RIGHT_STICK	= 102,
		OUYA_LEFT_STICK		= 103,

		PS_R1		= 100,
		PS_L1		= 101,
		PS_R3		= 102,
		PS_L3		= 103,

		XBOX_RB		= 100,
		XBOX_LB		= 101,
		XBOX_R3		= 102,
		XBOX_L3		= 103,

		//////////////////////////////////////////////////////////////////////////

		START		= 200,
		SELECT		= 201,
		HOME		= 203,

		PAD_UP		= 300,
		PAD_DOWN	= 301,
		PAD_LEFT	= 302,
		PAD_RIGHT	= 303,
	};

	enum class Stick_Id
	{
		LEFT,
		RIGHT
	};

	enum  class Trigger_Id
	{
		LEFT,
		RIGHT
	};


	struct Gamepad_Event
	{
		enum class Type
		{
			CONNECT,
			DISCONNECT,
			STICK_CHANGE,
			TRIGGER_CHANGE,
			BUTTON_PRESS,
			BUTTON_RELEASE,
			SENSOR_EVENT
		};

		//all events should set the type and timestamp
		Type type;
        q::Clock::time_point timestamp;

		//all events should set the gamepadId
        uint32_t gamepad_id = uint32_t(-1);

		//STICK_CHANGE events
		Stick_Id stick_id;
		math::vec2f stick_value;

		//TRIGGER_CHANGE events
		Trigger_Id trigger_id;
		float trigger_value;

		//BUTTON_PRESS/RELEASE events
		Button_Id button_id;

		//SENSOR_EVENTS
		Sensor_Event sensor_event;
	};

	class Gamepad : q::util::Noncopyable
	{
		friend class Input_Mgr;
	public:
		Gamepad(uint32_t id);

		uint32_t get_id() const;
		
		bool is_connected() const;

		struct Stick_Data
		{
			math::vec2f value;
		};
		struct Trigger_Data
		{
			Trigger_Data() : value(0) {}
			Trigger_Data(Trigger_Data const& other) : value(other.value) {}
			Trigger_Data& operator=(Trigger_Data const& other) { value = other.value; return *this; }

			float value;
		};

		Stick_Data			get_stick_data(Stick_Id id) const;
		Trigger_Data		get_trigger_data(Trigger_Id id) const;

		bool				is_button_inactive(Button_Id button) const;
		bool				is_button_pressed(Button_Id button) const;
		bool				is_button_released(Button_Id button) const;

		std::vector<Button_Id>	get_all_pressed_buttons() const;
		std::vector<Button_Id>	get_all_released_buttons() const;

		Sensors const&		get_sensors() const;

	protected:
		void update(q::Clock::duration dt);
		void add_event(Gamepad_Event const& event);


	private:
		void process_events(q::Clock::duration dt);

		uint32_t m_id;

		mutable std::mutex	m_mutex;

		mutable std::mutex	m_events_mutex;
		std::deque<Gamepad_Event> m_events;

		bool				m_is_connected;
		std::array<Stick_Data, 2> m_sticks;
		std::array<Trigger_Data, 2> m_triggers;
		std::set<Button_Id>	m_buttons_pressed;
		std::set<Button_Id>	m_buttons_released;

		Sensors				m_sensors;
	};


}
