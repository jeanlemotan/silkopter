#pragma once

namespace qinput
{
	class Input_Mgr;

	struct Sensor_Event
	{
		enum class Type
		{
			ACCELEROMETER_CHANGE
		};

		Type type;
		q::Clock::time_point timestamp;

		math::vec3f accelerometer_value;
	};


	class Sensors : q::util::Noncopyable
	{
		friend class Input_Mgr;
	public:
		Sensors();

		struct Acceleration_Data
		{
			math::vec3f value;
			q::Clock::time_point timestamp;
		};

		Acceleration_Data	get_acceleration_data() const;
		std::vector<Acceleration_Data>	get_acceleration_data_history() const;

	protected:
		void				update(q::Clock::duration dt);
		void				add_event(Sensor_Event const& event);

	private:
		void				process_events(q::Clock::duration dt);

		mutable std::mutex	m_mutex;
		std::deque<Acceleration_Data>	m_acceleration_data;

		mutable std::mutex	m_events_mutex;
		std::deque<Sensor_Event> m_events;
	};

}
