#pragma once

namespace qinput
{
	struct Key_Code
	{
		enum
		{
			//all alphanum keys are with their UTF code, uppercase.
			//the following are just special keys			

			UNKNOWN = 0xFFFFFFF, //to force a huge value

			//////////////////////////////////////////////////////////////////////////
			// Special keys

			//Many devices have these.
			//On PC, they map to enter and escape
			CONFIRM,
			BACK,
			MENU,

			//direction keys
			UP,
			DOWN,
			LEFT,
			RIGHT,

			//multimedia
			MM_VOLUME_UP,
			MM_VOLUME_DOWN,
			MM_VOLUME_MUTE,
			MM_PLAY,
			MM_PAUSE,
			MM_STOP,
			MM_NEXT,
			MM_PREVIOUS,

			ESCAPE,
			CONTROL,
			ALT,
			SHIFT,
			INSERT,
			HOME,
			END,
			PAGE_UP,
			PAGE_DOWN,
			DEL, //cannot use delete due to a stupid macro
			BACKSPACE,

			NUM_0,
			NUM_1,
			NUM_2,
			NUM_3,
			NUM_4,
			NUM_5,
			NUM_6,
			NUM_7,
			NUM_8,
			NUM_9,
			NUM_ADD,
			NUM_SUBSTRACT,
			NUM_MULTIPLY,
			NUM_DIVIDE,
			NUM_DECIMAL,

			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
		};
	};

	struct Keyboard_Event
	{
		enum class Type
		{
			PRESS,
			RELEASE
		};

		Type type;
		q::Clock::time_point timestamp;

		int key_code;
	};

	class Input_Mgr;

	class Keyboard : q::util::Noncopyable
	{
		friend class Input_Mgr;
	public:
		Keyboard();

		//Keys
		//test for a value with the UTF8 code of the key in uppercase, or the special value above
		bool				is_key_inactive(int key) const;
		bool				is_key_pressed(int key) const;
		bool				is_key_released(int key) const;

		std::vector<int>	get_all_pressed_keys() const;
		std::vector<int>	get_all_released_keys() const;

	protected:
		void				update(q::Clock::duration dt);
		void				add_event(Keyboard_Event const& event);

	private:
		void				process_events(q::Clock::duration dt);

	private:
		mutable std::mutex	m_mutex;
		std::set<int>	m_keys_pressed;
		std::set<int>	m_keys_released;

		mutable std::mutex	m_events_mutex;
		std::deque<Keyboard_Event> m_events;

	};


}
