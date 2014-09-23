#pragma once

namespace qinput
{
	class Input_Mgr;
//	struct Gamepad_Event;

	class Gamepad : q::util::Noncopyable
	{
		friend class Input_Mgr;
	public:
        enum class Button
        {
            //////////////////////////////////////////////////////////////////////////

            RIGHT_BUMPER    = 100,
            LEFT_BUMPER     = 101,
            RIGHT_TRIGGER   = 102,
            LEFT_TRIGGER    = 103,
            RIGHT_STICK     = 104,
            LEFT_STICK      = 105,

            //////////////////////////////////////////////////////////////////////////

            START		= 200,
            SELECT		= 201,
            HOME		= 203,

            LPAD_UP		= 300,
            LPAD_DOWN	= 301,
            LPAD_LEFT	= 302,
            LPAD_RIGHT	= 303,

            RPAD_UP		= 400, //triangle on the ps3, Y on xbox an ouya
            RPAD_DOWN	= 401, //X on ps3, A on xbox, O on ouya
            RPAD_LEFT	= 402, //square on ps3, X on xbox, U on ouya
            RPAD_RIGHT	= 403, //circle on ps3, B on xbox, A on ouya

            //gamepad specific for the RPAD
            OUYA_O		= 401,
            OUYA_U		= 402,
            OUYA_Y		= 400,
            OUYA_A		= 403,

            PS_X		= 401,
            PS_SQUARE	= 402,
            PS_TRIANGLE	= 400,
            PS_CIRCLE	= 403,

            XBOX_A		= 401,
            XBOX_X		= 402,
            XBOX_Y		= 400,
            XBOX_B		= 403,
        };

        enum class Stick
        {
            LEFT,
            RIGHT
        };

        enum  class Axis
        {
            LEFT_TRIGGER,
            RIGHT_TRIGGER,
        };

        enum class Type
        {
            OUYA
        };

        Gamepad(q::String const& name, Type type);

        auto get_name() const -> q::String const&;
        auto get_type() const -> Type;
		
		bool is_connected() const;

		struct Stick_Data
		{
			math::vec2f value;
		};
        struct Axis_Data
		{
            float value = 0;
		};

        auto get_stick_data(Stick id) const -> Stick_Data;
        auto get_axis_data(Axis id) const -> Axis_Data;

        bool is_button_inactive(Button button) const;
        bool is_button_pressed(Button button) const;
        bool is_button_released(Button button) const;

        auto get_all_pressed_buttons() const -> std::vector<Button>;
        auto get_all_released_buttons() const -> std::vector<Button>;

        auto get_sensors() const -> Sensors const&;

	protected:
        void update(q::Clock::duration dt);
//		void add_event(Gamepad_Event const& event);
        void set_connected(bool yes);

        void set_stick_data(Stick stick, Stick_Data const& data);
        void set_axis_data(Axis axis, Axis_Data const& data);
        void set_button_pressed(Button button);
        void set_button_released(Button button);

	private:
//		void process_events(q::Clock::duration dt);

        q::String m_name;
        Type m_type;

        mutable std::mutex	m_mutex;

//		mutable std::mutex	m_events_mutex;
//		std::deque<Gamepad_Event> m_events;

        bool				m_is_connected = false;
		std::array<Stick_Data, 2> m_sticks;
        std::array<Axis_Data, 2> m_axes;
        std::set<Button>	m_buttons_pressed;
        std::set<Button>	m_buttons_released;

		Sensors				m_sensors;
	};


}
