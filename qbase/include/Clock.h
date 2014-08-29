#pragma once

namespace q
{
	
	class Clock
	{
	public:
		typedef int64_t									rep;
		typedef std::micro								period;
		typedef std::chrono::duration<rep, period>      duration;
		typedef std::chrono::time_point<Clock>			time_point;
		static const bool is_steady = true;

		static time_point now();

		static time_t to_time_t(time_point tp);
		static time_point from_time_t(time_t t);
	};

	typedef Clock::duration Duration;
	typedef std::chrono::duration<float> Seconds;

}