#pragma once

namespace qinput
{
	class Touchscreen;

	class Touch : q::util::Noncopyable
	{
		friend class Touchscreen;
	public:

		//Note, if you change this, change the mPositions array length to fit them all
        enum class State: uint8_t
		{
			INACTIVE	= 1 << 0,
			PRESSED		= 1 << 1,
			MOVED		= 1 << 2,
			RELEASED	= 1 << 3,
			CANCELED	= 1 << 4
		};
        typedef q::util::Flag_Set<State, uint8_t> States;

		Touch(uint32_t id);

		uint32_t			get_id() const;
		State				get_state() const;
		bool				operator==(State state) const;
		math::vec2f			get_position() const;
		math::vec2f			get_position(State state) const;
		math::vec2f			get_delta() const;

	protected:
		void				set_state(State state);
		void				set_position(State state, math::vec2f const& position);
		void				update_delta();

	private:
		mutable std::mutex m_mutex;
		uint32_t	m_id;
		State		m_state;

		//mPositions[0] is always the current position
		std::array<math::vec2f, 64> m_positions; //until 1 << 6

		math::vec2f m_delta;
		math::vec2f m_new_delta;
	};

	inline Touch::Touch(uint32_t id)
		: m_id(id)
		, m_state(State::INACTIVE)
	{
		;
	}

	inline uint32_t Touch::get_id() const
	{
		return m_id;
	}
	inline Touch::State Touch::get_state() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		State myState = m_state;
		return myState;
	}
	inline bool Touch::operator==(State state) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		State myState = m_state;
		return myState == state;
	}
	inline math::vec2f Touch::get_position() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto pos = m_positions[0];
		return pos;
	}
	inline math::vec2f Touch::get_delta() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto delta = m_delta;
		return delta;
	}
	inline void Touch::update_delta()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_delta = m_new_delta;
		m_new_delta = math::vec2f(0);
	}
	inline math::vec2f Touch::get_position(State state) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto pos = m_positions[(int)state];
		return pos;
	}
	inline void Touch::set_state(State state)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_state = state;
	}
	inline void Touch::set_position(State state, math::vec2f const& position)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_new_delta = (state == State::PRESSED) ? math::vec2f(0) : position - m_positions[0];
		m_positions[0] = position;
		m_positions[(int)state] = position;
	}

}
